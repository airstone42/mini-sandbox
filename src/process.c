#include "process.h"
#include "jail.h"

const char *CC = "gcc";
const char *CXX = "g++";
const char *JAVAC = "javac";
const char *JAVA = "java";
const char *DEV_NULL = "/dev/null";
const char *CC_OUT = "-o";
const char *JAVAC_DIR = "-d";
const char *JAVA_CP = "-classpath";

struct tlimit_t tlimit_default() {
    struct tlimit_t result = {1000, 20};
    return result;
}

bool procedure(struct tlimit_t *limit, struct tusage_t *usage,
               void (*func)(void *), void *arg) {
    int status = 0;
    pid_t proc = fork();

    /* call action, compilation or execution */
    if (!proc)
        (*func)(arg);

    struct rusage proc_usage;
    int poll_count = 0;

    /* poll child process information */
    while (poll_count <= limit->total / limit->interval) {
        usleep(limit->interval * 1000);
        poll_count++;

        if (!wait4(proc, &status, WNOHANG, &proc_usage)) {
            if (poll_count <= limit->total / limit->interval) {
                continue;
            } else {
                kill(proc, SIGKILL);
                return false;
            }
        } else {
            /* set time usage message */
            usage->mem = proc_usage.ru_maxrss;
            usage->time = proc_usage.ru_utime.tv_sec * 1000 +
                          proc_usage.ru_utime.tv_usec / 1000;
            break;
        }
    }
    return true;
}

bool compile(struct tlimit_t *limit, struct tusage_t *usage, void *arg) {
    return procedure(limit, usage, do_compile, arg);
}

bool execute(struct tlimit_t *limit, struct tusage_t *usage, void *arg) {
    return procedure(limit, usage, do_execute, arg);
}

void do_compile(void *arg) {
    struct comp_t *comp = (struct comp_t *) arg;

    /* redirect I/O for child process */
    int fd1 = open(DEV_NULL, O_RDWR);
    dup2(fd1, 1);
    close(fd1);
    int fd2 = open(DEV_NULL, O_WRONLY);
    dup2(fd2, 2);
    close(fd2);

    switch (comp->type) {
        case SRC_C:
        case SRC_CXX:
            c_compile(arg);
            break;
        case SRC_JAVA:
            j_compile(arg);
            break;
        case SRC_OTHERS:
            break;
    }
}

void c_compile(void *arg) {
    struct c_comp_t *comp = (struct c_comp_t *) arg;
    if (comp->src.type == SRC_C)
        execlp(CC, CC, comp->src.file, CC_OUT, comp->bin, NULL);
    else
        execlp(CXX, CXX, comp->src.file, CC_OUT, comp->bin, NULL);
}

void j_compile(void *arg) {
    struct j_comp_t *comp = (struct j_comp_t *) arg;
    if (comp->dir && strcmp(comp->dir, "") != 0) {
        mkdir(comp->dir, 0755);
        execlp(JAVAC, JAVAC, JAVAC_DIR, comp->dir, comp->src.file, NULL);
    } else
        execlp(JAVAC, JAVAC, comp->src.file, NULL);
}

void do_execute(void *arg) {
    struct exec_t *exec = (struct exec_t *) arg;
    if (access(exec->in, F_OK) != -1) {

        /* redirect I/O for child process */
        if (exec->in && strcmp(exec->in, "") != 0) {
            int fd = open(exec->in, O_RDONLY);
            dup2(fd, 0);
            close(fd);
        }
        if (exec->out && strcmp(exec->out, "") != 0) {
            int fd = open(exec->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(fd, 1);
            close(fd);
        }
        int fd = (exec->err && strcmp(exec->err, "") != 0)
                 ? open(exec->err, O_WRONLY, 0644)
                 : open(DEV_NULL, O_WRONLY, 0644);
        dup2(fd, 2);
        close(fd);

        seccomp_rules_add(exec->type);
        switch (exec->type) {
            case SRC_C:
            case SRC_CXX:
                c_execute(arg);
                break;
            case SRC_JAVA:
                j_execute(arg);
                break;
            case SRC_OTHERS:
                break;
        }
    }
}

void c_execute(void *arg) {
    struct c_exec_t *exec = (struct c_exec_t *) arg;
    execlp(exec->bin, exec->bin, NULL);
}

void j_execute(void *arg) {
    struct j_exec_t *exec = (struct j_exec_t *) arg;
    if (exec->dir && strcmp(exec->dir, "") != 0) {
        if (access(exec->dir, F_OK) != -1)
            execlp(JAVA, JAVA, JAVA_CP, exec->dir, exec->class, NULL);
    } else
        execlp(JAVA, JAVA, exec->class, NULL);
}

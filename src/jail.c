#include "jail.h"

const int strict_rules[] = {SCMP_SYS(rt_sigreturn), SCMP_SYS(exit),
                            SCMP_SYS(read), SCMP_SYS(write)};

const int extra_rules[] = {
        SCMP_SYS(close), SCMP_SYS(fstat), SCMP_SYS(lseek),
        SCMP_SYS(mmap), SCMP_SYS(mprotect), SCMP_SYS(munmap),
        SCMP_SYS(brk), SCMP_SYS(access), SCMP_SYS(execve),
        SCMP_SYS(arch_prctl), SCMP_SYS(openat), SCMP_SYS(exit_group)};

const int java_rules[] = {SCMP_SYS(futex),
                          SCMP_SYS(clone),
                          SCMP_SYS(getpid),
                          SCMP_SYS(stat),
                          SCMP_SYS(rt_sigaction),
                          SCMP_SYS(rt_sigprocmask),
                          SCMP_SYS(readlink),
                          SCMP_SYS(set_tid_address),
                          SCMP_SYS(set_robust_list),
                          SCMP_SYS(prlimit64),
                          SCMP_SYS(getdents64),
                          SCMP_SYS(sysinfo),
                          SCMP_SYS(clock_getres),
                          SCMP_SYS(lstat),
                          SCMP_SYS(sched_getaffinity),
                          SCMP_SYS(geteuid),
                          SCMP_SYS(socket),
                          SCMP_SYS(connect),
                          SCMP_SYS(fchdir),
                          SCMP_SYS(kill),
                          SCMP_SYS(mkdir),
                          SCMP_SYS(ftruncate),
                          SCMP_SYS(unlink),
                          SCMP_SYS(gettid),
                          SCMP_SYS(fcntl),
                          SCMP_SYS(prctl),
                          SCMP_SYS(uname),
                          SCMP_SYS(getuid),
                          SCMP_SYS(getcwd),
                          SCMP_SYS(sched_yield),
                          SCMP_SYS(madvise)};

void seccomp_rules_add(enum src_type_t src_type) {
    prctl(PR_SET_NO_NEW_PRIVS, 1);
    prctl(PR_SET_DUMPABLE, 0);
    scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_KILL);
    for (int i = 0; i < (int) (sizeof(strict_rules) / sizeof(*strict_rules)); i++)
        seccomp_rule_add(ctx, SCMP_ACT_ALLOW, strict_rules[i], 0);
    for (int i = 0; i < (int) (sizeof(extra_rules) / sizeof(*extra_rules)); i++)
        seccomp_rule_add(ctx, SCMP_ACT_ALLOW, extra_rules[i], 0);
    if (src_type == JAVA_SRC) {
        for (int i = 0; i < (int) (sizeof(java_rules) / sizeof(*java_rules)); i++)
            seccomp_rule_add(ctx, SCMP_ACT_ALLOW, java_rules[i], 0);
    }
    seccomp_load(ctx);
}

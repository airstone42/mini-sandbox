#include "cli.h"

const char *CLI_BAD_OPT = "Invalid operation\n";
const char *CLI_BAD_ARG = "Invalid argument: %c, use -h for help\n";

void cli_default_option(struct cli_arg_t *arg) {
    if (arg->option == 0)
        arg->option = CLI_PROC;
}

bool cli_set_option(struct cli_arg_t *arg, enum cli_option_t option) {
    if (arg->option != 0)
        return false;
    arg->option = option;
    return true;
}

bool cli_set_src_type(struct cli_arg_t *arg, char *lang) {
    if (strcmp(lang, "c") == 0)
        arg->type = SRC_C;
    else if (strcmp(lang, "c++") == 0 || strcmp(lang, "cc") == 0 ||
             strcmp(lang, "cxx") == 0 || strcmp(lang, "cpp") == 0)
        arg->type = SRC_CXX;
    else if (strcmp(lang, "java") == 0)
        arg->type = SRC_JAVA;
    else
        return false;
    return true;
}

bool cli_set_src_file(struct cli_arg_t *arg, char *file) {
    switch (arg->type) {
        case SRC_C:
            arg->comp.c.src.type = SRC_C;
            arg->exec.c.exe.type = SRC_C;
            arg->comp.c.src.file = file;
            break;
        case SRC_CXX:
            arg->comp.c.src.type = SRC_CXX;
            arg->exec.c.exe.type = SRC_CXX;
            arg->comp.c.src.file = file;
            break;
        case SRC_JAVA:
            arg->comp.j.src.type = SRC_JAVA;
            arg->exec.j.exe.type = SRC_JAVA;
            arg->comp.j.src.file = file;
            break;
        case SRC_OTHERS:
        default:
            return false;
    }
    return true;
}

bool cli_set_exe_file(struct cli_arg_t *arg, char *file) {
    switch (arg->type) {
        case SRC_C:
        case SRC_CXX:
            arg->comp.c.bin = file;
            arg->exec.c.bin = complete_bin_path(file);
            /* Mark if bin is added with "./", causing pointer address unequal */
            if (arg->exec.c.bin != file)
                arg->info.dyn_bin_path = true;
            break;
        case SRC_JAVA:
            arg->comp.j.class = file;
            arg->exec.j.class = file;
            break;
        case SRC_OTHERS:
        default:
            return false;
    }
    return true;
}

bool cli_set_java_path(struct cli_arg_t *arg, char *dir) {
    if (arg->type != SRC_JAVA)
        return false;
    arg->comp.j.dir = dir;
    arg->exec.j.dir = dir;
    return true;
}

bool cli_set_io(struct cli_arg_t *arg, char *file, enum cli_io_t flag) {
    switch (arg->type) {
        case SRC_C:
        case SRC_CXX:
            switch (flag) {
                case FILE_IN:
                    arg->exec.c.exe.in = file;
                    break;
                case FILE_OUT:
                    arg->exec.c.exe.out = file;
                    break;
                case FILE_ERR:
                    arg->exec.c.exe.err = file;
                    break;
                default:
                    return false;
            }
            break;
        case SRC_JAVA:
            switch (flag) {
                case FILE_IN:
                    arg->exec.j.exe.in = file;
                    break;
                case FILE_OUT:
                    arg->exec.j.exe.out = file;
                    break;
                case FILE_ERR:
                    arg->exec.j.exe.err = file;
                    break;
                default:
                    return false;
            }
            break;
        case SRC_OTHERS:
        default:
            return false;
    }
    return true;
}

void cli_help() {
    printf("-c                compile only\n");
    printf("-r                execute only\n");
    printf("-p                compile and run\n");
    printf("-l lang           set language, c, c++ or java\n");
    printf("-f source         source code filename\n");
    printf("-e executable     executable file filename/ java class filename\n");
    printf("-d directory      java classpath argument\n");
    printf("-i input          input file redirection\n");
    printf("-o output         output file redirection\n");
    printf("-x error          stderr redirection\n");
}

void delete_cli_arg(struct cli_arg_t *arg) {
    if (arg->info.dyn_bin_path)
        free(arg->exec.c.bin);
}

char *complete_bin_path(char *src) {
    if (strchr(src, '/')) {
        return src;
    } else {
        char *dst = (char *) malloc(strlen("./") + strlen(src) + 1);
        strcat(dst, "./");
        strcat(dst, src);
        return dst;
    }
}

#ifndef CLI_H
#define CLI_H

#include "process.h"

#include <stdio.h>

extern const char *CLI_BAD_OPT;
extern const char *CLI_BAD_ARG;

enum cli_option_t {
    CLI_COMP = 1, /* compile only */
    CLI_EXEC = 2, /* execute only */
    CLI_PROC = 3, /* compile and execute */
};

enum cli_io_t {
    FILE_IN = 0,
    FILE_OUT = 1,
    FILE_ERR = 2,
};

/* Information for compiling different languages */
union cli_comp_t {
    struct c_comp_t c;
    struct j_comp_t j;
};

/* Information for executing different programs */
union cli_exec_t {
    struct c_exec_t c;
    struct j_exec_t j;
};

struct cli_info_t {
    bool dyn_bin_path; /* mark if binary filename is dynamically allocated */
};

struct cli_arg_t {
    enum cli_option_t option;
    enum src_type_t type;
    union cli_comp_t comp; /* store information for C, C++ or Java program */
    union cli_exec_t exec; /* same as above */
    struct cli_info_t info;
};

void cli_default_option(struct cli_arg_t *arg);

bool cli_set_option(struct cli_arg_t *arg, enum cli_option_t option);

bool cli_set_src_type(struct cli_arg_t *arg, char *lang);

bool cli_set_src_file(struct cli_arg_t *arg, char *file);

/* Set C/C++ binary filename or Java class filename */
bool cli_set_exe_file(struct cli_arg_t *arg, char *file);

/* Set Java classpath if exists */
bool cli_set_java_path(struct cli_arg_t *arg, char *dir);

/* Set I/O redirection */
bool cli_set_io(struct cli_arg_t *arg, char *file, enum cli_io_t flag);

void cli_help();

void delete_cli_arg(struct cli_arg_t *arg);

/* Add "./" if executable file is in current directory */
char *complete_bin_path(char *src);

#endif

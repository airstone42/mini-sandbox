#ifndef PROCESS_H
#define PROCESS_H

#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern const char *CC;  /* C compiler */
extern const char *CXX; /* C++ compiler */
extern const char *JAVAC;
extern const char *JAVA;
extern const char *DEV_NULL;
extern const char *CC_OUT;    /* gcc -o / clang -o */
extern const char *JAVAC_DIR; /* javac -d */
extern const char *JAVA_CP;   /* java -cp / java -classpath */

/* Source code type */
enum src_type_t {
    C_SRC = 0,
    CXX_SRC = 1,
    JAVA_SRC = 2,
    OTHER_SRC = 127,
};

/* Time limit, millisecond */
struct tlimit_t {
    int total;
    int interval;
};

/* Time usage, millisecond */
struct tusage_t {
    long mem;
    long time;
};

/* Information for compilation */
struct comp_t {
    enum src_type_t type; /* source code type */
    char *file;           /* source code filename */
};

/* Information of C/C++ program for compilation */
struct c_comp_t {
    struct comp_t src;
    char *bin; /* executable file filename */
};

/* Information of Java program for compilation */
struct j_comp_t {
    struct comp_t src;
    char *class; /* Java class file filename */
    char *dir;   /* subdirectory for Java class file, if exists */
};

/* Information for execution */
struct exec_t {
    enum src_type_t type;
    char *in;  /* redirection filename for stdin */
    char *out; /* redirection filename for stdout */
    char *err; /* redirection filename for stderr */
};

/* Information for executing C/C++ program */
struct c_exec_t {
    struct exec_t exe;
    char *bin; /* executable file filename */
};

/* Information for executing Java program */
struct j_exec_t {
    struct exec_t exe;
    char *class; /* Java class file filename */
    char *dir;   /* subdirectory for Java class file, if exists */
};

/* Template for compiling or executing with child processes */
bool procedure(struct tlimit_t *limit, struct tusage_t *usage,
               void (*func)(void *), void *arg);

bool compile(struct tlimit_t *limit, struct tusage_t *usage, void *arg);

bool execute(struct tlimit_t *limit, struct tusage_t *usage, void *arg);

void do_compile(void *arg);

void c_compile(void *arg);

void j_compile(void *arg);

void do_execute(void *arg);

void c_execute(void *arg);

void j_execute(void *arg);

#endif

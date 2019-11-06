#ifndef JAIL_H
#define JAIL_H

#include "process.h"

#include <seccomp.h>
#include <sys/prctl.h>

/* add seccomp filter rules based on source code type */
void seccomp_rules_add(enum src_type_t type);

/* permitted syscalls in seccomp strict mode */
extern const int strict_rules[];

/* syscalls by C/C++ program */
extern const int extra_rules[];

/* syscalls by Java program */
extern const int java_rules[];

#endif

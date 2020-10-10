#ifndef __SYS_INFO_H
#define __SYS_INFO_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

void read_cpu();
void read_memory();
void display_sys();

#endif
#ifndef LOGGER
#define LOGGER

#include <stdio.h>

#include "log_levels.h"

FILE *log_init(char *filename);
void logcat(FILE *log_file, char *message, enum log_level level);
void log_close(FILE *log_file);

#endif

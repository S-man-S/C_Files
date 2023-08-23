#include "logger.h"

#include "time.h"

FILE *log_init(char *filename) {
    FILE *log = fopen(filename, "a");
    return log;
}

void logcat(FILE *file, char *message, enum log_level level) {
    switch (level) {
        case 0:
            fputs("DEBUG: ", file);
            break;
        case 1:
            fputs("TRACE: ", file);
            break;
        case 2:
            fputs("INFO: ", file);
            break;
        case 3:
            fputs("WARNING: ", file);
            break;
        case 4:
            fputs("ERROR: ", file);
            break;
    }
    long int ttime = time(NULL);
    fputs(ctime(&ttime), file);
    fputs("\b ", file);
    fputs(message, file);
    fputs("\n", file);
}

void log_close(FILE *log_file) { fclose(log_file); }

#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include "logger.h"

int output(FILE *file);
void cesar(char *ch, int shift);
void cesarmain(char *data, int shift);
void logging(char *logname, char *Start, char *Middle, char *End, char *End2, enum log_level lvl);

int main() {
    char check[2];
    FILE *file = NULL;
    char fname[101];
    char data[101];
    int shift;
#ifdef LOG
    char logname[101];
    fgets(logname, 101, stdin);
    logname[strlen(logname) - 1] = '\0';
#endif
    while (scanf("%c%c", &check[0], &check[1]) == 2) {
        if (check[0] == '-' && check[1] == '1') break;
        rewind(stdin);
        DIR *folder;
        struct dirent *entry;
        int files = 0;
        switch (check[0]) {
            case '1':
                fgets(fname, 101, stdin);
                fname[strlen(fname) - 1] = '\0';
                if ((file = fopen(fname, "r")) != NULL) {
#ifdef LOG
                    logging(logname, "Open file ", fname, "", "", debug);
#endif
                    if (!output(file)) {
#ifdef LOG
                        logging(logname, "File ", fname, " is empty", "", warning);
#endif
                        fputs("n/a", stdout);
                    } else {
#ifdef LOG
                        logging(logname, "File ", fname, " outputed successful", "", debug);
#endif
                    }
                } else {
#ifdef LOG
                    logging(logname, "File ", fname, " wasn't open", "", error);
#endif
                    fputs("n/a", stdout);
                }
                fputs("\n", stdout);
                break;

            case '2':
                if ((file = fopen(fname, "r+")) != NULL) {
#ifdef LOG
                    logging(logname, "Open file ", fname, "", "", debug);
#endif
                    fgets(data, 101, stdin);
                    if (output(file)) {
#ifdef LOG
                        logging(logname, "File ", fname, " outputed successful", "", debug);
#endif
                    } else {
#ifdef LOG
                        logging(logname, "File ", fname, " is empty", "", warning);
#endif
                        fputs("n/a\n", stdout);
                    }

                    fputs(data, file);
                    fputs(data, stdout);
#ifdef LOG
                    logging(logname, "File ", fname, " was updated with string ", data, error);
#endif
                } else {
                    fgets(data, 101, stdin);
#ifdef LOG
                    logging(logname, "File ", fname, " wasn't open", "", error);
#endif
                    fputs("n/a", stdout);
                }
                fputs("\n", stdout);
                break;

            case '3':
                fgets(fname, 101, stdin);
                fname[strlen(fname) - 1] = '\0';
                folder = opendir(fname);
                scanf("%d", &shift);
                rewind(stdin);
                if (folder == NULL) {
                    fputs("n/a", stdout);
#ifdef LOG
                    logging(logname, "Directory ", fname, " wasn't open", "", error);
#endif
                } else {
#ifdef LOG
                    logging(logname, "Open directory ", fname, "", "", debug);
#endif
                    while ((entry = readdir(folder))) {
                        strcpy(data, fname);
                        if (strlen(entry->d_name) > 2 && (entry->d_name)[strlen(entry->d_name) - 2] == '.') {
                            strcat(data, entry->d_name);
                            if ((entry->d_name)[strlen(entry->d_name) - 1] == 'c') {
                                cesarmain(data, shift);
#ifdef LOG
                                logging(logname, "File ", data, "was ciphered", "", debug);
#endif
                            }
                            if ((entry->d_name)[strlen(entry->d_name) - 1] == 'h') {
                                FILE *h = fopen(data, "w");
                                fclose(h);
#ifdef LOG
                                logging(logname, "File ", data, "was cleaned", "", debug);
#endif
                            }
                        }
                        files++;
                    }
                }
                closedir(folder);
#ifdef LOG
                logging(logname, "Close directory ", fname, "", "", debug);
#endif
                break;

            default:
                fputs("n/a\n", stdout);
                break;
        }
        if (file != NULL) {
            fclose(file);
#ifdef LOG
            logging(logname, "Close file ", fname, "", "", debug);
#endif
        }
    }
    return 0;
}

void cesarmain(char *data, int shift) {
    FILE *h = fopen(data, "r");
    char str[1001];
    str[0] = '\0';
    char ch[101];
    while (fgets(ch, 101, h)) {
        cesar(ch, shift);
        strcat(str, ch);
    }
    fclose(h);
    h = fopen(data, "w");
    fputs(str, h);
    fclose(h);
}

void cesar(char *ch, int shift) {
    shift = shift % 26;
    for (size_t i = 0; i < strlen(ch); i++) {
        if (ch[i] >= 'a' && ch[i] <= 'z') {
            ch[i] += shift;
            if (ch[i] > 'z') ch[i] -= 26;
            if (ch[i] < 'a') ch[i] += 26;
        } else if (ch[i] >= 'A' && ch[i] <= 'Z') {
            ch[i] += shift;
            if (ch[i] > 'Z') ch[i] -= 26;
            if (ch[i] < 'A') ch[i] += 26;
        }
    }
}

int output(FILE *file) {
    char data[101];
    int ret = 1;
    if (fgets(data, 101, file) == NULL) {
        ret = 0;
    } else {
        fputs(data, stdout);
        while (fgets(data, 101, file) != NULL) {
            fputs(data, stdout);
        }
    }
    return ret;
}

#ifdef LOG
void logging(char *logname, char *Start, char *Middle, char *End, char *End2, enum log_level lvl) {
    FILE *logfile = log_init(logname);
    char Out[101];
    Out[0] = '\0';
    strcat(Out, Start);
    strcat(Out, Middle);
    strcat(Out, End);
    strcat(Out, End2);
    logcat(logfile, Out, lvl);
    log_close(logfile);
}
#endif

#ifndef __CRTL_BITS_LINENOISE_H
#define __CRTL_BITS_LINENOISE_H 1


#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>



typedef struct crtl_linenoise_completions_t {
    size_t len;
    char **cvec;
} crtl_linenoise_completions_t;

typedef void(*linenoiseCompletionCallback)(const char *, crtl_linenoise_completions_t *);
typedef char*(*linenoiseHintsCallback)(const char *, int *color, int *bold);
typedef void(*linenoiseFreeHintsCallback)(void *);

void linenoiseSetCompletionCallback(linenoiseCompletionCallback);
void linenoiseSetHintsCallback(linenoiseHintsCallback );
void linenoiseSetFreeHintsCallback(linenoiseFreeHintsCallback);
void linenoiseAddCompletion(crtl_linenoise_completions_t *, const char *);

char *linenoise(const char *prompt);
void linenoiseFree(void *ptr);
int linenoiseHistoryAdd(const char *line);
int linenoiseHistorySetMaxLen(int len);
int linenoiseHistorySave(const char *filename);
int linenoiseHistoryLoad(const char *filename);
void linenoiseClearScreen(void);
void linenoiseSetMultiLine(int ml);
void linenoisePrintKeyCodes(void);



#endif /*<__CRTL_BITS_LINENOISE_H>*/



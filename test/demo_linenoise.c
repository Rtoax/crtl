#include "crtl/bits/crtl_linenoise.h"
#include "crtl/log.h"
#include "crtl/crtl_task.h"


static void completion(const char *buf, crtl_linenoise_completions_t *lc) 
{
    if (buf[0] == 'h') {
        linenoiseAddCompletion(lc,"hello");
        linenoiseAddCompletion(lc,"hello there");
        linenoiseAddCompletion(lc,"hello that rong");
    }
}

static char *hints(const char *buf, int *color, int *bold) 
{
    if (!strcasecmp(buf,"hello")) {
        *color = 35;
        *bold = 0;
        return " World111";
    }
    else if(!strcasecmp(buf,"hello there"))
    {
        *color = 35;
        *bold = 0;
        return " you are";
    }
    return NULL;
}

static int cli_linenoise_main(int argc, char **argv) {
    char *line;
    char *prgname = argv[0];

    /* Parse options, with --multiline we enable multi line editing. */
    while(argc > 1) 
    {
        argc--;
        argv++;
        if (strcmp(*argv,"--multiline")==0) 
        {
            crtl_print_info("multiline\n");
            linenoiseSetMultiLine(1);
            printf("Multi-line mode enabled.\n");
        } 
        else if (strcmp(*argv,"--keycodes")==0) 
        {
            crtl_print_info("keycodes\n");
            linenoisePrintKeyCodes();
            exit(0);
        } 
        else 
        {
            fprintf(stderr, "Usage: %s [--multiline] [--keycodes]\n", prgname);
            exit(1);
        }
    }

    /* Set the completion callback. This will be called every time the
     * user uses the <tab> key. */
    linenoiseSetCompletionCallback(completion);
    linenoiseSetHintsCallback(hints);

    /* Load history from file. The history file is just a plain text file
     * where entries are separated by newlines. */
    linenoiseHistoryLoad("history.txt"); /* Load the history at startup */

    /* Now this is the main loop of the typical linenoise-based application.
     * The call to linenoise() will block as long as the user types something
     * and presses enter.
     *
     * The typed string is returned as a malloc() allocated string by
     * linenoise, so the user needs to free() it. */
    while((line = linenoise("hello> ")) != NULL) {
        /* Do something with the string. */
        if (line[0] != '\0' && line[0] != '/') {
            printf("echo: >> '%s'\n", line);
            linenoiseHistoryAdd(line); /* Add to the history. */
            linenoiseHistorySave("history.txt"); /* Save the history on disk. */
        } else if (!strncmp(line,"/historylen",11)) {
            /* The "/historylen" command will change the history len. */
            int len = atoi(line+11);
            linenoiseHistorySetMaxLen(len);
        } else if (line[0] == '/') {
            printf("Unreconized command: %s\n", line);
        }
        free(line);
    }
    return 0;
}

static void* __linenoise_thread_cb(void *arg)
{
    char *argv[] = 
    {
        "a.out",
        "--multiline"
            
    };
    int argc = 2;

    /* 会把终端阻塞独占 */
    cli_linenoise_main(argc, argv);
    
    return NULL;
}


void demo_linenoise_test1() 
{
    crtl_thread_t _unused task;

    crtl_thread_normal(&task, __linenoise_thread_cb, NULL);

    while(1)
    {
        sleep(1);
    }
}


int main()
{
    demo_linenoise_test1();

    
    return 0;
}


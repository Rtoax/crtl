#ifndef __CRTL_BITS_LIBCLI_H
#define __CRTL_BITS_LIBCLI_H 1


#ifdef __cplusplus
extern "C" {
#endif

#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <errno.h>
#include <memory.h>
#include <stdarg.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <crypt.h>
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE       /* See feature_test_macros(7) */
#endif
#include <unistd.h>
#include <regex.h>

#include <sys/time.h>
#include <limits.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <signal.h>
#include <strings.h>

/* major, minor, revision, version */
#define LIBCLI_VERSION_MAJOR        1
#define LIBCLI_VERISON_MINOR        10
#define LIBCLI_VERISON_REVISION     0
#define LIBCLI_VERSION ((LIBCLI_VERSION_MAJOR << 16) | (LIBCLI_VERISON_MINOR << 8) | LIBCLI_VERISON_REVISION)

/* cli errno */
#define CLI_OK                   0
#define CLI_ERROR               -1
#define CLI_QUIT                -2
#define CLI_ERROR_ARG           -3
#define CLI_AMBIGUOUS           -4
#define CLI_UNRECOGNIZED        -5
#define CLI_MISSING_ARGUMENT    -6
#define CLI_MISSING_VALUE       -7
#define CLI_BUILDMODE_START     -8
#define CLI_BUILDMODE_ERROR     -9
#define CLI_BUILDMODE_EXTEND    -10
#define CLI_BUILDMODE_CANCEL    -11
#define CLI_BUILDMODE_EXIT      -12

/* maximul history cmd */
#define LIBCLI_MAX_HISTORY      256

/* privilege */
#define LIBCLI_PRIVILEGE_UNPRIVILEGED   0
#define LIBCLI_PRIVILEGE_PRIVILEGED     15

/* mode */
#define LIBCLI_MODE_ANY         -1
#define LIBCLI_MODE_EXEC        0
#define LIBCLI_MODE_CONFIG      1

/* enable */
#define LIBCLI_HAS_ENABLE       1

/* print */
#define LIBCLI_PRINT_PLAIN      0x00
#define LIBCLI_PRINT_FILTERED   0x01
#define LIBCLI_PRINT_BUFFERED   0x02

/* maximul line(length, words) */
#define CLI_MAX_LINE_LENGTH     4096
#define CLI_MAX_LINE_WORDS      128


#define CLI_ENABLE_PASSWORD cli_enable_password
extern const char cli_enable_password[];




/* cmd type */
enum command_types {
    CLI_ANY_COMMAND,
    CLI_REGULAR_COMMAND,
    CLI_FILTER_COMMAND,
    CLI_BUILDMODE_COMMAND,
};


/* cli struct */
struct crtl_cli_struct {
    int completion_callback;
    struct crtl_cli_command *commands;
    int (*auth_callback)(const char *, const char *);
    int (*regular_callback)(struct crtl_cli_struct *cli);
    int (*enable_callback)(const char *);
    char *banner;
    struct unp *users;
    char *enable_password;
    char *history[LIBCLI_MAX_HISTORY];
    char showprompt;
    char *promptchar;
    char *hostname;
    char *modestring;
    int privilege;
    int mode;
    int state;
    struct crtl_cli_filter *filters;
    void (*print_callback)(struct crtl_cli_struct *cli, const char *string);
    FILE *client;
    /* internal buffers */
    void *conn;
    void *service;
    char *commandname;  // temporary buffer for crtl_cli_command_name() to prevent leak
    char *buffer;
    unsigned buf_size;
    struct timeval timeout_tm;
    time_t idle_timeout;
    int (*idle_timeout_callback)(struct crtl_cli_struct *);
    time_t last_action;
    int telnet_protocol;
    void *user_context;
    struct crtl_cli_optarg_pair *found_optargs;
    int transient_mode;
    struct crtl_cli_pipeline *pipeline;
    struct crtl_cli_buildmode *buildmode;
};

/* filter */
struct crtl_cli_filter {
    int (*filter)(struct crtl_cli_struct *cli, const char *string, void *data);
    void *data;
    struct crtl_cli_filter *next;
};


struct crtl_cli_command {
    char *command;
    int (*callback)(struct crtl_cli_struct *, const char *, char **, int);
    unsigned int unique_len;
    char *help;
    int privilege;
    int mode;
    struct crtl_cli_command *previous;
    struct crtl_cli_command *next;
    struct crtl_cli_command *children;
    struct crtl_cli_command *parent;
    struct crtl_cli_optarg *optargs;
    int (*filter)(struct crtl_cli_struct *cli, const char *string, void *data);
    int (*init)(struct crtl_cli_struct *cli, int, char **, struct crtl_cli_filter *filt);
    int command_type;
};

struct crtl_cli_comphelp {
    int comma_separated;
    char **entries;
    int num_entries;
};

enum optarg_flags {
    CLI_CMD_OPTIONAL_FLAG       = 1 << 0,
    CLI_CMD_OPTIONAL_ARGUMENT   = 1 << 1,
    CLI_CMD_ARGUMENT            = 1 << 2,
    CLI_CMD_ALLOW_BUILDMODE     = 1 << 3,
    CLI_CMD_OPTION_MULTIPLE     = 1 << 4,
    CLI_CMD_OPTION_SEEN         = 1 << 5,
    CLI_CMD_TRANSIENT_MODE      = 1 << 6,
    CLI_CMD_DO_NOT_RECORD       = 1 << 7,
    CLI_CMD_REMAINDER_OF_LINE   = 1 << 8,
    CLI_CMD_HYPHENATED_OPTION   = 1 << 9,
    CLI_CMD_SPOT_CHECK          = 1 << 10,
};

/* cmd optarg */
struct crtl_cli_optarg {
    char *name;
    int flags;
    char *help;
    int mode;
    int privilege;
    unsigned int unique_len;
    int (*get_completions)(struct crtl_cli_struct *, const char *, const char *, struct crtl_cli_comphelp *);
    int (*validator)(struct crtl_cli_struct *, const char *, const char *);
    int (*transient_mode)(struct crtl_cli_struct *, const char *, const char *);
    struct crtl_cli_optarg *next;
};

/* optarg pair */
struct crtl_cli_optarg_pair {
    char *name;
    char *value;
    struct crtl_cli_optarg_pair *next;
};

/* pipeline stage */
struct crtl_cli_pipeline_stage {
    struct crtl_cli_command *command;
    struct crtl_cli_optarg_pair *found_optargs;
    char **words;
    int num_words;
    int status;
    int first_unmatched;
    int first_optarg;
    int stage_num;
    char *error_word;
};

/* pipeline */
struct crtl_cli_pipeline {
    char *cmdline;
    char *words[CLI_MAX_LINE_WORDS];
    int num_words;
    int num_stages;
    struct crtl_cli_pipeline_stage stage[CLI_MAX_LINE_WORDS];
    struct crtl_cli_pipeline_stage *current_stage;
};

/* build mode */
struct crtl_cli_buildmode {
    struct crtl_cli_command *command;
    struct crtl_cli_optarg_pair *found_optargs;
    char *cname;
    int mode;
    int transient_mode;
    char *mode_text;
};

/* APIs */
struct crtl_cli_struct *crtl_cli_init(const char *banner, const char *hostname);
int crtl_cli_done(struct crtl_cli_struct *cli);
struct crtl_cli_command *crtl_cli_register_command(struct crtl_cli_struct *cli, struct crtl_cli_command *parent, const char *command,
                                         int (*callback)(struct crtl_cli_struct *, const char *, char **, int), int privilege,
                                         int mode, const char *help);
int crtl_cli_unregister_command(struct crtl_cli_struct *cli, const char *command);
int crtl_cli_run_command(struct crtl_cli_struct *cli, const char *command);
int crtl_cli_loop(struct crtl_cli_struct *cli, int sockfd);
int crtl_cli_file(struct crtl_cli_struct *cli, FILE *fh, int privilege, int mode);
void crtl_cli_set_auth_callback(struct crtl_cli_struct *cli, int (*auth_callback)(const char *, const char *));
void crtl_cli_set_enable_callback(struct crtl_cli_struct *cli, int (*enable_callback)(const char *));
void crtl_cli_allow_user(struct crtl_cli_struct *cli, const char *username, const char *password);
void crtl_cli_allow_enable(struct crtl_cli_struct *cli, const char *password);
void crtl_cli_deny_user(struct crtl_cli_struct *cli, const char *username);
void crtl_cli_set_banner(struct crtl_cli_struct *cli, const char *banner);
void crtl_cli_set_hostname(struct crtl_cli_struct *cli, const char *hostname);
void crtl_cli_set_promptchar(struct crtl_cli_struct *cli, const char *promptchar);
void crtl_cli_set_modestring(struct crtl_cli_struct *cli, const char *modestring);
int crtl_cli_set_privilege(struct crtl_cli_struct *cli, int privilege);
int crtl_cli_set_configmode(struct crtl_cli_struct *cli, int mode, const char *config_desc);
void crtl_cli_reprompt(struct crtl_cli_struct *cli);
void crtl_cli_regular(struct crtl_cli_struct *cli, int (*callback)(struct crtl_cli_struct *cli));
void crtl_cli_regular_interval(struct crtl_cli_struct *cli, int seconds);
void crtl_cli_print(struct crtl_cli_struct *cli, const char *format, ...) __attribute__((format(printf, 2, 3)));
void crtl_cli_bufprint(struct crtl_cli_struct *cli, const char *format, ...) __attribute__((format(printf, 2, 3)));
void crtl_cli_vabufprint(struct crtl_cli_struct *cli, const char *format, va_list ap);
void crtl_cli_error(struct crtl_cli_struct *cli, const char *format, ...) __attribute__((format(printf, 2, 3)));
void crtl_cli_print_callback(struct crtl_cli_struct *cli, void (*callback)(struct crtl_cli_struct *, const char *));
void crtl_cli_free_history(struct crtl_cli_struct *cli);
void crtl_cli_set_idle_timeout(struct crtl_cli_struct *cli, unsigned int seconds);
void crtl_cli_set_idle_timeout_callback(struct crtl_cli_struct *cli, unsigned int seconds, int (*callback)(struct crtl_cli_struct *));

// Enable or disable telnet protocol negotiation.
// Note that this is enabled by default and must be changed before crtl_cli_loop() is run.
void crtl_cli_telnet_protocol(struct crtl_cli_struct *cli, int telnet_protocol);

// Set/get user context
void crtl_cli_set_context(struct crtl_cli_struct *cli, void *context);
void *crtl_cli_get_context(struct crtl_cli_struct *cli);

void crtl_cli_free_comphelp(struct crtl_cli_comphelp *comphelp);
int crtl_cli_add_comphelp_entry(struct crtl_cli_comphelp *comphelp, const char *entry);
void crtl_cli_set_transient_mode(struct crtl_cli_struct *cli, int transient_mode);
struct crtl_cli_command *crtl_cli_register_filter(struct crtl_cli_struct *cli, const char *command,
                                        int (*init)(struct crtl_cli_struct *cli, int, char **, struct crtl_cli_filter *filt),
                                        int (*filter)(struct crtl_cli_struct *, const char *, void *), int privilege, int mode,
                                        const char *help);

int crtl_cli_unregister_filter(struct crtl_cli_struct *cli, const char *command);
int crtl_cli_register_optarg(struct crtl_cli_command *cmd, const char *name, int flags, int priviledge, int mode,
                        const char *help,
                        int (*get_completions)(struct crtl_cli_struct *cli, const char *, const char *, struct crtl_cli_comphelp *),
                        int (*validator)(struct crtl_cli_struct *cli, const char *, const char *),
                        int (*transient_mode)(struct crtl_cli_struct *, const char *, const char *));
char *crtl_cli_find_optarg_value(struct crtl_cli_struct *cli, char *name, char *find_after);
struct crtl_cli_optarg_pair *crtl_cli_get_all_found_optargs(struct crtl_cli_struct *cli);
int crtl_cli_unregister_optarg(struct crtl_cli_command *cmd, const char *name);
char *crtl_cli_get_optarg_value(struct crtl_cli_struct *cli, const char *name, char *find_after);
int crtl_cli_set_optarg_value(struct crtl_cli_struct *cli, const char *name, const char *value, int allow_multiple);
void crtl_cli_unregister_all_optarg(struct crtl_cli_command *c);
void crtl_cli_unregister_all_filters(struct crtl_cli_struct *cli);
void crtl_cli_unregister_all_commands(struct crtl_cli_struct *cli);
void crtl_cli_unregister_all(struct crtl_cli_struct *cli, struct crtl_cli_command *command);


#ifdef __cplusplus
}
#endif

#if 0
//Libcli provides a shared C library for including a Cisco-like command-line
//interface into other software.
//
//It¡¯s a telnet interface which supports command-line editing, history,
//authentication and callbacks for a user-definable function tree.
//
//To compile:
//
//```sh
//$ make
//$ make install
//```
//
//This will install `libcli.so` into `/usr/local/lib`. If you want to change the
//location, edit Makefile.
//
//There is a test application built called clitest. Run this and telnet to port
//8000.
//
//By default, a single username and password combination is enabled.
//
//```
//Username: fred
//Password: nerk
//```
//
//Get help by entering `help` or hitting `?`.
//
//libcli provides support for using the arrow keys for command-line editing. Up
//and Down arrows will cycle through the command history, and Left & Right can be
//used for editing the current command line.
//
//libcli also works out the shortest way of entering a command, so if you have a
//command `show users | grep foobar` defined, you can enter `sh us | g foobar` if that
//is the shortest possible way of doing it.
//
//Enter `sh?` at the command line to get a list of commands starting with `sh`
//
//A few commands are defined in every libcli program:
//
//* `ls`
//* `quit`
//* `exit`
//* `logout`
//* `history`
//
//Use in your own code:
//
//First of all, make sure you `#include <libcli.h>` in your C code, and link with
//`-lcli`.
//
//If you have any trouble with this, have a look at clitest.c for a
//demonstration.
//
//Start your program off with a `cli_init()`.
//This sets up the internal data structures required.
//
//When a user connects, they are presented with a greeting if one is set using the
//`cli_set_banner(banner)` function.
//
//By default, the command-line session is not authenticated, which means users
//will get full access as soon as they connect. As this may not be always the best
//thing, 2 methods of authentication are available.
//
//First, you can add username / password combinations with the
//`cli_allow_user(username, password)` function. When a user connects, they can
//connect with any of these username / password combinations.
//
//Secondly, you can add a callback using the `cli_set_auth_callback(callback)`
//function. This function is passed the username and password as `char *`, and must
//return `CLI_OK` if the user is to have access and `CLI_ERROR` if they are not.
//
//The library itself will take care of prompting the user for credentials.
//
//Commands are built using a tree-like structure. You define commands with the
//`cli_register_command(parent, command, callback, privilege, mode, help)` function.
//
//`parent` is a `cli_command *` reference to a previously added command. Using a
//parent you can build up complex commands.
//
//e.g. to provide commands `show users`, `show sessions` and `show people`, use
//the following sequence:
//
//```c
//cli_command *c = cli_register_command(NULL, "show", NULL, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC, NULL);
//cli_register_command(c, "sessions", fn_sessions, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC, "Show the sessions connected");
//cli_register_command(c, "users", fn_users, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC, "Show the users connected");
//cli_register_command(c, "people", fn_people, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC, "Show a list of the people I like");
//```
//
//If callback is `NULL`, the command can be used as part of a tree, but cannot be
//individually run. 
//
//If you decide later that you don't want a command to be run, you can call
//`cli_unregister_command(command)`.
//You can use this to build dynamic command trees.
//
//It is possible to carry along a user-defined context to all command callbacks
//using `cli_set_context(cli, context)` and `cli_get_context(cli)` functions.
//
//
//You are responsible for accepting a TCP connection, and for creating a
//process or thread to run the cli.  Once you are ready to process the
//connection, call `cli_loop(cli, sock)` to interact with the user on the
//given socket.
//
//This function will return when the user exits the cli, either by breaking the
//connection or entering `quit`.
//
//Call `cli_done()` to free the data structures.



#endif



#endif /*<__CRTL_BITS_LIBCLI_H>*/

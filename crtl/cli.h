#ifndef __CRTL_BITS_LIBCLI_H
#define __CRTL_BITS_LIBCLI_H 1


#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <malloc.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>


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
enum crtl_cli_command_types {
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

enum crtl_cli_optarg_flags {
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
struct crtl_cli_optarg_pair *crtl_cli_get_next_optargs(struct crtl_cli_optarg_pair *optarg);
const char *crtl_cli_get_name_optargs(struct crtl_cli_optarg_pair *optarg);
const char *crtl_cli_get_value_optargs(struct crtl_cli_optarg_pair *optarg);


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

#endif /*<__CRTL_BITS_LIBCLI_H>*/


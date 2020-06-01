#ifndef _GNU_SOURCE
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#endif
#include <memory.h>
#include <stdarg.h>
#include <ctype.h>
#include <crypt.h>
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE       /* See feature_test_macros(7) */
#endif
#include <unistd.h>
#include <regex.h>

#include <sys/time.h>
#include <limits.h>
#include <sys/types.h>
#include <signal.h>
#include <strings.h>

#include "crtl/cli.h"
#include "crtl/log.h"

#include "crypto/attribute.h"
#include "crypto/once.h"
#include "crypto/type/check.h"


#define MATCH_REGEX     1
#define MATCH_INVERT    2

/* maximul history cmd */
#define CLI_MAX_HISTORY      256

/* maximul line(length, words) */
#define CLI_MAX_LINE_LENGTH     4096
#define CLI_MAX_LINE_WORDS      128

enum crtl_cli_states {
    STATE_LOGIN,
    STATE_PASSWORD,
    STATE_NORMAL,
    STATE_ENABLE_PASSWORD,
    STATE_ENABLE,
};
    
struct crtl_cli_comphelp {
    int comma_separated;
    char **entries;
    int num_entries;
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
    char *history[CLI_MAX_HISTORY];
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

struct unp {
    char *username;
    char *password;
    struct unp *next;
};

struct crtl_cli_filter_cmds {
    const char *cmd;
    const char *help;
};

// Free and zero (to avoid double-free)
#define free_z(p) \
  do {            \
    if (p) {      \
      free(p);    \
      (p) = 0;    \
    }             \
  } while (0)

    
const char cli_enable_password[] = {"rong "};


// Forward defines of *INTERNAL* library function as static here
static int crtl_cli_search_flags_validator(struct crtl_cli_struct *cli, const char *word, const char *value);
static int crtl_cli_match_filter_init(struct crtl_cli_struct *cli, int argc, char **argv, struct crtl_cli_filter *filt);
static int crtl_cli_range_filter_init(struct crtl_cli_struct *cli, int argc, char **argv, struct crtl_cli_filter *filt);
static int crtl_cli_count_filter_init(struct crtl_cli_struct *cli, int argc, char **argv, struct crtl_cli_filter *filt);
static int crtl_cli_match_filter(struct crtl_cli_struct *cli, const char *string, void *data);
static int crtl_cli_range_filter(struct crtl_cli_struct *cli, const char *string, void *data);
static int crtl_cli_count_filter(struct crtl_cli_struct *cli, const char *string, void *data);
static void crtl_cli_int_parse_optargs(struct crtl_cli_struct *cli, struct crtl_cli_pipeline_stage *stage, struct crtl_cli_command *cmd,
                                  char lastchar, struct crtl_cli_comphelp *comphelp);
static int crtl_cli_int_enter_buildmode(struct crtl_cli_struct *cli, struct crtl_cli_pipeline_stage *stage, char *mode_text);
static char *crtl_cli_int_buildmode_extend_cmdline(char *, char *word);
static void crtl_cli_int_free_buildmode(struct crtl_cli_struct *cli);
static void crtl_cli_free_command(struct crtl_cli_struct *cli, struct crtl_cli_command *cmd);
static int crtl_cli_int_unregister_command_core(struct crtl_cli_struct *cli, const char *command, int command_type);
static int crtl_cli_int_unregister_buildmode_command(struct crtl_cli_struct *cli, const char *command) __attribute__((unused));
static struct crtl_cli_command *crtl_cli_int_register_buildmode_command(
                struct crtl_cli_struct *cli, struct crtl_cli_command *parent, const char *command,
                int (*callback)(struct crtl_cli_struct *cli, const char *, char **, int), int privilege, int mode, const char *help);
static int crtl_cli_int_buildmode_cmd_cback(struct crtl_cli_struct *cli, const char *command, char *argv[], int argc);
static int crtl_cli_int_buildmode_flag_cback(struct crtl_cli_struct *cli, const char *command, char *argv[], int argc);
static int crtl_cli_int_buildmode_flag_multiple_cback(struct crtl_cli_struct *cli, const char *command, char *argv[], int argc);
static int crtl_cli_int_buildmode_cancel_cback(struct crtl_cli_struct *cli, const char *command, char *argv[], int argc);
static int crtl_cli_int_buildmode_exit_cback(struct crtl_cli_struct *cli, const char *command, char *argv[], int argc);
static int crtl_cli_int_buildmode_show_cback(struct crtl_cli_struct *cli, const char *command, char *argv[], int argc);
static int crtl_cli_int_buildmode_unset_cback(struct crtl_cli_struct *cli, const char *command, char *argv[], int argc);
static int crtl_cli_int_buildmode_unset_completor(struct crtl_cli_struct *cli, const char *name, const char *word,
                                             struct crtl_cli_comphelp *comphelp);
static int crtl_cli_int_buildmode_unset_validator(struct crtl_cli_struct *cli, const char *name, const char *value);
static int crtl_cli_int_execute_buildmode(struct crtl_cli_struct *cli);
static void crtl_cli_int_free_found_optargs(struct crtl_cli_optarg_pair **optarg_pair);
static void crtl_cli_int_unset_optarg_value(struct crtl_cli_struct *cli, const char *name);
static struct crtl_cli_pipeline *crtl_cli_int_generate_pipeline(struct crtl_cli_struct *cli, const char *command);
static int crtl_cli_int_validate_pipeline(struct crtl_cli_struct *cli, struct crtl_cli_pipeline *pipeline);
static int crtl_cli_int_execute_pipeline(struct crtl_cli_struct *cli, struct crtl_cli_pipeline *pipeline);
inline void crtl_cli_int_show_pipeline(struct crtl_cli_struct *cli, struct crtl_cli_pipeline *pipeline);
static void crtl_cli_int_free_pipeline(struct crtl_cli_pipeline *pipeline);
static void crtl_cli_register_command_core(struct crtl_cli_struct *cli, struct crtl_cli_command *parent, struct crtl_cli_command *c);

/* write syscall wrap */
static ssize_t _write(int fd, const void *buf, size_t count) 
{
    size_t written = 0;
    ssize_t thisTime = 0;
    while (count != written) {
        thisTime = write(fd, (char *)buf + written, count - written);
        if (thisTime == -1) {
            if (errno == EINTR)
                continue;
            else
                return -1;
        }
        written += thisTime;
    }
    return written;
}

char *crtl_cli_command_name(struct crtl_cli_struct *cli, struct crtl_cli_command *command) 
{
    char *name;
    char *o;

    if (cli->commandname) {
        free(cli->commandname);
        cli->commandname = NULL;
    }
    name = cli->commandname;

    if (!(name = calloc(1, 1))) return NULL;

    while (command) {
        o = name;
        if (asprintf(&name, "%s%s%s", command->command, *o ? " " : "", o) == -1) {
            crtl_print_err("Couldn't allocate memory for command_name: %s", strerror(errno));
            free(o);
            return NULL;
        }
        command = command->parent;
        free(o);
    }
    cli->commandname = name;
    return name;
}


void crtl_cli_set_auth_callback(struct crtl_cli_struct *cli, int (*auth_callback)(const char *, const char *)) 
{
    typecheck_fn(typeof(cli->auth_callback), auth_callback);
    cli->auth_callback = auth_callback;
}


void crtl_cli_set_enable_callback(struct crtl_cli_struct *cli, int (*enable_callback)(const char *)) 
{
    typecheck_fn(typeof(cli->enable_callback), enable_callback);
    cli->enable_callback = enable_callback;
}


void crtl_cli_allow_user(struct crtl_cli_struct *cli, const char *username, const char *password) 
{
    struct unp *u, *n;
    if (!(n = malloc(sizeof(struct unp)))) {
        fprintf(stderr, "Couldn't allocate memory for user: %s", strerror(errno));
        return;
    }
    if (!(n->username = strdup(username))) {
        fprintf(stderr, "Couldn't allocate memory for username: %s", strerror(errno));
        free(n);
        return;
    }
    if (!(n->password = strdup(password))) {
        fprintf(stderr, "Couldn't allocate memory for password: %s", strerror(errno));
        free(n->username);
        free(n);
        return;
    }
    n->next = NULL;
    
    if (!cli->users) {
        cli->users = n;
    } else {
        for (u = cli->users; u && u->next; u = u->next)
        ;
        if (u) u->next = n;
    }
}


void crtl_cli_allow_enable(struct crtl_cli_struct *cli, const char *password) 
{
    free_z(cli->enable_password);
    if (!(cli->enable_password = strdup(password))) 
    {
        fprintf(stderr, "Couldn't allocate memory for enable password: %s", strerror(errno));
    }
}


void crtl_cli_deny_user(struct crtl_cli_struct *cli, const char *username) 
{
    struct unp *u, *p = NULL;
    if (!cli->users) return;
    for (u = cli->users; u; u = u->next) 
    {
        if (strcmp(username, u->username) == 0) 
        {
            if (p)
                p->next = u->next;
            else
                cli->users = u->next;
            free(u->username);
            free(u->password);
            free(u);
            break;
        }
        p = u;
    }
}


void crtl_cli_set_banner(struct crtl_cli_struct *cli, const char *banner) 
{
    free_z(cli->banner);
    if (banner && *banner) 
        cli->banner = strdup(banner);
}

void crtl_cli_set_hostname(struct crtl_cli_struct *cli, const char *hostname) 
{
    free_z(cli->hostname);
    if (hostname && *hostname) 
        cli->hostname = strdup(hostname);
}

void crtl_cli_set_client_timeout(struct crtl_cli_struct *cli, struct timeval *tv)
{
    cli->timeout_tm.tv_sec = tv->tv_sec;
    cli->timeout_tm.tv_usec = tv->tv_usec;
}

void crtl_cli_set_promptchar(struct crtl_cli_struct *cli, const char *promptchar) 
{
    free_z(cli->promptchar);
    cli->promptchar = strdup(promptchar);
}

static int crtl_cli_build_shortest(struct crtl_cli_struct *cli, struct crtl_cli_command *commands) 
{
    struct crtl_cli_command *c, *p;
    char *cp, *pp;
    unsigned len;

    for (c = commands; c; c = c->next) 
    {
        c->unique_len = strlen(c->command);
        if ((c->mode != CLI_MODE_ANY && c->mode != cli->mode) || c->privilege > cli->privilege) 
            continue;

        c->unique_len = 1;
        for (p = commands; p; p = p->next) 
        {
            if (c == p) continue;
            if (c->command_type != p->command_type) continue;
            if ((p->mode != CLI_MODE_ANY && p->mode != cli->mode) || p->privilege > cli->privilege) 
                continue;

            cp = c->command;
            pp = p->command;
            len = 1;

            while (*cp && *pp && *cp++ == *pp++) 
                len++;

            if (len > c->unique_len) 
                c->unique_len = len;
        }

        if (c->children) 
            crtl_cli_build_shortest(cli, c->children);
    }

    return CLI_OK;
}

int crtl_cli_set_privilege(struct crtl_cli_struct *cli, int priv) 
{
    int old = cli->privilege;
    cli->privilege = priv;

    if (priv != old) 
    {
        crtl_cli_set_promptchar(cli, priv == CLI_PRIVILEGE_PRIVILEGED ? "# " : "$ ");
        crtl_cli_build_shortest(cli, cli->commands);
    }

    return old;
}

void crtl_cli_set_modestring(struct crtl_cli_struct *cli, const char *modestring) 
{
    free_z(cli->modestring);
    if (modestring) 
        cli->modestring = strdup(modestring);
}

int crtl_cli_set_configmode(struct crtl_cli_struct *cli, int mode, const char *config_desc) 
{
    int old = cli->mode;
    cli->mode = mode;

    if (mode != old) 
    {
        if (!cli->mode) 
        {
            // Not config mode
            crtl_cli_set_modestring(cli, NULL);
        } 
        else if (config_desc && *config_desc) 
        {
            char string[64];
            snprintf(string, sizeof(string), "(config-%s)", config_desc);
            crtl_cli_set_modestring(cli, string);
        } 
        else 
        {
            crtl_cli_set_modestring(cli, "(config)");
        }

        crtl_cli_build_shortest(cli, cli->commands);
    }

    return old;
}

void crtl_cli_register_command_core(struct crtl_cli_struct *cli, struct crtl_cli_command *parent, struct crtl_cli_command *c) 
{
    struct crtl_cli_command *p = NULL;

    if (!c) return;

    c->parent = parent;

    /*
    * Figure out we have a chain, or would be the first element on it.
    * If we'd be the first element, assign as such.
    * Otherwise find the lead element so we can trace it below.
    */

    if (parent) {
        if (!parent->children) {
            parent->children = c;
        } else {
            p = parent->children;
        }
    } else {
        if (!cli->commands) {
            cli->commands = c;
        } else {
            p = cli->commands;
        }
    }

    /*
    * If we have a chain (p is not null), run down to the last element and place this command at the end
    */
    for (; p && p->next; p = p->next)
    ;

    if (p) {
        p->next = c;
        c->previous = p;
    }
    return;
}

struct crtl_cli_command *crtl_cli_register_command(struct crtl_cli_struct *cli, struct crtl_cli_command *parent, const char *command,
                                         int (*callback)(struct crtl_cli_struct *cli, const char *, char **, int),
                                         int privilege, int mode, const char *help) 
{
    struct crtl_cli_command *c;

    if (!command) return NULL;
    if (!(c = calloc(sizeof(struct crtl_cli_command), 1))) 
        return NULL;
    c->command_type = CLI_REGULAR_COMMAND;
    c->callback = callback;
    c->next = NULL;
    
    if (!(c->command = strdup(command))) 
    {
        free(c);
        return NULL;
    }

    c->privilege = privilege;
    c->mode = mode;
    if (help && !(c->help = strdup(help))) 
    {
        free(c->command);
        free(c);
        return NULL;
    }

    crtl_cli_register_command_core(cli, parent, c);
    return c;
}

static void crtl_cli_free_command(struct crtl_cli_struct *cli, struct crtl_cli_command *cmd) 
{
    struct crtl_cli_command *c, *p;

    for (c = cmd->children; c;) {
        p = c->next;
        crtl_cli_free_command(cli, c);
        c = p;
    }

    free(cmd->command);
    if (cmd->help) free(cmd->help);
    if (cmd->optargs) crtl_cli_unregister_all_optarg(cmd);

    /*
    * Ok, update the pointers of anyone who pointed to us.
    * We have 3 pointers to worry about - parent, previous, and next.
    * We don't have to worry about children since they've been cleared above.
    * If both cli->command points to us we need to update cli->command to point to whatever command is 'next'.
    * Otherwise ensure that any item before/behind us points around us.
    *
    * Important - there is no provision for deleting a discrete subcommand.
    * For example, suppose we define foo, then bar with foo as the parent, then baz with bar as the parent.  We cannot
    * delete 'bar' and have a new chain of foo -> baz.
    * The above freeing of children prevents this in the first place.
    */

    if (cmd == cli->commands) {
        cli->commands = cmd->next;
        if (cmd->next) {
            cmd->next->parent = NULL;
            cmd->next->previous = NULL;
        }
    } else {
        if (cmd->previous) {
            cmd->previous->next = cmd->next;
        }
        if (cmd->next) {
            cmd->next->previous = cmd->previous;
        }
    }
    free(cmd);
}

int crtl_cli_int_unregister_command_core(struct crtl_cli_struct *cli, const char *command, int command_type) 
{
    struct crtl_cli_command *c, *p = NULL;

    if (!command) return -1;
    if (!cli->commands) return CLI_OK;

    for (c = cli->commands; c;) {
        p = c->next;
        if (strcmp(c->command, command) == 0 && c->command_type == command_type) {
            crtl_cli_free_command(cli, c);
            return CLI_OK;
        }
        c = p;
    }

    return CLI_OK;
}

int crtl_cli_unregister_command(struct crtl_cli_struct *cli, const char *command) {
    return crtl_cli_int_unregister_command_core(cli, command, CLI_REGULAR_COMMAND);
}

int crtl_cli_show_help(struct crtl_cli_struct *cli, struct crtl_cli_command *c) 
{
    struct crtl_cli_command *p;

    for (p = c; p; p = p->next) 
    {
        if (p->command && 
            p->callback && 
            cli->privilege >= p->privilege && 
            (p->mode == cli->mode || p->mode == CLI_MODE_ANY)) 
        {
            crtl_cli_error(cli, "  %-20s %s", crtl_cli_command_name(cli, p), (p->help != NULL ? p->help : "No help line"));
        }

        if (p->children) 
            crtl_cli_show_help(cli, p->children);
    }

    return CLI_OK;
}

int crtl_cli_int_enable(struct crtl_cli_struct *cli, const char _unused *command, char _unused *argv[], int _unused argc) 
{
    if (cli->privilege == CLI_PRIVILEGE_PRIVILEGED) 
        return CLI_OK;

    if (!cli->enable_password && !cli->enable_callback) 
    {
        // No password required, set privilege immediately.
        crtl_cli_set_privilege(cli, CLI_PRIVILEGE_PRIVILEGED);
        crtl_cli_set_configmode(cli, CLI_MODE_EXEC, NULL);
    } 
    else 
    {
        // Require password entry
        crtl_print_notice("Require privileged commands.\n");
        crtl_cli_print(cli, "notice: %s", cli_enable_password);
        cli->state = STATE_ENABLE_PASSWORD;
    }

    return CLI_OK;
}

int crtl_cli_int_disable(struct crtl_cli_struct *cli, const char _unused *command, char _unused *argv[], int _unused argc) 
{
    crtl_cli_set_privilege(cli, CLI_PRIVILEGE_UNPRIVILEGED);
    crtl_cli_set_configmode(cli, CLI_MODE_EXEC, NULL);
    return CLI_OK;
}

int crtl_cli_int_help(struct crtl_cli_struct *cli, const char _unused *command, char _unused *argv[], int _unused argc) 
{
    crtl_cli_error(cli, "\nCommands available:");
    crtl_cli_show_help(cli, cli->commands);
    return CLI_OK;
}

int crtl_cli_int_history(struct crtl_cli_struct *cli, const char _unused *command, char _unused *argv[], int _unused argc) 
{
    int i;

    crtl_cli_error(cli, "\nCommand history:");
    for (i = 0; i < CLI_MAX_HISTORY; i++) 
    {
        if (cli->history[i]) 
            crtl_cli_error(cli, "%3d- %s", i, cli->history[i]);
    }

    return CLI_OK;
}

int crtl_cli_int_quit(struct crtl_cli_struct *cli, const char _unused *command, char _unused *argv[], int _unused argc) 
{
    crtl_cli_set_privilege(cli, CLI_PRIVILEGE_UNPRIVILEGED);
    crtl_cli_set_configmode(cli, CLI_MODE_EXEC, NULL);
    return CLI_QUIT;
}

int crtl_cli_int_exit(struct crtl_cli_struct *cli, const char *command, char *argv[], int argc) 
{
    if (cli->mode == CLI_MODE_EXEC) 
        return crtl_cli_int_quit(cli, command, argv, argc);

    if (cli->mode > CLI_MODE_CONFIG)
        crtl_cli_set_configmode(cli, CLI_MODE_CONFIG, NULL);
    else
        crtl_cli_set_configmode(cli, CLI_MODE_EXEC, NULL);

    cli->service = NULL;
    return CLI_OK;
}

int crtl_cli_int_idle_timeout(struct crtl_cli_struct *cli) 
{
    crtl_cli_print(cli, "Idle timeout");
    return CLI_QUIT;
}

int crtl_cli_int_configure_terminal(struct crtl_cli_struct *cli, const char _unused *command, char _unused *argv[], int _unused argc) 
{
    crtl_cli_set_configmode(cli, CLI_MODE_CONFIG, NULL);
    return CLI_OK;
}

/* init cli  */
struct crtl_cli_struct *crtl_cli_init(const char *banner, const char *hostname) 
{
    struct crtl_cli_struct *cli;
    struct crtl_cli_command *c;
    
    if (!(cli = calloc(sizeof(struct crtl_cli_struct), 1))) return 0;

    cli->buf_size = 1024;
    if (!(cli->buffer = calloc(cli->buf_size, 1))) 
    {
        free_z(cli);
        return 0;
    }
    cli->telnet_protocol = 1;

    crtl_cli_register_command(cli, 0, "ls", crtl_cli_int_help, CLI_PRIVILEGE_UNPRIVILEGED, CLI_MODE_ANY, "Show available commands");
    crtl_cli_register_command(cli, 0, "quit", crtl_cli_int_quit, CLI_PRIVILEGE_UNPRIVILEGED, CLI_MODE_ANY, "Disconnect");
    crtl_cli_register_command(cli, 0, "logout", crtl_cli_int_quit, CLI_PRIVILEGE_UNPRIVILEGED, CLI_MODE_ANY, "Disconnect");
    crtl_cli_register_command(cli, 0, "exit", crtl_cli_int_exit, CLI_PRIVILEGE_UNPRIVILEGED, CLI_MODE_ANY, "Exit from current mode");
    crtl_cli_register_command(cli, 0, "history", crtl_cli_int_history, CLI_PRIVILEGE_UNPRIVILEGED, CLI_MODE_ANY,"Show a list of previously run commands");
    crtl_cli_register_command(cli, 0, "enable", crtl_cli_int_enable, CLI_PRIVILEGE_UNPRIVILEGED, CLI_MODE_EXEC,"Turn on privileged commands");
    crtl_cli_allow_enable(cli, cli_enable_password);
    
    crtl_cli_register_command(cli, 0, "disable", crtl_cli_int_disable, CLI_PRIVILEGE_PRIVILEGED, CLI_MODE_EXEC,"Turn off privileged commands");

    c = crtl_cli_register_command(cli, 0, "configure", 0, CLI_PRIVILEGE_PRIVILEGED, CLI_MODE_EXEC, "Enter configuration mode");
    crtl_cli_register_command(cli, c, "terminal", crtl_cli_int_configure_terminal, CLI_PRIVILEGE_PRIVILEGED, CLI_MODE_EXEC,"Configure from the terminal");

    // And now the built in filters
    c = crtl_cli_register_filter(cli, "begin", crtl_cli_range_filter_init, crtl_cli_range_filter, CLI_PRIVILEGE_UNPRIVILEGED, CLI_MODE_ANY,"Begin with lines that match");
    crtl_cli_register_optarg(c, "range_start", CLI_CMD_ARGUMENT | CLI_CMD_REMAINDER_OF_LINE, CLI_PRIVILEGE_UNPRIVILEGED, CLI_MODE_ANY,"Begin showing lines that match", NULL, NULL, NULL);

    c = crtl_cli_register_filter(cli, "between", crtl_cli_range_filter_init, crtl_cli_range_filter, CLI_PRIVILEGE_UNPRIVILEGED, CLI_MODE_ANY, "Between lines that match");
    crtl_cli_register_optarg(c, "range_start", CLI_CMD_ARGUMENT, CLI_PRIVILEGE_UNPRIVILEGED, CLI_MODE_ANY,"Begin showing lines that match", NULL, NULL, NULL);
    crtl_cli_register_optarg(c, "range_end", CLI_CMD_ARGUMENT | CLI_CMD_REMAINDER_OF_LINE, CLI_PRIVILEGE_UNPRIVILEGED, CLI_MODE_ANY,"Stop showing lines that match", NULL, NULL, NULL);

    crtl_cli_register_filter(cli, "count", crtl_cli_count_filter_init, crtl_cli_count_filter, CLI_PRIVILEGE_UNPRIVILEGED, CLI_MODE_ANY,"Count of lines");

    c = crtl_cli_register_filter(cli, "exclude", crtl_cli_match_filter_init, crtl_cli_match_filter, CLI_PRIVILEGE_UNPRIVILEGED, CLI_MODE_ANY,"Exclude lines that match");
    crtl_cli_register_optarg(c, "search_pattern", CLI_CMD_ARGUMENT | CLI_CMD_REMAINDER_OF_LINE, CLI_PRIVILEGE_UNPRIVILEGED,CLI_MODE_ANY, "Search pattern", NULL, NULL, NULL);

    c = crtl_cli_register_filter(cli, "include", crtl_cli_match_filter_init, crtl_cli_match_filter, CLI_PRIVILEGE_UNPRIVILEGED, CLI_MODE_ANY,"Include lines that match");
    crtl_cli_register_optarg(c, "search_pattern", CLI_CMD_ARGUMENT | CLI_CMD_REMAINDER_OF_LINE, CLI_PRIVILEGE_UNPRIVILEGED,CLI_MODE_ANY, "Search pattern", NULL, NULL, NULL);

    c = crtl_cli_register_filter(cli, "grep", crtl_cli_match_filter_init, crtl_cli_match_filter, CLI_PRIVILEGE_UNPRIVILEGED, CLI_MODE_ANY,"Include lines that match regex (options: -v, -i, -e)");
    crtl_cli_register_optarg(c, "search_flags", CLI_CMD_HYPHENATED_OPTION, CLI_PRIVILEGE_UNPRIVILEGED, CLI_MODE_ANY,"Search flags (-[ivx]", NULL, crtl_cli_search_flags_validator, NULL);
    crtl_cli_register_optarg(c, "search_pattern", CLI_CMD_ARGUMENT | CLI_CMD_REMAINDER_OF_LINE, CLI_PRIVILEGE_UNPRIVILEGED,CLI_MODE_ANY, "Search pattern", NULL, NULL, NULL);
    
    c = crtl_cli_register_filter(cli, "egrep", crtl_cli_match_filter_init, crtl_cli_match_filter, CLI_PRIVILEGE_UNPRIVILEGED, CLI_MODE_ANY,"Include lines that match extended regex");
    crtl_cli_register_optarg(c, "search_flags", CLI_CMD_HYPHENATED_OPTION, CLI_PRIVILEGE_UNPRIVILEGED, CLI_MODE_ANY,"Search flags (-[ivx]", NULL, crtl_cli_search_flags_validator, NULL);
    crtl_cli_register_optarg(c, "search_pattern", CLI_CMD_ARGUMENT | CLI_CMD_REMAINDER_OF_LINE, CLI_PRIVILEGE_UNPRIVILEGED,CLI_MODE_ANY, "Search pattern", NULL, NULL, NULL);

    cli->privilege = cli->mode = -1;
    crtl_cli_set_privilege(cli, CLI_PRIVILEGE_UNPRIVILEGED);
    crtl_cli_set_configmode(cli, CLI_MODE_EXEC, 0);

    // Default to 1 second timeout intervals
    cli->timeout_tm.tv_sec = 1;
    cli->timeout_tm.tv_usec = 0;

    // Set default idle timeout callback, but no timeout
    crtl_cli_set_idle_timeout_callback(cli, 0, crtl_cli_int_idle_timeout);

    
    crtl_cli_set_banner(cli, banner);
    crtl_cli_set_hostname(cli, hostname);
    
    return cli;
}

void crtl_cli_unregister_tree(struct crtl_cli_struct *cli, struct crtl_cli_command *command, int command_type) 
{
    struct crtl_cli_command *c, *p = NULL;

    if (!command) command = cli->commands;

    for (c = command; c;) {
        p = c->next;
        if (c->command_type == command_type || command_type == CLI_ANY_COMMAND) {
            if (c == cli->commands) cli->commands = c->next;
            // Unregister all child commands
            crtl_cli_free_command(cli, c);
        }
        c = p;
    }
}

void crtl_cli_unregister_all(struct crtl_cli_struct *cli, struct crtl_cli_command *command) 
{
    crtl_cli_unregister_tree(cli, command, CLI_REGULAR_COMMAND);
}

int crtl_cli_done(struct crtl_cli_struct *cli) 
{
    if (!cli) return CLI_OK;
    struct unp *u = cli->users, *n;

    crtl_cli_free_history(cli);

    // Free all users
    while (u) {
        if (u->username) free(u->username);
        if (u->password) free(u->password);
        n = u->next;
        free(u);
        u = n;
    }

    if (cli->buildmode) crtl_cli_int_free_buildmode(cli);
    crtl_cli_unregister_tree(cli, cli->commands, CLI_ANY_COMMAND);
    free_z(cli->commandname);
    free_z(cli->modestring);
    free_z(cli->banner);
    free_z(cli->promptchar);
    free_z(cli->hostname);
    free_z(cli->buffer);
    free_z(cli);

    return CLI_OK;
}

static int crtl_cli_add_history(struct crtl_cli_struct *cli, const char *cmd) 
{
    int i;
    for (i = 0; i < CLI_MAX_HISTORY; i++) {
        if (!cli->history[i]) {
            if (i == 0 || strcasecmp(cli->history[i - 1], cmd))
                if (!(cli->history[i] = strdup(cmd))) 
                    return CLI_ERROR;
            return CLI_OK;
        }
    }
    // No space found, drop one off the beginning of the list
    free(cli->history[0]);

    for (i = 0; i < CLI_MAX_HISTORY - 1; i++) 
        cli->history[i] = cli->history[i + 1];
    
    if (!(cli->history[CLI_MAX_HISTORY - 1] = strdup(cmd))) 
        return CLI_ERROR;
    return CLI_OK;
}

void crtl_cli_free_history(struct crtl_cli_struct *cli) 
{
    int i;
    for (i = 0; i < CLI_MAX_HISTORY; i++) {
        if (cli->history[i]) 
            free_z(cli->history[i]);
    }
}

static int crtl_cli_parse_line(const char *line, char *words[], int max_words) 
{
    int nwords = 0;
    const char *p = line;
    const char *word_start = 0;
    int inquote = 0;

    while (*p) {
        if (!isspace(*p)) {
            word_start = p;
            break;
        }
        p++;
    }

    while (nwords < max_words - 1) {
        if (!*p || *p == inquote || (word_start && !inquote && (isspace(*p) || *p == '|'))) {
            if (word_start) {
                int len = p - word_start;

                memcpy(words[nwords] = malloc(len + 1), word_start, len);
                words[nwords++][len] = 0;
            }

            if (!*p) break;

            if (inquote) p++;  // Skip over trailing quote

            inquote = 0;
            word_start = 0;
        } else if (*p == '"' || *p == '\'') {
            inquote = *p++;
            word_start = p;
        } else {
            if (!word_start) {
                if (*p == '|') {
                    if (!(words[nwords++] = strdup("|"))) return 0;
                } else if (!isspace(*p))
                    word_start = p;
            }

            p++;
        }
    }

    return nwords;
}

static char *join_words(int argc, char **argv) 
{
    char *p;
    int len = 0;
    int i;

    for (i = 0; i < argc; i++) {
        if (i) len += 1;
        len += strlen(argv[i]);
    }

    p = malloc(len + 1);
    p[0] = 0;

    for (i = 0; i < argc; i++) {
        if (i) strcat(p, " ");
        strcat(p, argv[i]);
    }

    return p;
}

int crtl_cli_run_command(struct crtl_cli_struct *cli, const char *command) 
{
    int rc = CLI_ERROR;
    struct crtl_cli_pipeline *pipeline;

    // Split command into pipeline stages
    pipeline = crtl_cli_int_generate_pipeline(cli, command);

    // crtl_cli_int_validate_pipeline will deal with buildmode command setup, and return CLI_BUILDMODE_START if found.
    if (pipeline) 
        rc = crtl_cli_int_validate_pipeline(cli, pipeline);

    if (rc == CLI_OK) {
        rc = crtl_cli_int_execute_pipeline(cli, pipeline);
    }
    crtl_cli_int_free_pipeline(pipeline);
    return rc;
}

void crtl_cli_get_completions(struct crtl_cli_struct *cli, const char *command, char lastchar, struct crtl_cli_comphelp *comphelp) 
{
    struct crtl_cli_command *c = NULL;
    struct crtl_cli_command *n = NULL;

    int i;
    int command_type;
    struct crtl_cli_pipeline *pipeline = NULL;
    struct crtl_cli_pipeline_stage *stage;

    if (!(pipeline = crtl_cli_int_generate_pipeline(cli, command))) 
        goto out;

    stage = &pipeline->stage[pipeline->num_stages - 1];

    // Check to see if either *no* input, or if the lastchar is a tab.
    if ((!stage->words[0] || (command[strlen(command) - 1] == ' ')) && (stage->words[stage->num_words - 1]))
        stage->num_words++;

    if (cli->buildmode)
        command_type = CLI_BUILDMODE_COMMAND;
    else if (pipeline->num_stages == 1)
        command_type = CLI_REGULAR_COMMAND;
    else
        command_type = CLI_FILTER_COMMAND;

    for (c = cli->commands, i = 0; c && i < stage->num_words; c = n) {
        char *strptr = NULL;
        n = c->next;

        if (c->command_type != command_type) continue;
        if (cli->privilege < c->privilege) continue;
        if (c->mode != cli->mode && c->mode != CLI_MODE_ANY) continue;
        if (stage->words[i] && strncasecmp(c->command, stage->words[i], strlen(stage->words[i]))) continue;

        // Special case for 'buildmode' - skip if the argument for this command was seen, unless MULTIPLE flag is set
        if (cli->buildmode) {
            struct crtl_cli_optarg *optarg;
            for (optarg = cli->buildmode->command->optargs; optarg; optarg = optarg->next) {
                if (!strcmp(optarg->name, c->command)) break;
            }
            if (optarg && crtl_cli_find_optarg_value(cli, optarg->name, NULL) && !(optarg->flags & (CLI_CMD_OPTION_MULTIPLE)))
                continue;
        }
        if (i < stage->num_words - 1) {
            if (stage->words[i] && (strlen(stage->words[i]) < c->unique_len) && strcmp(stage->words[i], c->command)) continue;

            n = c->children;

            // If we have no more children, we've matched the *command* - remember this
            if (!c->children) break;

            i++;
            continue;
        }

        if (lastchar == '?') {
            if (asprintf(&strptr, "  %-20s %s", c->command, (c->help) ? c->help : "") != -1) {
                crtl_cli_add_comphelp_entry(comphelp, strptr);
                free_z(strptr);
            }
        } else {
            crtl_cli_add_comphelp_entry(comphelp, c->command);
        }
    }

out:
    if (c) {
        // Advance past first word of stage
        i++;
        stage->first_unmatched = i;
        if (c->optargs) {
            crtl_cli_int_parse_optargs(cli, stage, c, lastchar, comphelp);
        } else if (lastchar == '?') {
            // Special case for getting help with no defined optargs....
            comphelp->num_entries = -1;
        }
    }

    crtl_cli_int_free_pipeline(pipeline);
}

static void crtl_cli_clear_line(int sockfd, char *cmd, int l, int cursor) 
{
    // Use cmd as our buffer, and overwrite contents as needed.
    // Backspace to beginning
    memset((char *)cmd, '\b', cursor);
    _write(sockfd, cmd, cursor);

    // Overwrite existing cmd with spaces
    memset((char *)cmd, ' ', l);
    _write(sockfd, cmd, l);

    // ..and backspace again to beginning
    memset((char *)cmd, '\b', l);
    _write(sockfd, cmd, l);

    // Null cmd buffer
    memset((char *)cmd, 0, l);
}

/* 提示 */
void crtl_cli_reprompt(struct crtl_cli_struct *cli) 
{
    if (!cli) return;
    cli->showprompt = 1;
}

void crtl_cli_regular(struct crtl_cli_struct *cli, int (*callback)(struct crtl_cli_struct *cli)) 
{
    if (!cli) return;
    cli->regular_callback = callback;
}

void crtl_cli_regular_interval(struct crtl_cli_struct *cli, int seconds) 
{
    if (seconds < 1) seconds = 1;
    cli->timeout_tm.tv_sec = seconds;
    cli->timeout_tm.tv_usec = 0;
}

#define DES_PREFIX "{crypt}"  // To distinguish clear text from DES crypted区分明文与DES加密
#define MD5_PREFIX "$1$"

static int pass_matches(const char *pass, const char *attempt) 
{
    int des;
    if ((des = !strncasecmp(pass, DES_PREFIX, sizeof(DES_PREFIX) - 1))) pass += sizeof(DES_PREFIX) - 1;

    // TODO(dparrish): Find a small crypt(3) function for use on windows
    if (des || !strncmp(pass, MD5_PREFIX, sizeof(MD5_PREFIX) - 1)) attempt = crypt(attempt, pass);

    return !strcmp(pass, attempt);
}

#define CTRL(c) (c - '@')

static int show_prompt(struct crtl_cli_struct *cli, int sockfd) {
  int len = 0;

  if (cli->hostname) len += write(sockfd, cli->hostname, strlen(cli->hostname));

  if (cli->modestring) len += write(sockfd, cli->modestring, strlen(cli->modestring));
  if (cli->buildmode) {
    len += write(sockfd, "[", 1);
    len += write(sockfd, cli->buildmode->cname, strlen(cli->buildmode->cname));
    len += write(sockfd, "...", 3);
    if (cli->buildmode->mode_text) len += write(sockfd, cli->buildmode->mode_text, strlen(cli->buildmode->mode_text));
    len += write(sockfd, "]", 1);
  }
  return len + write(sockfd, cli->promptchar, strlen(cli->promptchar));
}

int crtl_cli_loop(struct crtl_cli_struct *cli, int sockfd) 
{
  int n, l, oldl = 0, is_telnet_option = 0, skip = 0, esc = 0, cursor = 0;
  char *cmd = NULL, *oldcmd = 0;
  char *username = NULL, *password = NULL;

  crtl_cli_build_shortest(cli, cli->commands);
  cli->state = STATE_LOGIN;

  crtl_cli_free_history(cli);
  if (cli->telnet_protocol) {
    static const char *negotiate =
        "\xFF\xFB\x03"
        "\xFF\xFB\x01"
        "\xFF\xFD\x03"
        "\xFF\xFD\x01";
    _write(sockfd, negotiate, strlen(negotiate));
  }

  if ((cmd = malloc(CLI_MAX_LINE_LENGTH)) == NULL) return CLI_ERROR;

  if (!(cli->client = fdopen(sockfd, "w+"))) {
    free(cmd);
    return CLI_ERROR;
  }

  setbuf(cli->client, NULL);
  if (cli->banner) crtl_cli_error(cli, "%s", cli->banner);

  // Set the last action now so we don't time immediately
  if (cli->idle_timeout) time(&cli->last_action);

  // Start off in unprivileged mode
  crtl_cli_set_privilege(cli, CLI_PRIVILEGE_UNPRIVILEGED);
  crtl_cli_set_configmode(cli, CLI_MODE_EXEC, NULL);

  // No auth required?
  if (!cli->users && !cli->auth_callback) cli->state = STATE_NORMAL;

  while (1) 
  {
    signed int in_history = 0;
    unsigned char lastchar = '\0';
    unsigned char c = '\0';
    struct timeval tm;

    cli->showprompt = 1;

    if (oldcmd) {
      l = cursor = oldl;
      oldcmd[l] = 0;
      cli->showprompt = 1;
      oldcmd = NULL;
      oldl = 0;
    } else {
      memset(cmd, 0, CLI_MAX_LINE_LENGTH);
      l = 0;
      cursor = 0;
    }

    memcpy(&tm, &cli->timeout_tm, sizeof(tm));

    while (1) 
    {
      int sr;
      fd_set r;

      /*
       * Ensure our transient mode is reset to the starting mode on *each* loop traversal transient mode is valid only
       * while a command is being evaluated/executed.
       */
      cli->transient_mode = cli->mode;

      if (cli->showprompt) {
        if (cli->state != STATE_PASSWORD && cli->state != STATE_ENABLE_PASSWORD) 
            _write(sockfd, "\r\n", 2);

        switch (cli->state) {
          case STATE_LOGIN:
            _write(sockfd, "Username: ", strlen("Username: "));
            break;

          case STATE_PASSWORD:
            _write(sockfd, "Password: ", strlen("Password: "));
            break;

          case STATE_NORMAL:
          case STATE_ENABLE:
            show_prompt(cli, sockfd);
            _write(sockfd, cmd, l);
            if (cursor < l) {
              int n = l - cursor;
              while (n--) _write(sockfd, "\b", 1);
            }
            break;

          case STATE_ENABLE_PASSWORD:
            _write(sockfd, "Password: ", strlen("Password: "));
            break;
        }

        cli->showprompt = 0;
      }

      FD_ZERO(&r);
      FD_SET(sockfd, &r);

      if ((sr = select(sockfd + 1, &r, NULL, NULL, &tm)) < 0) {
        if (errno == EINTR) continue;
        perror("select");
        l = -1;
        break;
      }

      if (sr == 0) {
        // Timeout every second
        if (cli->regular_callback && cli->regular_callback(cli) != CLI_OK) 
        {
          l = -1;
          break;
        }

        if (cli->idle_timeout) {
          if (time(NULL) - cli->last_action >= cli->idle_timeout) {
            if (cli->idle_timeout_callback) {
              // Call the callback and continue on if successful
              if (cli->idle_timeout_callback(cli) == CLI_OK) {
                // Reset the idle timeout counter
                time(&cli->last_action);
                continue;
              }
            }
            // Otherwise, break out of the main loop
            l = -1;
            break;
          }
        }

        memcpy(&tm, &cli->timeout_tm, sizeof(tm));
        continue;
      }

      if ((n = read(sockfd, &c, 1)) < 0) {
        if (errno == EINTR) continue;

        perror("read");
        l = -1;
        break;
      }

      if (cli->idle_timeout) time(&cli->last_action);

      if (n == 0) {
        l = -1;
        break;
      }

      if (skip) {
        skip--;
        continue;
      }

      if (c == 255 && !is_telnet_option) {
        is_telnet_option++;
        continue;
      }

      if (is_telnet_option) {
        if (c >= 251 && c <= 254) {
          is_telnet_option = c;
          continue;
        }

        if (c != 255) {
          is_telnet_option = 0;
          continue;
        }

        is_telnet_option = 0;
      }

      // Handle ANSI arrows
      if (esc) {
        if (esc == '[') {
          // Remap to readline control codes
          switch (c) {
            case 'A':  // Up
              c = CTRL('P');
              break;

            case 'B':  // Down
              c = CTRL('N');
              break;

            case 'C':  // Right
              c = CTRL('F');
              break;

            case 'D':  // Left
              c = CTRL('B');
              break;

            default:
              c = 0;
          }

          esc = 0;
        } else {
          esc = (c == '[') ? c : 0;
          continue;
        }
      }

      if (c == 0) continue;
      if (c == '\n') continue;

      if (c == '\r') {
        if (cli->state != STATE_PASSWORD && cli->state != STATE_ENABLE_PASSWORD) 
            _write(sockfd, "\r\n", 2);
        break;
      }

      if (c == 27) {
        esc = 1;
        continue;
      }

      if (c == CTRL('C')) {
        _write(sockfd, "\a", 1);
        continue;
      }

      // Back word, backspace/delete
      if (c == CTRL('W') || c == CTRL('H') || c == 0x7f) {
        int back = 0;

        if (c == CTRL('W')) {
          // Word
          int nc = cursor;

          if (l == 0 || cursor == 0) continue;

          while (nc && cmd[nc - 1] == ' ') {
            nc--;
            back++;
          }

          while (nc && cmd[nc - 1] != ' ') {
            nc--;
            back++;
          }
        } else {
          // Char
          if (l == 0 || cursor == 0) {
            _write(sockfd, "\a", 1);
            continue;
          }

          back = 1;
        }

        if (back) {
          while (back--) {
            if (l == cursor) {
              cmd[--cursor] = 0;
              if (cli->state != STATE_PASSWORD && cli->state != STATE_ENABLE_PASSWORD) 
                _write(sockfd, "\b \b", 3);
            } else {
              int i;
              if (cli->state != STATE_PASSWORD && cli->state != STATE_ENABLE_PASSWORD) {
                // Back up one space, then write current buffer followed by a space
                _write(sockfd, "\b", 1);
                _write(sockfd, cmd + cursor, l - cursor);
                _write(sockfd, " ", 1);

                // Move everything one char left
                memmove(cmd + cursor - 1, cmd + cursor, l - cursor);

                // Set former last char to null
                cmd[l - 1] = 0;

                // And reposition cursor
                for (i = l; i >= cursor; i--) _write(sockfd, "\b", 1);
              }
              cursor--;
            }
            l--;
          }

          continue;
        }
      }

      // Redraw
      if (c == CTRL('L')) {
        int i;
        int cursorback = l - cursor;

        if (cli->state == STATE_PASSWORD || cli->state == STATE_ENABLE_PASSWORD) 
            continue;

        _write(sockfd, "\r\n", 2);
        show_prompt(cli, sockfd);
        _write(sockfd, cmd, l);

        for (i = 0; i < cursorback; i++) 
            _write(sockfd, "\b", 1);

        continue;
      }

      // Clear line
      if (c == CTRL('U')) {
        if (cli->state == STATE_PASSWORD || cli->state == STATE_ENABLE_PASSWORD)
          memset(cmd, 0, l);
        else
          crtl_cli_clear_line(sockfd, cmd, l, cursor);

        l = cursor = 0;
        continue;
      }

      // Kill to EOL
      if (c == CTRL('K')) {
        if (cursor == l) continue;

        if (cli->state != STATE_PASSWORD && cli->state != STATE_ENABLE_PASSWORD) {
          int cptr;
          for (cptr = cursor; cptr < l; cptr++) _write(sockfd, " ", 1);

          for (cptr = cursor; cptr < l; cptr++) _write(sockfd, "\b", 1);
        }

        memset(cmd + cursor, 0, l - cursor);
        l = cursor;
        continue;
      }

      // EOT
      if (c == CTRL('D')) {
        if (cli->state == STATE_PASSWORD || cli->state == STATE_ENABLE_PASSWORD) 
            break;

        if (l) continue;

        l = -1;
        break;
      }

      // Disable
      if (c == CTRL('Z')) {
        if (cli->mode != CLI_MODE_EXEC) {
          if (cli->buildmode) crtl_cli_int_free_buildmode(cli);
          crtl_cli_clear_line(sockfd, cmd, l, cursor);
          crtl_cli_set_configmode(cli, CLI_MODE_EXEC, NULL);
          l = cursor = 0;
          cli->showprompt = 1;
        }

        continue;
      }

      // TAB completion
      if (c == CTRL('I')) {
        struct crtl_cli_comphelp comphelp = {0};

        if (cli->state == STATE_LOGIN || cli->state == STATE_PASSWORD || cli->state == STATE_ENABLE_PASSWORD) 
            continue;
        if (cursor != l) continue;

        crtl_cli_get_completions(cli, cmd, c, &comphelp);
        if (comphelp.num_entries == 0) {
          _write(sockfd, "\a", 1);
        } else if (lastchar == CTRL('I')) {
          // Double tab
          int i;
          for (i = 0; i < comphelp.num_entries; i++) {
            if (i % 4 == 0)
              _write(sockfd, "\r\n", 2);
            else
              _write(sockfd, " ", 1);
            _write(sockfd, comphelp.entries[i], strlen(comphelp.entries[i]));
          }
          _write(sockfd, "\r\n", 2);
          cli->showprompt = 1;
        } else if (comphelp.num_entries == 1) {
          // Single completion - show *unless* the optional/required 'prefix' is present
          if (comphelp.entries[0][0] != '[' && comphelp.entries[0][0] != '<') {
            for (; l > 0; l--, cursor--) {
              if (cmd[l - 1] == ' ' || cmd[l - 1] == '|' || (comphelp.comma_separated && cmd[l - 1] == ',')) break;
              _write(sockfd, "\b", 1);
            }
            strcpy((cmd + l), comphelp.entries[0]);
            l += strlen(comphelp.entries[0]);
            cmd[l++] = ' ';
            cursor = l;
            _write(sockfd, comphelp.entries[0], strlen(comphelp.entries[0]));
            _write(sockfd, " ", 1);
            // And now forget the tab, since we just found a single match
            lastchar = '\0';
          } else {
            // Yes, we had a match, but it wasn't required - remember the tab in case the user double tabs....
            lastchar = CTRL('I');
          }
        } else if (comphelp.num_entries > 1) {
          /*
           * More than one completion.
           * Show as many characters as we can until the completions start to differ.
           */
          lastchar = c;
          int i, j, k = 0;
          char *tptr = comphelp.entries[0];

          /*
           * Quickly try to see where our entries differ.
           * Corner cases:
           * - If all entries are optional, don't show *any* options unless user has provided a letter.
           * - If any entry starts with '<' then don't fill in anything.
           */

          // Skip a leading '['
          k = strlen(tptr);
          if (*tptr == '[')
            tptr++;
          else if (*tptr == '<')
            k = 0;

          for (i = 1; k != 0 && i < comphelp.num_entries; i++) {
            char *wptr = comphelp.entries[i];

            if (*wptr == '[')
              wptr++;
            else if (*wptr == '<')
              k = 0;

            for (j = 0; (j < k) && (j < (int)strlen(wptr)); j++) {
              if (strncasecmp(tptr + j, wptr + j, 1)) break;
            }
            k = j;
          }

          // Try to show minimum match string if we have a non-zero k and the first letter of the last word is not '['.
          if (k && comphelp.entries[comphelp.num_entries - 1][0] != '[') {
            for (; l > 0; l--, cursor--) {
              if (cmd[l - 1] == ' ' || cmd[l - 1] == '|' || (comphelp.comma_separated && cmd[l - 1] == ',')) break;
              _write(sockfd, "\b", 1);
            }
            strncpy(cmd + l, tptr, k);
            l += k;
            cursor = l;
            _write(sockfd, tptr, k);

          } else {
            _write(sockfd, "\a", 1);
          }
        }
        crtl_cli_free_comphelp(&comphelp);
        continue;
      }

      // '?' at end of line - generate applicable 'help' messages
      if (c == '?' && cursor == l) {
        struct crtl_cli_comphelp comphelp = {0};
        int i;
        int show_cr = 1;

        if (cli->state == STATE_LOGIN || cli->state == STATE_PASSWORD || cli->state == STATE_ENABLE_PASSWORD) 
            continue;
        if (cursor != l) 
            continue;

        crtl_cli_get_completions(cli, cmd, c, &comphelp);
        if (comphelp.num_entries == 0) {
          _write(sockfd, "\a", 1);
        } else if (comphelp.num_entries > 0) {
          cli->showprompt = 1;
          _write(sockfd, "\r\n", 2);
          for (i = 0; i < (int)comphelp.num_entries; i++) {
            if (comphelp.entries[i][2] != '[') show_cr = 0;
            crtl_cli_error(cli, "%s", comphelp.entries[i]);
          }
          if (show_cr) crtl_cli_error(cli, "  <cr>");
        }

        crtl_cli_free_comphelp(&comphelp);

        if (comphelp.num_entries >= 0) continue;
      }

      // History
      if (c == CTRL('P') || c == CTRL('N')) {
        int history_found = 0;

        if (cli->state == STATE_LOGIN || cli->state == STATE_PASSWORD || cli->state == STATE_ENABLE_PASSWORD) 
            continue;

        if (c == CTRL('P')) {
          // Up
          in_history--;
          if (in_history < 0) {
            for (in_history = CLI_MAX_HISTORY - 1; in_history >= 0; in_history--) {
              if (cli->history[in_history]) {
                history_found = 1;
                break;
              }
            }
          } else {
            if (cli->history[in_history]) history_found = 1;
          }
        } else {
          // Down
          in_history++;
          if (in_history >= CLI_MAX_HISTORY || !cli->history[in_history]) {
            int i = 0;
            for (i = 0; i < CLI_MAX_HISTORY; i++) {
              if (cli->history[i]) {
                in_history = i;
                history_found = 1;
                break;
              }
            }
          } else {
            if (cli->history[in_history]) history_found = 1;
          }
        }
        if (history_found && cli->history[in_history]) {
          // Show history item
          crtl_cli_clear_line(sockfd, cmd, l, cursor);
          memset(cmd, 0, CLI_MAX_LINE_LENGTH);
          strncpy(cmd, cli->history[in_history], CLI_MAX_LINE_LENGTH - 1);
          l = cursor = strlen(cmd);
          _write(sockfd, cmd, l);
        }

        continue;
      }

      // Left/right cursor motion
      if (c == CTRL('B') || c == CTRL('F')) {
        if (c == CTRL('B')) {
          // Left
          if (cursor) {
            if (cli->state != STATE_PASSWORD && cli->state != STATE_ENABLE_PASSWORD) 
                _write(sockfd, "\b", 1);

            cursor--;
          }
        } else {
          // Right
          if (cursor < l) {
            if (cli->state != STATE_PASSWORD && cli->state != STATE_ENABLE_PASSWORD) 
                _write(sockfd, &cmd[cursor], 1);

            cursor++;
          }
        }

        continue;
      }

      if (c == CTRL('A')) {
        // Start of line
        if (cursor) {
          if (cli->state != STATE_PASSWORD && cli->state != STATE_ENABLE_PASSWORD) {
            _write(sockfd, "\r", 1);
            show_prompt(cli, sockfd);
          }

          cursor = 0;
        }

        continue;
      }

      if (c == CTRL('E')) {
        // End of line
        if (cursor < l) {
          if (cli->state != STATE_PASSWORD && cli->state != STATE_ENABLE_PASSWORD)
            _write(sockfd, &cmd[cursor], l - cursor);

          cursor = l;
        }

        continue;
      }

      if (cursor == l) {
        // Normal character typed.
        // Append to end of line if not at end-of-buffer.
        if (l < CLI_MAX_LINE_LENGTH - 1) {
          cmd[cursor] = c;
          l++;
          cursor++;
        } else {
          // End-of-buffer, ensure null terminated
          cmd[cursor] = 0;
          _write(sockfd, "\a", 1);
          continue;
        }
      } else {
        // Middle of text
        int i;
        // Move everything one character to the right
        memmove(cmd + cursor + 1, cmd + cursor, l - cursor);

        // Insert new character
        cmd[cursor] = c;

        // IMPORTANT - if at end of buffer, set last char to NULL and don't change length, otherwise bump length by 1
        if (l == CLI_MAX_LINE_LENGTH - 1) {
          cmd[l] = 0;
        } else {
          l++;
        }

        // Write buffer, then backspace to where we were
        _write(sockfd, cmd + cursor, l - cursor);

        for (i = 0; i < (l - cursor); i++) _write(sockfd, "\b", 1);
        cursor++;
      }

      if (cli->state != STATE_PASSWORD && cli->state != STATE_ENABLE_PASSWORD) {
        if (c == '?' && cursor == l) {
          _write(sockfd, "\r\n", 2);
          oldcmd = cmd;
          oldl = cursor = l - 1;
          break;
        }
        _write(sockfd, &c, 1);
      }

      oldcmd = 0;
      oldl = 0;
      lastchar = c;
    }

    if (l < 0) break;

    if (cli->state == STATE_LOGIN) {
      if (l == 0) continue;

      // Require login
      free_z(username);
      if (!(username = strdup(cmd))) return 0;
      cli->state = STATE_PASSWORD;
      cli->showprompt = 1;
    } else if (cli->state == STATE_PASSWORD) {
      // Require password
      int allowed = 0;

      free_z(password);
      if (!(password = strdup(cmd))) return 0;
      if (cli->auth_callback) {
        if (cli->auth_callback(username, password) == CLI_OK) allowed++;
      }

      if (!allowed) {
        struct unp *u;
        for (u = cli->users; u; u = u->next) {
          if (!strcmp(u->username, username) && pass_matches(u->password, password)) {
            allowed++;
            break;
          }
        }
      }

      if (allowed) {
        crtl_cli_error(cli, " ");
        cli->state = STATE_NORMAL;
      } else {
        crtl_cli_error(cli, "\n\nAccess denied");
        free_z(username);
        free_z(password);
        cli->state = STATE_LOGIN;
      }

      cli->showprompt = 1;
    } else if (cli->state == STATE_ENABLE_PASSWORD) {
      int allowed = 0;
      if (cli->enable_password) {
        // Check stored static enable password
        if (pass_matches(cli->enable_password, cmd)) allowed++;
      }

      if (!allowed && cli->enable_callback) {
        // Check callback
        if (cli->enable_callback(cmd)) allowed++;
      }

      if (allowed) {
        cli->state = STATE_ENABLE;
        crtl_cli_set_privilege(cli, CLI_PRIVILEGE_PRIVILEGED);
      } else {
        crtl_cli_error(cli, "\n\nAccess denied");
        cli->state = STATE_NORMAL;
      }
    } else {
      int rc;
      if (l == 0) continue;
      if (cmd[l - 1] != '?' && strcasecmp(cmd, "history") != 0) crtl_cli_add_history(cli, cmd);

      rc = crtl_cli_run_command(cli, cmd);
      switch (rc) {
        case CLI_BUILDMODE_ERROR:
          // Unable to enter buildmode successfully
          crtl_cli_print(cli, "Failure entering build mode for '%s'", cli->buildmode->cname);
          crtl_cli_int_free_buildmode(cli);
          continue;
        case CLI_BUILDMODE_CANCEL:
          // Called if user enters 'cancel'
          crtl_cli_print(cli, "Canceling build mode for '%s'", cli->buildmode->cname);
          crtl_cli_int_free_buildmode(cli);
          break;
        case CLI_BUILDMODE_EXIT:
          // Called when user enters exit - rebuild *entire* command line.
          // Recall all located optargs
          cli->found_optargs = cli->buildmode->found_optargs;
          rc = crtl_cli_int_execute_buildmode(cli);
        case CLI_QUIT:
          break;
        case CLI_BUILDMODE_START:
        case CLI_BUILDMODE_EXTEND:
        default:
          break;
      }

      // Process is done if we get a CLI_QUIT,
      if (rc == CLI_QUIT) break;
    }

    // Update the last_action time now as the last command run could take a long time to return
    if (cli->idle_timeout) time(&cli->last_action);
  }

  crtl_cli_free_history(cli);
  free_z(username);
  free_z(password);
  free_z(cmd);

  fclose(cli->client);
  cli->client = 0;
  return CLI_OK;
}

int crtl_cli_file(struct crtl_cli_struct *cli, FILE *fh, int privilege, int mode) {
  int oldpriv = crtl_cli_set_privilege(cli, privilege);
  int oldmode = crtl_cli_set_configmode(cli, mode, NULL);
  char buf[CLI_MAX_LINE_LENGTH];

  while (1) {
    char *p;
    char *cmd;
    char *end;

    // End of file
    if (fgets(buf, CLI_MAX_LINE_LENGTH - 1, fh) == NULL) break;

    if ((p = strpbrk(buf, "#\r\n"))) *p = 0;

    cmd = buf;
    while (isspace(*cmd)) cmd++;

    if (!*cmd) continue;

    for (p = end = cmd; *p; p++)
      if (!isspace(*p)) end = p;

    *++end = 0;
    if (strcasecmp(cmd, "quit") == 0) break;

    if (crtl_cli_run_command(cli, cmd) == CLI_QUIT) break;
  }

  crtl_cli_set_privilege(cli, oldpriv);
  crtl_cli_set_configmode(cli, oldmode, NULL);

  return CLI_OK;
}

static void _print(struct crtl_cli_struct *cli, int print_mode, const char *format, va_list ap) {
  int n;
  char *p = NULL;

  if (!cli) return;

  n = vasprintf(&p, format, ap);
  if (n < 0) return;
  if (cli->buffer) free(cli->buffer);
  cli->buffer = p;
  cli->buf_size = n;

  p = cli->buffer;
  do {
    char *next = strchr(p, '\n');
    struct crtl_cli_filter *f = (print_mode & CLI_PRINT_FILTERED) ? cli->filters : 0;
    int print = 1;

    if (next)
      *next++ = 0;
    else if (print_mode & CLI_PRINT_BUFFERED)
      break;

    while (print && f) {
      print = (f->filter(cli, p, f->data) == CLI_OK);
      f = f->next;
    }
    if (print) {
      if (cli->print_callback)
        cli->print_callback(cli, p);
      else if (cli->client)
        fprintf(cli->client, "%s\r\n", p);
    }

    p = next;
  } while (p);

  if (p && *p) {
    if (p != cli->buffer) memmove(cli->buffer, p, strlen(p));
  } else
    *cli->buffer = 0;
}

void crtl_cli_bufprint(struct crtl_cli_struct *cli, const char *format, ...) {
  va_list ap;

  va_start(ap, format);
  _print(cli, CLI_PRINT_BUFFERED | CLI_PRINT_FILTERED, format, ap);
  va_end(ap);
}

void crtl_cli_vabufprint(struct crtl_cli_struct *cli, const char *format, va_list ap) {
  _print(cli, CLI_PRINT_BUFFERED, format, ap);
}

void crtl_cli_print(struct crtl_cli_struct *cli, const char *format, ...) {
  va_list ap;

  va_start(ap, format);
  _print(cli, CLI_PRINT_FILTERED, format, ap);
  va_end(ap);
}

void crtl_cli_error(struct crtl_cli_struct *cli, const char *format, ...) {
  va_list ap;

  va_start(ap, format);
  _print(cli, CLI_PRINT_PLAIN, format, ap);
  va_end(ap);
}

struct crtl_cli_match_filter_state {
  int flags;
  union {
    char *string;
    regex_t re;
  } match;
};

int crtl_cli_search_flags_validator(struct crtl_cli_struct *cli, const char *word, const char *value) {
  // Valid search flags starts with a hyphen, then any number of i, v, or e characters.

  if ((*value++ == '-') && (*value) && (strspn(value, "vie") == strlen(value))) return CLI_OK;
  return CLI_ERROR;
}

int crtl_cli_match_filter_init(struct crtl_cli_struct *cli, int argc, char **argv, struct crtl_cli_filter *filt) {
  struct crtl_cli_match_filter_state *state;
  char *search_pattern = crtl_cli_get_optarg_value(cli, "search_pattern", NULL);
  char *search_flags = crtl_cli_get_optarg_value(cli, "search_flags", NULL);

  filt->filter = crtl_cli_match_filter;
  filt->data = state = calloc(sizeof(struct crtl_cli_match_filter_state), 1);
  if (!state) return CLI_ERROR;

  if (!strcmp(cli->pipeline->current_stage->words[0], "include")) {
    state->match.string = strdup(search_pattern);
  } else if (!strcmp(cli->pipeline->current_stage->words[0], "exclude")) {
    state->match.string = strdup(search_pattern);
    state->flags = MATCH_INVERT;
  } else {
    int rflags = REG_NOSUB;
    if (!strcmp(cli->pipeline->current_stage->words[0], "grep")) {
      state->flags = MATCH_REGEX;
    } else if (!strcmp(cli->pipeline->current_stage->words[0], "egrep")) {
      state->flags = MATCH_REGEX;
      rflags |= REG_EXTENDED;
    }
    if (search_flags) {
      char *p = search_flags++;
      while (*p) {
        switch (*p++) {
          case 'v':
            state->flags |= MATCH_INVERT;
            break;

          case 'i':
            rflags |= REG_ICASE;
            break;

          case 'e':
            // Implies next term is search string, so stop processing flags
            break;
        }
      }
    }
    if (regcomp(&state->match.re, search_pattern, rflags)) {
      if (cli->client) fprintf(cli->client, "Invalid pattern \"%s\"\r\n", search_pattern);
      return CLI_ERROR;
    }
  }

  return CLI_OK;
}

int crtl_cli_match_filter(struct crtl_cli_struct _unused *cli, const char *string, void *data) 
{
  struct crtl_cli_match_filter_state *state = data;
  int r = CLI_ERROR;

  if (!string) {
    if (state->flags & MATCH_REGEX)
      regfree(&state->match.re);
    else
      free(state->match.string);

    free(state);
    return CLI_OK;
  }

  if (state->flags & MATCH_REGEX) {
    if (!regexec(&state->match.re, string, 0, NULL, 0)) r = CLI_OK;
  } else {
    if (strstr(string, state->match.string)) r = CLI_OK;
  }

  if (state->flags & MATCH_INVERT) {
    if (r == CLI_OK)
      r = CLI_ERROR;
    else
      r = CLI_OK;
  }

  return r;
}

struct crtl_cli_range_filter_state {
    int matched;
    char *from;
    char *to;
};

int crtl_cli_range_filter_init(struct crtl_cli_struct *cli, int argc, char **argv, struct crtl_cli_filter *filt) {
  struct crtl_cli_range_filter_state *state;
  char *from = strdup(crtl_cli_get_optarg_value(cli, "range_start", NULL));
  char *to = strdup(crtl_cli_get_optarg_value(cli, "range_end", NULL));

  // Do not have to check from/to since we would not have gotten here if we were missing a required argument.
  filt->filter = crtl_cli_range_filter;
  filt->data = state = calloc(sizeof(struct crtl_cli_range_filter_state), 1);
  if (state) {
    state->from = from;
    state->to = to;
    return CLI_OK;
  } else {
    free_z(from);
    free_z(to);
    return CLI_ERROR;
  }
}

int crtl_cli_range_filter(struct crtl_cli_struct _unused *cli, const char *string, void *data) 
{
  struct crtl_cli_range_filter_state *state = data;
  int r = CLI_ERROR;

  if (!string) {
    free_z(state->from);
    free_z(state->to);
    free_z(state);
    return CLI_OK;
  }

  if (!state->matched) state->matched = !!strstr(string, state->from);

  if (state->matched) {
    r = CLI_OK;
    if (state->to && strstr(string, state->to)) state->matched = 0;
  }

  return r;
}

int crtl_cli_count_filter_init(struct crtl_cli_struct *cli, int argc, char _unused **argv, struct crtl_cli_filter *filt) 
{
  if (argc > 1) {
    if (cli->client) fprintf(cli->client, "Count filter does not take arguments\r\n");

    return CLI_ERROR;
  }

  filt->filter = crtl_cli_count_filter;
  if (!(filt->data = calloc(sizeof(int), 1))) return CLI_ERROR;

  return CLI_OK;
}

int crtl_cli_count_filter(struct crtl_cli_struct *cli, const char *string, void *data) {
  int *count = data;

  if (!string) {
    // Print count
    if (cli->client) fprintf(cli->client, "%d\r\n", *count);

    free(count);
    return CLI_OK;
  }

  while (isspace(*string)) string++;

  // Only count non-blank lines
  if (*string) (*count)++;

  return CLI_ERROR;
}

void crtl_cli_print_callback(struct crtl_cli_struct *cli, void (*callback)(struct crtl_cli_struct *, const char *)) {
  cli->print_callback = callback;
}

void crtl_cli_set_idle_timeout(struct crtl_cli_struct *cli, unsigned int seconds) {
  if (seconds < 1) seconds = 0;
  cli->idle_timeout = seconds;
  time(&cli->last_action);
}

void crtl_cli_set_idle_timeout_callback(struct crtl_cli_struct *cli, unsigned int seconds, int (*callback)(struct crtl_cli_struct *)) {
  crtl_cli_set_idle_timeout(cli, seconds);
  cli->idle_timeout_callback = callback;
}

void crtl_cli_telnet_protocol(struct crtl_cli_struct *cli, int telnet_protocol) {
  cli->telnet_protocol = !!telnet_protocol;
}
void crtl_cli_set_context(struct crtl_cli_struct *cli, void *context) {
  cli->user_context = context;
}

void *crtl_cli_get_context(struct crtl_cli_struct *cli) {
  return cli->user_context;
}

struct crtl_cli_command *crtl_cli_register_filter(struct crtl_cli_struct *cli, const char *command,
                                        int (*init)(struct crtl_cli_struct *cli, int, char **, struct crtl_cli_filter *),
                                        int (*filter)(struct crtl_cli_struct *, const char *, void *), int privilege, int mode,
                                        const char *help) {
  struct crtl_cli_command *c;

  if (!command) return NULL;
  if (!(c = calloc(sizeof(struct crtl_cli_command), 1))) return NULL;

  c->command_type = CLI_FILTER_COMMAND;
  c->init = init;
  c->filter = filter;
  c->next = NULL;
  if (!(c->command = strdup(command))) {
    free(c);
    return NULL;
  }

  c->privilege = privilege;
  c->mode = mode;
  if (help && !(c->help = strdup(help))) {
    free(c->command);
    free(c);
    return NULL;
  }

  // Filters are all registered at the top level.
  crtl_cli_register_command_core(cli, NULL, c);
  return c;
}

int crtl_cli_unregister_filter(struct crtl_cli_struct *cli, const char *command) {
  return crtl_cli_int_unregister_command_core(cli, command, CLI_FILTER_COMMAND);
}

void crtl_cli_int_free_found_optargs(struct crtl_cli_optarg_pair **optarg_pair) {
  struct crtl_cli_optarg_pair *c;

  if (!optarg_pair || !*optarg_pair) return;

  for (c = *optarg_pair; c;) {
    *optarg_pair = c->next;
    free_z(c->name);
    free_z(c->value);
    free_z(c);
    c = *optarg_pair;
  }
}

char *crtl_cli_find_optarg_value(struct crtl_cli_struct *cli, char *name, char *find_after) {
  char *value = NULL;
  struct crtl_cli_optarg_pair *optarg_pair;
  if (!name || !cli->found_optargs) return NULL;

  for (optarg_pair = cli->found_optargs; optarg_pair && !value; optarg_pair = optarg_pair->next) {
    if (strcmp(optarg_pair->name, name) == 0) {
      if (find_after && find_after == optarg_pair->value) {
        find_after = NULL;
        continue;
      }
      value = optarg_pair->value;
    }
  }
  return value;
}

static void crtl_cli_optarg_build_shortest(struct crtl_cli_optarg *optarg) {
  struct crtl_cli_optarg *c, *p;
  char *cp, *pp;
  unsigned int len;

  for (c = optarg; c; c = c->next) {
    c->unique_len = 1;
    for (p = optarg; p; p = p->next) {
      if (c == p) continue;
      cp = c->name;
      pp = p->name;
      len = 1;
      while (*cp && *pp && *cp++ == *pp++) len++;
      if (len > c->unique_len) c->unique_len = len;
    }
  }
}

void crtl_cli_free_optarg(struct crtl_cli_optarg *optarg) {
  free_z(optarg->help);
  free_z(optarg->name);
  free_z(optarg);
}

int crtl_cli_register_optarg(struct crtl_cli_command *cmd, const char *name, int flags, int privilege, int mode, const char *help,
                        int (*get_completions)(struct crtl_cli_struct *cli, const char *, const char *, struct crtl_cli_comphelp *),
                        int (*validator)(struct crtl_cli_struct *cli, const char *, const char *),
                        int (*transient_mode)(struct crtl_cli_struct *cli, const char *, const char *)) {
  struct crtl_cli_optarg *optarg;
  struct crtl_cli_optarg *lastopt = NULL;
  struct crtl_cli_optarg *ptr = NULL;
  int retval = CLI_ERROR;

  // Name must not already exist with this priv/mode
  for (ptr = cmd->optargs, lastopt = NULL; ptr; lastopt = ptr, ptr = ptr->next) {
    if (!strcmp(name, ptr->name) && ptr->mode == mode && ptr->privilege == privilege) {
      return CLI_ERROR;
    }
  }
  if (!(optarg = calloc(sizeof(struct crtl_cli_optarg), 1))) goto CLEANUP;
  if (!(optarg->name = strdup(name))) goto CLEANUP;
  if (help && !(optarg->help = strdup(help))) goto CLEANUP;

  optarg->mode = mode;
  optarg->privilege = privilege;
  optarg->get_completions = get_completions;
  optarg->validator = validator;
  optarg->transient_mode = transient_mode;
  optarg->flags = flags;

  if (lastopt)
    lastopt->next = optarg;
  else
    cmd->optargs = optarg;
  crtl_cli_optarg_build_shortest(cmd->optargs);
  retval = CLI_OK;

CLEANUP:
  if (retval != CLI_OK) {
    crtl_cli_free_optarg(optarg);
  }
  return retval;
}

int crtl_cli_unregister_optarg(struct crtl_cli_command *cmd, const char *name) {
  struct crtl_cli_optarg *ptr;
  struct crtl_cli_optarg *lastptr;
  int retval = CLI_ERROR;
  // Iterate looking for this option name, stopping at end or if name matches
  for (lastptr = NULL, ptr = cmd->optargs; ptr && strcmp(ptr->name, name); lastptr = ptr, ptr = ptr->next)
    ;

  // If ptr, then we found the optarg to delete
  if (ptr) {
    if (lastptr) {
      // Not first optarg
      lastptr->next = ptr->next;
      ptr->next = NULL;
    } else {
      // First optarg
      cmd->optargs = ptr->next;
      ptr->next = NULL;
    }
    crtl_cli_free_optarg(ptr);
    crtl_cli_optarg_build_shortest(cmd->optargs);
    retval = CLI_OK;
  }
  return retval;
}

void crtl_cli_unregister_all_optarg(struct crtl_cli_command *c) {
  struct crtl_cli_optarg *o, *p;

  for (o = c->optargs; o; o = p) {
    p = o->next;
    crtl_cli_free_optarg(o);
  }
}

void crtl_cli_int_unset_optarg_value(struct crtl_cli_struct *cli, const char *name) {
  struct crtl_cli_optarg_pair **p, *c;
  for (p = &cli->found_optargs, c = *p; *p;) {
    c = *p;

    if (!strcmp(c->name, name)) {
      *p = c->next;
      free_z(c->name);
      free_z(c->value);
      free_z(c);
    } else {
      p = &(*p)->next;
    }
  }
}

int crtl_cli_set_optarg_value(struct crtl_cli_struct *cli, const char *name, const char *value, int allow_multiple) {
  struct crtl_cli_optarg_pair *optarg_pair, **anchor;
  int rc = CLI_ERROR;

  for (optarg_pair = cli->found_optargs, anchor = &cli->found_optargs; optarg_pair;
       anchor = &optarg_pair->next, optarg_pair = optarg_pair->next) {
    // Break if we found this name *and* allow_multiple is false
    if (!strcmp(optarg_pair->name, name) && !allow_multiple) {
      break;
    }
  }
  // If we *didn't* find this, then allocate a new entry before proceeding
  if (!optarg_pair) {
    optarg_pair = (struct crtl_cli_optarg_pair *)calloc(1, sizeof(struct crtl_cli_optarg_pair));
    *anchor = optarg_pair;
  }
  // Set the value
  if (optarg_pair) {
    // Name is null only if we didn't find it
    if (!optarg_pair->name) optarg_pair->name = strdup(name);

    // Value may be overwritten, so free any old value.
    if (optarg_pair->value) free_z(optarg_pair->value);
    optarg_pair->value = strdup(value);

    rc = CLI_OK;
  }
  return rc;
}

struct crtl_cli_optarg_pair *crtl_cli_get_all_found_optargs(struct crtl_cli_struct *cli) {
  if (cli) return cli->found_optargs;
  return NULL;
}

struct crtl_cli_optarg_pair *crtl_cli_get_next_optargs(struct crtl_cli_optarg_pair *optarg) {
    if(optarg) return optarg->next;
    return NULL;
}

const char *crtl_cli_get_name_optargs(struct crtl_cli_optarg_pair *optarg)
{
    if(optarg) return optarg->name;
    return NULL;
}
const char *crtl_cli_get_value_optargs(struct crtl_cli_optarg_pair *optarg)
{
    if(optarg) return optarg->value;
    return NULL;
}


char *crtl_cli_get_optarg_value(struct crtl_cli_struct *cli, const char *name, char *find_after) {
  char *value = NULL;
  struct crtl_cli_optarg_pair *optarg_pair;

  for (optarg_pair = cli->found_optargs; !value && optarg_pair; optarg_pair = optarg_pair->next) {
    // Check next entry if this isn't our name
    if (strcasecmp(optarg_pair->name, name)) continue;

    // Did we have a find_after, then ignore anything up until our find_after match
    if (find_after && optarg_pair->value == find_after) {
      find_after = NULL;
      continue;
    } else if (!find_after) {
      value = optarg_pair->value;
    }
  }
  return value;
}

void crtl_cli_int_free_buildmode(struct crtl_cli_struct *cli) {
  if (!cli || !cli->buildmode) return;
  crtl_cli_unregister_tree(cli, cli->commands, CLI_BUILDMODE_COMMAND);
  cli->mode = cli->buildmode->mode;
  free_z(cli->buildmode->cname);
  free_z(cli->buildmode->mode_text);
  crtl_cli_int_free_found_optargs(&cli->buildmode->found_optargs);
  free_z(cli->buildmode);
}

int crtl_cli_int_enter_buildmode(struct crtl_cli_struct *cli, struct crtl_cli_pipeline_stage *stage, char *mode_text) {
  struct crtl_cli_optarg *optarg;
  struct crtl_cli_command *c;
  struct crtl_cli_buildmode *buildmode;
  int rc = CLI_BUILDMODE_START;

  if (!cli || !(buildmode = (struct crtl_cli_buildmode *)calloc(1, sizeof(struct crtl_cli_buildmode)))) {
    crtl_cli_error(cli, "Unable to build buildmode mode for command %s", stage->command->command);
    rc = CLI_BUILDMODE_ERROR;
    goto out;
  }

  // Clean up any shrapnel from earlier - shouldn't be any but....
  if (cli->buildmode) crtl_cli_int_free_buildmode(cli);

  // Assign it so crtl_cli_int_register_buildmode_command() has something to work with
  cli->buildmode = buildmode;
  cli->buildmode->mode = cli->mode;
  cli->buildmode->transient_mode = cli->transient_mode;
  if (mode_text) cli->buildmode->mode_text = strdup(mode_text);

  // Need this to verify we have all *required* arguments
  cli->buildmode->command = stage->command;

  // Build new *limited* list of commands from this commands optargs
  for (optarg = stage->command->optargs; optarg; optarg = optarg->next) {
    // Don't allow anything that could redefine our mode or buildmode mode, or redefine exit/cancel/show/unset
    if (!strcmp(optarg->name, "cancel") || !strcmp(optarg->name, "exit") || !strcmp(optarg->name, "show") ||
        !strcmp(optarg->name, "unset")) {
      crtl_cli_error(cli, "Default buildmode command conflicts with optarg named %s", optarg->name);
      rc = CLI_BUILDMODE_ERROR;
      goto out;
    }
    if (optarg->flags & (CLI_CMD_ALLOW_BUILDMODE | CLI_CMD_TRANSIENT_MODE)) continue;
    if (optarg->mode != cli->mode && optarg->mode != cli->transient_mode)
      continue;
    else if (optarg->flags & (CLI_CMD_OPTIONAL_ARGUMENT | CLI_CMD_ARGUMENT)) {
      if ((c = crtl_cli_int_register_buildmode_command(cli, NULL, optarg->name, crtl_cli_int_buildmode_cmd_cback,
                                                  optarg->privilege, cli->mode, optarg->help))) {
        crtl_cli_register_optarg(c, optarg->name, CLI_CMD_ARGUMENT | (optarg->flags & CLI_CMD_OPTION_MULTIPLE),
                            optarg->privilege, cli->mode, optarg->help, optarg->get_completions, optarg->validator,
                            NULL);
      } else {
        rc = CLI_BUILDMODE_ERROR;
        goto out;
      }
    } else {
      if (optarg->flags & CLI_CMD_OPTION_MULTIPLE) {
        if (!crtl_cli_int_register_buildmode_command(cli, NULL, optarg->name, crtl_cli_int_buildmode_flag_multiple_cback,
                                                optarg->privilege, cli->mode, optarg->help)) {
          rc = CLI_BUILDMODE_ERROR;
          goto out;
        }
      } else {
        if (!crtl_cli_int_register_buildmode_command(cli, NULL, optarg->name, crtl_cli_int_buildmode_flag_cback,
                                                optarg->privilege, cli->mode, optarg->help)) {
          rc = CLI_BUILDMODE_ERROR;
          goto out;
        }
      }
    }
  }
  cli->buildmode->cname = strdup(crtl_cli_command_name(cli, stage->command));
  // And lastly four 'always there' commands to cancel current mode and to execute the command, show settings, and unset
  crtl_cli_int_register_buildmode_command(cli, NULL, "cancel", crtl_cli_int_buildmode_cancel_cback, CLI_PRIVILEGE_UNPRIVILEGED,
                                     cli->mode, "Cancel command");
  crtl_cli_int_register_buildmode_command(cli, NULL, "exit", crtl_cli_int_buildmode_exit_cback, CLI_PRIVILEGE_UNPRIVILEGED, cli->mode,
                                     "Exit and execute command");
  crtl_cli_int_register_buildmode_command(cli, NULL, "show", crtl_cli_int_buildmode_show_cback, CLI_PRIVILEGE_UNPRIVILEGED, cli->mode,
                                     "Show current settings");
  c = crtl_cli_int_register_buildmode_command(cli, NULL, "unset", crtl_cli_int_buildmode_unset_cback, CLI_PRIVILEGE_UNPRIVILEGED,
                                         cli->mode, "Unset a setting");
  crtl_cli_register_optarg(c, "setting", CLI_CMD_ARGUMENT | CLI_CMD_DO_NOT_RECORD, CLI_PRIVILEGE_UNPRIVILEGED, cli->mode,
                      "setting to clear", crtl_cli_int_buildmode_unset_completor, crtl_cli_int_buildmode_unset_validator, NULL);

out:
  if (rc != CLI_BUILDMODE_START) crtl_cli_int_free_buildmode(cli);
  return rc;
}

int crtl_cli_int_unregister_buildmode_command(struct crtl_cli_struct *cli, const char *command) {
  return crtl_cli_int_unregister_command_core(cli, command, CLI_BUILDMODE_COMMAND);
}

struct crtl_cli_command *crtl_cli_int_register_buildmode_command(struct crtl_cli_struct *cli, struct crtl_cli_command *parent,
                                                       const char *command,
                                                       int (*callback)(struct crtl_cli_struct *cli, const char *, char **, int),
                                                       int privilege, int mode, const char *help) {
  struct crtl_cli_command *c;

  if (!command) return NULL;
  if (!(c = calloc(sizeof(struct crtl_cli_command), 1))) return NULL;

  c->callback = callback;
  c->next = NULL;
  if (!(c->command = strdup(command))) {
    free(c);
    return NULL;
  }

  c->command_type = CLI_BUILDMODE_COMMAND;
  c->privilege = privilege;
  c->mode = mode;
  if (help && !(c->help = strdup(help))) {
    free(c->command);
    free(c);
    return NULL;
  }

  // Buildmode commmands are all registered at the top level
  crtl_cli_register_command_core(cli, NULL, c);
  return c;
}

int crtl_cli_int_execute_buildmode(struct crtl_cli_struct *cli) {
  struct crtl_cli_optarg *optarg = NULL;
  int rc = CLI_OK;
  char *cmdline;
  char *value = NULL;

  cmdline = strdup(crtl_cli_command_name(cli, cli->buildmode->command));
  for (optarg = cli->buildmode->command->optargs; rc == CLI_OK && optarg; optarg = optarg->next) {
    value = NULL;
    do {
      if (cli->privilege < optarg->privilege) continue;
      if ((optarg->mode != cli->buildmode->mode) && (optarg->mode != cli->buildmode->transient_mode) &&
          (optarg->mode != CLI_MODE_ANY))
        continue;

      value = crtl_cli_get_optarg_value(cli, optarg->name, value);
      if (!value && optarg->flags & CLI_CMD_ARGUMENT) {
        crtl_cli_error(cli, "Missing required argument %s", optarg->name);
        rc = CLI_MISSING_ARGUMENT;
      } else if (value) {
        if (optarg->flags & (CLI_CMD_OPTIONAL_FLAG | CLI_CMD_ARGUMENT)) {
          if (!(cmdline = crtl_cli_int_buildmode_extend_cmdline(cmdline, value))) {
            crtl_cli_error(cli, "Unable to append to building commandlne");
            rc = CLI_ERROR;
          }
        } else {
          if (!(cmdline = crtl_cli_int_buildmode_extend_cmdline(cmdline, optarg->name))) {
            crtl_cli_error(cli, "Unable to append to building commandlne");
            rc = CLI_ERROR;
          }
          if (!(cmdline = crtl_cli_int_buildmode_extend_cmdline(cmdline, value))) {
            crtl_cli_error(cli, "Unable to append to building commandlne");
            rc = CLI_ERROR;
          }
        }
      }
    } while (rc == CLI_OK && value && optarg->flags & CLI_CMD_OPTION_MULTIPLE);
  }

  if (rc == CLI_OK) {
    crtl_cli_int_free_buildmode(cli);
    crtl_cli_add_history(cli, cmdline);
    rc = crtl_cli_run_command(cli, cmdline);
  }
  free_z(cmdline);
  crtl_cli_int_free_buildmode(cli);
  return rc;
}

char *crtl_cli_int_buildmode_extend_cmdline(char *cmdline, char *word) {
  char *tptr = NULL;
  char *cptr = NULL;
  size_t oldlen = strlen(cmdline);
  size_t wordlen = strlen(word);
  int add_quotes = 0;

  // Allocate enough space to hold the old string, a space, and the new string (including null terminator).
  // Also include enough space for a quote around the string if it contains a whitespace character
  if ((tptr = (char *)realloc(cmdline, oldlen + 1 + wordlen + 1 + 2))) {
    strcat(tptr, " ");
    for (cptr = word; *cptr; cptr++) {
      if (isspace(*cptr)) {
        add_quotes = 1;
        break;
      }
    }
    if (add_quotes) strcat(tptr, "'");
    strcat(tptr, word);
    if (add_quotes) strcat(tptr, "'");
  }
  return tptr;
}

int crtl_cli_int_buildmode_cmd_cback(struct crtl_cli_struct *cli, const char *command, char *argv[], int argc) {
  int rc = CLI_BUILDMODE_EXTEND;

  if (argc) {
    crtl_cli_error(cli, "Extra arguments on command line, command ignored.");
    rc = CLI_ERROR;
  }
  return rc;
}

// A 'flag' callback has no optargs, so we need to set it ourself based on *this* command
int crtl_cli_int_buildmode_flag_cback(struct crtl_cli_struct *cli, const char *command, char *argv[], int argc) {
  int rc = CLI_BUILDMODE_EXTEND;

  if (argc) {
    crtl_cli_error(cli, "Extra arguments on command line, command ignored.");
    rc = CLI_ERROR;
  }
  if (crtl_cli_set_optarg_value(cli, command, command, 0)) {
    crtl_cli_error(cli, "Problem setting value for optional flag %s", command);
    rc = CLI_ERROR;
  }
  return rc;
}

// A 'flag' callback has no optargs, so we need to set it ourself based on *this* command
int crtl_cli_int_buildmode_flag_multiple_cback(struct crtl_cli_struct *cli, const char *command, char *argv[], int argc) {
  int rc = CLI_BUILDMODE_EXTEND;

  if (argc) {
    crtl_cli_error(cli, "Extra arguments on command line, command ignored.");
    rc = CLI_ERROR;
  }
  if (crtl_cli_set_optarg_value(cli, command, command, CLI_CMD_OPTION_MULTIPLE)) {
    crtl_cli_error(cli, "Problem setting value for optional flag %s", command);
    rc = CLI_ERROR;
  }

  return rc;
}

int crtl_cli_int_buildmode_cancel_cback(struct crtl_cli_struct *cli, const char *command, char *argv[], int argc) {
  int rc = CLI_BUILDMODE_CANCEL;

  if (argc > 0) {
    crtl_cli_error(cli, "Extra arguments on command line, command ignored.");
    rc = CLI_ERROR;
  }
  return rc;
}

int crtl_cli_int_buildmode_exit_cback(struct crtl_cli_struct *cli, const char *command, char *argv[], int argc) {
  int rc = CLI_BUILDMODE_EXIT;

  if (argc > 0) {
    crtl_cli_error(cli, "Extra arguments on command line, command ignored.");
    rc = CLI_ERROR;
  }
  return rc;
}

int crtl_cli_int_buildmode_show_cback(struct crtl_cli_struct *cli, const char *command, char *argv[], int argc) {
  struct crtl_cli_optarg_pair *optarg_pair;
  if (cli && cli->buildmode) {
    for (optarg_pair = cli->found_optargs; optarg_pair; optarg_pair = optarg_pair->next) {
      // Only show vars that are also current 'commands'
      struct crtl_cli_command *c = cli->commands;
      for (; c; c = c->next) {
        if (c->command_type != CLI_BUILDMODE_COMMAND) continue;
        if (!strcmp(c->command, optarg_pair->name)) {
          crtl_cli_print(cli, "  %-20s = %s", optarg_pair->name, optarg_pair->value);
          break;
        }
      }
    }
  }
  return CLI_OK;
}

int crtl_cli_int_buildmode_unset_cback(struct crtl_cli_struct *cli, const char *command, char *argv[], int argc) {
  // Iterate over our 'set' variables to see if that variable is also a 'valid' command right now
  struct crtl_cli_command *c;

  // Is this 'optarg' to remove one of the current commands?
  for (c = cli->commands; c; c = c->next) {
    if (c->command_type != CLI_BUILDMODE_COMMAND) continue;
    if (cli->privilege < c->privilege) continue;
    if ((cli->buildmode->mode != c->mode) && (cli->buildmode->transient_mode != c->mode) && (c->mode != CLI_MODE_ANY))
      continue;
    if (strcmp(c->command, argv[0])) continue;
    // Go fry anything by this name

    crtl_cli_int_unset_optarg_value(cli, argv[0]);
    break;
  }

  return CLI_OK;
}

// Generate a list of variables that *have* been set
int crtl_cli_int_buildmode_unset_completor(struct crtl_cli_struct *cli, const char *name, const char *word,
                                      struct crtl_cli_comphelp *comphelp) {
  return CLI_OK;
}

int crtl_cli_int_buildmode_unset_validator(struct crtl_cli_struct *cli, const char *name, const char *value) {
  return CLI_OK;
}

void crtl_cli_set_transient_mode(struct crtl_cli_struct *cli, int transient_mode) {
  cli->transient_mode = transient_mode;
}

int crtl_cli_add_comphelp_entry(struct crtl_cli_comphelp *comphelp, const char *entry) {
  int retval = CLI_ERROR;
  if (comphelp && entry) {
    char *dupelement = strdup(entry);
    char **duparray = (char **)realloc((void *)comphelp->entries, sizeof(char *) * (comphelp->num_entries + 1));
    if (dupelement && duparray) {
      comphelp->entries = duparray;
      comphelp->entries[comphelp->num_entries++] = dupelement;
      retval = CLI_OK;
    } else {
      free_z(dupelement);
      free_z(duparray);
    }
  }
  return retval;
}

void crtl_cli_free_comphelp(struct crtl_cli_comphelp *comphelp) {
  if (comphelp) {
    int idx;

    for (idx = 0; idx < comphelp->num_entries; idx++) free_z(comphelp->entries[idx]);
    free_z(comphelp->entries);
  }
}

static int crtl_cli_int_locate_command(struct crtl_cli_struct *cli, struct crtl_cli_command *commands, int command_type, int start_word,
                                  struct crtl_cli_pipeline_stage *stage) {
  struct crtl_cli_command *c, *again_config = NULL, *again_any = NULL;
  int c_words = stage->num_words;

  for (c = commands; c; c = c->next) {
    if (c->command_type != command_type) continue;
    if (cli->privilege < c->privilege) continue;

    if (strncasecmp(c->command, stage->words[start_word], c->unique_len)) continue;
    if (strncasecmp(c->command, stage->words[start_word], strlen(stage->words[start_word]))) continue;

  AGAIN:
    if (c->mode == cli->mode || (c->mode == CLI_MODE_ANY && again_any != NULL)) {
      int rc = CLI_OK;

      // Found a word!
      if (!c->children) {
        // Last word
        if (!c->callback && !c->filter) {
          crtl_cli_error(cli, "No callback for \"%s\"", crtl_cli_command_name(cli, c));
          return CLI_ERROR;
        }
      } else {
        if (start_word == c_words - 1) {
          if (c->callback) goto CORRECT_CHECKS;

          crtl_cli_error(cli, "Incomplete command");
          return CLI_ERROR;
        }
        rc = crtl_cli_int_locate_command(cli, c->children, command_type, start_word + 1, stage);
        if (rc == CLI_ERROR_ARG) {
          if (c->callback) {
            rc = CLI_OK;
            goto CORRECT_CHECKS;
          } else {
            crtl_cli_error(cli, "Invalid %s \"%s\"", commands->parent ? "argument" : "command", stage->words[start_word]);
          }
        }
        return rc;
      }

      if (!c->callback && !c->filter) {
        crtl_cli_error(cli, "Internal server error processing \"%s\"", crtl_cli_command_name(cli, c));
        return CLI_ERROR;
      }

    CORRECT_CHECKS:

      if (rc == CLI_OK) {
        stage->command = c;
        stage->first_unmatched = start_word + 1;
        stage->first_optarg = stage->first_unmatched;
        crtl_cli_int_parse_optargs(cli, stage, c, '\0', NULL);
        rc = stage->status;
      }
      return rc;
    } else if (cli->mode > CLI_MODE_CONFIG && c->mode == CLI_MODE_CONFIG) {
      // Command matched but from another mode, remember it if we fail to find correct command
      again_config = c;
    } else if (c->mode == CLI_MODE_ANY) {
      // Command matched but for any mode, remember it if we fail to find correct command
      again_any = c;
    }
  }

  // Drop out of config submode if we have matched command on CLI_MODE_CONFIG
  if (again_config) {
    c = again_config;
    goto AGAIN;
  }
  if (again_any) {
    c = again_any;
    goto AGAIN;
  }

  if (start_word == 0)
    crtl_cli_error(cli, "Invalid %s \"%s\"", commands->parent ? "argument" : "command", stage->words[start_word]);

  return CLI_ERROR_ARG;
}

int crtl_cli_int_validate_pipeline(struct crtl_cli_struct *cli, struct crtl_cli_pipeline *pipeline) {
  int i;
  int rc = CLI_OK;
  int command_type;

  if (!pipeline) return CLI_ERROR;
  cli->pipeline = pipeline;

  cli->found_optargs = NULL;
  for (i = 0; i < pipeline->num_stages; i++) {
    // In 'buildmode' we only have one pipeline, but we need to recall if we had started with any optargs

    if (cli->buildmode && i == 0)
      command_type = CLI_BUILDMODE_COMMAND;
    else if (i > 0)
      command_type = CLI_FILTER_COMMAND;
    else
      command_type = CLI_REGULAR_COMMAND;

    cli->pipeline->current_stage = &pipeline->stage[i];
    if (cli->buildmode)
      cli->found_optargs = cli->buildmode->found_optargs;
    else
      cli->found_optargs = NULL;
    rc = crtl_cli_int_locate_command(cli, cli->commands, command_type, 0, &pipeline->stage[i]);

    // And save our found optargs for later use
    if (cli->buildmode)
      cli->buildmode->found_optargs = cli->found_optargs;
    else
      pipeline->stage[i].found_optargs = cli->found_optargs;

    if (rc != CLI_OK) break;
  }
  cli->pipeline = NULL;

  return rc;
}

void crtl_cli_int_free_pipeline(struct crtl_cli_pipeline *pipeline) {
  int i;
  if (!pipeline) return;
  for (i = 0; i < pipeline->num_stages; i++) crtl_cli_int_free_found_optargs(&pipeline->stage[i].found_optargs);
  for (i = 0; i < pipeline->num_words; i++) free_z(pipeline->words[i]);
  free_z(pipeline->cmdline);
  free_z(pipeline);
  pipeline = NULL;
}

void crtl_cli_int_show_pipeline(struct crtl_cli_struct *cli, struct crtl_cli_pipeline *pipeline) {
  int i, j;
  struct crtl_cli_pipeline_stage *stage;
  char **word;
  struct crtl_cli_optarg_pair *optarg_pair;

  for (i = 0, word = pipeline->words; i < pipeline->num_words; i++, word++) printf("[%s] ", *word);
  fprintf(stderr, "\n");
  fprintf(stderr, "#stages=%d, #words=%d\n", pipeline->num_stages, pipeline->num_words);

  for (i = 0; i < pipeline->num_stages; i++) {
    stage = &(pipeline->stage[i]);
    fprintf(stderr, "  #%d(%d words) first_unmatched=%d: ", i, stage->num_words, stage->first_unmatched);
    for (j = 0; j < stage->num_words; j++) {
      fprintf(stderr, " [%s]", stage->words[j]);
    }
    fprintf(stderr, "\n");

    if (stage->command) {
      fprintf(stderr, "  Command: %s\n", stage->command->command);
    }
    for (optarg_pair = stage->found_optargs; optarg_pair; optarg_pair = optarg_pair->next) {
      fprintf(stderr, "    %s: %s\n", optarg_pair->name, optarg_pair->value);
    }
  }
}

// Take an array of words and return a pipeline, using '|' to split command into different 'stages'.
// Pipeline is broken down by '|' characters and within each p.
struct crtl_cli_pipeline *crtl_cli_int_generate_pipeline(struct crtl_cli_struct *cli, const char *command) {
  int i;
  struct crtl_cli_pipeline_stage *stage;
  char **word;
  struct crtl_cli_pipeline *pipeline = NULL;

  cli->found_optargs = NULL;
  if (cli->buildmode) cli->found_optargs = cli->buildmode->found_optargs;
  if (!command) return NULL;
  while (*command && isspace(*command)) command++;

  if (!(pipeline = (struct crtl_cli_pipeline *)calloc(1, sizeof(struct crtl_cli_pipeline)))) return NULL;
  pipeline->cmdline = (char *)strdup(command);

  pipeline->num_words = crtl_cli_parse_line(command, pipeline->words, CLI_MAX_LINE_WORDS);

  pipeline->stage[0].num_words = 0;
  stage = &pipeline->stage[0];
  word = pipeline->words;
  stage->words = word;
  for (i = 0; i < pipeline->num_words; i++, word++) {
    if (*word[0] == '|') {
      if (cli->buildmode) {
        // Can't allow filters in buildmode commands
        crtl_cli_int_free_pipeline(pipeline);
        return NULL;
      }
      stage->stage_num = pipeline->num_stages;
      stage++;
      stage->num_words = 0;
      pipeline->num_stages++;
      stage->words = word + 1;  // First word of the next stage is one past where we are (possibly NULL)
    } else {
      stage->num_words++;
    }
  }
  stage->stage_num = pipeline->num_stages;
  pipeline->num_stages++;
  return pipeline;
}

int crtl_cli_int_execute_pipeline(struct crtl_cli_struct *cli, struct crtl_cli_pipeline *pipeline) {
  int stage_num;
  int rc = CLI_OK;
  struct crtl_cli_filter **filt = &cli->filters;

  if (!pipeline | !cli) return CLI_ERROR;

  cli->pipeline = pipeline;
  for (stage_num = 1; stage_num < pipeline->num_stages; stage_num++) {
    struct crtl_cli_pipeline_stage *stage = &pipeline->stage[stage_num];
    pipeline->current_stage = stage;
    cli->found_optargs = stage->found_optargs;
    *filt = calloc(sizeof(struct crtl_cli_filter), 1);
    if (*filt) {
      if ((rc = stage->command->init(cli, stage->num_words, stage->words, *filt) != CLI_OK)) {
        break;
      }
      filt = &(*filt)->next;
    }
  }
  pipeline->current_stage = NULL;

  // Did everything init?  If so, execute, otherwise skip execution
  if ((rc == CLI_OK) && pipeline->stage[0].command->callback) {
    struct crtl_cli_pipeline_stage *stage = &pipeline->stage[0];

    pipeline->current_stage = &pipeline->stage[0];
    if (pipeline->current_stage->command->command_type == CLI_BUILDMODE_COMMAND)
      cli->found_optargs = cli->buildmode->found_optargs;
    else
      cli->found_optargs = pipeline->stage[0].found_optargs;
    rc = stage->command->callback(cli, crtl_cli_command_name(cli, stage->command), stage->words + stage->first_unmatched,
                                  stage->num_words - stage->first_unmatched);
    if (pipeline->current_stage->command->command_type == CLI_BUILDMODE_COMMAND)
      cli->buildmode->found_optargs = cli->found_optargs;
    pipeline->current_stage = NULL;
  }

  // Now teardown any filters
  while (cli->filters) {
    struct crtl_cli_filter *filt = cli->filters;
    if (filt->filter) filt->filter(cli, NULL, cli->filters->data);
    cli->filters = filt->next;
    free_z(filt);
  }
  cli->found_optargs = NULL;
  cli->pipeline = NULL;
  return rc;
}

static char DELIM_OPT_START[] = "[";
static char DELIM_OPT_END[] = "]";
static char DELIM_ARG_START[] = "<";
static char DELIM_ARG_END[] = ">";
static char DELIM_NONE[] = "";
static char BUILDMODE_YES[] = " (enter buildmode)";
static char BUILDMODE_NO[] = "";

static void crtl_cli_get_optarg_comphelp(struct crtl_cli_struct *cli, struct crtl_cli_optarg *optarg, struct crtl_cli_comphelp *comphelp,
                                    int num_candidates, const char lastchar, const char *anchor_word,
                                    const char *next_word) {
  int help_insert = 0;
  char *delim_start = DELIM_NONE;

  char *delim_end = DELIM_NONE;
  char *allow_buildmode = BUILDMODE_NO;
  int (*get_completions)(struct crtl_cli_struct *, const char *, const char *, struct crtl_cli_comphelp *) = NULL;
  char *tptr = NULL;
  char *tname = NULL;
  
  // If we've already seen a value by this exact name, skip it, unless the multiple flag is set
  if (crtl_cli_find_optarg_value(cli, optarg->name, NULL) && !(optarg->flags & (CLI_CMD_OPTION_MULTIPLE))) return;

  get_completions = optarg->get_completions;
  if (optarg->flags & CLI_CMD_OPTIONAL_FLAG) {
    if (!(anchor_word && !strncmp(anchor_word, optarg->name, strlen(anchor_word)))) {
      delim_start = DELIM_OPT_START;
      delim_end = DELIM_OPT_END;
      get_completions = NULL;  // No point, completor of field is the name itself
    }
  } else if (optarg->flags & CLI_CMD_HYPHENATED_OPTION) {
    delim_start = DELIM_OPT_START;
    delim_end = DELIM_OPT_END;
  } else if (optarg->flags & CLI_CMD_ARGUMENT) {
    delim_start = DELIM_ARG_START;
    delim_end = DELIM_ARG_END;
  } else if (optarg->flags & CLI_CMD_OPTIONAL_ARGUMENT) {
    /*
     * Optional args can match against the name the value.
     * Here 'anchor_word' is the name, and 'next_word' is what we're matching against.
     * So if anchor_word==next_word we're looking at the 'name' of the optarg, otherwise we know the name and are going
     * against the value.
     */
    if (anchor_word != next_word) {
      // Matching against optional argument 'value'
      help_insert = 0;
      if (!get_completions) {
        delim_start = DELIM_ARG_START;
        delim_end = DELIM_ARG_END;
      }
    } else {
      // Matching against optional argument 'name'
      help_insert = 1;
      get_completions = NULL;  // Matching against the name, not the following field value
      if (!(anchor_word && !strncmp(anchor_word, optarg->name, strlen(anchor_word)))) {
        delim_start = DELIM_OPT_START;
        delim_end = DELIM_OPT_END;
      }
    }
  }

  // Fill in with help text or completor value(s) as indicated
  if (lastchar == '?' && asprintf(&tname, "%s%s%s", delim_start, optarg->name, delim_end) != -1) {
    if (optarg->flags & CLI_CMD_ALLOW_BUILDMODE) allow_buildmode = BUILDMODE_YES;
    if (help_insert && (asprintf(&tptr, "  %-20s enter '%s' to %s%s", tname, optarg->name,
                                 (optarg->help) ? optarg->help : "", allow_buildmode) != -1)) {
      crtl_cli_add_comphelp_entry(comphelp, tptr);
      free_z(tptr);
    } else if (asprintf(&tptr, "  %-20s %s%s", tname, (optarg->help) ? optarg->help : "", allow_buildmode) != -1) {
      crtl_cli_add_comphelp_entry(comphelp, tptr);
      free_z(tptr);
    }
    free_z(tname);
  } else if (lastchar == CTRL('I')) {
    if (get_completions) {
      (*get_completions)(cli, optarg->name, next_word, comphelp);
    } else if ((!anchor_word || !strncmp(anchor_word, optarg->name, strlen(anchor_word))) &&
               (asprintf(&tptr, "%s%s%s", delim_start, optarg->name, delim_end) != -1)) {
      crtl_cli_add_comphelp_entry(comphelp, tptr);
      free_z(tptr);
    }
  }
}

static void crtl_cli_int_parse_optargs(struct crtl_cli_struct *cli, struct crtl_cli_pipeline_stage *stage, struct crtl_cli_command *cmd,
                                  char lastchar, struct crtl_cli_comphelp *comphelp) {
  struct crtl_cli_optarg *optarg = NULL, *oaptr = NULL;
  int word_idx, word_incr, candidate_idx;
  struct crtl_cli_optarg *candidates[CLI_MAX_LINE_WORDS];
  char *value;
  int num_candidates = 0;
  int is_last_word = 0;
  int (*validator)(struct crtl_cli_struct *, const char *name, const char *value);

  if (cli->buildmode) cli->found_optargs = cli->buildmode->found_optargs;
  else cli->found_optargs = stage->found_optargs;
  /*
   * Tab completion and help are *only* allowed at end of string, but we need to process the entire command to know what
   * has already been found.  There should be no ambiguities before the 'last' word.
   * Note specifically that for tab completions and help the *last* word can be a null pointer.
   */
  stage->error_word = NULL;

  /* Start our optarg and word pointers at the beginning.
   * optarg will be incremented *only* when an argument is identified.
   * word_idx will be incremented either by 1 (optflag or argument) or 2 (optional argument).
   */
  word_idx = stage->first_unmatched;
  optarg = cmd->optargs;
  num_candidates = 0;
  while (optarg && word_idx < stage->num_words && num_candidates <= 1) {
    num_candidates = 0;
    word_incr = 1;  // Assume we're only incrementing by a word - if we match an optional argument bump to 2

    /*
     * The initial loop here is to identify candidates based matching *this* word in order against:
     * - An exact match of the word to the optinal flag/argument name (yield exactly one match and exit the loop)
     * - A partial match for optional flag/argument name
     * - Candidate an argument.
     */

    for (oaptr = optarg; oaptr; oaptr = oaptr->next) {
      // Skip this option unless it matches privileges, CLI_MODE_ANY, the current mode, or the transient_mode
      if (cli->privilege < oaptr->privilege) continue;
      if ((oaptr->mode != cli->mode) && (oaptr->mode != cli->transient_mode) && (oaptr->mode != CLI_MODE_ANY)) continue;

      /*
       * Two special cases - a hphenated option and an 'exact' match optional flag or optional argument.
       * If our word starts with a '-' and we have a CMD_CLI_HYPHENATED_OPTION or an exact match for an optional
       * flag/argument name trumps anything and will be the *only* candidate.
       * Otherwise if the word is 'blank', could be an argument, or matches 'enough' of an option/flag it is a
       * candidate.
       * Once we accept an argument as a candidate, we're done looking for candidates as straight arguments are
       * required.
       */
      if (oaptr->flags & CLI_CMD_SPOT_CHECK && num_candidates == 0) {
        stage->status = (*oaptr->validator)(cli, NULL, NULL);
        if (stage->status != CLI_OK) {
          stage->error_word = stage->words[word_idx];
          crtl_cli_reprompt(cli);
          goto done;
        }
      } else if (stage->words[word_idx] && (oaptr->flags & (CLI_CMD_OPTIONAL_FLAG | CLI_CMD_OPTIONAL_ARGUMENT)) &&
                 !strcmp(oaptr->name, stage->words[word_idx])) {
        candidates[0] = oaptr;
        num_candidates = 1;
        break;
      } else if (stage->words[word_idx] && stage->words[word_idx][0] == '-' &&
                 (oaptr->flags & (CLI_CMD_HYPHENATED_OPTION))) {
        candidates[0] = oaptr;
        num_candidates = 1;
        break;
      } else if (!stage->words[word_idx] || (oaptr->flags & CLI_CMD_ARGUMENT) ||
                 !strncasecmp(oaptr->name, stage->words[word_idx], strlen(stage->words[word_idx]))) {
        candidates[num_candidates++] = oaptr;
      }
      if (oaptr->flags & CLI_CMD_ARGUMENT) {
        break;
      }
    }

    /*
     * Iterate over the list of candidates for this word.  There are several early exit cases to consider:
     * - If we have no candidates then we're done - any remaining words must be processed by the command callback
     * - If we have more than one candidate evaluating for execution punt hard after complaining.
     * - If we have more than one candidate and we're not at end-of-line (
     */
    if (num_candidates == 0) break;
    if (num_candidates > 1 && (lastchar == '\0' || word_idx < (stage->num_words - 1))) {
      stage->error_word = stage->words[word_idx];
      stage->status = CLI_AMBIGUOUS;
      crtl_cli_error(cli, "Ambiguous option/argument for command %s", stage->command->command);
      goto done;
    }

    /*
     * So now we could have one or more candidates.  We need to call get help/completions *only* if this is the
     * 'last-word'.
     * Remember that last word for optional arguments is last or next to last....
     */
    if (lastchar != '\0') {
      int called_comphelp = 0;
      for (candidate_idx = 0; candidate_idx < num_candidates; candidate_idx++) {
        oaptr = candidates[candidate_idx];

        // Need to know *which* word we're trying to complete for optional_args, hence the difference calls
        if (((oaptr->flags & (CLI_CMD_OPTIONAL_FLAG | CLI_CMD_ARGUMENT)) && (word_idx == (stage->num_words - 1))) ||
            (oaptr->flags & (CLI_CMD_OPTIONAL_ARGUMENT | CLI_CMD_HYPHENATED_OPTION) &&
             word_idx == (stage->num_words - 1))) {
          crtl_cli_get_optarg_comphelp(cli, oaptr, comphelp, num_candidates, lastchar, stage->words[word_idx],
                                  stage->words[word_idx]);
          called_comphelp = 1;
        } else if (oaptr->flags & CLI_CMD_OPTIONAL_ARGUMENT && word_idx == (stage->num_words - 2)) {
          crtl_cli_get_optarg_comphelp(cli, oaptr, comphelp, num_candidates, lastchar, stage->words[word_idx],
                                  stage->words[word_idx + 1]);
          called_comphelp = 1;
        }
      }
      // If we were 'end-of-word' and looked for completions/help, return to user
      if (called_comphelp) {
        stage->status = CLI_OK;
        goto done;
      }
    }

    // Set some values for use later - makes code much easier to read
    value = stage->words[word_idx];
    oaptr = candidates[0];
    validator = oaptr->validator;
    if ((oaptr->flags & (CLI_CMD_OPTIONAL_FLAG | CLI_CMD_ARGUMENT) && word_idx == (stage->num_words - 1)) ||
        (oaptr->flags & CLI_CMD_OPTIONAL_ARGUMENT && word_idx == (stage->num_words - 2))) {
      is_last_word = 1;
    }

    if (oaptr->flags & CLI_CMD_OPTIONAL_ARGUMENT) {
      word_incr = 2;
      if (!stage->words[word_idx + 1] && lastchar == '\0') {
        // Hit an optional argument that does not have a value with it
        crtl_cli_error(cli, "Optional argument %s requires a value", stage->words[word_idx]);
        stage->error_word = stage->words[word_idx];
        stage->status = CLI_MISSING_VALUE;
        goto done;
      }
      value = stage->words[word_idx + 1];
    }

    /*
     * We're not at end of string and doing help/completions.
     * So see if our value is 'valid', to save it, and see if we have any extra processing to do such as a transient
     * mode check or enter build mode.
     */

    if (!validator || (*validator)(cli, oaptr->name, value) == CLI_OK) {
      if (oaptr->flags & CLI_CMD_DO_NOT_RECORD) {
        // We want completion and validation, but then leave this 'value' to be seen - used *only* by buildmode as
        // argv[0] with argc=1
        break;
      } else {
        // Need to combine remaining words if the CLI_CMD_REMAINDER_OF_LINE flag it set, then we're done processing
        int set_value_return = 0;

        if (oaptr->flags & CLI_CMD_REMAINDER_OF_LINE) {
          char *combined = NULL;
          combined = join_words(stage->num_words - word_idx, stage->words + word_idx);
          set_value_return = crtl_cli_set_optarg_value(cli, oaptr->name, combined, 0);
          free_z(combined);
        } else {
          set_value_return = crtl_cli_set_optarg_value(cli, oaptr->name, value, oaptr->flags & CLI_CMD_OPTION_MULTIPLE);
        }

        if (set_value_return != CLI_OK) {
          crtl_cli_error(cli, "%sProblem setting value for command argument %s", lastchar == '\0' ? "" : "\n",
                    stage->words[word_idx]);
          crtl_cli_reprompt(cli);
          stage->error_word = stage->words[word_idx];
          stage->status = CLI_ERROR;
          goto done;
        }
      }
    } else {
      crtl_cli_error(cli, "%sProblem parsing command setting %s with value %s", lastchar == '\0' ? "" : "\n", oaptr->name,
                stage->words[word_idx]);
      crtl_cli_reprompt(cli);
      stage->error_word = stage->words[word_idx];
      stage->status = CLI_ERROR;
      goto done;
    }

    // If this optarg can set the transient mode, then evaluate it if we're not at last word
    if (oaptr->transient_mode && oaptr->transient_mode(cli, oaptr->name, value)) {
      stage->error_word = stage->words[word_idx];
      stage->status = CLI_ERROR;
      goto done;
    }

    // Only do buildmode optargs if we're a executing a command, parsing command (stage 0), and this is the last word
    if ((stage->status == CLI_OK) && (oaptr->flags & CLI_CMD_ALLOW_BUILDMODE) && is_last_word) {
      stage->status = crtl_cli_int_enter_buildmode(cli, stage, value);
      goto done;
    }

    // Optional flags and arguments can appear multiple times, but true arguments only once.  Advance our optarg
    // starting point when we see a true argument
    if (oaptr->flags & CLI_CMD_ARGUMENT) {
      // Advance past this argument entry
      optarg = oaptr->next;
    }

    word_idx += word_incr;
    stage->first_unmatched = word_idx;
  }

  // If we're evaluating the command for execution, ensure we have all required arguments.
  if (lastchar == '\0') {
    for (; optarg; optarg = optarg->next) {
      if (cli->privilege < optarg->privilege) continue;
      if ((optarg->mode != cli->mode) && (optarg->mode != cli->transient_mode) && (optarg->mode != CLI_MODE_ANY)) continue;
      if (optarg->flags & CLI_CMD_DO_NOT_RECORD) continue;
      if (optarg->flags & CLI_CMD_ARGUMENT) {
        crtl_cli_error(cli, "Incomplete command, missing required argument '%s'", optarg->name);
        stage->status = CLI_MISSING_ARGUMENT;
        goto done;
      }
    }
  }

done:
  if (cli->buildmode) cli->buildmode->found_optargs = cli->found_optargs;
  else stage->found_optargs = cli->found_optargs;
  return;
}

void crtl_cli_unregister_all_commands(struct crtl_cli_struct *cli) {
  crtl_cli_unregister_tree(cli, cli->commands, CLI_REGULAR_COMMAND);
}

void crtl_cli_unregister_all_filters(struct crtl_cli_struct *cli) {
  crtl_cli_unregister_tree(cli, cli->commands, CLI_FILTER_COMMAND);
}




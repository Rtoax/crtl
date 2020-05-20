#include "crtl/cli.h"
#include "crtl/log.h"
#include "crtl/task.h"




#define CLITEST_PORT        8000
#define MODE_CONFIG_INT     10


unsigned int regular_count = 0;
unsigned int debug_regular = 0;


const char *cli_user = "root";
const char *cli_passwd = "123456";


struct my_context {
    int value;
    char *message;
};


int cmd_test(struct crtl_cli_struct *cli, const char *command, char *argv[], int argc) 
{
    int i;
    crtl_cli_print(cli, "called %s with \"%s\"", __func__, command);
    crtl_cli_print(cli, "%d arguments:", argc);
    
    for (i = 0; i < argc; i++) 
        crtl_cli_print(cli, "        %s", argv[i]);

    return CLI_OK;
}

int cmd_set(struct crtl_cli_struct *cli, const char   *command, char *argv[], int argc) 
{
    if (argc < 2 || strcmp(argv[0], "?") == 0) {
        crtl_cli_print(cli, "Specify a variable to set");
        return CLI_OK;
    }

    if (strcmp(argv[1], "?") == 0) {
        crtl_cli_print(cli, "Specify a value");
        return CLI_OK;
    }

    if (strcmp(argv[0], "regular_interval") == 0) {
        unsigned int sec = 0;
        if (!argv[1] && !*argv[1]) {
            crtl_cli_print(cli, "Specify a regular callback interval in seconds");
            return CLI_OK;
        }
        sscanf(argv[1], "%u", &sec);
        if (sec < 1) {
            crtl_cli_print(cli, "Specify a regular callback interval in seconds");
            return CLI_OK;
        }
        cli->timeout_tm.tv_sec = sec;
        cli->timeout_tm.tv_usec = 0;
        crtl_cli_print(cli, "Regular callback interval is now %d seconds", sec);
        return CLI_OK;
    }

    crtl_cli_print(cli, "Setting \"%s\" to \"%s\"", argv[0], argv[1]);
    return CLI_OK;
}

int cmd_config_int(struct crtl_cli_struct *cli, const char   *command, char *argv[], int argc) 
{
    if (argc < 1) {
        crtl_cli_print(cli, "Specify an interface to configure");
        return CLI_OK;
    }

    if (strcmp(argv[0], "?") == 0)
        crtl_cli_print(cli, "  test0/0");
    else if (strcasecmp(argv[0], "test0/0") == 0)
        crtl_cli_set_configmode(cli, MODE_CONFIG_INT, "test");
    else
        crtl_cli_print(cli, "Unknown interface %s", argv[0]);

    return CLI_OK;
}

int cmd_config_int_exit(struct crtl_cli_struct *cli, const char   *command, char   *argv[], int   argc) 
{
    crtl_cli_set_configmode(cli, LIBCLI_MODE_CONFIG, NULL);
    return CLI_OK;
}

int cmd_show_regular(struct crtl_cli_struct *cli, const char   *command, char   *argv[], int   argc) 
{
    crtl_cli_print(cli, "crtl_cli_regular() has run %u times", regular_count);
    return CLI_OK;
}

int cmd_debug_regular(struct crtl_cli_struct *cli, const char   *command, char   *argv[], int   argc) 
{
    debug_regular = !debug_regular;
    crtl_cli_print(cli, "crtl_cli_regular() debugging is %s", debug_regular ? "enabled" : "disabled");
    return CLI_OK;
}

int cmd_context(struct crtl_cli_struct *cli, const char   *command, char   *argv[], int   argc) 
{
    struct my_context *myctx = (struct my_context *)crtl_cli_get_context(cli);
    crtl_cli_print(cli, "User context has a value of %d and message saying %s", myctx->value, myctx->message);
    return CLI_OK;
}

int check_auth(const char *username, const char *password) 
{
    if (strcasecmp(username, cli_user) != 0) return CLI_ERROR;
    if (strcasecmp(password, cli_passwd) != 0) return CLI_ERROR;
    return CLI_OK;
}

int regular_callback(struct crtl_cli_struct *cli) 
{
    regular_count++;
    if (debug_regular) {
        crtl_cli_print(cli, "Regular callback - %u times so far", regular_count);
        crtl_cli_reprompt(cli);
    }
    return CLI_OK;
}

int check_enable(const char *password) 
{
    return !strcasecmp(password, "topsecret");
}

int idle_timeout(struct crtl_cli_struct *cli) 
{
    crtl_cli_print(cli, "Custom idle timeout");
    return CLI_QUIT;
}

void pc(struct crtl_cli_struct   *cli, const char *string) 
{
    printf("%s\n", string);
}

#define MODE_POLYGON_TRIANGLE 20
#define MODE_POLYGON_RECTANGLE 21

int cmd_perimeter(struct crtl_cli_struct *cli, const char *command, char *argv[], int argc) 
{
    struct crtl_cli_optarg_pair *optargs = crtl_cli_get_all_found_optargs(cli);
    int i = 1, numSides = 0;
    int perimeter = 0;
    int verbose_count = 0;
    char *verboseArg = NULL;
    char *shapeName = NULL;

    crtl_cli_print(cli, "perimeter callback, with %d args", argc);
    
    for (; optargs; optargs = crtl_cli_get_next_optargs(optargs)) 
        crtl_cli_print(cli, "%d, %s=%s", i++, crtl_cli_get_name_optargs(optargs), crtl_cli_get_value_optargs(optargs));

    if ((verboseArg = crtl_cli_get_optarg_value(cli, "verbose", verboseArg))) {
        do {
            verbose_count++;
        } while ((verboseArg = crtl_cli_get_optarg_value(cli, "verbose", verboseArg)));
    }
    crtl_cli_print(cli, "verbose argument was seen  %d times", verbose_count);

    shapeName = crtl_cli_get_optarg_value(cli, "shape", NULL);
    
    if (!shapeName) {
        crtl_cli_error(cli, "No shape name given");
        return CLI_ERROR;
    } else if (strcmp(shapeName, "triangle") == 0) {
        numSides = 3;
    } else if (strcmp(shapeName, "rectangle") == 0) {
        numSides = 4;
    } else {
        crtl_cli_error(cli, "Unrecognized shape given");
        return CLI_ERROR;
    }
    
    for (i = 1; i <= numSides; i++) {
        char sidename[50], *value;
        int length;
        snprintf(sidename, 50, "side_%d", i);
        value = crtl_cli_get_optarg_value(cli, sidename, NULL);
        length = strtol(value, NULL, 10);
        perimeter += length;
    }
    crtl_cli_print(cli, "Perimeter is %d", perimeter);
    return CLI_OK;
}


const char *KnownShapes[] = {"rectangle", "triangle", NULL};

int shape_completor(struct crtl_cli_struct *cli, const char *name, const char *value, struct crtl_cli_comphelp *comphelp) 
{
    const char **shape;
    int rc = CLI_OK;

    printf("shape_completor called with <%s>\n", value);
    
    for (shape = KnownShapes; *shape && (rc == CLI_OK); shape++) {
        if (!value || !strncmp(*shape, value, strlen(value))) {
            rc = crtl_cli_add_comphelp_entry(comphelp, *shape);
        }
    }
    return rc;
}

int shape_validator(struct crtl_cli_struct *cli, const char *name, const char *value) 
{
    const char **shape;

    printf("shape_validator called with <%s>\n", value);
    for (shape = KnownShapes; *shape; shape++) {
        if (!strcmp(value, *shape)) return CLI_OK;
    }
    
    return CLI_ERROR;
}

int verbose_validator(struct crtl_cli_struct *cli, const char *name, const char *value) 
{
    printf("verbose_validator called\n");
    return CLI_OK;
}

int shape_transient_eval(struct crtl_cli_struct *cli, const char *name, const char *value) 
{
    printf("shape_transient_eval called with <%s>\n", value);
    
    if (!strcmp(value, "rectangle")) {
        crtl_cli_set_transient_mode(cli, MODE_POLYGON_RECTANGLE);
        crtl_cli_set_optarg_value(cli, "duplicateShapeValue", value, 0);
        return CLI_OK;
    } else if (!strcmp(value, "triangle")) {
        crtl_cli_set_transient_mode(cli, MODE_POLYGON_TRIANGLE);
        crtl_cli_set_optarg_value(cli, "duplicateShapeValue", value, 0);
        return CLI_OK;
    }
    
    crtl_cli_error(cli, "unrecognized value for setting %s -> %s", name, value);
    
    return CLI_ERROR;
}

const char *KnownColors[] = {"black",    "white",    "gray",      "red",        "blue",
                             "green",    "lightred", "lightblue", "lightgreen", "darkred",
                             "darkblue", "darkgree", "lavender",  "yellow",     NULL};

int color_completor(struct crtl_cli_struct *cli, const char *name, const char *word, struct crtl_cli_comphelp *comphelp) 
{
    // Attempt to show matches against the following color strings
    const char **color;
    int rc = CLI_OK;
    printf("color_completor called with <%s>\n", word);
    for (color = KnownColors; *color && (rc == CLI_OK); color++) {
        if (!word || !strncmp(*color, word, strlen(word))) {
            rc = crtl_cli_add_comphelp_entry(comphelp, *color);
        }
    }
    return rc;
}

int color_validator(struct crtl_cli_struct *cli, const char *name, const char *value) 
{
    const char **color;
    int rc = CLI_ERROR;
    printf("color_validator called for %s\n", name);
    for (color = KnownColors; *color; color++) {
        if (!strcmp(value, *color)) return CLI_OK;
    }
    return rc;
}

int side_length_validator(struct crtl_cli_struct *cli, const char *name, const char *value) 
{
    // Verify 'value' is a positive number
    long len;
    char *endptr;
    int rc = CLI_OK;

    printf("side_length_validator called\n");
    errno = 0;
    len = strtol(value, &endptr, 10);
    if ((endptr == value) || (*endptr != '\0') || ((errno == ERANGE) && ((len == LONG_MIN) || (len == LONG_MAX))))
        return CLI_ERROR;
    return rc;
}

int check1_validator(struct crtl_cli_struct *cli, const char   *name, const char   *value) 
{
    char *color;
    char *transparent;

    printf("check1_validator called \n");
    color = crtl_cli_get_optarg_value(cli, "color", NULL);
    transparent = crtl_cli_get_optarg_value(cli, "transparent", NULL);

    if (!color && !transparent) {
        crtl_cli_error(cli, "\nMust supply either a color or transparent!");
        return CLI_ERROR;
    } else if (color && !strcmp(color, "black") && transparent) {
        crtl_cli_error(cli, "\nCan not have a transparent black object!");
        return CLI_ERROR;
    }
    return CLI_OK;
}

void run_child(int acceptfd) 
{
    struct crtl_cli_command *c;
    struct crtl_cli_struct *cli;

    // Prepare a small user context
    char mymessage[] = "I contain user data!";
    struct my_context myctx;
    myctx.value = 5;
    myctx.message = mymessage;

#define CLI_BANNER  "# Welcome TO Crtl Telnet CLI \n"\
                    "#  ls - show the list of cmd\n"
#define CLI_HOSTNAME    "[telnet@crtl]"

    cli = crtl_cli_init(CLI_BANNER, CLI_HOSTNAME);
    
    crtl_cli_telnet_protocol(cli, 1);
    crtl_cli_regular(cli, regular_callback);

    // change regular update to 5 seconds rather than default of 1 second
    crtl_cli_regular_interval(cli, 5);
    
    // set 60 second idle timeout
    crtl_cli_set_idle_timeout_callback(cli, 60, idle_timeout);
    
    crtl_cli_register_command(cli, NULL, "test", cmd_test, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC, NULL);
    crtl_cli_register_command(cli, NULL, "simple", NULL, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC, NULL);
    crtl_cli_register_command(cli, NULL, "simon", NULL, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC, NULL);
    
    crtl_cli_register_command(cli, NULL, "set", cmd_set, LIBCLI_PRIVILEGE_PRIVILEGED, LIBCLI_MODE_EXEC, NULL);
    
    c = crtl_cli_register_command(cli, NULL, "show", NULL, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC, NULL);
    crtl_cli_register_command(cli, c, "regular", cmd_show_regular, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC,
                                                    "Show the how many times crtl_cli_regular has run");
    crtl_cli_register_command(cli, c, "counters", cmd_test, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC,
                                                        "Show the counters that the system uses");
    crtl_cli_register_command(cli, c, "junk", cmd_test, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC, NULL);
    
    crtl_cli_register_command(cli, NULL, "interface", cmd_config_int, LIBCLI_PRIVILEGE_PRIVILEGED, LIBCLI_MODE_CONFIG,
                                                        "Configure an interface");
    crtl_cli_register_command(cli, NULL, "exit", cmd_config_int_exit, LIBCLI_PRIVILEGE_PRIVILEGED, MODE_CONFIG_INT,
                                                                "Exit from interface configuration");
    crtl_cli_register_command(cli, NULL, "address", cmd_test, LIBCLI_PRIVILEGE_PRIVILEGED, MODE_CONFIG_INT, 
                                                            "Set IP address");
    
    c = crtl_cli_register_command(cli, NULL, "debug", NULL, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC, NULL);
    crtl_cli_register_command(cli, c, "regular", cmd_debug_regular, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC,
                                                                    "Enable crtl_cli_regular() callback debugging");

    // Register some commands/subcommands to demonstrate opt/arg and buildmode operations

    c = crtl_cli_register_command(cli, NULL, "perimeter", cmd_perimeter, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC,
                                                            "Calculate perimeter of polygon");
    crtl_cli_register_optarg(c, "transparent", CLI_CMD_OPTIONAL_FLAG, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC,
                                                            "Set transparent flag", NULL, NULL, NULL);
    crtl_cli_register_optarg(c, "verbose", CLI_CMD_OPTIONAL_FLAG | CLI_CMD_OPTION_MULTIPLE, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC,
                                                        "Set transparent flag", NULL, NULL, NULL);
    crtl_cli_register_optarg(c, "color", CLI_CMD_OPTIONAL_ARGUMENT, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC, "Set color",
                                                        color_completor, color_validator, NULL);
    crtl_cli_register_optarg(c, "__check1__", CLI_CMD_SPOT_CHECK, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC, NULL, NULL,
                                                        check1_validator, NULL);
    crtl_cli_register_optarg(c, "shape", CLI_CMD_ARGUMENT | CLI_CMD_ALLOW_BUILDMODE, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC,
                                                        "Specify shape to calclate perimeter for", shape_completor, shape_validator,
                                                        shape_transient_eval);
    crtl_cli_register_optarg(c, "side_1", CLI_CMD_ARGUMENT, LIBCLI_PRIVILEGE_UNPRIVILEGED, MODE_POLYGON_TRIANGLE,
                                                        "Specify side 1 length", NULL, side_length_validator, NULL);
    crtl_cli_register_optarg(c, "side_1", CLI_CMD_ARGUMENT, LIBCLI_PRIVILEGE_UNPRIVILEGED, MODE_POLYGON_RECTANGLE,
                                                        "Specify side 1 length", NULL, side_length_validator, NULL);

    crtl_cli_register_optarg(c, "side_2", CLI_CMD_ARGUMENT, LIBCLI_PRIVILEGE_UNPRIVILEGED, MODE_POLYGON_TRIANGLE,
                                                        "Specify side 2 length", NULL, side_length_validator, NULL);
    crtl_cli_register_optarg(c, "side_2", CLI_CMD_ARGUMENT, LIBCLI_PRIVILEGE_UNPRIVILEGED, MODE_POLYGON_RECTANGLE,
                                                        "Specify side 2 length", NULL, side_length_validator, NULL);
    crtl_cli_register_optarg(c, "side_3", CLI_CMD_ARGUMENT, LIBCLI_PRIVILEGE_UNPRIVILEGED, MODE_POLYGON_TRIANGLE,
                                                        "Specify side 3 length", NULL, side_length_validator, NULL);
    crtl_cli_register_optarg(c, "side_3", CLI_CMD_ARGUMENT, LIBCLI_PRIVILEGE_UNPRIVILEGED, MODE_POLYGON_RECTANGLE,
                                                        "Specify side 3 length", NULL, side_length_validator, NULL);
    crtl_cli_register_optarg(c, "side_4", CLI_CMD_ARGUMENT, LIBCLI_PRIVILEGE_UNPRIVILEGED, MODE_POLYGON_RECTANGLE,
                                                        "Specify side 4 length", NULL, side_length_validator, NULL);

    // Set user context and its command
    crtl_cli_set_context(cli, (void *)&myctx);
    crtl_cli_register_command(cli, NULL, "context", cmd_context, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC,
                                                        "Test a user-specified context");

    crtl_cli_set_auth_callback(cli, check_auth);
    crtl_cli_set_enable_callback(cli, check_enable);
    
    {// Test reading from a file
        FILE *fh;

        if ((fh = fopen("clitest.txt", "r"))) {
            // This sets a callback which just displays the crtl_cli_print() text to stdout
            crtl_cli_print_callback(cli, pc);
            crtl_cli_file(cli, fh, LIBCLI_PRIVILEGE_UNPRIVILEGED, LIBCLI_MODE_EXEC);
            crtl_cli_print_callback(cli, NULL);
            fclose(fh);
        }
    }
    crtl_cli_loop(cli, acceptfd);
    crtl_cli_done(cli);
}



void *   __libcrtl_cli_callback(void*arg)
{
    int sockfd, acceptfd;
    struct sockaddr_in addr;
    int on = 1;

    signal(SIGCHLD/*Child status has changed (POSIX).*/, SIG_IGN/*sigignore*/);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return NULL;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) {
        perror("setsockopt");
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(CLITEST_PORT);
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
    {
        perror("bind");
        return NULL;
    }
    
    if (listen(sockfd, 50) < 0) 
    {
        perror("listen");
        return NULL;
    }
    
    printf("Listening on port %d\n", CLITEST_PORT);
    while ((acceptfd = accept(sockfd, NULL, 0))) 
    {
        int pid = fork();
        if (pid < 0) {
            perror("fork");
            return NULL;
        }

        /* parent */
        if (pid > 0) {
            socklen_t len = sizeof(addr);
            if (getpeername(acceptfd, (struct sockaddr *)&addr, &len) >= 0)
            printf(" * accepted connection from %s, port %d\n", inet_ntoa(addr.sin_addr), addr.sin_port);

            close(acceptfd);
            continue;
        }

        /* child */
        close(sockfd);
        run_child(acceptfd);
        exit(0);
    }

    return NULL;
}


void demo_libcrtl_cli_test1_terminal() 
{
    run_child(fileno(stdout));
    while(1)
    {
        sleep(1);
    }
}
void demo_libcrtl_cli_test2() 
{
    crtl_thread_t   task;

    crtl_thread_normal(&task, __libcrtl_cli_callback, NULL);
    
    while(1)
    {
        sleep(5);
        printf("CRTL main rutinue.\n");
    }
}


int main()
{
//    demo_libcrtl_cli_test1_terminal();

    demo_libcrtl_cli_test2();

    
    return 0;
}




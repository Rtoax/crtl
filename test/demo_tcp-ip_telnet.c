#include "crtl/crtl_signal.h"
#include <crtl/crtl_mask.h>
#include <crtl/log.h>
#include <crtl/string.h>
#include "crtl/tcp-ip/crtl_ether.h"
#include "crtl/tcp-ip/crtl_icmp.h"
#include "crtl/tcp-ip/crtl_telnet.h"



#ifdef HAVE_ZLIB
#include "zlib.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


static struct termios demo_tcp_ip_crtl_telnet_test1_orig_tios;
static crtl_telnet_t *demo_tcp_ip_crtl_telnet_test1_telnet;
static int demo_tcp_ip_crtl_telnet_test1_do_echo;

static const crtl_telnet_telopt_t demo_tcp_ip_crtl_telnet_test1_telopts[] = {
	{ CRTL_TELNET_TELOPT_ECHO,		CRTL_TELNET_WONT, CRTL_TELNET_DO   },
	{ CRTL_TELNET_TELOPT_TTYPE,		CRTL_TELNET_WILL, CRTL_TELNET_DONT },
	{ CRTL_TELNET_TELOPT_COMPRESS2,	CRTL_TELNET_WONT, CRTL_TELNET_DO   },
	{ CRTL_TELNET_TELOPT_MSSP,		CRTL_TELNET_WONT, CRTL_TELNET_DO   },
	{ -1, 0, 0 }
};

static void demo_tcp_ip_crtl_telnet_test1_cleanup(void) {
	tcsetattr(STDOUT_FILENO, TCSADRAIN, &demo_tcp_ip_crtl_telnet_test1_orig_tios);
}

static void demo_tcp_ip_crtl_telnet_test1_input(char *buffer, int size) {
	static char crlf[] = { '\r', '\n' };
	int i;

	for (i = 0; i != size; ++i) {
		/* if we got a CR or LF, replace with CRLF
		 * NOTE that usually you'd get a CR in UNIX, but in raw
		 * mode we get LF instead (not sure why)
		 */
		if (buffer[i] == '\r' || buffer[i] == '\n') {
			if (demo_tcp_ip_crtl_telnet_test1_do_echo)
				printf("\r\n");
			crtl_telnet_send(demo_tcp_ip_crtl_telnet_test1_telnet, crlf, 2);
		} else {
			if (demo_tcp_ip_crtl_telnet_test1_do_echo)
				putchar(buffer[i]);
			crtl_telnet_send(demo_tcp_ip_crtl_telnet_test1_telnet, buffer + i, 1);
		}
	}
	fflush(stdout);
}

static void demo_tcp_ip_crtl_telnet_test1_send(int sock, const char *buffer, size_t size) {
	int rs;

	/* send data */
	while (size > 0) {
		if ((rs = send(sock, buffer, size, 0)) == -1) {
			fprintf(stderr, "send() failed: %s\n", strerror(errno));
			exit(1);
		} else if (rs == 0) {
			fprintf(stderr, "send() unexpectedly returned 0\n");
			exit(1);
		}

		/* update pointer and size to see if we've got more to send */
		buffer += rs;
		size -= rs;
	}
}

static void demo_tcp_ip_crtl_telnet_test1_event_handler(crtl_telnet_t *demo_tcp_ip_crtl_telnet_test1_telnet, crtl_telnet_event_t *ev,	void *user_data) 
{
	int sock = *(int*)user_data;

	switch (ev->type) {
	/* data received */
	case CRTL_TELNET_EV_DATA:
		if (ev->data.size && fwrite(ev->data.buffer, 1, ev->data.size, stdout) != ev->data.size) {
              		fprintf(stderr, "ERROR: Could not write complete buffer to stdout");
		}
		fflush(stdout);
		break;
	/* data must be sent */
	case CRTL_TELNET_EV_SEND:
		demo_tcp_ip_crtl_telnet_test1_send(sock, ev->data.buffer, ev->data.size);
		break;
	/* request to enable remote feature (or receipt) */
	case CRTL_TELNET_EV_WILL:
		/* we'll agree to turn off our echo if server wants us to stop */
		if (ev->neg.telopt == CRTL_TELNET_TELOPT_ECHO)
			demo_tcp_ip_crtl_telnet_test1_do_echo = 0;
		break;
	/* notification of disabling remote feature (or receipt) */
	case CRTL_TELNET_EV_WONT:
		if (ev->neg.telopt == CRTL_TELNET_TELOPT_ECHO)
			demo_tcp_ip_crtl_telnet_test1_do_echo = 1;
		break;
	/* request to enable local feature (or receipt) */
	case CRTL_TELNET_EV_DO:
		break;
	/* demand to disable local feature (or receipt) */
	case CRTL_TELNET_EV_DONT:
		break;
	/* respond to TTYPE commands */
	case CRTL_TELNET_EV_TTYPE:
		/* respond with our terminal type, if requested */
		if (ev->ttype.cmd == CRTL_TELNET_TTYPE_SEND) {
			crtl_telnet_ttype_is(demo_tcp_ip_crtl_telnet_test1_telnet, getenv("TERM"));
		}
		break;
	/* respond to particular subnegotiations */
	case CRTL_TELNET_EV_SUBNEGOTIATION:
		break;
	/* error */
	case CRTL_TELNET_EV_ERROR:
		fprintf(stderr, "ERROR: %s\n", ev->err.msg);
		exit(1);
	default:
		/* ignore */
		break;
	}
}

static int _unused demo_tcp_ip_crtl_telnet_test1_crtl_telnet_client(int argc ,char *argv[])
{
	char buffer[512];
	int rs;
	int sock;
	struct sockaddr_in addr;
	struct pollfd pfd[2];
	struct addrinfo *ai;
	struct addrinfo hints;
	struct termios tios;
	const char *servname;
	const char *hostname;

	/* check usage */
	if (argc < 2) {
		fprintf(stderr, "Usage:\n ./demo_tcp_ip_crtl_telnet_test1_telnet-client <host> [port]\n");
		return 1;
	}

	/* process arguments */
	servname = (argc < 3) ? "23" : argv[2];
	hostname = argv[1];

	/* look up server host */
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((rs = getaddrinfo(hostname, servname, &hints, &ai)) != 0) {
		fprintf(stderr, "getaddrinfo() failed for %s: %s\n", hostname,
				gai_strerror(rs));
		return 1;
	}
	
	/* create server socket */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "socket() failed: %s\n", strerror(errno));
		return 1;
	}

	/* bind server socket */
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		fprintf(stderr, "bind() failed: %s\n", strerror(errno));
		close(sock);
		return 1;
	}

	/* connect */
	if (connect(sock, ai->ai_addr, ai->ai_addrlen) == -1) {
		fprintf(stderr, "connect() failed: %s\n", strerror(errno));
		close(sock);
		return 1;
	}

	/* free address lookup info */
	freeaddrinfo(ai);

	/* get current terminal settings, set raw mode, make sure we
	 * register atexit handler to restore terminal settings
	 */
	tcgetattr(STDOUT_FILENO, &demo_tcp_ip_crtl_telnet_test1_orig_tios);
	atexit(demo_tcp_ip_crtl_telnet_test1_cleanup);
	tios = demo_tcp_ip_crtl_telnet_test1_orig_tios;
	cfmakeraw(&tios);
	tcsetattr(STDOUT_FILENO, TCSADRAIN, &tios);

	/* set input echoing on by default */
	demo_tcp_ip_crtl_telnet_test1_do_echo = 1;

	/* initialize demo_tcp_ip_crtl_telnet_test1_telnet box */
	demo_tcp_ip_crtl_telnet_test1_telnet = crtl_telnet_init(demo_tcp_ip_crtl_telnet_test1_telopts, demo_tcp_ip_crtl_telnet_test1_event_handler, 0, &sock);

	/* initialize poll descriptors */
	memset(pfd, 0, sizeof(pfd));
	pfd[0].fd = STDIN_FILENO;
	pfd[0].events = POLLIN;
	pfd[1].fd = sock;
	pfd[1].events = POLLIN;

	/* loop while both connections are open */
	while (poll(pfd, 2, -1) != -1) {
		/* read from stdin */
		if (pfd[0].revents & POLLIN) {
			if ((rs = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
				demo_tcp_ip_crtl_telnet_test1_input(buffer, rs);
			} else if (rs == 0) {
				break;
			} else {
				fprintf(stderr, "recv(server) failed: %s\n",
						strerror(errno));
				exit(1);
			}
		}

		/* read from client */
		if (pfd[1].revents & POLLIN) {
			if ((rs = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
				crtl_telnet_recv(demo_tcp_ip_crtl_telnet_test1_telnet, buffer, rs);
			} else if (rs == 0) {
				break;
			} else {
				fprintf(stderr, "recv(client) failed: %s\n",
						strerror(errno));
				exit(1);
			}
		}
	}

	/* clean up */
	crtl_telnet_free(demo_tcp_ip_crtl_telnet_test1_telnet);
	close(sock);

	return 0;
}




int demo_tcp_ip_crtl_telnet_test1()
{
    int argc = 3;
    char *argv[] = {
        "demo_tcp_ip_crtl_telnet_test1_crtl_telnet_client",
        "10.170.6.66",
        "8000"
    };
    demo_tcp_ip_crtl_telnet_test1_crtl_telnet_client(argc, argv);


    return 0;    
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
 * Sean Middleditch
 * sean@sourcemud.org
 *
 * The author or authors of this code dedicate any and all copyright interest
 * in this code to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and successors. We
 * intend this dedication to be an overt act of relinquishment in perpetuity of
 * all present and future rights to this code under copyright law. 
 */


#define MAX_USERS 64
#define LINEBUFFER_SIZE 256

static const crtl_telnet_telopt_t demo_tcp_ip_crtl_telnet_test2_telopts[] = {
	{ CRTL_TELNET_TELOPT_COMPRESS2,	CRTL_TELNET_WILL, CRTL_TELNET_DONT },
	{ -1, 0, 0 }
};

struct user_t {
	char *name;
	int sock;
	crtl_telnet_t *telnet;
	char linebuf[256];
	int linepos;
};

static struct user_t demo_tcp_ip_crtl_telnet_test2_users[MAX_USERS];

static void demo_tcp_ip_crtl_telnet_test2_linebuffer_push(char *buffer, size_t size, int *linepos,
		char ch, void (*cb)(const char *line, size_t overflow, void *ud),
		void *ud) {

	/* CRLF -- line terminator */
	if (ch == '\n' && *linepos > 0 && buffer[*linepos - 1] == '\r') {
		/* NUL terminate (replaces \r in buffer), notify app, clear */
		buffer[*linepos - 1] = 0;
		cb(buffer, 0, ud);
		*linepos = 0;

	/* CRNUL -- just a CR */
	} else if (ch == 0 && *linepos > 0 && buffer[*linepos - 1] == '\r') {
		/* do nothing, the CR is already in the buffer */

	/* anything else (including technically invalid CR followed by
	 * anything besides LF or NUL -- just buffer if we have room
	 * \r
	 */
	} else if (*linepos != (int)size) {
		buffer[(*linepos)++] = ch;

	/* buffer overflow */
	} else {
		/* terminate (NOTE: eats a byte), notify app, clear buffer */
		buffer[size - 1] = 0;
		cb(buffer, size - 1, ud);
		*linepos = 0;
	}
}

static void demo_tcp_ip_crtl_telnet_test2__message(const char *from, const char *msg) {
	int i;
	for (i = 0; i != MAX_USERS; ++i) {
		if (demo_tcp_ip_crtl_telnet_test2_users[i].sock != -1) {
			crtl_telnet_printf(demo_tcp_ip_crtl_telnet_test2_users[i].telnet, "%s: %s\n", from, msg);
		}
	}
}

static void demo_tcp_ip_crtl_telnet_test2__send(int sock, const char *buffer, size_t size) {
	int rs;

	/* ignore on invalid socket */
	if (sock == -1)
		return;

	/* send data */
	while (size > 0) {
		if ((rs = send(sock, buffer, (int)size, 0)) == -1) {
			if (errno != EINTR && errno != ECONNRESET) {
				fprintf(stderr, "send() failed: %s\n", strerror(errno));
				exit(1);
			} else {
				return;
			}
		} else if (rs == 0) {
			fprintf(stderr, "send() unexpectedly returned 0\n");
			exit(1);
		}

		/* update pointer and size to see if we've got more to send */
		buffer += rs;
		size -= rs;
	}
}

/* process input line */
static void demo_tcp_ip_crtl_telnet_test2___online(const char *line, size_t overflow, void *ud) {
	struct user_t *user = (struct user_t*)ud;
	int i;

	(void)overflow;

	/* if the user has no name, this is his "login" */
	if (user->name == 0) {
		/* must not be empty, must be at least 32 chars */
		if (strlen(line) == 0 || strlen(line) > 32) {
			crtl_telnet_printf(user->telnet, "Invalid name.\nEnter name: ");
			return;
		}

		/* must not already be in use */
		for (i = 0; i != MAX_USERS; ++i) {
			if (demo_tcp_ip_crtl_telnet_test2_users[i].name != 0 && strcmp(demo_tcp_ip_crtl_telnet_test2_users[i].name, line) == 0) {
				crtl_telnet_printf(user->telnet, "Name in use.\nEnter name: ");
				return;
			}
		}

		/* keep name */
		user->name = strdup(line);
		crtl_telnet_printf(user->telnet, "Welcome, %s!\n", line);
		return;
	}

	/* if line is "quit" then, well, quit */
	if (strcmp(line, "quit") == 0) {
		close(user->sock);
		user->sock = -1;
		demo_tcp_ip_crtl_telnet_test2__message(user->name, "** HAS QUIT **");
		free(user->name);
		user->name = 0;
		return;
	}

	/* just a message -- send to all demo_tcp_ip_crtl_telnet_test2_users */
	demo_tcp_ip_crtl_telnet_test2__message(user->name, line);
}

static void demo_tcp_ip_crtl_telnet_test2__input(struct user_t *user, const char *buffer,
		size_t size) {
	unsigned int i;
	for (i = 0; i != size; ++i)
		demo_tcp_ip_crtl_telnet_test2_linebuffer_push(user->linebuf, sizeof(user->linebuf), &user->linepos,
				(char)buffer[i], demo_tcp_ip_crtl_telnet_test2___online, user);
}

static void demo_tcp_ip_crtl_telnet_test2__event_handler(crtl_telnet_t *telnet, crtl_telnet_event_t *ev,
		void *user_data) {
	struct user_t *user = (struct user_t*)user_data;

	switch (ev->type) {
	/* data received */
	case CRTL_TELNET_EV_DATA:
		demo_tcp_ip_crtl_telnet_test2__input(user, ev->data.buffer, ev->data.size);
					crtl_telnet_negotiate(telnet, CRTL_TELNET_WONT, CRTL_TELNET_TELOPT_ECHO);
			crtl_telnet_negotiate(telnet, CRTL_TELNET_WILL, CRTL_TELNET_TELOPT_ECHO);
		break;
	/* data must be sent */
	case CRTL_TELNET_EV_SEND:
		demo_tcp_ip_crtl_telnet_test2__send(user->sock, ev->data.buffer, ev->data.size);
		break;
	/* enable compress2 if accepted by client */
	case CRTL_TELNET_EV_DO:
		if (ev->neg.telopt == CRTL_TELNET_TELOPT_COMPRESS2)
			crtl_telnet_begin_compress2(telnet);
		break;
	/* error */
	case CRTL_TELNET_EV_ERROR:
		close(user->sock);
		user->sock = -1;
		if (user->name != 0) {
			demo_tcp_ip_crtl_telnet_test2__message(user->name, "** HAS HAD AN ERROR **");
			free(user->name);
			user->name = 0;
		}
		crtl_telnet_free(user->telnet);
		break;
	default:
		/* ignore */
		break;
	}
}

static int _unused demo_tcp_ip_crtl_telnet_test2_crtl_telnet_chat(int argc, char **argv) 
{
	char buffer[512];
	short listen_port;
	int listen_sock;
	int client_sock;
	int rs;
	int i;
	struct sockaddr_in addr;
	socklen_t addrlen;
	struct pollfd pfd[MAX_USERS + 1];

	/* initialize Winsock */
#if defined(_WIN32)
	WSADATA wsd;
	WSAStartup(MAKEWORD(2, 2), &wsd);
#endif

	/* check usage */
	if (argc != 2) {
		fprintf(stderr, "Usage:\n ./telnet-chatd <port>\n");
		return 1;
	}

	/* initialize data structures */
	memset(&pfd, 0, sizeof(pfd));
	memset(demo_tcp_ip_crtl_telnet_test2_users, 0, sizeof(demo_tcp_ip_crtl_telnet_test2_users));
	for (i = 0; i != MAX_USERS; ++i)
		demo_tcp_ip_crtl_telnet_test2_users[i].sock = -1;

	/* parse listening port */
	listen_port = (short)strtol(argv[1], 0, 10);

	/* create listening socket */
	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "socket() failed: %s\n", strerror(errno));
		return 1;
	}

	/* reuse address option */
	rs = 1;
	setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&rs, sizeof(rs));

	/* bind to listening addr/port */
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(listen_port);
	if (bind(listen_sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		fprintf(stderr, "bind() failed: %s\n", strerror(errno));
		close(listen_sock);
		return 1;
	}

	/* listen for clients */
	if (listen(listen_sock, 5) == -1) {
		fprintf(stderr, "listen() failed: %s\n", strerror(errno));
		close(listen_sock);
		return 1;
	}

	printf("LISTENING ON PORT %d\n", listen_port);

	/* initialize listening descriptors */
	pfd[MAX_USERS].fd = listen_sock;
	pfd[MAX_USERS].events = POLLIN;

	/* loop for ever */
	for (;;) {
		/* prepare for poll */
		for (i = 0; i != MAX_USERS; ++i) {
			if (demo_tcp_ip_crtl_telnet_test2_users[i].sock != -1) {
				pfd[i].fd = demo_tcp_ip_crtl_telnet_test2_users[i].sock;
				pfd[i].events = POLLIN;
			} else {
				pfd[i].fd = -1;
				pfd[i].events = 0;
			}
		}

		/* poll */
		rs = poll(pfd, MAX_USERS + 1, -1);
		if (rs == -1 && errno != EINTR) {
			fprintf(stderr, "poll() failed: %s\n", strerror(errno));
			close(listen_sock);
			return 1;
		}

		/* new connection */
		if (pfd[MAX_USERS].revents & POLLIN) {
			/* acept the sock */
			addrlen = sizeof(addr);
			if ((client_sock = accept(listen_sock, (struct sockaddr *)&addr,
					&addrlen)) == -1) {
				fprintf(stderr, "accept() failed: %s\n", strerror(errno));
				return 1;
			}

			printf("Connection received.\n");

			/* find a free user */
			for (i = 0; i != MAX_USERS; ++i)
				if (demo_tcp_ip_crtl_telnet_test2_users[i].sock == -1)
					break;
			if (i == MAX_USERS) {
				printf("  rejected (too many demo_tcp_ip_crtl_telnet_test2_users)\n");
				demo_tcp_ip_crtl_telnet_test2__send(client_sock, "Too many demo_tcp_ip_crtl_telnet_test2_users.\r\n", 14);
				close(client_sock);
			}

			/* init, welcome */
			demo_tcp_ip_crtl_telnet_test2_users[i].sock = client_sock;
			demo_tcp_ip_crtl_telnet_test2_users[i].telnet = crtl_telnet_init(demo_tcp_ip_crtl_telnet_test2_telopts, demo_tcp_ip_crtl_telnet_test2__event_handler, 0,
					&demo_tcp_ip_crtl_telnet_test2_users[i]);
			crtl_telnet_negotiate(demo_tcp_ip_crtl_telnet_test2_users[i].telnet, CRTL_TELNET_WILL,
					CRTL_TELNET_TELOPT_COMPRESS2);
			crtl_telnet_printf(demo_tcp_ip_crtl_telnet_test2_users[i].telnet, "Enter name: ");

			crtl_telnet_negotiate(demo_tcp_ip_crtl_telnet_test2_users[i].telnet, CRTL_TELNET_WILL, CRTL_TELNET_TELOPT_ECHO);
		}

		/* read from client */
		for (i = 0; i != MAX_USERS; ++i) {
			/* skip demo_tcp_ip_crtl_telnet_test2_users that aren't actually connected */
			if (demo_tcp_ip_crtl_telnet_test2_users[i].sock == -1)
				continue;

			if (pfd[i].revents & POLLIN) {
				if ((rs = recv(demo_tcp_ip_crtl_telnet_test2_users[i].sock, buffer, sizeof(buffer), 0)) > 0) {
					crtl_telnet_recv(demo_tcp_ip_crtl_telnet_test2_users[i].telnet, buffer, rs);
				} else if (rs == 0) {
					printf("Connection closed.\n");
					close(demo_tcp_ip_crtl_telnet_test2_users[i].sock);
					if (demo_tcp_ip_crtl_telnet_test2_users[i].name != 0) {
						demo_tcp_ip_crtl_telnet_test2__message(demo_tcp_ip_crtl_telnet_test2_users[i].name, "** HAS DISCONNECTED **");
						free(demo_tcp_ip_crtl_telnet_test2_users[i].name);
						demo_tcp_ip_crtl_telnet_test2_users[i].name = 0;
					}
					crtl_telnet_free(demo_tcp_ip_crtl_telnet_test2_users[i].telnet);
					demo_tcp_ip_crtl_telnet_test2_users[i].sock = -1;
					break;
				} else if (errno != EINTR) {
					fprintf(stderr, "recv(client) failed: %s\n",
							strerror(errno));
					exit(1);
				}
			}
		}
	}

	/* not that we can reach this, but GCC will cry if it's not here */
	return 0;
}

int demo_tcp_ip_crtl_telnet_test2()
{
    int argc = 2;
    char *argv[] = {
        "demo_tcp_ip_crtl_telnet_test2_crtl_telnet_chat",
        "8000"
    };
    demo_tcp_ip_crtl_telnet_test2_crtl_telnet_chat(argc, argv);


    return 0; 
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef ENABLE_COLOR
# define COLOR_SERVER "\e[35m"
# define COLOR_CLIENT "\e[34m"
# define COLOR_BOLD "\e[1m"
# define COLOR_UNBOLD "\e[22m"
# define COLOR_NORMAL "\e[0m"
#else
# define COLOR_SERVER ""
# define COLOR_CLIENT ""
# define COLOR_BOLD ""
# define COLOR_UNBOLD ""
# define COLOR_NORMAL ""
#endif

struct conn_t {
	const char *name;
	int sock;
	crtl_telnet_t *telnet;
	struct conn_t *remote;
};

static const char *demo_tcp_ip_crtl_telnet_test3_get_cmd(unsigned char cmd) {
	static char buffer[4];

	switch (cmd) {
	case 255: return "IAC";
	case 254: return "DONT";
	case 253: return "DO";
	case 252: return "WONT";
	case 251: return "WILL";
	case 250: return "SB";
	case 249: return "GA";
	case 248: return "EL";
	case 247: return "EC";
	case 246: return "AYT";
	case 245: return "AO";
	case 244: return "IP";
	case 243: return "BREAK";
	case 242: return "DM";
	case 241: return "NOP";
	case 240: return "SE";
	case 239: return "EOR";
	case 238: return "ABORT";
	case 237: return "SUSP";
	case 236: return "xEOF";
	default:
		snprintf(buffer, sizeof(buffer), "%d", (int)cmd);
		return buffer;
	}
}

static const char *demo_tcp_ip_crtl_telnet_test3_get_opt(unsigned char opt) {
	switch (opt) {
	case 0: return "BINARY";
	case 1: return "ECHO";
	case 2: return "RCP";
	case 3: return "SGA";
	case 4: return "NAMS";
	case 5: return "STATUS";
	case 6: return "TM";
	case 7: return "RCTE";
	case 8: return "NAOL";
	case 9: return "NAOP";
	case 10: return "NAOCRD";
	case 11: return "NAOHTS";
	case 12: return "NAOHTD";
	case 13: return "NAOFFD";
	case 14: return "NAOVTS";
	case 15: return "NAOVTD";
	case 16: return "NAOLFD";
	case 17: return "XASCII";
	case 18: return "LOGOUT";
	case 19: return "BM";
	case 20: return "DET";
	case 21: return "SUPDUP";
	case 22: return "SUPDUPOUTPUT";
	case 23: return "SNDLOC";
	case 24: return "TTYPE";
	case 25: return "EOR";
	case 26: return "TUID";
	case 27: return "OUTMRK";
	case 28: return "TTYLOC";
	case 29: return "3270REGIME";
	case 30: return "X3PAD";
	case 31: return "NAWS";
	case 32: return "TSPEED";
	case 33: return "LFLOW";
	case 34: return "LINEMODE";
	case 35: return "XDISPLOC";
	case 36: return "ENVIRON";
	case 37: return "AUTHENTICATION";
	case 38: return "ENCRYPT";
	case 39: return "NEW-ENVIRON";
	case 70: return "MSSP";
	case 85: return "COMPRESS";
	case 86: return "COMPRESS2";
	case 93: return "ZMP";
	case 255: return "EXOPL";
	default: return "unknown";
	}
}

static void demo_tcp_ip_crtl_telnet_test3__print_buffer(const char *buffer, size_t size) {
	size_t i;

	printf("%.*s [", (int)size, buffer);
	for (i = 0; i != size; ++i) {
		printf("<" COLOR_BOLD "0x%02X" COLOR_UNBOLD ">", (unsigned char)buffer[i]);
		if(buffer[i] == '\n') printf("%c", '\n');
	}
	printf("]");
}

static void demo_tcp_ip_crtl_telnet_test3___send(int sock, const char *buffer, size_t size) {
	int rs;

	/* send data */
	while (size > 0) {
		if ((rs = send(sock, buffer, (int)size, 0)) == -1) {
			if (errno != EINTR && errno != ECONNRESET) {
				fprintf(stderr, "send() failed: %s\n", strerror(errno));
				exit(1);
			} else {
				return;
			}
		} else if (rs == 0) {
			fprintf(stderr, "send() unexpectedly returned 0\n");
			exit(1);
		}

		/* update pointer and size to see if we've got more to send */
		buffer += rs;
		size -= rs;
	}
}

static void demo_tcp_ip_crtl_telnet_test3__event_handler(crtl_telnet_t *telnet, crtl_telnet_event_t *ev,
		void *user_data) {
	struct conn_t *conn = (struct conn_t*)user_data;

	(void)telnet;

	switch (ev->type) {
	/* data received */
	case CRTL_TELNET_EV_DATA:
		printf("%s DATA: ", conn->name);
		demo_tcp_ip_crtl_telnet_test3__print_buffer(ev->data.buffer, ev->data.size);
		printf(COLOR_NORMAL "\n");

		crtl_telnet_send(conn->remote->telnet, ev->data.buffer, ev->data.size);
		break;
	/* data must be sent */
	case CRTL_TELNET_EV_SEND:
		/* DONT SPAM
		printf("%s SEND: ", conn->name);
		demo_tcp_ip_crtl_telnet_test3__print_buffer(ev->buffer, ev->size);
		printf(COLOR_BOLD "\n");
		*/

		demo_tcp_ip_crtl_telnet_test3___send(conn->sock, ev->data.buffer, ev->data.size);
		break;
	/* IAC command */
	case CRTL_TELNET_EV_IAC:
		printf("%s IAC %s" COLOR_NORMAL "\n", conn->name,
				demo_tcp_ip_crtl_telnet_test3_get_cmd(ev->iac.cmd));

		crtl_telnet_iac(conn->remote->telnet, ev->iac.cmd);
		break;
	/* negotiation, WILL */
	case CRTL_TELNET_EV_WILL:
		printf("%s IAC WILL %d (%s)" COLOR_NORMAL "\n", conn->name,
				(int)ev->neg.telopt, demo_tcp_ip_crtl_telnet_test3_get_opt(ev->neg.telopt));
		crtl_telnet_negotiate(conn->remote->telnet, CRTL_TELNET_WILL,
				ev->neg.telopt);
		break;
	/* negotiation, WONT */
	case CRTL_TELNET_EV_WONT:
		printf("%s IAC WONT %d (%s)" COLOR_NORMAL "\n", conn->name,
				(int)ev->neg.telopt, demo_tcp_ip_crtl_telnet_test3_get_opt(ev->neg.telopt));
		crtl_telnet_negotiate(conn->remote->telnet, CRTL_TELNET_WONT,
				ev->neg.telopt);
		break;
	/* negotiation, DO */
	case CRTL_TELNET_EV_DO:
		printf("%s IAC DO %d (%s)" COLOR_NORMAL "\n", conn->name,
				(int)ev->neg.telopt, demo_tcp_ip_crtl_telnet_test3_get_opt(ev->neg.telopt));
		crtl_telnet_negotiate(conn->remote->telnet, CRTL_TELNET_DO,
				ev->neg.telopt);
		break;
	case CRTL_TELNET_EV_DONT:
		printf("%s IAC DONT %d (%s)" COLOR_NORMAL "\n", conn->name,
				(int)ev->neg.telopt, demo_tcp_ip_crtl_telnet_test3_get_opt(ev->neg.telopt));
		crtl_telnet_negotiate(conn->remote->telnet, CRTL_TELNET_DONT,
				ev->neg.telopt);
		break;
	/* generic subnegotiation */
	case CRTL_TELNET_EV_SUBNEGOTIATION:
		printf("%s SUB %d (%s)", conn->name, (int)ev->sub.telopt,
				demo_tcp_ip_crtl_telnet_test3_get_opt(ev->sub.telopt));
		if (ev->sub.size > 0) {
			printf(" [%ld bytes]: ", (long)ev->sub.size);
			demo_tcp_ip_crtl_telnet_test3__print_buffer(ev->sub.buffer, ev->sub.size);
		}
		printf(COLOR_NORMAL "\n");

		/* forward */
		crtl_telnet_subnegotiation(conn->remote->telnet, ev->sub.telopt,
				ev->sub.buffer, ev->sub.size);
		break;
	/* ZMP command */
	case CRTL_TELNET_EV_ZMP:
		if (ev->zmp.argc != 0) {
			size_t i;
			printf("%s ZMP [%ld params]", conn->name, (long)ev->zmp.argc);
			for (i = 0; i != ev->zmp.argc; ++i) {
				printf(" \"");
				demo_tcp_ip_crtl_telnet_test3__print_buffer(ev->zmp.argv[i], strlen(ev->zmp.argv[i]));
				printf("\"");
			}
			printf(COLOR_NORMAL "\n");
		}
		break;
	/* TERMINAL-TYPE command */
	case CRTL_TELNET_EV_TTYPE:
		printf("%s TTYPE %s %s", conn->name, ev->ttype.cmd ? "SEND" : "IS",
				ev->ttype.name ? ev->ttype.name : "");
		break;
	/* ENVIRON/NEW-ENVIRON commands */
	case CRTL_TELNET_EV_ENVIRON: {
		size_t i;
		printf("%s ENVIRON (%s) [%ld parts]", conn->name, ev->environ.cmd == CRTL_TELNET_ENVIRON_IS ? "IS" : ev->environ.cmd == CRTL_TELNET_ENVIRON_SEND ? "SEND" : "INFO", (long)ev->environ.size);
		for (i = 0; i != ev->environ.size; ++i) {
			printf(" %s \"", ev->environ.values[i].type == CRTL_TELNET_ENVIRON_VAR ? "VAR" : "USERVAR");
			if (ev->environ.values[i].var != 0) {
				demo_tcp_ip_crtl_telnet_test3__print_buffer(ev->environ.values[i].var, strlen(ev->environ.values[i].var));
			}
			if (ev->environ.cmd != CRTL_TELNET_ENVIRON_SEND) {
				printf("\"=\"");
				if (ev->environ.values[i].value != 0) {
					demo_tcp_ip_crtl_telnet_test3__print_buffer(ev->environ.values[i].value, strlen(ev->environ.values[i].value));
				}
				printf("\"");
			}
		}
		printf(COLOR_NORMAL "\n");
		break;
	}
	case CRTL_TELNET_EV_MSSP: {
		size_t i;
		printf("%s MSSP [%ld parts]", conn->name, (long)ev->mssp.size);
		for (i = 0; i != ev->mssp.size; ++i) {
			printf(" \"");
			demo_tcp_ip_crtl_telnet_test3__print_buffer(ev->mssp.values[i].var, strlen(ev->mssp.values[i].var));
			printf("\"=\"");
			demo_tcp_ip_crtl_telnet_test3__print_buffer(ev->mssp.values[i].value, strlen(ev->mssp.values[i].value));
			printf("\"");
		}
		printf(COLOR_NORMAL "\n");
		break;
	}
	/* compression notification */
	case CRTL_TELNET_EV_COMPRESS:
		printf("%s COMPRESSION %s" COLOR_NORMAL "\n", conn->name,
				ev->compress.state ? "ON" : "OFF");
		break;
	/* warning */
	case CRTL_TELNET_EV_WARNING:
		printf("%s WARNING: %s in %s,%d: %s" COLOR_NORMAL "\n", conn->name,
				ev->err.func, ev->err.file, ev->err.line, ev->err.msg);
		break;
	/* error */
	case CRTL_TELNET_EV_ERROR:
		printf("%s ERROR: %s in %s,%d: %s" COLOR_NORMAL "\n", conn->name,
				ev->err.func, ev->err.file, ev->err.line, ev->err.msg);
		exit(1);
	}
}

static int _unused demo_tcp_ip_crtl_telnet_test3_crtl_telnet_proxy(int argc, char **argv) 
{
	char buffer[512];
	short listen_port;
	int listen_sock;
	int rs;
	struct sockaddr_in addr;
	socklen_t addrlen;
	struct pollfd pfd[2];
	struct conn_t server;
	struct conn_t client;
	struct addrinfo *ai;
	struct addrinfo hints;
    
	/* check usage */
	if (argc != 4) {
		fprintf(stderr, "Usage:\n ./telnet-proxy <remote ip> <remote port> "
				"<local port>\n");
		return 1;
	}

	/* parse listening port */
	listen_port = (short)strtol(argv[3], 0, 10);

	/* loop forever, until user kills process */
	for (;;) {
		/* create listening socket */
		if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			fprintf(stderr, "socket() failed: %s\n", strerror(errno));
			return 1;
		}

		/* reuse address option */
		rs = 1;
		setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&rs, sizeof(rs));

		/* bind to listening addr/port */
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(listen_port);
		if (bind(listen_sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
			fprintf(stderr, "bind() failed: %s\n", strerror(errno));
			close(listen_sock);
			return 1;
		}

		printf("LISTENING ON PORT %d\n", listen_port);

		/* wait for client */
		if (listen(listen_sock, 1) == -1) {
			fprintf(stderr, "listen() failed: %s\n", strerror(errno));
			close(listen_sock);
			return 1;
		}
		addrlen = sizeof(addr);
		if ((client.sock = accept(listen_sock, (struct sockaddr *)&addr,
				&addrlen)) == -1) {
			fprintf(stderr, "accept() failed: %s\n", strerror(errno));
			close(listen_sock);
			return 1;
		}

		printf("CLIENT CONNECTION RECEIVED\n");
		
		/* stop listening now that we have a client */
		close(listen_sock);

		/* look up server host */
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		if ((rs = getaddrinfo(argv[1], argv[2], &hints, &ai)) != 0) {
			fprintf(stderr, "getaddrinfo() failed for %s: %s\n", argv[1],
					gai_strerror(rs));
			close(client.sock);
			return 1;
		}
		
		/* create server socket */
		if ((server.sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			fprintf(stderr, "socket() failed: %s\n", strerror(errno));
			close(server.sock);
			return 1;
		}

		/* bind server socket */
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		if (bind(server.sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
			fprintf(stderr, "bind() failed: %s\n", strerror(errno));
			close(server.sock);
			return 1;
		}

		/* connect */
		if (connect(server.sock, ai->ai_addr, (int)ai->ai_addrlen) == -1) {
			fprintf(stderr, "server() failed: %s\n", strerror(errno));
			close(server.sock);
			return 1;
		}

		/* free address lookup info */
		freeaddrinfo(ai);

		printf("SERVER CONNECTION ESTABLISHED\n");

		/* initialize connection structs */
		server.name = COLOR_SERVER "SERVER";
		server.remote = &client;
		client.name = COLOR_CLIENT "CLIENT";
		client.remote = &server;

		/* initialize telnet boxes */
		server.telnet = crtl_telnet_init(0, demo_tcp_ip_crtl_telnet_test3__event_handler, CRTL_TELNET_FLAG_PROXY,
				&server);
		client.telnet = crtl_telnet_init(0, demo_tcp_ip_crtl_telnet_test3__event_handler, CRTL_TELNET_FLAG_PROXY,
				&client);

		/* initialize poll descriptors */
		memset(pfd, 0, sizeof(pfd));
		pfd[0].fd = server.sock;
		pfd[0].events = POLLIN;
		pfd[1].fd = client.sock;
		pfd[1].events = POLLIN;

		/* loop while both connections are open */
		while (poll(pfd, 2, -1) != -1) {
			/* read from server */
			if (pfd[0].revents & POLLIN) {
				if ((rs = recv(server.sock, buffer, sizeof(buffer), 0)) > 0) {
					crtl_telnet_recv(server.telnet, buffer, rs);
				} else if (rs == 0) {
					printf("%s DISCONNECTED" COLOR_NORMAL "\n", server.name);
					break;
				} else {
					if (errno != EINTR && errno != ECONNRESET) {
						fprintf(stderr, "recv(server) failed: %s\n",
								strerror(errno));
						exit(1);
					}
				}
			}

			/* read from client */
			if (pfd[1].revents & POLLIN) {
				if ((rs = recv(client.sock, buffer, sizeof(buffer), 0)) > 0) {
					crtl_telnet_recv(client.telnet, buffer, rs);
				} else if (rs == 0) {
					printf("%s DISCONNECTED" COLOR_NORMAL "\n", client.name);
					break;
				} else {
					if (errno != EINTR && errno != ECONNRESET) {
						fprintf(stderr, "recv(server) failed: %s\n",
								strerror(errno));
						exit(1);
					}
				}
			}
		}

		/* clean up */
		crtl_telnet_free(server.telnet);
		crtl_telnet_free(client.telnet);
		close(server.sock);
		close(client.sock);

		/* all done */
		printf("BOTH CONNECTIONS CLOSED\n");
	}

	/* not that we can reach this, but GCC will cry if it's not here */
	return 0;
}


int demo_tcp_ip_crtl_telnet_test3()
{
    int argc = 4;
    char *argv[] = {
        "demo_tcp_ip_crtl_telnet_test3_crtl_telnet_proxy",
        "10.170.6.66",
        "8000",
        "8000"
    };
    demo_tcp_ip_crtl_telnet_test3_crtl_telnet_proxy(argc, argv);

    return 0;
}


int main()
{
//    demo_tcp_ip_crtl_telnet_test1();
    demo_tcp_ip_crtl_telnet_test2();
    demo_tcp_ip_crtl_telnet_test3();
    
    return 0;
}


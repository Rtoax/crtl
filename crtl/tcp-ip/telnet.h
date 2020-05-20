#ifndef __CRTL_TCP_IP_CRTL_TELNET_H
#define __CRTL_TCP_IP_CRTL_TELNET_H 1

#if !defined(_BSD_SOURCE)
#define _BSD_SOURCE
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>

#include "crtl/easy/attribute.h"


#ifndef __RT_TCP_IP_LIBCRTL_TELNET_H
#define __RT_TCP_IP_LIBCRTL_TELNET_H 1

/*!
 * \brief libtelnet - TELNET protocol handling library
 *
 * SUMMARY:
 *
 * libtelnet is a library for handling the TELNET protocol.  It includes
 * routines for parsing incoming data from a remote peer as well as formatting
 * data to send to the remote peer.
 *
 * libtelnet uses a callback-oriented API, allowing application-specific
 * handling of various events.  The callback system is also used for buffering
 * outgoing protocol data, allowing the application to maintain control over
 * the actual socket connection.
 *
 * Features supported include the full TELNET protocol, Q-method option
 * negotiation, ZMP, MCCP2, MSSP, and NEW-ENVIRON.
 *
 * CONFORMS TO:
 *
 * RFC854  - http://www.faqs.org/rfcs/rfc854.html
 * RFC855  - http://www.faqs.org/rfcs/rfc855.html
 * RFC1091 - http://www.faqs.org/rfcs/rfc1091.html
 * RFC1143 - http://www.faqs.org/rfcs/rfc1143.html
 * RFC1408 - http://www.faqs.org/rfcs/rfc1408.html
 * RFC1572 - http://www.faqs.org/rfcs/rfc1572.html
 *
 * LICENSE:
 *
 * The author or authors of this code dedicate any and all copyright interest
 * in this code to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and successors. We
 * intend this dedication to be an overt act of relinquishment in perpetuity of
 * all present and future rights to this code under copyright law.
 *
 * \file telnet.h
 *
 * \version 0.23
 *
 * \author Sean Middleditch <sean@sourcemud.org>
 */

#if !defined(LIBCRTL_TELNET_INCLUDE)
#define LIBCRTL_TELNET_INCLUDE 1

/* standard C headers necessary for the libtelnet API */
#include <stdarg.h>
#include <stddef.h>

/* C++ support */
#if defined(__cplusplus)
extern "C" {
#endif

/* printf type checking feature in GCC and some other compilers */
#if __GNUC__
# define CRTL_TELNET_GNU_PRINTF(f,a) __attribute__((format(printf, f, a))) /*!< internal helper */
# define CRTL_TELNET_GNU_SENTINEL __attribute__((sentinel)) /*!< internal helper */
#else
# define CRTL_TELNET_GNU_PRINTF(f,a) /*!< internal helper */
# define CRTL_TELNET_GNU_SENTINEL /*!< internal helper */
#endif

/* Disable environ macro for Visual C++ 2015. */
#undef environ

/*! Telnet state tracker object type. */
typedef struct crtl_telnet_t crtl_telnet_t;

/*! Telnet event object type. */
typedef union crtl_telnet_event_t crtl_telnet_event_t;

/*! Telnet option table element type. */
typedef struct crtl_telnet_telopt_t crtl_telnet_telopt_t;

/*! \name Telnet commands */
/*@{*/
/*! Telnet commands and special values. */
#define CRTL_TELNET_IAC 255
#define CRTL_TELNET_DONT 254
#define CRTL_TELNET_DO 253
#define CRTL_TELNET_WONT 252
#define CRTL_TELNET_WILL 251
#define CRTL_TELNET_SB 250
#define CRTL_TELNET_GA 249
#define CRTL_TELNET_EL 248
#define CRTL_TELNET_EC 247
#define CRTL_TELNET_AYT 246
#define CRTL_TELNET_AO 245
#define CRTL_TELNET_IP 244
#define CRTL_TELNET_BREAK 243
#define CRTL_TELNET_DM 242
#define CRTL_TELNET_NOP 241
#define CRTL_TELNET_SE 240
#define CRTL_TELNET_EOR 239
#define CRTL_TELNET_ABORT 238
#define CRTL_TELNET_SUSP 237
#define CRTL_TELNET_EOF 236
/*@}*/

/*! \name Telnet option values. */
/*@{*/
/*! Telnet options. */
#define CRTL_TELNET_TELOPT_BINARY 0
#define CRTL_TELNET_TELOPT_ECHO 1
#define CRTL_TELNET_TELOPT_RCP 2
#define CRTL_TELNET_TELOPT_SGA 3
#define CRTL_TELNET_TELOPT_NAMS 4
#define CRTL_TELNET_TELOPT_STATUS 5
#define CRTL_TELNET_TELOPT_TM 6
#define CRTL_TELNET_TELOPT_RCTE 7
#define CRTL_TELNET_TELOPT_NAOL 8
#define CRTL_TELNET_TELOPT_NAOP 9
#define CRTL_TELNET_TELOPT_NAOCRD 10
#define CRTL_TELNET_TELOPT_NAOHTS 11
#define CRTL_TELNET_TELOPT_NAOHTD 12
#define CRTL_TELNET_TELOPT_NAOFFD 13
#define CRTL_TELNET_TELOPT_NAOVTS 14
#define CRTL_TELNET_TELOPT_NAOVTD 15
#define CRTL_TELNET_TELOPT_NAOLFD 16
#define CRTL_TELNET_TELOPT_XASCII 17
#define CRTL_TELNET_TELOPT_LOGOUT 18
#define CRTL_TELNET_TELOPT_BM 19
#define CRTL_TELNET_TELOPT_DET 20
#define CRTL_TELNET_TELOPT_SUPDUP 21
#define CRTL_TELNET_TELOPT_SUPDUPOUTPUT 22
#define CRTL_TELNET_TELOPT_SNDLOC 23
#define CRTL_TELNET_TELOPT_TTYPE 24
#define CRTL_TELNET_TELOPT_EOR 25
#define CRTL_TELNET_TELOPT_TUID 26
#define CRTL_TELNET_TELOPT_OUTMRK 27
#define CRTL_TELNET_TELOPT_TTYLOC 28
#define CRTL_TELNET_TELOPT_3270REGIME 29
#define CRTL_TELNET_TELOPT_X3PAD 30
#define CRTL_TELNET_TELOPT_NAWS 31
#define CRTL_TELNET_TELOPT_TSPEED 32
#define CRTL_TELNET_TELOPT_LFLOW 33
#define CRTL_TELNET_TELOPT_LINEMODE 34
#define CRTL_TELNET_TELOPT_XDISPLOC 35
#define CRTL_TELNET_TELOPT_ENVIRON 36
#define CRTL_TELNET_TELOPT_AUTHENTICATION 37
#define CRTL_TELNET_TELOPT_ENCRYPT 38
#define CRTL_TELNET_TELOPT_NEW_ENVIRON 39
#define CRTL_TELNET_TELOPT_MSSP 70
#define CRTL_TELNET_TELOPT_COMPRESS 85
#define CRTL_TELNET_TELOPT_COMPRESS2 86
#define CRTL_TELNET_TELOPT_ZMP 93
#define CRTL_TELNET_TELOPT_EXOPL 255

#define CRTL_TELNET_TELOPT_MCCP2 86
/*@}*/

/*! \name Protocol codes for TERMINAL-TYPE commands. */
/*@{*/
/*! TERMINAL-TYPE codes. */
#define CRTL_TELNET_TTYPE_IS 0
#define CRTL_TELNET_TTYPE_SEND 1
/*@}*/

/*! \name Protocol codes for NEW-ENVIRON/ENVIRON commands. */
/*@{*/
/*! NEW-ENVIRON/ENVIRON codes. */
#define CRTL_TELNET_ENVIRON_IS 0
#define CRTL_TELNET_ENVIRON_SEND 1
#define CRTL_TELNET_ENVIRON_INFO 2
#define CRTL_TELNET_ENVIRON_VAR 0
#define CRTL_TELNET_ENVIRON_VALUE 1
#define CRTL_TELNET_ENVIRON_ESC 2
#define CRTL_TELNET_ENVIRON_USERVAR 3
/*@}*/

/*! \name Protocol codes for MSSP commands. */
/*@{*/
/*! MSSP codes. */
#define CRTL_TELNET_MSSP_VAR 1
#define CRTL_TELNET_MSSP_VAL 2
/*@}*/

/*! \name Telnet state tracker flags. */
/*@{*/
/*! Control behavior of telnet state tracker. */
#define CRTL_TELNET_FLAG_PROXY (1<<0)
#define CRTL_TELNET_FLAG_NVT_EOL (1<<1)

/* Internal-only bits in option flags */
#define CRTL_TELNET_FLAG_TRANSMIT_BINARY (1<<5)
#define CRTL_TELNET_FLAG_RECEIVE_BINARY (1<<6)
#define CRTL_TELNET_PFLAG_DEFLATE (1<<7)
/*@}*/

/*! 
 * error codes 
 */
enum crtl_telnet_error_t {
	CRTL_TELNET_EOK = 0,   /*!< no error */
	CRTL_TELNET_EBADVAL,   /*!< invalid parameter, or API misuse */
	CRTL_TELNET_ENOMEM,    /*!< memory allocation failure */
	CRTL_TELNET_EOVERFLOW, /*!< data exceeds buffer size */
	CRTL_TELNET_EPROTOCOL, /*!< invalid sequence of special bytes */
	CRTL_TELNET_ECOMPRESS  /*!< error handling compressed streams */
};
typedef enum crtl_telnet_error_t crtl_telnet_error_t; /*!< Error code type. */

/*! 
 * event codes 
 */
enum crtl_telnet_event_type_t {
	CRTL_TELNET_EV_DATA = 0,        /*!< raw text data has been received */
	CRTL_TELNET_EV_SEND,            /*!< data needs to be sent to the peer */
	CRTL_TELNET_EV_IAC,             /*!< generic IAC code received */
	CRTL_TELNET_EV_WILL,            /*!< WILL option negotiation received */
	CRTL_TELNET_EV_WONT,            /*!< WONT option neogitation received */
	CRTL_TELNET_EV_DO,              /*!< DO option negotiation received */
	CRTL_TELNET_EV_DONT,            /*!< DONT option negotiation received */
	CRTL_TELNET_EV_SUBNEGOTIATION,  /*!< sub-negotiation data received */
	CRTL_TELNET_EV_COMPRESS,        /*!< compression has been enabled */
	CRTL_TELNET_EV_ZMP,             /*!< ZMP command has been received */
	CRTL_TELNET_EV_TTYPE,           /*!< TTYPE command has been received */
	CRTL_TELNET_EV_ENVIRON,         /*!< ENVIRON command has been received */
	CRTL_TELNET_EV_MSSP,            /*!< MSSP command has been received */
	CRTL_TELNET_EV_WARNING,         /*!< recoverable error has occured */
	CRTL_TELNET_EV_ERROR            /*!< non-recoverable error has occured */
};
typedef enum crtl_telnet_event_type_t crtl_telnet_event_type_t; /*!< Telnet event type. */

/*! 
 * environ/MSSP command information 
 */
struct crtl_telnet_environ_t {
	unsigned char type; /*!< either CRTL_TELNET_ENVIRON_VAR or CRTL_TELNET_ENVIRON_USERVAR */
	char *var;          /*!< name of the variable being set */
	char *value;        /*!< value of variable being set; empty string if no value */
}_packed;

/*! 
 * event information 
 */
union crtl_telnet_event_t {
	/*! 
	 * \brief Event type
	 *
	 * The type field will determine which of the other event structure fields
	 * have been filled in.  For instance, if the event type is CRTL_TELNET_EV_ZMP,
	 * then the zmp event field (and ONLY the zmp event field) will be filled
	 * in.
	 */ 
	enum crtl_telnet_event_type_t type;

	/*! 
	 * data event: for DATA and SEND events 
	 */
	struct data_t {
		enum crtl_telnet_event_type_t _type; /*!< alias for type */
		const char *buffer;             /*!< byte buffer */
		size_t size;                    /*!< number of bytes in buffer */
	} data; /*!< DATA and SEND */

	/*! 
	 * WARNING and ERROR events 
	 */
	struct error_t {
		enum crtl_telnet_event_type_t _type; /*!< alias for type */
		const char *file;               /*!< file the error occured in */
		const char *func;               /*!< function the error occured in */
		const char *msg;                /*!< error message string */
		int line;                       /*!< line of file error occured on */
		crtl_telnet_error_t errcode;         /*!< error code */
	} err; /*!< WARNING and ERROR */

	/*! 
	 * command event: for IAC 
	 */
	struct iac_t {
		enum crtl_telnet_event_type_t _type; /*!< alias for type */
		unsigned char cmd;              /*!< telnet command received */
	} iac; /*!< IAC */

	/*! 
	 * negotiation event: WILL, WONT, DO, DONT 
	 */
	struct negotiate_t {
		enum crtl_telnet_event_type_t _type; /*!< alias for type */
		unsigned char telopt;           /*!< option being negotiated */
	} neg; /*!< WILL, WONT, DO, DONT */

	/*! 
	 * subnegotiation event 
	 */
	struct subnegotiate_t {
		enum crtl_telnet_event_type_t _type; /*!< alias for type */
		const char *buffer;             /*!< data of sub-negotiation */
		size_t size;                    /*!< number of bytes in buffer */
		unsigned char telopt;           /*!< option code for negotiation */
	} sub; /*!< SB */

	/*! 
	 * ZMP event 
	 */
	struct zmp_t {
		enum crtl_telnet_event_type_t _type; /*!< alias for type */
		const char **argv;              /*!< array of argument string */
		size_t argc;                    /*!< number of elements in argv */
	} zmp; /*!< ZMP */

	/*! 
	 * TTYPE event 
	 */
	struct ttype_t {
		enum crtl_telnet_event_type_t _type; /*!< alias for type */
		unsigned char cmd;              /*!< CRTL_TELNET_TTYPE_IS or CRTL_TELNET_TTYPE_SEND */
		const char* name;               /*!< terminal type name (IS only) */
	} ttype; /*!< TTYPE */

	/*! 
	 * COMPRESS event 
	 */
	struct compress_t {
		enum crtl_telnet_event_type_t _type; /*!< alias for type */
		unsigned char state;            /*!< 1 if compression is enabled,
	                                         0 if disabled */
	} compress; /*!< COMPRESS */

	/*! 
	 * ENVIRON/NEW-ENVIRON event
	 */
	struct environ_t {
		enum crtl_telnet_event_type_t _type;        /*!< alias for type */
		const struct crtl_telnet_environ_t *values; /*!< array of variable values */
		size_t size;                           /*!< number of elements in values */
		unsigned char cmd;                     /*!< SEND, IS, or INFO */
	} environ; /*!< ENVIRON, NEW-ENVIRON */
	
	/*!
	 * MSSP event
	 */
	struct mssp_t {
		enum crtl_telnet_event_type_t _type;        /*!< alias for type */
		const struct crtl_telnet_environ_t *values; /*!< array of variable values */
		size_t size;                           /*!< number of elements in values */
	} mssp; /*!< MSSP */
};

/*! 
 * \brief event handler
 *
 * This is the type of function that must be passed to
 * crtl_telnet_init() when creating a new telnet object.  The
 * function will be invoked once for every event generated
 * by the libtelnet protocol parser.
 *
 * \param telnet    The telnet object that generated the event
 * \param event     Event structure with details about the event
 * \param user_data User-supplied pointer
 */
typedef void (*crtl_telnet_event_handler_t)(crtl_telnet_t *telnet,
		crtl_telnet_event_t *event, void *user_data);

/*! 
 * telopt support table element; use telopt of -1 for end marker 
 */
struct crtl_telnet_telopt_t {
	short telopt;      /*!< one of the TELOPT codes or -1 */
	unsigned char us;  /*!< CRTL_TELNET_WILL or CRTL_TELNET_WONT */
	unsigned char him; /*!< CRTL_TELNET_DO or CRTL_TELNET_DONT */
}_packed;

/*! 
 * state tracker -- private data structure 
 */
struct crtl_telnet_t;

/*!
 * \brief Initialize a telnet state tracker.
 *
 * This function initializes a new state tracker, which is used for all
 * other libtelnet functions.  Each connection must have its own
 * telnet state tracker object.
 *
 * \param telopts   Table of TELNET options the application supports.
 * \param eh        Event handler function called for every event.
 * \param flags     0 or CRTL_TELNET_FLAG_PROXY.
 * \param user_data Optional data pointer that will be passsed to eh.
 * \return Telnet state tracker object.
 */
extern crtl_telnet_t* crtl_telnet_init(const crtl_telnet_telopt_t *telopts,
		crtl_telnet_event_handler_t eh, unsigned char flags, void *user_data);

/*!
 * \brief Free up any memory allocated by a state tracker.
 *
 * This function must be called when a telnet state tracker is no
 * longer needed (such as after the connection has been closed) to
 * release any memory resources used by the state tracker.
 *
 * \param telnet Telnet state tracker object.
 */
extern void crtl_telnet_free(crtl_telnet_t *telnet);

/*!
 * \brief Push a byte buffer into the state tracker.
 *
 * Passes one or more bytes to the telnet state tracker for
 * protocol parsing.  The byte buffer is most often going to be
 * the buffer that recv() was called for while handling the
 * connection.
 *
 * \param telnet Telnet state tracker object.
 * \param buffer Pointer to byte buffer.
 * \param size   Number of bytes pointed to by buffer.
 */
extern void crtl_telnet_recv(crtl_telnet_t *telnet, const char *buffer,
		size_t size);

/*!
 * \brief Send a telnet command.
 *
 * \param telnet Telnet state tracker object.
 * \param cmd    Command to send.
 */
extern void crtl_telnet_iac(crtl_telnet_t *telnet, unsigned char cmd);

/*!
 * \brief Send negotiation command.
 *
 * Internally, libtelnet uses RFC1143 option negotiation rules.
 * The negotiation commands sent with this function may be ignored
 * if they are determined to be redundant.
 *
 * \param telnet Telnet state tracker object.
 * \param cmd    CRTL_TELNET_WILL, CRTL_TELNET_WONT, CRTL_TELNET_DO, or CRTL_TELNET_DONT.
 * \param opt    One of the CRTL_TELNET_TELOPT_* values.
 */
extern void crtl_telnet_negotiate(crtl_telnet_t *telnet, unsigned char cmd,
		unsigned char opt);

/*!
 * Send non-command data (escapes IAC bytes).
 *
 * \param telnet Telnet state tracker object.
 * \param buffer Buffer of bytes to send.
 * \param size   Number of bytes to send.
 */
extern void crtl_telnet_send(crtl_telnet_t *telnet,
		const char *buffer, size_t size);

/*!
 * Send non-command text (escapes IAC bytes and translates
 * \\r -> CR-NUL and \\n -> CR-LF unless in BINARY mode.
 *
 * \param telnet Telnet state tracker object.
 * \param buffer Buffer of bytes to send.
 * \param size   Number of bytes to send.
 */
extern void crtl_telnet_send_text(crtl_telnet_t *telnet,
		const char *buffer, size_t size);

/*!
 * \brief Begin a sub-negotiation command.
 *
 * Sends IAC SB followed by the telopt code.  All following data sent
 * will be part of the sub-negotiation, until crtl_telnet_finish_sb() is
 * called.
 *
 * \param telnet Telnet state tracker object.
 * \param telopt One of the CRTL_TELNET_TELOPT_* values.
 */
extern void crtl_telnet_begin_sb(crtl_telnet_t *telnet,
		unsigned char telopt);

/*!
 * \brief Finish a sub-negotiation command.
 *
 * This must be called after a call to crtl_telnet_begin_sb() to finish a
 * sub-negotiation command.
 *
 * \param telnet Telnet state tracker object.
 */
#define crtl_telnet_finish_sb(telnet) crtl_telnet_iac((telnet), CRTL_TELNET_SE)

/*!
 * \brief Shortcut for sending a complete subnegotiation buffer.
 *
 * Equivalent to:
 *   crtl_telnet_begin_sb(telnet, telopt);
 *   crtl_telnet_send(telnet, buffer, size);
 *   crtl_telnet_finish_sb(telnet);
 *
 * \param telnet Telnet state tracker format.
 * \param telopt One of the CRTL_TELNET_TELOPT_* values.
 * \param buffer Byte buffer for sub-negotiation data.
 * \param size   Number of bytes to use for sub-negotiation data.
 */
extern void crtl_telnet_subnegotiation(crtl_telnet_t *telnet, unsigned char telopt,
		const char *buffer, size_t size);

/*!
 * \brief Begin sending compressed data.
 *
 * This function will begein sending data using the COMPRESS2 option,
 * which enables the use of zlib to compress data sent to the client.
 * The client must offer support for COMPRESS2 with option negotiation,
 * and zlib support must be compiled into libtelnet.
 *
 * Only the server may call this command.
 *
 * \param telnet Telnet state tracker object.
 */
extern void crtl_telnet_begin_compress2(crtl_telnet_t *telnet);

/*!
 * \brief Send formatted data.
 *
 * This function is a wrapper around crtl_telnet_send().  It allows using
 * printf-style formatting.
 *
 * Additionally, this function will translate \\r to the CR NUL construct and
 * \\n with CR LF, as well as automatically escaping IAC bytes like
 * crtl_telnet_send().
 *
 * \param telnet Telnet state tracker object.
 * \param fmt    Format string.
 * \return Number of bytes sent.
 */
extern int crtl_telnet_printf(crtl_telnet_t *telnet, const char *fmt, ...)
		CRTL_TELNET_GNU_PRINTF(2, 3);

/*!
 * \brief Send formatted data.
 *
 * See crtl_telnet_printf().
 */
extern int crtl_telnet_vprintf(crtl_telnet_t *telnet, const char *fmt, va_list va);

/*!
 * \brief Send formatted data (no newline escaping).
 *
 * This behaves identically to crtl_telnet_printf(), except that the \\r and \\n
 * characters are not translated.  The IAC byte is still escaped as normal
 * with crtl_telnet_send().
 *
 * \param telnet Telnet state tracker object.
 * \param fmt    Format string.
 * \return Number of bytes sent.
 */
extern int crtl_telnet_raw_printf(crtl_telnet_t *telnet, const char *fmt, ...)
		CRTL_TELNET_GNU_PRINTF(2, 3);

/*!
 * \brief Send formatted data (no newline escaping).
 *
 * See crtl_telnet_raw_printf().
 */
extern int crtl_telnet_raw_vprintf(crtl_telnet_t *telnet, const char *fmt, va_list va);

/*!
 * \brief Begin a new set of NEW-ENVIRON values to request or send.
 *
 * This function will begin the sub-negotiation block for sending or
 * requesting NEW-ENVIRON values.
 *
 * The crtl_telnet_finish_newenviron() macro must be called after this
 * function to terminate the NEW-ENVIRON command.
 *
 * \param telnet Telnet state tracker object.
 * \param type   One of CRTL_TELNET_ENVIRON_SEND, CRTL_TELNET_ENVIRON_IS, or
 *               CRTL_TELNET_ENVIRON_INFO.
 */
extern void crtl_telnet_begin_newenviron(crtl_telnet_t *telnet, unsigned char type);

/*!
 * \brief Send a NEW-ENVIRON variable name or value.
 *
 * This can only be called between calls to crtl_telnet_begin_newenviron() and
 * crtl_telnet_finish_newenviron().
 *
 * \param telnet Telnet state tracker object.
 * \param type   One of CRTL_TELNET_ENVIRON_VAR, CRTL_TELNET_ENVIRON_USERVAR, or
 *               CRTL_TELNET_ENVIRON_VALUE.
 * \param string Variable name or value.
 */
extern void crtl_telnet_newenviron_value(crtl_telnet_t* telnet, unsigned char type,
		const char *string);

/*!
 * \brief Finish a NEW-ENVIRON command.
 *
 * This must be called after a call to crtl_telnet_begin_newenviron() to finish a
 * NEW-ENVIRON variable list.
 *
 * \param telnet Telnet state tracker object.
 */
#define crtl_telnet_finish_newenviron(telnet) crtl_telnet_finish_sb((telnet))

/*!
 * \brief Send the TERMINAL-TYPE SEND command.
 *
 * Sends the sequence IAC TERMINAL-TYPE SEND.
 *
 * \param telnet Telnet state tracker object.
 */
extern void crtl_telnet_ttype_send(crtl_telnet_t *telnet);

/*!
 * \brief Send the TERMINAL-TYPE IS command.
 *
 * Sends the sequence IAC TERMINAL-TYPE IS "string".
 *
 * According to the RFC, the recipient of a TERMINAL-TYPE SEND shall
 * send the next possible terminal-type the client supports.  Upon sending
 * the type, the client should switch modes to begin acting as the terminal
 * type is just sent.
 *
 * The server may continue sending TERMINAL-TYPE IS until it receives a
 * terminal type is understands.  To indicate to the server that it has
 * reached the end of the available optoins, the client must send the last
 * terminal type a second time.  When the server receives the same terminal
 * type twice in a row, it knows it has seen all available terminal types.
 *
 * After the last terminal type is sent, if the client receives another
 * TERMINAL-TYPE SEND command, it must begin enumerating the available
 * terminal types from the very beginning.  This allows the server to
 * scan the available types for a preferred terminal type and, if none
 * is found, to then ask the client to switch to an acceptable
 * alternative.
 *
 * Note that if the client only supports a single terminal type, then
 * simply sending that one type in response to every SEND will satisfy
 * the behavior requirements.
 *
 * \param telnet Telnet state tracker object.
 * \param ttype  Name of the terminal-type being sent.
 */
extern void crtl_telnet_ttype_is(crtl_telnet_t *telnet, const char* ttype);

/*!
 * \brief Send a ZMP command.
 *
 * \param telnet Telnet state tracker object.
 * \param argc   Number of ZMP commands being sent.
 * \param argv   Array of argument strings.
 */
extern void crtl_telnet_send_zmp(crtl_telnet_t *telnet, size_t argc, const char **argv);

/*!
 * \brief Send a ZMP command.
 *
 * Arguments are listed out in var-args style.  After the last argument, a
 * NULL pointer must be passed in as a sentinel value.
 *
 * \param telnet Telnet state tracker object.
 */
extern void crtl_telnet_send_zmpv(crtl_telnet_t *telnet, ...) CRTL_TELNET_GNU_SENTINEL;

/*!
 * \brief Send a ZMP command.
 *
 * See crtl_telnet_send_zmpv().
 */
extern void crtl_telnet_send_vzmpv(crtl_telnet_t *telnet, va_list va);

/*!
 * \brief Begin sending a ZMP command
 *
 * \param telnet Telnet state tracker object.
 * \param cmd    The first argument (command name) for the ZMP command.
 */
extern void crtl_telnet_begin_zmp(crtl_telnet_t *telnet, const char *cmd);

/*!
 * \brief Send a ZMP command argument.
 *
 * \param telnet Telnet state tracker object.
 * \param arg    Telnet argument string.
 */
extern void crtl_telnet_zmp_arg(crtl_telnet_t *telnet, const char *arg);

/*!
 * \brief Finish a ZMP command.
 *
 * This must be called after a call to crtl_telnet_begin_zmp() to finish a
 * ZMP argument list.
 *
 * \param telnet Telnet state tracker object.
 */
#define crtl_telnet_finish_zmp(telnet) crtl_telnet_finish_sb((telnet))

/* C++ support */
#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* !defined(LIBCRTL_TELNET_INCLUDE) */



#endif /*<__RT_TCP_IP_LIBCRTL_TELNET_H>*/
#endif /*<__CRTL_TCP_IP_CRTL_TELNET_H>*/


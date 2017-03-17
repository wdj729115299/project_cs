#ifndef _TUNABLE_H
#define _TUNABLE_H

#define TUNABLE_LISTEN_ADDRESS	"127.0.0.1"
#define TUNABLE_LISTEN_PORT		5000
#define TUNABLE_IDLE_SESSION_TIMEOUT    300

extern const char *tunable_listen_address;
extern unsigned short tunable_listen_port;
extern int tunable_idle_session_timeout;
#endif

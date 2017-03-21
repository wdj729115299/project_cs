#ifndef _TUNABLE_H
#define _TUNABLE_H

#define TUNABLE_LISTEN_ADDRESS	"200.200.79.11"
#define TUNABLE_LISTEN_PORT		54321
#define TUNABLE_IDLE_SESSION_TIMEOUT    300
#define TUNABLE_CONNECT_TIMEOUT     60

extern const char *tunable_listen_address;
extern unsigned short tunable_listen_port;
extern unsigned int tunable_idle_session_timeout;
extern unsigned int 	tunable_connect_timeout;
#endif

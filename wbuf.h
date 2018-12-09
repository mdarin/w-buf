#ifndef WBUF_H
#define WBUF_H
#include <string.h>

#define WBF_ERROR -1
#define first 1
#define second 2
#define empty 0
#define full 1

typedef struct WBUFFER {
	char toggle;
	char status;
	int block;
	char pos;
	int* unload; 
	int* reload;
	int* stor1;
	int* stor2;
	size_t length;
} WBUFFER;

WBUFFER* wbuf_init(unsigned int buflen);
void wbuf_destroy(WBUFFER* buffer);
char wbuf_get_toggle(WBUFFER* buffer);
int wbuf_get_value(WBUFFER* buffer);
void wbuf_next(WBUFFER** buffer);
void wbuf_check_toggle(WBUFFER* buffer);
void wbuf_reload(WBUFFER** buffer, void* data, size_t datalen/*ohter params...*/);
#endif // WBUF_H


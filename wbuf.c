#include "wbuf.h"

//-----------------------------------------------------------------------------
WBUFFER* wbuf_init(unsigned int buflen) 
{
	WBUFFER* buffer = NULL;	
	if (buflen > 0) {   
		buffer = malloc(sizeof *buffer);
		if (NULL != buffer) {
			buffer->stor1 = malloc(sizeof *buffer->stor1 * buflen);		
			buffer->stor2 = malloc(sizeof *buffer->stor2 * buflen);
			if (NULL != buffer->stor1 && NULL != buffer->stor2) {
				buffer->unload = buffer->stor1;
				buffer->reload = buffer->stor2;
				buffer->status = full;
				buffer->toggle = first;
				buffer->pos = 0;
				buffer->block = 1;
				buffer->length = buflen;
			} else {
				free(buffer->stor1);
				free(buffer->stor2);
				free(buffer);
				buffer = NULL;
			} 
		} else {
			free(buffer);
			buffer = NULL;
		}
	}	
	return buffer;
}
//-----------------------------------------------------------------------------
void wbuf_destroy(WBUFFER* buffer) 
{
	if (NULL != buffer) {
		free(buffer->stor1);
		free(buffer->stor2);
		free(buffer);
		buffer = NULL;
	}
	return;
}
//-----------------------------------------------------------------------------
//TODO: fix this in future
char wbuf_get_toggle(WBUFFER* buffer)
{
	char toggle = WBF_ERROR;	
	if (NULL != buffer) {
		toggle = buffer->toggle;
	}	
	return toggle;
}
//-----------------------------------------------------------------------------
int wbuf_get_value(WBUFFER* buffer)
{
	int value = 0;	
	if (NULL != buffer && buffer->pos >= 0) {
		value = buffer->unload[buffer->pos];
	}
	return value;	
}
//-----------------------------------------------------------------------------
void wbuf_next(WBUFFER** buffer) 
{
	if (NULL != *buffer) {
		(*buffer)->pos += 1;
		//TODO: experimental do not work :)
		//if (buffer->pos >= buffer->length) {
		//	buffer->pos = 0;
		//}
	}
	return;
}
//-----------------------------------------------------------------------------
void wbuf_check_toggle(WBUFFER* buffer) 
{
	if (NULL != buffer) {
		switch (buffer->toggle) {
		case first:
			if (buffer->length == buffer->pos) {
				printf("toggle to second!\n");
				buffer->unload = buffer->stor2;//buffer2;
				buffer->reload = buffer->stor1;//buffer1;
				buffer->status = empty;
				buffer->toggle = second;
				buffer->pos = 0;
			}		
			break;
		case second:
			if (buffer->length == buffer->pos) {
				printf("toggle to first!\n");
				buffer->unload = buffer->stor1;//buffer1;
				buffer->reload = buffer->stor2;//buffer2;
				buffer->status = empty;
				buffer->toggle = first;
				buffer->pos = 0;
			}	
			break;
		}
	}
	return;
}
//-----------------------------------------------------------------------------
void wbuf_reload(WBUFFER** buffer, void* data, size_t datalen/*ohter params...*/) 
{
	if (NULL != *buffer && NULL != data) {
		if (empty == (*buffer)->status) {			
			int* p = data;			
			int addr = (*buffer)->block * (*buffer)->length - (*buffer)->length + 1;		
			for (int i = 0; i < (*buffer)->length; i++) {				
				(*buffer)->reload[i] = p[addr++];
				if ((*buffer)->length-1 == i) {
					(*buffer)->status = full;
				}
				//TODO: dependendent part... neet some mind force :)
				if (addr >= datalen) {
					(*buffer)->block = 1;
				} 
			}
			(*buffer)->block++;
		}
	}
	return;
}


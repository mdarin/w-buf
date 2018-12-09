/*
	starting position 
*/

#include <stdio.h>
#include <signal.h>



#define BLOCKSZ 4

enum buf_toggle_t {
	first = 1,
	second,
};

enum buf_status_t {
	empty = 0,
	full,
};


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

char wbuf_get_toggle(WBUFFER* buffer)
{
	char toggle = WBF_ERROR;	
	if (NULL != buffer) {
		toggle = buffer->toggle;
	}	
	return toggle;
}

int wbuf_get_value(WBUFFER* buffer)
{
	int value = 0;	
	if (NULL != buffer && buffer->pos >= 0) {
		value = buffer->unload[buffer->pos];
	}
	return value;	
}

WBUFFER* wbuf_get_next(WBUFFER* buffer) {
	if (NULL != buffer) {
		buffer->pos += 1;
		// exptrimental
		//if (buffer->pos >= buffer->length) {
		//	buffer->pos = 0;
		//}
	}
	return buffer;
}

void wbuf_check_toggle(WBUFFER* buffer) {
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

WBUFFER* wbuf_reload(WBUFFER* buffer, void* data, size_t datalen/*ohter params...*/) 
{
	if (NULL != buffer && NULL != data) {
		if (empty == buffer->status) {			
			int* p = data;			
			int addr = buffer->block * buffer->length - buffer->length + 1;		
			for (int i = 0; i < buffer->length; i++) {				
				buffer->reload[i] = p[addr++];
				if (buffer->length-1 == i) {
					buffer->status = full;
				}
				// dependendent part... neet some mind force :)
				if (addr >= datalen) {
					buffer->block = 1;
				} 
			}
			buffer->block++;
		}
	}
	return buffer;
}


// в дальнейшем выделять память динамически!
//int buffer1[BLOCKSZ] = {0}; 
//int buffer2[BLOCKSZ] = {0};

//int* buffer1;
//int* buffer2;

#define DATA_LEN 594

int data[] ={  
	1, // массив читается с 0 а ПЗУ с 1 :) для совместимости
	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	16,	17,	18,	19,	20,	21,
	22,	23,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,
	41,	42,	43,	44,	45,	46,	47,	48,	49,	50,	51,	52,	53,	54,	55,	56,	57,	58,
	59,	60,	61,	62,	63,	64,	65,	66,	67,	68,	69,	70,	71,	72,	73,	74,	75,	76,	77,	78,	79,	80,	81,	82,	83,	84,
	85,	86,	87,	88,	89,	90,	91,	92,	93,	94,	95,	96,	97,	98,	99,	100,	101,	102,	103,	104,	105,	106,
	107,	108,	109,	110,	111,	112,	113,	114,	115,	116,	117,	118,	119,	120,	121,	122,	123,
	124,	125,	126,	127,	128,	129,	130,	131,	132,	133,	134,	135,	136,	137,	138,	139,	140,
	141,	142,	143,	144,	145,	146,	147,	148,	149,	150,	151,	152,	153,	154,	155,	156,	157,
	158,	159,	160,	161,	162,	163,	164,	165,	166,	167,	168,	169,	170,	171,	172,	173,	174,
	175,	176,	177,	178,	179,	180,	181,	182,	183,	184,	185,	186,	187,	188,	189,	190,	191,
	192,	193,	194,	195,	196,	197,	198,	199,	200,	201,	202,	203,	204,	205,	206,	207,	208,
	209,	210,	211,	212,	213,	214,	215,	216,	217,	218,	219,	220,	221,	222,	223,	224,	225,
	226,	227,	228,	229,	230,	231,	232,	233,	234,	235,	236,	237,	238,	239,	240,	241,	242,
	243,	244,	245,	246,	247,	248,	249,	250,	251,	252,	253,	254,	255,	256,	257,	258,	259,
	260,	261,	262,	263,	264,	265,	266,	267,	268,	269,	270,	271,	272,	273,	274,	275,	276,
	277,	278,	279,	280,	281,	282,	283,	284,	285,	286,	287,	288,	289,	290,	291,	292,	293,
	294,	295,	296,	297,	298,	299,	300,	301,	302,	303,	304,	305,	306,	307,	308,	309,	310,
	311,	312,	313,	314,	315,	316,	317,	318,	319,	320,	321,	322,	323,	324,	325,	326,	327,
	328,	329,	330,	331,	332,	333,	334,	335,	336,	337,	338,	339,	340,	341,	342,	343,	344,
	345,	346,	347,	348,	349,	350,	351,	352,	353,	354,	355,	356,	357,	358,	359,	360,	361,
	362,	363,	364,	365,	366,	367,	368,	369,	370,	371,	372,	373,	374,	375,	376,	377,	378,
	379,	380,	381,	382,	383,	384,	385,	386,	387,	388,	389,	390,	391,	392,	393,	394,	395,
	396,	397,	398,	399,	400,	401,	402,	403,	404,	405,	406,	407,	408,	409,	410,	411,	412,
	413,	414,	415,	416,	417,	418,	419,	420,	421,	422,	423,	424,	425,	426,	427,	428,	429,
	430,	431,	432,	433,	434,	435,	436,	437,	438,	439,	440,	441,	442,	443,	444,	445,	446,
	447,	448,	449,	450,	451,	452,	453,	454,	455,	456,	457,	458,	459,	460,	461,	462,	463,
	464,	465,	466,	467,	468,	469,	470,	471,	472,	473,	474,	475,	476,	477,	478,	479,	480,
	481,	482,	483,	484,	485,	486,	487,	488,	489,	490,	491,	492,	493,	494,	495,	496,	497,
	498,	499,	500,	501,	502,	503,	504,	505,	506,	507,	508,	509,	510,	511,	512,	513,	514,
	515,	516,	517,	518,	519,	520,	521,	522,	523,	524,	525,	526,	527,	528,	529,	530,	531,
	532,	533,	534,	535,	536,	537,	538,	539,	540,	541,	542,	543,	544,	545,	546,	547,	548,
	549,	550,	551,	552,	553,	554,	555,	556,	557,	558,	559,	560,	561,	562,	563,	564,	565,
	566,	567,	568,	569,	570,	571,	572,	573,	574,	575,	576,	577,	578,	579,	580,	581,	582,
	583,	584,	585,	586,	587,	588,	589,	590,	591,	592,	593,	594,
};


void fn(void)
{
  printf("timeout!\n");
	return;
}

WBUFFER* buf;


/*
   void tick(nsig){
       time_t tim; char *s;
       signal (SIGALRM, tick);
       alarm(3); time(&tim);
       s = ctime(&tim);
       s[ strlen(s)-1 ] = '\0'; // обрубить '\n' 
       fprintf(stderr, "\r%s", s);
    }
    main(){ tick(0);
            for(;;) pause();
    }
*/

void ISR_timer(void) {
	signal (SIGALRM, ISR_timer);
	alarm(1);
	int OCR2 = -100;
	//printf("timeout!\n");
	/*
	switch (buf->toggle) {
	case first:
		if (BLOCKSZ == buf->pos) {
      printf("toggle to second!\n");
			buf->unload = buf->stor2;//buffer2;
			buf->reload = buf->stor1;//buffer1;
			buf->status = empty;
			buf->toggle = second;
			buf->pos = 0;
		}		
		break;
	case second:
		if (BLOCKSZ == buf->pos) {
			printf("toggle to first!\n");
			buf->unload = buf->stor1;//buffer1;
			buf->reload = buf->stor2;//buffer2;
			buf->status = empty;
			buf->toggle = first;
			buf->pos = 0;
		}	
		break;
	}
	//printf(" pos: %d\n", buf->pos++);
	printf("OCR2 = %d\n", buf->unload[buf->pos++]);
	*/
	
	wbuf_check_toggle(buf);
	OCR2 = wbuf_get_value(buf);
	buf = wbuf_get_next(buf);
	printf("OCR2 = %d\n", OCR2);
	
}

int main(void) {
	// inti_buf
	//buffer1 = malloc(sizeof *buffer1 * BLOCKSZ);	
	//buffer2 = malloc(sizeof *buffer2 * BLOCKSZ);
	buf = wbuf_init(BLOCKSZ);//malloc(sizeof *buf);
	if (NULL != buf ) { //&& NULL != buffer1 && NULL != buffer2) {		
		/*		
		buf->unload = buffer1;
		buf->reload = buffer2;
		buf->status = full;
		buf->toggle = first;
		buf->pos = 0;
		buf->block = 1;
		*/
		// at the begining fillin two buffer atonce
		int addr = buf->block;		
		for (int i = 0; i < BLOCKSZ; i++) {
			buf->unload[i] = data[addr++];
		}
		buf->block++;

    addr = buf->block * BLOCKSZ - BLOCKSZ + 1;		
		for (int i = 0; i < BLOCKSZ; i++) {
			buf->reload[i] = data[addr++];
		}
		buf->block++;
				
		
		/*
		for (int i = 0; i < BLOCKSZ; i++) {
			printf(" unload[%d]:%d\n", i, buf->unload[i]);
		}

		for (int i = 0; i < BLOCKSZ; i++) {
			printf(" reload[%d]:%d\n", i, buf->reload[i]);
		} 
		*/
	  
		
		char done = 0;
		// read data to reloading buffer till the end
		ISR_timer(); // start timer 		
		while(!done) { 
			// read data to buffer	
			/*
			if (empty == buf->status) {			
				addr = buf->block * BLOCKSZ - BLOCKSZ + 1;		
				for (int i = 0; i < BLOCKSZ; i++) {
					buf->reload[i] = data[addr++];
					if (BLOCKSZ-1 == i) {
						buf->status = full;
					}

					if (addr >= DATA_LEN) {
						buf->block = 1;
					} 
				}
				buf->block++;
			}*/
			buf = wbuf_reload(buf, data, DATA_LEN);
		}
		  


	}
	wbuf_destroy(buf);
	//free(buffer1);
	//free(buffer2);
	//free(buf);
	return 0;
}

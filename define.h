#include<stdlib.h>
#include<stdio.h>
 
#ifndef DEFINE
#define DEFINE

typedef unsigned char   uint8_t;//无符号8位数
typedef unsigned short int    uint16_t;    //无符号16位数
typedef unsigned int    uint32_t;    //无符号32位数
typedef unsigned long long int    uint64_t;    //无符号64位数

//读取字节结构体  
typedef struct Tag_bs_t  
{  
    unsigned char *p_start;  //缓冲区首地址(这个开始是最低地址)  
    unsigned char *p;       //缓冲区当前的读写指针 当前字节的地址，这个会不断的++，每次++，进入一个新的字节  
    unsigned char *p_end;   //缓冲区尾地址     //typedef unsigned char   uint8_t;  
    int     i_left;        // p所指字节当前还有多少 “位” 可读写 count number of available(可用的)位   
}bs_t; 

enum{
	ftp     = 0x66747970,
	mdat    = 0x6D646174,
	moov    = 0x6D6F6F76,
	free0    = 0x66726565,
	mvhd    = 0x6D766864,
	trak   = 0x7472616B,
	tkhd    = 0x746B6864,
	edts    = 0x65647473,
	mdia    = 0x6D646961,
	mdhd    = 0x6D646864,
	hdlr    = 0x68646C72,
	minf    = 0x6D696E66,
	vmhd    = 0x766D6864,
	smhd    = 0x736D6864,
	dinf    = 0x64696E66,
	stbl    = 0x7374626C,
	stsd    = 0x73747364,
	avc1    = 0x61766331,
	avcC    = 0x61766343,
	pasp    = 0x70617370,
	stts    = 0x73747473,
	stss    = 0x73747373,
	ctts    = 0x63747473,
	stsc    = 0x73747363,
	stsz    = 0x7374737A,
	stco    = 0x7374636F
};

//nal类型  
enum nal_unit_type_e  
{  
    NAL_UNKNOWN     = 0,  
    NAL_SLICE       = 1,  
    NAL_SLICE_DPA   = 2,  
    NAL_SLICE_DPB   = 3,  
    NAL_SLICE_DPC   = 4,  
    NAL_SLICE_IDR   = 5,    /* ref_idc != 0 */  
    NAL_SEI         = 6,    /* ref_idc == 0 */  
    NAL_SPS         = 7,  
    NAL_PPS         = 8  
    /* ref_idc == 0 for 6,9,10,11,12 */  
};  

//帧类型  
enum Frametype_e  
{  
    FRAME_I  = 15,  
    FRAME_P  = 16,  
    FRAME_B  = 17  
};  



#endif 

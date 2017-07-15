#include<stdlib.h>
#include<stdio.h>
 
#ifndef DEFINE
#define DEFINE

typedef unsigned char   uint8_t;//�޷���8λ��
typedef unsigned short int    uint16_t;    //�޷���16λ��
typedef unsigned int    uint32_t;    //�޷���32λ��
typedef unsigned long long int    uint64_t;    //�޷���64λ��

//��ȡ�ֽڽṹ��  
typedef struct Tag_bs_t  
{  
    unsigned char *p_start;  //�������׵�ַ(�����ʼ����͵�ַ)  
    unsigned char *p;       //��������ǰ�Ķ�дָ�� ��ǰ�ֽڵĵ�ַ������᲻�ϵ�++��ÿ��++������һ���µ��ֽ�  
    unsigned char *p_end;   //������β��ַ     //typedef unsigned char   uint8_t;  
    int     i_left;        // p��ָ�ֽڵ�ǰ���ж��� ��λ�� �ɶ�д count number of available(���õ�)λ   
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

//nal����  
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

//֡����  
enum Frametype_e  
{  
    FRAME_I  = 15,  
    FRAME_P  = 16,  
    FRAME_B  = 17  
};  



#endif 

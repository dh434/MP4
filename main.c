#include"define.h"

uint32_t decodeUint32(uint8_t head[4])
{
	uint32_t x;
	x = head[0]*256*256*256 + head[1]*256*256 + head[2]*256 + head[3];
	return x;
}

uint32_t decodeUint32_2(uint8_t head[4])   //���� 
{
	uint32_t x;
	x = head[3]*256*256*256 + head[2]*256*256 + head[1]*256 + head[0];
	return x;
}

uint32_t decodeUint32_length(uint8_t head[4],int length)   //�涨���ȵ� 
{
	uint32_t x;
	if(length == 4)
		x = head[0]*256*256*256 + head[1]*256*256 + head[2]*256 + head[3];
	else if(length == 3){
		x = head[0]*256*256 + head[1]*256 + head[2];
	}
	else if(length == 1){
		x = head[0];
	}
		
	return x;
}

void bs_init( bs_t *s, void *p_data, int i_data )  
{  
    s->p_start = (unsigned char *)p_data; //�ô����p_data�׵�ַ��ʼ��p_start��ֻ������Ч���ݵ��׵�ַ  
    s->p       = (unsigned char *)p_data;//�ֽ��׵�ַ��һ��ʼ��p_data��ʼ����ÿ����һ�����ֽڣ����ƶ�����һ�ֽ��׵�ַ  
    s->p_end   = s->p + i_data;                   //β��ַ�����һ���ֽڵ��׵�ַ?  
    s->i_left  = 8;                              //��û�п�ʼ��д����ǰ�ֽ�ʣ��δ��ȡ��λ��8  
}  


int bs_read( bs_t *s, int i_count )  
{  
     static int i_mask[33] ={0x00,  
                                  0x01,      0x03,      0x07,      0x0f,  
                                  0x1f,      0x3f,      0x7f,      0xff,  
                                  0x1ff,     0x3ff,     0x7ff,     0xfff,  
                                  0x1fff,    0x3fff,    0x7fff,    0xffff,  
                                  0x1ffff,   0x3ffff,   0x7ffff,   0xfffff,  
                                  0x1fffff,  0x3fffff,  0x7fffff,  0xffffff,  
                                  0x1ffffff, 0x3ffffff, 0x7ffffff, 0xfffffff,  
                                  0x1fffffff,0x3fffffff,0x7fffffff,0xffffffff};  
    int      i_shr;             //  
    int i_result = 0;           //������Ŷ�ȡ���ĵĽ�� typedef unsigned   uint32_t;  
  
    while( i_count > 0 )     //Ҫ��ȡ�ı�����  
    {  
        if( s->p >= s->p_end ) //�ֽ����ĵ�ǰλ��>=����β��������˱�����s�Ѿ������ˡ�  
        {                       //  
            break;  
        }  
  
        if( ( i_shr = s->i_left - i_count ) >= 0 )    //��ǰ�ֽ�ʣ���δ��λ������Ҫ��ȡ��λ���࣬�������  
        {                                           //i_left��ǰ�ֽ�ʣ���δ��λ��������Ҫ��i_count���أ�i_shr=i_left-i_count�Ľ�����>=0��˵��Ҫ��ȡ�Ķ��ڵ�ǰ�ֽ���  
                                                    //i_shr>=0��˵��Ҫ��ȡ�ı��ض����ڵ�ǰ�ֽ���  
            //����׶Σ�һ���ԾͶ����ˣ�Ȼ�󷵻�i_result(�˳��˺���)  
            /* more in the buffer than requested */  
            i_result |= ( *s->p >> i_shr )&i_mask[i_count];//��|=��:��λ��ֵ��A |= B �� A = A|B  
                                    //|=Ӧ�������ִ�У��ѽ������i_result(��λ�����ȼ����ڸ��ϲ�����|=)  
                                    //i_mask[i_count]���Ҳ��λ����1,�������еİ�λ�룬���԰������еĽ�����ƹ���  
                                    //!=,��ߵ�i_result�����ȫ��0���Ҳ�������λ�򣬻��Ǹ��ƽ�������ˣ�����ü���������  
            /*��ȡ�󣬸��½ṹ������ֶ�ֵ*/  
            s->i_left -= i_count; //��i_left = i_left - i_count����ǰ�ֽ�ʣ���δ��λ����ԭ���ļ�ȥ��ζ�ȡ��  
            if( s->i_left == 0 ) //�����ǰ�ֽ�ʣ���δ��λ��������0��˵����ǰ�ֽڶ����ˣ���Ҫ��ʼ��һ���ֽ�  
            {  
                s->p++;              //�ƶ�ָ�룬����p���������ֽ�Ϊ�����ƶ�ָ���  
                s->i_left = 8;       //�¿�ʼ������ֽ���˵����ǰ�ֽ�ʣ���δ��λ��������8������  
            }  
            return( i_result );     //���ܵķ���ֵ֮һΪ��00000000 00000000 00000000 00000001 (4�ֽڳ�)  
        }  
        else    /* i_shr < 0 ,���ֽڵ����*/  
        {  
            //����׶Σ���while��һ��ѭ�������ܻ��������һ��ѭ������һ�κ����һ�ζ����ܶ�ȡ�ķ����ֽڣ������һ�ζ���3���أ��м��ȡ��2�ֽ�(��2x8����)�����һ�ζ�ȡ��1���أ�Ȼ���˳�whileѭ��  
            //��ǰ�ֽ�ʣ���δ��λ������Ҫ��ȡ��λ���٣����統ǰ�ֽ���3λδ������������Ҫ��7λ  
            //???�Ե�ǰ�ֽ���˵��Ҫ���ı��أ��������ұߣ����Բ�����λ��(��λ��Ŀ���ǰ�Ҫ���ı��ط��ڵ�ǰ�ֽ�����)  
            /* less(���ٵ�) in the buffer than requested */  
            i_result |= (*s->p&i_mask[s->i_left]) << -i_shr;    //"-i_shr"�൱��ȡ�˾���ֵ  
                                    //|= �� << ����λ�����������ȼ���ͬ�����Դ�������˳��ִ��  
                                    //����:int|char ������int��4�ֽڣ�char��1�ֽڣ�sizeof(int|char)��4�ֽ�  
                                    //i_left�����8����С��0��ȡֵ��Χ��[0,8]  
            i_count  -= s->i_left;   //����ȡ�ı�����������ԭi_count��ȥi_left��i_left�ǵ�ǰ�ֽ�δ�����ı�����������else�׶Σ�i_left����ĵ�ǰ�ֽ�δ���ı���ȫ�������ˣ����Լ���  
            s->p++;  //��λ����һ���µ��ֽ�  
            s->i_left = 8;   //��һ�����ֽ���˵��δ������λ����Ȼ��8�������ֽ�����λ��û��ȡ��  
        }  
    }  
  
    return( i_result );//���ܵķ���ֵ֮һΪ��00000000 00000000 00000000 00000001 (4�ֽڳ�)  
}  


/*��Ҫ���ı����Ƶ���ǰ�ֽ����ң�Ȼ����0x01:00000001�����߼��������
  ��ΪҪ����ֻ��һ�����أ�������ز���0����1����0000 0001��λ��Ϳ��Ե�֪�����  */ 
int bs_read1( bs_t *s )  
{  
  
    if( s->p < s->p_end )    
    {  
        unsigned int i_result;  
  
        s->i_left--;                           //��ǰ�ֽ�δ��ȡ��λ������1λ  
        
        i_result = ( *s->p >> s->i_left )&0x01;
        if( s->i_left == 0 )                   //�����ǰ�ֽ�ʣ��δ��λ����0������˵��ǰ�ֽ�ȫ������  
        {  
            s->p++;                             //ָ��s->p �Ƶ���һ�ֽ�  
            s->i_left = 8;                     //���ֽ��У�δ��λ����Ȼ��8λ  
        }  
        return i_result;                       //unsigned int  
    }  
  
    return 0;                                  //����0Ӧ����û�ж�������  
}  


int bs_read_ue( bs_t *s )  
{  
    int i = 0;  
    
    //����Ϊ�������ĵ�ǰ����=0��ָ��δԽ�磬���ֻ�ܶ�32����  
    for(i=0;bs_read1( s ) == 0 && s->p < s->p_end && i < 32;++i) ; 
  
    return( ( 1 << i) - 1 + bs_read( s, i ) );      
}  

/*
	NAL_SLICE and IDR
	bufΪnalu�����ݣ�������ͷ
	lenҲ������ͷ 
*/
unsigned char  getFrameType(unsigned char * buf,unsigned int  len) 
{
	bs_t s;
	int frame_type = 0; 
	unsigned char result;
	bs_init( &s,buf,len);
	 
	/* i_first_mb */  
    bs_read_ue( &s );  
    /* picture type */  
    frame_type =  bs_read_ue( &s );
//	return   frame_type;
	switch(frame_type){
		
		case 0: case 5: /* P */  
            result = FRAME_P;  
//            printf("P֡ ");
            break;  
        case 1: case 6: /* B */  
            result = FRAME_B; 
			printf("B֡ "); 
            break;  
        case 3: case 8: /* SP */  
            result = FRAME_P;  
            printf("SP֡ ");
            break;  
        case 2: case 7: /* I */  
            result = FRAME_I;  
            printf("I֡ ");
            break;  
        case 4: case 9: /* SI */  
            result = FRAME_I;  
            printf("SI֡ ");
            break;  
	} 
	
	return  result;
}

uint32_t findBox(FILE* fp, uint32_t target)    // 
{
	uint32_t boxSize, boxType;
	uint32_t offset = 0x0;
	uint8_t size[4],type[4];
	int m,n;                //�����ж��ļ�ĩβ 
	
	m=fread(size,1,4,fp);
    n=fread(type,1,4,fp);
    boxSize = decodeUint32(size);
    boxType = decodeUint32(type);
    
    while(boxType!=target&&m!=0&&n!=0){
    	offset += boxSize;
    	fseek(fp,boxSize-8,SEEK_CUR);
    	m=fread(size,1,4,fp);
    	n=fread(type,1,4,fp);
    	boxSize = decodeUint32(size);
    	boxType = decodeUint32(type);
	}
	offset += 0x00000008;
	
	if(m==0||n==0)
		printf("error[1]"); 
	return offset;
}

uint32_t getParameter(FILE* fp)
{
	uint32_t boxSize, boxType;
	uint8_t size[4],type[4]; 
	uint8_t buf[78];
	int control;
	uint32_t tempLength, tempOffset,offset; //��¼mdat��offset 
	uint32_t result; 
	
	
	control = 0;
	fread(size,1,4,fp);
	while(!control&&!feof(fp)){     //��trak  
    	fread(type,1,4,fp);
    	boxSize = decodeUint32(size);
    	boxType = decodeUint32(type);
    	if(boxType == trak) {
    		tempLength = boxSize;
    		offset = 0x0;
    		
    		tempOffset = findBox(fp,mdia); //��mdia
			offset  +=  tempOffset;
			
			tempOffset = findBox(fp,minf); //��minf
			offset  +=  tempOffset;
			
			fread(size,1,4,fp);             //��vmhd
    		fread(type,1,4,fp);
    		boxSize = decodeUint32(size);
    		boxType = decodeUint32(type);
    		if(boxType == vmhd){
    			control = 1;
    			fseek(fp,boxSize-8,SEEK_CUR);
    			findBox(fp,stbl);    //��stbl
				findBox(fp,stsd);    //��stsd
				fread(size,1,4,fp);             //��ȥ���汾��֮�� 
    			fread(type,1,4,fp);
				
				fread(size,1,4,fp);             //��avc1
    			fread(type,1,4,fp);
    			boxSize = decodeUint32(size);
    			boxType = decodeUint32(type);
				if(boxType == avc1){
					fread(buf,1,78,fp); 
					findBox(fp,avcC);    //��avcC
					fread(size,1,4,fp);  //ȥ���汾��֮���
					fread(size,1,4,fp);
					result = decodeUint32_2(size);
					
					result = result&0x00000003 + 0x00000001;
					return result;
				}
				else{
					printf("���Ǳ�׼��avc����.");
				}
			}
			else{
				tempOffset += 0x00000008;
				fseek(fp,tempLength-8-tempOffset,SEEK_CUR);	
			}
		}
		else{
			fseek(fp,boxSize-8,SEEK_CUR);
		}
		
		fread(size,1,4,fp);
	} 
	
}



int main()
{
	FILE* fp;
	uint32_t boxSize, boxType;
	uint32_t offset = 0x0; //��¼mdat��offset 
	int control, naluLength,flag;
	uint8_t size[4],type[4]; 
	uint8_t* buf;
	uint8_t slice_type=0;
	int m,n; 
	
	if((fp=fopen("new.mp4","r+"))==NULL){
        printf("Can not open output file.\n");
        return 0;
    }

    control = 0;
    flag =0;
    m=1,n=1;
    
    fread(size,1,4,fp);
    while(control!= 1&&!feof(fp)){   //�ҵ�moov����� 
    	fread(type,1,4,fp);
    	boxSize = decodeUint32(size);
    	boxType = decodeUint32(type);
    	if(boxType == moov){
    		control = 1;
    		//�Ӻ��������������box��ȡpps��sps���͹���table 
    		naluLength=(int)getParameter(fp);   //naluLength���ֽڱ�ʾ���� 
		}
		else if(boxType == mdat){
			flag = 1; 
			fseek(fp,boxSize-8,SEEK_CUR);
		}
		else{
			fseek(fp,boxSize-8,SEEK_CUR);
			offset += boxSize;
		}
		fread(size,1,4,fp);
	}
	
	if(flag == 1){                        //����mdat 
		fseek(fp,offset,SEEK_SET);
		fread(size,1,4,fp);
    	fread(type,1,4,fp);
    	boxSize = decodeUint32(size);
    	boxType = decodeUint32(type);
    	if(boxType == mdat){
    		offset = boxSize-8;
    		while(offset!= 0x0)
    		{
    			fread(size,1,naluLength,fp);
	    		boxSize = decodeUint32_length(size,naluLength);
	    		fread(type,1,1,fp);
	    		boxType = decodeUint32_length(type,1);
	    		boxType = boxType&0x0000001F;
	    		if(boxType == NAL_SLICE || boxType == NAL_SLICE_IDR){
	    			buf = (uint8_t*)malloc(sizeof(uint8_t)*(boxSize-1)); //nalu
	    			fread(buf,1,boxSize-1,fp);
	    			getFrameType(buf,boxSize-1);
				}
				else if(boxType == NAL_SEI){
					fseek(fp,boxSize-1,SEEK_CUR);
					printf("SEI֡ ");	
				}
				else{
					fseek(fp,boxSize-1,SEEK_CUR);
					printf("���� ");
				}
				offset = offset - boxSize - naluLength;
			}	
		} 
		else{
			printf("error");
		}
	} 
	else{
		fseek(fp,0,SEEK_SET);
		fread(size,1,4,fp);
    	fread(type,1,4,fp);
    	boxSize = decodeUint32(size);
    	boxType = decodeUint32(type);
		m=1;
    	while(boxType != mdat&&m!=0){
    		fseek(fp,boxSize-8,SEEK_CUR);
    		m= fread(size,1,4,fp);
    		fread(type,1,4,fp);
    		boxSize = decodeUint32(size);
    		boxType = decodeUint32(type);
		}
    	if(boxType == mdat){
    		
    		offset = boxSize-8;
    		while(offset!= 0x0)
    		{
    			fread(size,1,naluLength,fp);
	    		boxSize = decodeUint32_length(size,naluLength);
	    		fread(type,1,1,fp);
	    		boxType = decodeUint32_length(type,1);
	    		boxType = boxType&0x0000001F;
	    		if(boxType == NAL_SLICE || boxType == NAL_SLICE_IDR){
	    			buf = (uint8_t*)malloc(sizeof(uint8_t)*(boxSize-1)); //nalu
	    			fread(buf,1,boxSize-1,fp);
	    			getFrameType(buf,boxSize-1);
				}
				else if(boxType == NAL_SEI){
					fseek(fp,boxSize-1,SEEK_CUR);
					printf("SEI֡ ");	
				}
				else{
					fseek(fp,boxSize-1,SEEK_CUR);
					printf("���� ");
				}
				offset = offset - boxSize - naluLength;
			}	
		} 
		else{
			printf("error");
		}
	}
	
	fclose(fp);
	return 0; 
    
    
}

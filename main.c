#include"define.h"

uint32_t decodeUint32(uint8_t head[4])
{
	uint32_t x;
	x = head[0]*256*256*256 + head[1]*256*256 + head[2]*256 + head[3];
	return x;
}

uint32_t decodeUint32_2(uint8_t head[4])   //倒序 
{
	uint32_t x;
	x = head[3]*256*256*256 + head[2]*256*256 + head[1]*256 + head[0];
	return x;
}

uint32_t decodeUint32_length(uint8_t head[4],int length)   //规定长度的 
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
    s->p_start = (unsigned char *)p_data; //用传入的p_data首地址初始化p_start，只记下有效数据的首地址  
    s->p       = (unsigned char *)p_data;//字节首地址，一开始用p_data初始化，每读完一个整字节，就移动到下一字节首地址  
    s->p_end   = s->p + i_data;                   //尾地址，最后一个字节的首地址?  
    s->i_left  = 8;                              //还没有开始读写，当前字节剩余未读取的位是8  
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
    int i_result = 0;           //用来存放读取到的的结果 typedef unsigned   uint32_t;  
  
    while( i_count > 0 )     //要读取的比特数  
    {  
        if( s->p >= s->p_end ) //字节流的当前位置>=流结尾，即代表此比特流s已经读完了。  
        {                       //  
            break;  
        }  
  
        if( ( i_shr = s->i_left - i_count ) >= 0 )    //当前字节剩余的未读位数，比要读取的位数多，或者相等  
        {                                           //i_left当前字节剩余的未读位数，本次要读i_count比特，i_shr=i_left-i_count的结果如果>=0，说明要读取的都在当前字节内  
                                                    //i_shr>=0，说明要读取的比特都处于当前字节内  
            //这个阶段，一次性就读完了，然后返回i_result(退出了函数)  
            /* more in the buffer than requested */  
            i_result |= ( *s->p >> i_shr )&i_mask[i_count];//“|=”:按位或赋值，A |= B 即 A = A|B  
                                    //|=应该在最后执行，把结果放在i_result(按位与优先级高于复合操作符|=)  
                                    //i_mask[i_count]最右侧各位都是1,与括号中的按位与，可以把括号中的结果复制过来  
                                    //!=,左边的i_result在这儿全是0，右侧与它按位或，还是复制结果过来了，好象好几步都多余  
            /*读取后，更新结构体里的字段值*/  
            s->i_left -= i_count; //即i_left = i_left - i_count，当前字节剩余的未读位数，原来的减去这次读取的  
            if( s->i_left == 0 ) //如果当前字节剩余的未读位数正好是0，说明当前字节读完了，就要开始下一个字节  
            {  
                s->p++;              //移动指针，所以p好象是以字节为步长移动指针的  
                s->i_left = 8;       //新开始的这个字节来说，当前字节剩余的未读位数，就是8比特了  
            }  
            return( i_result );     //可能的返回值之一为：00000000 00000000 00000000 00000001 (4字节长)  
        }  
        else    /* i_shr < 0 ,跨字节的情况*/  
        {  
            //这个阶段，是while的一次循环，可能还会进入下一次循环，第一次和最后一次都可能读取的非整字节，比如第一次读了3比特，中间读取了2字节(即2x8比特)，最后一次读取了1比特，然后退出while循环  
            //当前字节剩余的未读位数，比要读取的位数少，比如当前字节有3位未读过，而本次要读7位  
            //???对当前字节来说，要读的比特，都在最右边，所以不再移位了(移位的目的是把要读的比特放在当前字节最右)  
            /* less(较少的) in the buffer than requested */  
            i_result |= (*s->p&i_mask[s->i_left]) << -i_shr;    //"-i_shr"相当于取了绝对值  
                                    //|= 和 << 都是位操作符，优先级相同，所以从左往右顺序执行  
                                    //举例:int|char ，其中int是4字节，char是1字节，sizeof(int|char)是4字节  
                                    //i_left最大是8，最小是0，取值范围是[0,8]  
            i_count  -= s->i_left;   //待读取的比特数，等于原i_count减去i_left，i_left是当前字节未读过的比特数，而此else阶段，i_left代表的当前字节未读的比特全被读过了，所以减它  
            s->p++;  //定位到下一个新的字节  
            s->i_left = 8;   //对一个新字节来说，未读过的位数当然是8，即本字节所有位都没读取过  
        }  
    }  
  
    return( i_result );//可能的返回值之一为：00000000 00000000 00000000 00000001 (4字节长)  
}  


/*把要读的比特移到当前字节最右，然后与0x01:00000001进行逻辑与操作，
  因为要读的只是一个比特，这个比特不是0就是1，与0000 0001按位与就可以得知此情况  */ 
int bs_read1( bs_t *s )  
{  
  
    if( s->p < s->p_end )    
    {  
        unsigned int i_result;  
  
        s->i_left--;                           //当前字节未读取的位数少了1位  
        
        i_result = ( *s->p >> s->i_left )&0x01;
        if( s->i_left == 0 )                   //如果当前字节剩余未读位数是0，即是说当前字节全读过了  
        {  
            s->p++;                             //指针s->p 移到下一字节  
            s->i_left = 8;                     //新字节中，未读位数当然是8位  
        }  
        return i_result;                       //unsigned int  
    }  
  
    return 0;                                  //返回0应该是没有读到东西  
}  


int bs_read_ue( bs_t *s )  
{  
    int i = 0;  
    
    //条件为：读到的当前比特=0，指针未越界，最多只能读32比特  
    for(i=0;bs_read1( s ) == 0 && s->p < s->p_end && i < 32;++i) ; 
  
    return( ( 1 << i) - 1 + bs_read( s, i ) );      
}  

/*
	NAL_SLICE and IDR
	buf为nalu的数据，不包括头
	len也不包括头 
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
//            printf("P帧 ");
            break;  
        case 1: case 6: /* B */  
            result = FRAME_B; 
			printf("B帧 "); 
            break;  
        case 3: case 8: /* SP */  
            result = FRAME_P;  
            printf("SP帧 ");
            break;  
        case 2: case 7: /* I */  
            result = FRAME_I;  
            printf("I帧 ");
            break;  
        case 4: case 9: /* SI */  
            result = FRAME_I;  
            printf("SI帧 ");
            break;  
	} 
	
	return  result;
}

uint32_t findBox(FILE* fp, uint32_t target)    // 
{
	uint32_t boxSize, boxType;
	uint32_t offset = 0x0;
	uint8_t size[4],type[4];
	int m,n;                //用于判断文件末尾 
	
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
	uint32_t tempLength, tempOffset,offset; //记录mdat的offset 
	uint32_t result; 
	
	
	control = 0;
	fread(size,1,4,fp);
	while(!control&&!feof(fp)){     //找trak  
    	fread(type,1,4,fp);
    	boxSize = decodeUint32(size);
    	boxType = decodeUint32(type);
    	if(boxType == trak) {
    		tempLength = boxSize;
    		offset = 0x0;
    		
    		tempOffset = findBox(fp,mdia); //找mdia
			offset  +=  tempOffset;
			
			tempOffset = findBox(fp,minf); //找minf
			offset  +=  tempOffset;
			
			fread(size,1,4,fp);             //找vmhd
    		fread(type,1,4,fp);
    		boxSize = decodeUint32(size);
    		boxType = decodeUint32(type);
    		if(boxType == vmhd){
    			control = 1;
    			fseek(fp,boxSize-8,SEEK_CUR);
    			findBox(fp,stbl);    //找stbl
				findBox(fp,stsd);    //找stsd
				fread(size,1,4,fp);             //找去掉版本号之类 
    			fread(type,1,4,fp);
				
				fread(size,1,4,fp);             //找avc1
    			fread(type,1,4,fp);
    			boxSize = decodeUint32(size);
    			boxType = decodeUint32(type);
				if(boxType == avc1){
					fread(buf,1,78,fp); 
					findBox(fp,avcC);    //找avcC
					fread(size,1,4,fp);  //去掉版本号之类的
					fread(size,1,4,fp);
					result = decodeUint32_2(size);
					
					result = result&0x00000003 + 0x00000001;
					return result;
				}
				else{
					printf("不是标准的avc编码.");
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
	uint32_t offset = 0x0; //记录mdat的offset 
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
    while(control!= 1&&!feof(fp)){   //找到moov的入口 
    	fread(type,1,4,fp);
    	boxSize = decodeUint32(size);
    	boxType = decodeUint32(type);
    	if(boxType == moov){
    		control = 1;
    		//子函数，处理里面的box，取pps，sps，和构造table 
    		naluLength=(int)getParameter(fp);   //naluLength个字节表示长度 
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
	
	if(flag == 1){                        //处理mdat 
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
					printf("SEI帧 ");	
				}
				else{
					fseek(fp,boxSize-1,SEEK_CUR);
					printf("其他 ");
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
					printf("SEI帧 ");	
				}
				else{
					fseek(fp,boxSize-1,SEEK_CUR);
					printf("其他 ");
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

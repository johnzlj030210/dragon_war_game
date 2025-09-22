#include "stdAfx.h"
#include "ge_font8x16.h"
#include "ge_fontgb16.h"

#define en8_index(c)    ((c) * 16)
#define gb16_index(l,r) (((((l)-0xa1)&0x7f)*94+(((r)-0xa1)&0x7f)) << 5)

// str can be English and Chinese
void print_string_argb(char *buf, int stride, char *str)
{
	int i, j, k;
	unsigned char * word;
	unsigned int strL, strH;

	for(i=0;;i++)
	{
		char *buf1 = buf;
		strL=str[i] & 0xff;
		if(!strL)
			break;
		if(strL>0xa1){
			strH=str[++i] & 0xff;
			word = (unsigned char *)&font_gb16[gb16_index(strL, strH)];
			for(j=0; j<32; j+=2)
			{
				unsigned short a = ((unsigned short)word[j+1]) & 0xff;
				unsigned short b = ((unsigned short)word[j]) & 0xff;
				unsigned short c = a | (b<<8);
				unsigned int *buf2 = (unsigned int *)buf1;
				for(k=0; k<16; k++)
					if(c & (0x8000>>k))
						buf2[k] = 0x00ff5500;
				buf1 += stride;
			}
			buf += 64;
		}else{
			word = (unsigned char *)&fontdata_8x16[en8_index(strL)];
			for(j=0; j<16; j++)
			{
				unsigned short a = ((unsigned short)word[j]) & 0xff;
				unsigned int *buf2 = (unsigned int *)buf1;
				for(k=0; k<8; k++)
					if(a & (0x80>>k))
						buf2[k] = 0x00ff5500;
				buf1 += stride;
			}
			buf += 32;
		}

	}
}

// str can be English and Chinese
void print_string_byte(char *buf, int stride, char *str)
{
	int i, j, k;
	unsigned char * word;
	unsigned int strL, strH;

	for(i=0;;i++)
	{
		char *buf1 = buf;
		strL=str[i] & 0xff;
		if(!strL)
			break;
		if(strL>0xa1){
			strH=str[++i] & 0xff;
			word = (unsigned char *)&font_gb16[gb16_index(strL, strH)];
			for(j=0; j<32; j+=2)
			{
				unsigned short a = ((unsigned short)word[j+1]) & 0xff;
				unsigned short b = ((unsigned short)word[j]) & 0xff;
				unsigned short c = a | (b<<8);
				for(k=0; k<16; k++)
				{
					if(c & (0x8000>>k))
						buf1[k] = 1;
					else
						buf1[k] = 0;
				}
				buf1 += stride;
			}
			buf += 16;
		}else{
			word = (unsigned char *)&fontdata_8x16[en8_index(strL)];
			for(j=0; j<16; j++)
			{
				unsigned short a = ((unsigned short)word[j]) & 0xff;
				for(k=0; k<8; k++)
				{
					if(a & (0x80>>k))
						buf1[k] = 1;
					else
						buf1[k] = 0;
				}
				buf1 += stride;
			}
			buf += 8;
		}

	}
}

// str can be English and Chinese
void print_string_16bit(char *buf, int stride, char *str)
{
	int i, j, k;
	unsigned char * word;
	unsigned int strL, strH;

	for(i=0;;i++)
	{
		char *buf1 = buf;
		strL=str[i] & 0xff;
		if(!strL)
			break;
		if(strL>0xa1){
			strH=str[++i] & 0xff;
			word = (unsigned char *)&font_gb16[gb16_index(strL, strH)];
			for(j=0; j<32; j+=2)
			{
				unsigned short a = ((unsigned short)word[j+1]) & 0xff;
				unsigned short b = ((unsigned short)word[j]) & 0xff;
				unsigned short c = a | (b<<8);
				unsigned short *buf2 = (unsigned short *)buf1;
				for(k=0; k<16; k++)
					if(c & (0x8000>>k))
						buf2[k] = 0;
				buf1 += stride;
			}
			buf += 32;
		}else{
			if(strL=='\n')
				return;
			word = (unsigned char *)&fontdata_8x16[en8_index(strL)];
			for(j=0; j<16; j++)
			{
				unsigned short a = ((unsigned short)word[j]) & 0xff;
				unsigned short *buf2 = (unsigned short *)buf1;
				for(k=0; k<8; k++)
					if(a & (0x80>>k))
						buf2[k] = 0;
				buf1 += stride;
			}
			buf += 16;
		}
	}
}


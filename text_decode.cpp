#include "stegosecure.h"
//start of decode function
//input:  image with hidden message encypted in it based on above technique 
//		  and password set during input
//output: hidden message
int decoder(char *ip,char *stegokey)
{
	IplImage *input;  //stego image
	uchar *inputdata;
	//char *stegokey="4123";
	long key=atol(stegokey);
	int height,width,nchannels,dataPos=0,msg_len=0;
	input=cvLoadImage(ip);
	height=input->height;
	width=input->width;
	nchannels=input->nChannels;

	//long keypr=nextPrime(key);
	long keypr=key;
	inputdata=(uchar *)input->imageData;
	int x=digSum(key);
	int a=2*x,h,p,bit_inp,inp_lsb;
	printf("stego key=%li",keypr);
	for( int i=0;i<20;i++)
	{
		h=(a*x)%key;
		p=h%7+1;                                 // pseudo random index calculated by discrete log
		bit_inp=getBit(inputdata[dataPos],p);	// LSB bit 
		inp_lsb=getBit(inputdata[dataPos],0);
		if(inp_lsb==1)                           // if LSB = 0 then random bit value = message bit, else message bit= ! ( random bit value)
		{
			msg_len=msg_len+(1<<i)*bit_inp;
		}
		else
		{
			msg_len=msg_len+(1<<i)*(1-bit_inp);  // complement of pth bit
		}
		x=h;
		dataPos++;
	}	
	char outdata[100000];	
	for (int i=0;i<msg_len;i++)
	{
		outdata[i]=0;
	}
	if(msg_len > height*width*nchannels+20)
	{
		printf("incorrect password.");
		exit(2);
	}
	outdata[msg_len]='\0';
	for ( int i=0;i<msg_len;i++)
	{
		for(int j=0;j<8;j++)
		{
			h=(a*x)%key;
			p=h%7+1;                                 // pseudo random index calculated by discrete log
			bit_inp=getBit(inputdata[dataPos],p);	// LSB bit 
			inp_lsb=getBit(inputdata[dataPos],0);
			if(inp_lsb==1)                           // if LSB = 1 then random bit value = message bit
			{
				outdata[i]=outdata[i]+(1<<j)*bit_inp;
			}
			else
			{
				outdata[i]=outdata[i]+(1<<j)*(1-bit_inp);  // complement of pth bit
			}
			x=h;
			dataPos++;
		}
	}
	printf("msg length= %d\n message=%s",msg_len,outdata);
	while(1);
	return 0;

}


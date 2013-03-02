#include "stegosecure.h"

int im_decoder(char *ip,char *stegokey)
{
	IplImage *input,*output;
	uchar *indata,*outdata;

	long key=atol(stegokey);
	int in_height,in_width,in_channels,dataPos=0,out_height=0,out_width=0,out_channels=3;
	input=cvLoadImage(ip);
	in_height=input->height;
	in_width=input->width;
	in_channels=input->nChannels;

	//long keypr=nextPrime(key);
	long keypr=key;
	indata=(uchar *)input->imageData;
	int x=digSum(key);
	int a=2*x,h,p,bit_inp,inp_lsb;
	printf("stego key=%li",keypr);
	for( int i=0;i<20;i++)
	{
		h=(a*x)%key;
		p=h%7+1;                                 // pseudo random index calculated by discrete log
		bit_inp=getBit(indata[dataPos],p);	// LSB bit 
		inp_lsb=getBit(indata[dataPos],0);
		if(inp_lsb==1)                           // if LSB = 0 then random bit value = message bit, else message bit= ! ( random bit value)
		{
			out_height=out_height+(1<<i)*bit_inp;
		}
		else
		{
			out_height=out_height+(1<<i)*(1-bit_inp);  // complement of pth bit
		}
		x=h;
		dataPos++;
	}	
	for( int i=0;i<20;i++)
	{
		h=(a*x)%key;
		p=h%7+1;                                 // pseudo random index calculated by discrete log
		bit_inp=getBit(indata[dataPos],p);	// LSB bit 
		inp_lsb=getBit(indata[dataPos],0);
		if(inp_lsb==1)                           // if LSB = 0 then random bit value = message bit, else message bit= ! ( random bit value)
		{
			out_width=out_width+(1<<i)*bit_inp;
		}
		else
		{
			out_width=out_width+(1<<i)*(1-bit_inp);  // complement of pth bit
		}
		x=h;
		dataPos++;
	}	
	printf("secret image dimensions:\nheight=%d  width=%d  \n",out_height,out_width);
	
	if( in_height*in_width*in_channels < out_height*out_width*out_channels + 40)
	{
		printf("incorrect password.");
		exit(2);
	}
	
	output=cvCreateImage(cvSize(out_width,out_height),8,3);
	outdata=(uchar *)output->imageData;
	//now dimensions has been achieved. now we attempt to recover secret image
	for ( int i=0;i<out_height*out_width*3;i++)
	{
		outdata[i]=0;
			for(int j=0;j<8;j++)
			{
				h=(a*x)%key;
				p=h%7+1;                                 // pseudo random index calculated by discrete log
				bit_inp=getBit(indata[dataPos],p);	// LSB bit 
				inp_lsb=getBit(indata[dataPos],0);
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
	cvSaveImage("secret_image_recovered.png",output,0);
	cvNamedWindow("stego image",CV_WINDOW_AUTOSIZE);
	cvShowImage("stego image",output);
	cvWaitKey(0);
	cvDestroyWindow("stego image");
	cvReleaseImage(&output);
	cvReleaseImage(&input);
	return 0;
}
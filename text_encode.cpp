#include "stegosecure.h"
int encoder(char *ip,char *hidedata,char *stegokey)
{
	IplImage *input;  //cover image
	IplImage *output; // stego image
	
	uchar *outdata;
	uchar *inputdata;
	int msg_len=strlen(hidedata);
	long key=atol(stegokey);

	int height,width,nchannels,dataPos=0;
	input=cvLoadImage(ip,-1);
	height=input->height;
	width=input->width;
	nchannels=input->nChannels;
	
	//long keypr=nextPrime(key);
	long keypr=key;
	//now we will make an empty image of same size as original
	output=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,nchannels);
	outdata=(uchar *)output->imageData;
	inputdata=(uchar *)input->imageData;
	int x=digSum(key);
	int a=2*x,h,p,bit_inp,bit_msg;
	printf("stego key=%li\n message length=%d",keypr,msg_len);
	for( int i=0;i<20;i++)
	{
		h=(a*x)%key;
		p=h%7+1;
		bit_inp=getBit(inputdata[dataPos],p);
		bit_msg=getBit(msg_len,i);
		if(bit_inp==bit_msg)
		{
			outdata[dataPos]=inputdata[dataPos] | 1;
		}
		else
		{
			outdata[dataPos]=inputdata[dataPos] & 254;
		}
		x=h;
		dataPos++;
	}	
	for( int i=0;i<msg_len;i++)
	{
		for( int j=0;j<8;j++)
		{
			h=(a*x)%key;
			p=h%7+1;
			bit_inp=getBit(inputdata[dataPos],p);
			bit_msg=getBit(hidedata[i],j);
			if(bit_inp==bit_msg)
			{
				outdata[dataPos]=inputdata[dataPos] | 1;
			}
			else
			{
				outdata[dataPos]=inputdata[dataPos] & 254;
			}
			x=h;
			dataPos++;
		}
	}
	
	for (int i=dataPos;i<height*width*nchannels;i++)
	{
		outdata[i]=inputdata[i];
	}
	cvSaveImage("hidden_text.png",output,0);
	cvNamedWindow("stego image",CV_WINDOW_AUTOSIZE);
	cvShowImage("stego image",output);
	cvWaitKey(0);
	cvDestroyWindow("stego image");
	cvReleaseImage(&output);
	cvReleaseImage(&input);
	return 0;
}

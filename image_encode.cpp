#include "stegosecure.h"

int im_encoder(char *ip,char *hidedata,char *stegokey)
{
	IplImage *input,*hide,*output;	 
	uchar *outdata,*indata,*hdata;
	
	//input of images - cover and secret images
	input=cvLoadImage(ip,-1);
	hide=cvLoadImage(hidedata,-1);

	int in_height,in_width,in_channels,dataPos=0,h_height,h_width,h_channels;
	
	in_height=input->height;
	in_width=input->width;
	in_channels=input->nChannels;

	h_height=hide->height;
	h_width=hide->width;
	h_channels=hide->nChannels;

	if( in_height*in_width*in_channels < h_height*h_width*h_channels + 40)
	{
		printf("secret image size too large.");
		exit(2);
	}
	indata=(uchar *)input->imageData;
	hdata=(uchar *)hide->imageData;

	//now we will make an empty image of same size as original
	output=cvCreateImage(cvSize(in_width,in_height),IPL_DEPTH_8U,3);
	outdata=(uchar *)output->imageData;

	//find the next prime to generate discrete logarithms
	long key=atol(stegokey);
	//long keypr=nextPrime(key);	
	long keypr=key;
	int x=digSum(key);
	int a=2*x,h,p,bit_inp,bit_msg;
	
	//now we are storing height in the cover image first
	for( int i=0;i<20;i++)
	{
		h=(a*x)%key;
		p=h%7+1;
		bit_inp=getBit(indata[dataPos],p);
		bit_msg=getBit(h_height,i);
		if(bit_inp==bit_msg)
		{
			outdata[dataPos]=indata[dataPos] | 1;
		}
		else
		{
			outdata[dataPos]=indata[dataPos] & 254;
		}
		x=h;
		dataPos++;
	}	
	//now we store the width in the cover image
	for( int i=0;i<20;i++)
	{
		h=(a*x)%key;
		p=h%7+1;
		bit_inp=getBit(indata[dataPos],p);
		bit_msg=getBit(h_width,i);
		if(bit_inp==bit_msg)
		{
			outdata[dataPos]=indata[dataPos] | 1;
		}
		else
		{
			outdata[dataPos]=indata[dataPos] & 254;
		}
		x=h;
		dataPos++;
	}	
	//now that we have stored the dimensions of the image, we can store our secret image
	for( int i=0;i<h_height*h_width*h_channels;i++)
	{
		for( int j=0;j<8;j++)
			{
				h=(a*x)%key;
				p=h%7+1;
				bit_inp=getBit(indata[dataPos],p);
				bit_msg=getBit(hdata[i],j);
				if(bit_inp==bit_msg)
				{
					outdata[dataPos]=indata[dataPos] | 1;
				}
				else
				{
					outdata[dataPos]=indata[dataPos] & 254;
				}
				x=h;
				dataPos++;
			}
	}
	//now the remaining pixels are simply equal to input image
	for (int i=dataPos;i<in_height*in_width*in_channels;i++)
	{
		outdata[i]=indata[i];
	}
	cvSaveImage("hidden_image.png",output,0);
	cvNamedWindow("stego image",CV_WINDOW_AUTOSIZE);
	cvShowImage("stego image",output);
	cvWaitKey(0);
	cvDestroyWindow("stego image");
	cvReleaseImage(&output);
	cvReleaseImage(&input);
	return 0;
}
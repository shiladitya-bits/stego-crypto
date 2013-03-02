#include<cv.h>
#include<highgui.h>
#include<stdlib.h>
#include<stdio.h>
#include<cxcore.h>
#include<string.h>
#include<stdio.h>

int encoder(char *input,char *msg,char *password);
int decoder(char *input,char *password);
int im_encoder(char *input,char *hide_im,char *password);
int im_decoder(char *input,char *password);

int getBit(int,int);
int digSum(long);
int checkPrime(long);
long nextPrime(long);

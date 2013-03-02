#include "stegosecure.h"
int main()
{
	int choice,hidetype;
	char passwd[100],msg[100],ip[100],hide_img[100];
	printf("enter cover image file name with extension ( indexed images not supported ) : ");
	scanf(" %[^\n]s",ip);
	do
	{
		printf("what do you want to hide?\n1.message\n2.image\nenter youur choice: ");
		scanf(" %d",&hidetype);
		if(hidetype==1)
		{
			printf("enter choice\n1.encode\n2.decode:\n");
			scanf(" %d",&choice);
			if(choice==1)
			{
				printf("enter message to hide: ");
				scanf(" %[^\n]s",msg);
				printf("enter password to protect message( only containing numbers): ");
				scanf(" %s",passwd);
				encoder(ip,msg,passwd);
				return 1;
			}
			else if(choice==2)
			{
				printf("enter password: ");
				scanf(" %s",passwd);
				decoder("hidden_text.png",passwd);
				return 1;
			}
			else
				printf("enter correct choice");
		}
		else if(hidetype==2)
		{
			printf("enter choice\n1.encode\n2.decode:\n");
			scanf(" %d",&choice);
			if(choice==1)
			{
				printf("enter secret image file name: ");
				scanf(" %[^\n]s",hide_img);
				printf("enter password to protect message( only containing numbers): ");
				scanf(" %s",passwd);
				im_encoder(ip,hide_img,passwd);
				return 1;
			}
			else if(choice==2)
			{
				printf("enter password: ");
				scanf(" %s",passwd);
				im_decoder("hidden_image.png",passwd);
				return 1;
			}
			else
				printf("enter correct choice");
		}
	}
	while(1);
	return 0;
}
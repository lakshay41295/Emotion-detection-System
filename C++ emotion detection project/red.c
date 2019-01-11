#include<Cips.h>
#include<stdio.h>
int main()
{
	struct ctstruct cm;
	short** theimage;
	long l,w;
	get_image_size("qt.bmp",&l,&w);
	read_color_table("qt.bmp",&cm,l);
	print_color_table(&cm,l);
	return (0);
}
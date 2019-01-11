#include<string.h>
#include<fstream.h>
#define GRAY_LEVELS 255
#define MAX_NAME_LENGTH       80
#define ROWS                 100
#define COLS                 100

void extract_ulong_from_buffer(char buffer[],int lsb,int start,unsigned long *number) ;
void extract_long_from_buffer(char buffer[],int lsb,int start, long *number);
void extract_ushort_from_buffer(char buffer[],int lsb,int start, unsigned short *number);
void read_bmp_file_header(char *file_name,struct bmpfileheader *file_header);
int get_image_size( char *file_name,long * rows,long * cols);



struct ctstruct{
unsigned char blue;
unsigned char green;
unsigned char red;
};

int free_image_array(short **the_array, long length)
{
int i;
for(i=0; i<length; i++)
free(the_array[i]);
return(1);
}

short **allocate_image_array(long length,long width)
{
int i;
short **the_array = (short **)malloc(length * sizeof(short *));
for(i=0; i<length; i++){
the_array[i] = (short *) malloc(width * sizeof(short ));
if(the_array[i] == '\0'){
printf("\n\tmalloc of the_image[%d] failed", i);
} /* ends if */
} /* ends loop over i */
return(the_array);
}

void flip_image_array(short **the_image,long rows,long cols)
{
int i, j;
long rd2;
short **temp;
temp = allocate_image_array(rows, cols);
rd2 = rows/2;
for(i=0; i<rd2; i++){
for(j=0; j<cols; j++){
temp[rows-1-i][j] = the_image[i][j];
} /* ends loop over j */
} /* ends loop over i */
for(i=rd2; i<rows; i++){
for(j=0; j<cols; j++){
temp[rows-1-i][j] = the_image[i][j];
} /* ends loop over j */
} /* ends loop over i */
for(i=0; i<rows; i++)
for(j=0; j<cols; j++)
the_image[i][j] = temp[i][j];
free_image_array(temp, rows);
}


struct bmpfileheader{
unsigned short filetype;         
unsigned long filesize;
short reserved1;
short reserved2;
unsigned long bitmapoffset;
};

struct tiff_header_struct{
short lsb;
long bits_per_pixel;
long image_length;
long image_width;
long strip_offset;
};


struct bitmapheader{
unsigned long size;
long width;
long height;
unsigned short planes;
unsigned short bitsperpixel;
unsigned long compression;
unsigned long sizeofbitmap;
unsigned long horzres;
unsigned long vertres;
unsigned long colorsused;
unsigned long colorsimp;
};


union long_char_union{
long l_num;
char l_alpha[4];
};

union short_char_union{
short s_num;
char s_alpha[2];
};

union ushort_char_union{
short s_num;
char s_alpha[2];
};




 
 union ulong_char_union{
long l_num;
char l_alpha[4];
};


int calculate_pad(long width)
{
int pad = 0;
pad = ( (width%4) == 0) ? 0 : (4-(width%4));
return(pad);
}


void read_color_table(char *file_name,struct ctstruct *rgb ,int size)
{
int i;
char buffer[10];
FILE *fp;
fp = fopen(file_name, "rb");
fseek(fp, 54, SEEK_SET);
for(i=0; i<size; i++){
fread(buffer, 1, 1, fp);
rgb[i].blue = buffer[0];
fread(buffer, 1, 1, fp);
rgb[i].green = buffer[0];
fread(buffer, 1, 1, fp);
rgb[i].red = buffer[0];
fread(buffer, 1, 1, fp);
/* fourth byte nothing */
} /* ends loop over i */
fclose(fp);
}

void read_bm_header(char *file_name,struct bitmapheader *bmheader)
{
char buffer[10];
long ll;
short ss;
unsigned long ull;
unsigned short uss;
FILE *fp;
fp = fopen(file_name, "rb");
/****************************************
*
* Seek past the first 14 byte header.
*
****************************************/
fseek(fp, 14, SEEK_SET);
fread(buffer, 1, 4, fp);
extract_ulong_from_buffer(buffer, 1, 0, &ull);
bmheader->size = ull;
fread(buffer, 1, 4, fp);
extract_long_from_buffer(buffer, 1, 0, &ll);
bmheader->width = ll;
fread(buffer, 1, 4, fp);
extract_long_from_buffer(buffer, 1, 0, &ll);
bmheader->height = ll;
fread(buffer, 1, 2, fp);
extract_ushort_from_buffer(buffer, 1, 0, &uss);
bmheader->planes = uss;
fread(buffer, 1, 2, fp);
extract_ushort_from_buffer(buffer, 1, 0, &uss);
bmheader->bitsperpixel = uss;
fread(buffer, 1, 4, fp);
extract_ulong_from_buffer(buffer, 1, 0, &ull);
bmheader->compression = ull;
fread(buffer, 1, 4, fp);
extract_ulong_from_buffer(buffer, 1, 0, &ull);
bmheader->sizeofbitmap = ull;
fread(buffer, 1, 4, fp);
extract_ulong_from_buffer(buffer, 1, 0, &ull);
bmheader->horzres = ull;
fread(buffer, 1, 4, fp);
extract_ulong_from_buffer(buffer, 1, 0, &ull);
bmheader->vertres = ull;
fread(buffer, 1, 4, fp);
extract_ulong_from_buffer(buffer, 1, 0, &ull);
bmheader->colorsused = ull;
fread(buffer, 1, 4, fp);
extract_ulong_from_buffer(buffer, 1, 0, &ull);
bmheader->colorsimp = ull;
fclose(fp);
}
 
void extract_ulong_from_buffer(char buffer[],int lsb,int start,unsigned long *number)   ////////bmp file used
{
int i;
union ulong_char_union lcu;
if(lsb == 1){
lcu.l_alpha[0] = buffer[start+0];
lcu.l_alpha[1] = buffer[start+1];
lcu.l_alpha[2] = buffer[start+2];
lcu.l_alpha[3] = buffer[start+3];
} /* ends if lsb = 1 */
if(lsb == 0){
lcu.l_alpha[0] = buffer[start+3];
lcu.l_alpha[1] = buffer[start+2];
lcu.l_alpha[2] = buffer[start+1];
lcu.l_alpha[3] = buffer[start+0];
} /* ends if lsb = 0 */
*number = lcu.l_num;
}

void extract_ushort_from_buffer(char buffer[],int lsb,int start, unsigned short *number)   /////bmp file
{
int i;
union ushort_char_union lcu;
if(lsb == 1){
lcu.s_alpha[0] = buffer[start+0];
lcu.s_alpha[1] = buffer[start+1];
} /* ends if lsb = 1 */
if(lsb == 0){
lcu.s_alpha[0] = buffer[start+1];
lcu.s_alpha[1] = buffer[start+0];
} /* ends if lsb = 0 */
*number = lcu.s_num;
}


void extract_long_from_buffer(char buffer[],int lsb,int start, long *number) /////tiff file
{
int i;
union long_char_union lcu;
if(lsb == 1){
lcu.l_alpha[0] = buffer[start+0];
lcu.l_alpha[1] = buffer[start+1];
lcu.l_alpha[2] = buffer[start+2];
lcu.l_alpha[3] = buffer[start+3];
} /* ends if lsb = 1 */
if(lsb == 0){
lcu.l_alpha[0] = buffer[start+3];
lcu.l_alpha[1] = buffer[start+2];
lcu.l_alpha[2] = buffer[start+1];
lcu.l_alpha[3] = buffer[start+0];
} /* ends if lsb = 0 */
*number = lcu.l_num;
}


void extract_short_from_buffer(char buffer[],int lsb,int start,short *number)   //////tiff file
{
int i;
union short_char_union lcu;
if(lsb == 1){
lcu.s_alpha[0] = buffer[start+0];
lcu.s_alpha[1] = buffer[start+1];
} /* ends if lsb = 1 */
if(lsb == 0){
lcu.s_alpha[0] = buffer[start+1];
lcu.s_alpha[1] = buffer[start+0];
} /* ends if lsb = 0 */
*number = lcu.s_num;
}

void read_tiff_header(char file_name[], struct tiff_header_struct *image_header)
{
char buffer[12], response[80];
FILE *image_file;   // file pointer to image  file
int bytes_read,closed,i,j,lsb,not_finished,position;
long bits_per_pixel,image_length,image_width,length_of_field,offset_to_ifd,strip_offset,subfile,value;
short entry_count,field_type,s_bits_per_pixel,s_image_length,s_image_width,s_strip_offset,tag_type;
image_file = fopen(file_name, "rb");
if(image_file != NULL){
/*************************************
*
* Determine if the file uses MSB
* first or LSB first
*
*************************************/
bytes_read = fread(buffer, 1, 8, image_file);   //////doubt  -------bytes read
if(buffer[0] == 0x49)
lsb = 1;
else
lsb = 0;
/*************************************
*
* Read the offset to the IFD
*
*************************************/
extract_long_from_buffer(buffer, lsb, 4,&offset_to_ifd);
not_finished = 1;
while(not_finished){
/*************************************
*
* Seek to the IFD and read the
* entry_count, i.e. the number of
* entries in the IFD.
*
*************************************/
position = fseek(image_file, offset_to_ifd,SEEK_SET);
bytes_read = fread(buffer, 1, 2, image_file);
extract_short_from_buffer(buffer, lsb, 0,&entry_count);
/***************************************
*
* Now loop over the directory entries.
* Look only for the tags we need. These
* are:
* ImageLength
* ImageWidth
* BitsPerPixel(BitsPerSample)
* StripOffset
*
*****************************************/
for(i=0; i<entry_count; i++){
bytes_read = fread(buffer, 1, 12, image_file);
extract_short_from_buffer(buffer, lsb, 0,&tag_type);    //////tag type =255
switch(tag_type){
case 255: /* Subfile Type */
extract_short_from_buffer(buffer, lsb, 2,&field_type);
extract_short_from_buffer(buffer, lsb, 4,(short *)&length_of_field);
extract_long_from_buffer(buffer, lsb, 8,&subfile);
break;
case 256: /* ImageWidth */
extract_short_from_buffer(buffer, lsb, 2,&field_type);
extract_short_from_buffer(buffer, lsb, 4,(short *)&length_of_field);
if(field_type == 3){
extract_short_from_buffer(buffer, lsb, 8,&s_image_width);
image_width = s_image_width;
}
else
extract_long_from_buffer(buffer, lsb, 8,&image_width);
break;
case 257: /* ImageLength */
extract_short_from_buffer(buffer, lsb, 2,&field_type);
extract_short_from_buffer(buffer, lsb, 4,(short *)&length_of_field);
if(field_type == 3){
extract_short_from_buffer(buffer, lsb, 8,&s_image_length);
image_length = s_image_length;
}
else
extract_long_from_buffer(buffer, lsb, 8,&image_length);
break;
case 258: /* BitsPerSample */
extract_short_from_buffer(buffer, lsb, 2,&field_type);
extract_short_from_buffer(buffer, lsb, 4,(short *)&length_of_field);
if(field_type == 3){
extract_short_from_buffer(buffer, lsb, 8,&s_bits_per_pixel);
bits_per_pixel = s_bits_per_pixel;
}
else
extract_long_from_buffer(buffer, lsb, 8,&bits_per_pixel);
break;
case 273: /* StripOffset */
extract_short_from_buffer(buffer, lsb, 2,&field_type);
extract_short_from_buffer(buffer, lsb, 4,(short *)&length_of_field);
if(field_type == 3){
extract_short_from_buffer(buffer, lsb, 8,&s_strip_offset);
strip_offset = s_strip_offset;
}
else
extract_long_from_buffer(buffer, lsb, 8,&strip_offset);
break;
default:
break;
} /* ends switch tag_type */
} /* ends loop over i directory entries */
bytes_read = fread(buffer, 1, 4, image_file);
extract_long_from_buffer(buffer, lsb, 0,
&offset_to_ifd);
if(offset_to_ifd == 0) not_finished = 0;
} /* ends while not_finished */
image_header->lsb = lsb;
image_header->bits_per_pixel = bits_per_pixel;
image_header->image_length = image_length;
image_header->image_width = image_width;
image_header->strip_offset = strip_offset;
closed = fclose(image_file);
} /* ends if file opened ok */
else{
printf("\n\nTIFF.C> ERROR - could not open "
"tiff file");
}
}







int read_line (FILE *image_file , short **the_image,int line_number, struct tiff_header_struct *image_header,int ie,int le)
{
char *buffer, first, second;
float a, b;
int bytes_read, i;
unsigned int bytes_to_read;
union short_char_union scu;
buffer = (char *) malloc(image_header->image_width * sizeof(char ));
for(i=0; i<image_header->image_width; i++)
buffer[i] = '\0';
/********************************************
*
* Use the number of bits per pixel to
* calculate how many bytes to read.
*
********************************************/
bytes_to_read = (le-ie)/(8/image_header->bits_per_pixel);
bytes_read = fread(buffer, 1, bytes_to_read,image_file);
for(i=0; i<bytes_read; i++){
/*********************************************
*
* Use unions defined in cips.h to stuff bytes
* into shorts.
*
**********************************************/
if(image_header->bits_per_pixel == 8){
scu.s_num = 0;
scu.s_alpha[0] = buffer[i];
the_image[line_number][i] = scu.s_num;
} /* ends if bits_per_pixel == 8 */
if(image_header->bits_per_pixel == 4){
scu.s_num = 0;
second = buffer[i] & 0X000F;
scu.s_alpha[0] = second;
the_image[line_number][i*2+1] = scu.s_num;
scu.s_num = 0;
first = buffer[i] >> 4;   //////doubt to be resolve
first = first & 0x000F;   ////
scu.s_alpha[0] = first;
the_image[line_number][i*2] = scu.s_num;
} /* ends if bits_per_pixel == 4 */
} /* ends loop over i */
free(buffer);
return(bytes_read);
}


 
void read_tiff_image(char image_file_name[], short **the_image)
{
char *buffer, rep[80];
int bytes_read,closed,position,i,j;
FILE *image_file;
float a;
long line_length, offset;
struct tiff_header_struct image_header;
read_tiff_header(image_file_name, &image_header);
/***********************************************
*
* Procedure:
* Seek to the strip offset where the data begins.
* Seek to the first line you want.
* Loop over the lines you want to read:
* Seek to the first element of the line.
* Read the line.
* Seek to the end of the data in that line.
*
************************************************/
image_file = fopen(image_file_name, "rb");
if(image_file != NULL){
position = fseek(image_file,image_header.strip_offset,SEEK_SET);
for(i=0; i<image_header.image_length; i++){
bytes_read = read_line(image_file, the_image,i, &image_header,0, image_header.image_width);   ///////bytes_read why used
} /* ends loop over i */
closed = fclose(image_file);
} /* ends if file opened ok */
else{
printf("\nRTIFF.C> ERROR - cannot open ""tiff file");
}
} 

int is_a_bmp(char *file_name)
{
   char   *cc;
   int    result = 0;
   struct bmpfileheader file_header;

   cc = strstr(file_name, ".bmp");
   if(cc == NULL)
      return(result);

   read_bmp_file_header(file_name, &file_header);
   if(file_header.filetype != 0x4d42)
      return(result);

   result = 1;
   return(result);
} 


int is_a_tiff(char *file_name)
{
   char   *cc;
   char   buffer[4];
   FILE   *fp;
   int    ok     = 0,
          result = 0;

   cc = strstr(file_name, ".tif");
   if(cc == NULL)
      return(result);

   fp = fopen(file_name, "rb");
   fread(buffer, 1, 4, fp);
   fclose(fp);

   if(buffer[0] == 0x49  &&
      buffer[1] == 0x49  &&
      buffer[2] == 0x2a  &&
      buffer[3] == 0x00)
      ok = 1;

   if(buffer[0] == 0x4d  &&
      buffer[1] == 0x4d  &&
      buffer[2] == 0x00  &&
      buffer[3] == 0x2a)
      ok = 1;

   if(ok == 0)
      return(result);

   result = 1;
   return(result);
}


void read_bmp_file_header(char *file_name,struct bmpfileheader *file_header)
{
char buffer[10];
long ll;
short ss;
unsigned long ull;
unsigned short uss;
FILE *fp;
fp = fopen(file_name, "rb");
fread(buffer, 1, 2, fp);
extract_ushort_from_buffer(buffer, 1, 0, &uss);
file_header->filetype = uss;
fread(buffer, 1, 4, fp);
extract_ulong_from_buffer(buffer, 1, 0, &ull);
file_header->filesize = ull;
fread(buffer, 1, 2, fp);
extract_short_from_buffer(buffer, 1, 0, &ss);
file_header->reserved1 = ss;
fread(buffer, 1, 2, fp);
extract_short_from_buffer(buffer, 1, 0, &ss);
file_header->reserved2 = ss;
fread(buffer, 1, 4, fp);
extract_ulong_from_buffer(buffer, 1, 0, &ull);
file_header->bitmapoffset = ull;
fclose(fp);
}




void read_bmp_image(char *file_name,short **image)
{
FILE *fp;
int i, j;
int negative = 0,pad = 0,place = 0;
long colors = 0,height = 0,position = 0,width = 0;
struct bmpfileheader file_header;
struct bitmapheader bmheader;
struct ctstruct rgb[GRAY_LEVELS+1];
unsigned char uc;
read_bmp_file_header(file_name, &file_header);
read_bm_header(file_name, &bmheader);
if(bmheader.bitsperpixel != 8){
printf("\nCannot read image when bits per"
"pixel is not 8");
exit(1);
}
if(bmheader.colorsused == 0)
colors = GRAY_LEVELS + 1;
else
colors = bmheader.colorsused;
read_color_table(file_name, rgb, colors);
fp = fopen(file_name, "rb");
fseek(fp, file_header.bitmapoffset, SEEK_SET);
width = bmheader.width;
if(bmheader.height < 0){
height = bmheader.height * (-1);
negative = 1;
}
else
height = bmheader.height;
pad = calculate_pad(width);
for(i=0; i<height; i++){
for(j=0; j<width; j++){
place = fgetc(fp);
uc = (place & 0xff);
place = uc;
image[i][j] = rgb[place].blue;
} /* ends loop over j */
if(pad != 0){
position = fseek(fp, pad, SEEK_CUR);
} /* ends if pad 1= 0 */
} /* ends loop over i */
if(negative == 0)
flip_image_array(image, height, width);
}



 void read_image_array(char *file_name,short **array)
{
int ok = 0;
if(is_a_tiff(file_name)){
read_tiff_image(file_name, array);
ok = 1;
}
if(is_a_bmp(file_name)){
read_bmp_image(file_name, array);
ok = 1;
}
if(ok == 0){
printf("\nERROR could not read file %s",
file_name);
exit(1);
}
}

int get_image_size( char *file_name,long * rows,long * cols)

{
   int is_bmp  = 0,
       is_tiff = 0,
       result  = 0;
   struct bitmapheader bmph;
   struct tiff_header_struct tiffh;

   if(is_a_bmp(file_name)){
      is_bmp = 1;
      read_bm_header(file_name, &bmph);
      *rows = bmph.height;
      *cols = bmph.width;
   }  /* ends if is_a_bmp */

   if(is_a_tiff(file_name)){
      is_tiff = 1;
      read_tiff_header(file_name, &tiffh);
      *rows = tiffh.image_length;
      *cols = tiffh.image_width;
   }  /* ends if is_a_bmp */

   if(is_bmp == 1  ||  is_tiff == 1)
      result = 1;

   return(result);
}


void print_color_table(struct ctstruct *rgb, int size)
{
   int i;

   for(i=0; i<size; i++){
      printf("\n %d %d %d",
      rgb[i].blue,rgb[i].green,rgb[i].red);
   }  /* ends loop over i */
}

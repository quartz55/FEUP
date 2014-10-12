#include <minix/drivers.h>
#include <sys/video.h>
#include <sys/mman.h>

#include <assert.h>

#include "vt_info.h"

#include "video_txt.h"

/* Private global variables */

static char *video_mem;		/* Address to which VRAM is mapped */

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;	/* Height of screen in lines */

void vt_fill(char ch, char attr) {

  char *vptr; /* Pointer to video RAM */
  vptr = video_mem; /* Point to first byte of video RAM */
  unsigned int i = 0;
  while (i<scr_width*scr_lines){  /* 25*80 characters */
    *vptr = ch; /* First byte, is for the character to display */
    vptr++; /* Point to second byte of video RAM */
    *vptr = attr; /* Second byte is for the attributes to display */
    vptr++; /* Point to next character */
    i++;
  }
}

void vt_blank() {
  vt_fill(0x00, 0x7); //Use the fill function to fill the screen with empty chars
}

int vt_print_char(char ch, char attr, int r, int c) {

  if (r >= scr_lines || r < 0 || c >= scr_width || c < 0) return 1;
  char *vptr; /* Pointer to video RAM */
  vptr = video_mem + 2*(r*scr_width+c);
  *vptr = ch;
  vptr++;
  *vptr = attr;
  return 0;
}

int vt_print_string(char *str, char attr, int r, int c) {

  if (r >= scr_lines || r < 0 || c >= scr_width || c < 0 || (c+strlen(str)>=scr_width)) return 1;
  unsigned int i;
  int position = c;
  for(i=0; i<strlen(str);i++){
    if(vt_print_char(str[i], attr, r, position) != 0)
      return 1;
    position++;
  }
  return 0;
}

int vt_print_int(int num, char attr, int r, int c) {

  char * str;
  unsigned int size = 0;
  if(num<0){ //If it is a negative number, insert a '-' in the beginning
    size++;
    num *= -1;
    str[0] = '-';
  }



  int n = num;
  do{ // Get the number of digits of the number 
    size++;
    n = n/10;
  }while(n);

  /* ALTERNATE METHDO
  str = malloc(size*sizeof(char));

  sprintf(str, "%d", num);
  */

     n = num;
     str = malloc(size*sizeof(char));
     int i;
     for(i = size-1; i >= 0; i--){ // Move back, inserting digits as u go
     str[i] = (n%10)+0x30; //Insert the digit converted to ASCII
     n = n/10;
     }

     str[size] = '\0';
     int res = vt_print_string(str,attr,r,c);
     free(str);
     return res;
}


int vt_draw_frame(int width, int height, char attr, int r, int c) {
  char border = '#'; //Char to use for the frame

  char *line = malloc(width*sizeof(char)); //Allocate space equivalent to the width of the frame

  unsigned int i;
  for(i=0;i<width;i++){
    line[i] = border; //Fill in the line with the border char defined previously
  }

  int result;

  for(i=0;i<height;i++){
    if(i==0 || i == height-1) //If it's the first and last line draw the frame horizontally
      result = vt_print_string(line, attr, r+i, c);
    else{ //Else only draw the first and last char
      result = vt_print_char(border, attr, r+i, c);
      result = vt_print_char(border, attr, r+i, c+width-1);
    }
  }
  free(line);
  return result;
}

/*
 * THIS FUNCTION IS FINALIZED, do NOT touch it
 */

char *vt_init(vt_info_t *vi_p) {

  int r;
  struct mem_range mr;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes)(vi_p->vram_base);
  mr.mr_limit = mr.mr_base + vi_p->vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vi_p->vram_size);

  if(video_mem == MAP_FAILED)
    panic("video_txt couldn't map video memory");

  /* Save text mode resolution */

  scr_lines = vi_p->scr_lines;
  scr_width = vi_p->scr_width;

  return video_mem;
}

/* zcc +zx -vn -lndos efla.c -o efla -create-app */

/*

Crude "extremely fast line algorithm" implementation, using the
Z88DK classic libraries.

From http://www.edepot.com/linee.html but it's nowhere near as
fast as the ASM line code in Z88DK (which, as far as I know,
doesn't use this algorithm).

*/

#include <graphics.h>
#include <stdlib.h> 

typedef void*  SURFACE;
typedef unsigned char bool;
#define false 0
#define true 1

void myPixel(SURFACE* surface, int x,int y) {
      plot(x,y);
}


void myLine(SURFACE* surface, int x, int y, int x2, int y2) {
   	bool yLonger=false;
	int shortLen=y2-y;
	int longLen=x2-x;
	int decInc;
		  int j;		  

	if (abs(shortLen)>abs(longLen)) {
		int swap=shortLen;
		shortLen=longLen;
		longLen=swap;				
		yLonger=true;
	}
	if (longLen==0) decInc=0;
	else decInc = (shortLen << 8) / longLen;

	if (yLonger) {
		if (longLen>0) {
			longLen+=y;
			for (j=0x80+(x<<8);y<=longLen;++y) {
				myPixel(surface,j >> 8,y);	
				j+=decInc;
			}
			return;
		}
		longLen+=y;
		for (j=0x80+(x<<8);y>=longLen;--y) {
			myPixel(surface,j >> 8,y);	
			j-=decInc;
		}
		return;	
	}

	if (longLen>0) {
		longLen+=x;
		for (j=0x80+(y<<8);x<=longLen;++x) {
			myPixel(surface,x,j >> 8);
			j+=decInc;
		}
		return;
	}
	longLen+=x;
	for (j=0x80+(y<<8);x>=longLen;--x) {
		myPixel(surface,x,j >> 8);
		j-=decInc;
	}

}

void mySquare(SURFACE* surface,int x, int y, int x2, int y2) {
	myLine(surface,x,y,x2,y2);
	myLine(surface,x2,y2,x2+(y-y2),y2+(x2-x));
	myLine(surface,x,y,x+(y-y2),y+(x2-x));
	myLine(surface,x+(y-y2),y+(x2-x),x2+(y-y2),y2+(x2-x));
}


void myRect(SURFACE* surface, int x, int y, int x2, int y2) {
	myLine(surface,x,y,x2,y);
	myLine(surface,x2,y,x2,y2);
	myLine(surface,x2,y2,x,y2);
	myLine(surface,x,y2,x,y);
}



int main()
{
  SURFACE surface = (SURFACE)0;

  int x;
    for( x=0; x<200; x++) 
    {
      /*myLine(surface,0,0,50,50); */
      draw(0,0,50,50);
    }
}

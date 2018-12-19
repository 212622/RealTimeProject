#include "lib/Headerfiles.h"
#include "lib/Init.h"

BITMAP *bitmap(char* filename){
	BITMAP	*bitmap;
	printf("%s",filename);
	bitmap = load_bitmap(filename, NULL);
	if(bitmap == NULL){
		printf("file not founf\n");
		exit(1);
	}
	return bitmap;
}

int main(int argc, char const *argv[])
{
	/* code */
	init();
	float x = 1024,y = 300;
	float x2 = 0, y2 = 300;
    int col = 14;
	int Defense = 0;
	BITMAP *razzo = bitmap("img/rocket.bmp");
	BITMAP *razzo2 = bitmap("img/rocket2.bmp");
	BITMAP *boom = bitmap("img/Pow.bmp");
	do{	
		if(key[KEY_ENTER]){		
			while(x > 0 && !key[KEY_ESC] && x > x2){	
				
				if(key[KEY_SPACE]){	
					Defense = 1; 
				}
				putpixel(screen, x+5+razzo->w, y+(razzo->h)/2, col);
				draw_sprite(screen, razzo, x, y);
				x = x - speed;

				if(Defense == 1){
					putpixel(screen, x2-20, y2+(razzo2->h)/2, col);
					draw_sprite(screen, razzo2, x2, y2);
					x2 = x2 + speed/2;
				}
				
				
			
			
			}
		draw_sprite(screen, boom, x, y-20-(razzo->h)/2);
		//draw_sprite(screen, boom, x, y2-20-(razzo2->h)/2);
		}
	}while (!key[KEY_ESC]);
	sleep(2);
	destroy_bitmap(razzo);
	destroy_bitmap(boom);
   	allegro_exit();
	return 0;
}



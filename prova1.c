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
	srand(time(NULL));
	init();
	BITMAP *razzo = bitmap("img/rocket.bmp");
	BITMAP *razzo2 = bitmap("img/rocket2.bmp");
	BITMAP *boom = bitmap("img/Pow.bmp");
	BITMAP *sfondo = bitmap("img/sfondo.bmp");
	do{
		float x = rand()%1024,y = 0;
		float x2 = x, y2 = 780;
    	int col = 14;
		int Defense = 0;
		while(y < 768-razzo->h && !key[KEY_SPACE] && y < y2){	
			if(key[KEY_ENTER] || y > 100+rand()%400){	
				Defense = 1; 
			}
			//putpixel(screen, x, y-5+(razzo->h)/2, col);
			draw_sprite(screen, razzo, x, y);
			y = y + speed;
			if(Defense == 1){
				//putpixel(screen, x2, y2+5+(razzo2->h)/2, col);
				draw_sprite(screen, razzo2, x2, y2);
				y2 = y2 - speed;
			}
			draw_sprite(screen, sfondo, 0, 600);
		}
		draw_sprite(screen, boom, x+5-(boom->w)/2, y);
		sleep(1);
		rectfill(screen,0,0,1024,768,BLACK);
	}while (!key[KEY_ESC]);
	destroy_bitmap(razzo2);
	destroy_bitmap(razzo);
	destroy_bitmap(boom);
	destroy_bitmap(sfondo);
   	allegro_exit();
	return 0;
}



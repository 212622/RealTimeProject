#include "lib/Headerfiles.h"
#include "lib/Init.h"

void make_p_b(char *old, char *new){
	BITMAP *img, *imgp;
	PALETTE pal;
	int x, y, c;
	int pink, white;
	white = makecol(255,255,255);
	pink = makecol(255,0,255);
	img = load_bitmap(old, NULL);
	imgp = create_bitmap(img->w,img->h);
	for(x=0;x<img->w;x++){
			for(y=0;y<img->h;y++){
				c=getpixel(img,x,y);
				if(c==white) c = pink;
				putpixel(imgp,x,y,c);
			}
	get_palette(pal);
	save_bitmap(new,imgp,pal);
	}
}


void enemy(){
	float x = rand()%1024, y = 0, alfa = rand()%3;
	int a;
	speed = 2 + rand()%20;

	BITMAP *aereo = load_bitmap("img/aereo.bmp", NULL);
	while(1){

		//alfa usata per decidere se il razzo si sposta a destra sinistra o centrale
		if(alfa == 0)
			a = 1;
		if(alfa == 1)
			a = -1;
		if(alfa == 2)
			a = 0;

		//se il razzo non è arrivato alla citta scende, altrimenti scoppia
		if(x < 1024 && x > 0 && y < 600-aereo->h){
			y = y + speed;
			x = x - (a * speed/5);
			draw_sprite(buf, aereo, x, y);
		}/*}else{
			al_draw_bitmap(boom, x+50-al_get_bitmap_width(boom), y, 0);
		}*/	
		
		ptask_wait_for_period();
	}
}

int main(int argc, char const *argv[])
{	
	
	init();
	int i=0, scan;
	buf = create_bitmap(SCREEN_W, SCREEN_H);
	BITMAP *sfondo = load_bitmap("img/sfondo.bmp", NULL);
	clear_to_color(buf, makecol(0,0,0));
	draw_sprite(buf, sfondo, 0, 600);
	//make_p_b("img/aereo.bmp", "img/aa.bmp");
	do{
		scan = 0;
		if(keypressed()) scan = readkey() >> 8;
		if(scan == KEY_SPACE && i < MAXT){
			ptask_create_prio(enemy, PER, PRIO, NOW);
			i++;
		}
		blit(buf, screen, 0, 0, 0, 0, buf->w, buf->h);
	}while(!key[KEY_ESC]);
	allegro_exit();
	return 0;
}



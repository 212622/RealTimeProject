#include "lib/Headerfiles.h"
#include "lib/Init.h"

ALLEGRO_BITMAP *bitmap(char* filename){
	ALLEGRO_BITMAP	*bitmap;
	bitmap = al_load_bitmap(filename);
	if(bitmap == NULL){
		printf("%s : file not found\n", filename);
		exit(1);
	}
	return bitmap;
}

int main(int argc, char const *argv[])
{
	/* code */
	
	init();
	
    ALLEGRO_BITMAP *sfondo = bitmap("img/sfondo.bmp");
	ALLEGRO_BITMAP *razzo = bitmap("img/rocket.png");
	ALLEGRO_BITMAP *boom = bitmap("img/Pow.bmp");

	float x = rand()%1024, y = 0, alfa = rand()%3;
	int a;
	speed = 2 + rand()%20;
	while(1){
		al_clear_to_color(BLACK);
		al_draw_bitmap(sfondo, 0, 600, 0);
		//alfa usata per decidere se il razzo si sposta a destra sinistra o centrale
		if(alfa == 0)
			a = 1;
		if(alfa == 1)
			a = -1;
		if(alfa == 2)
			a = 0;
		//Se premo ESC esco
		al_get_keyboard_state(&state);
		if(al_key_down(&state, ALLEGRO_KEY_ESCAPE)){
			break;
		}
		//se il razzo non è arrivato alla citta scende, altrimenti scoppia
		if(x < 1024 && x > 0 && y < 600-al_get_bitmap_height(razzo)){
			y = y + speed;
			x = x - (a * speed/5);
			al_draw_bitmap(razzo, x, y, 0);
		}else{
			al_draw_bitmap(boom, x+50-al_get_bitmap_width(boom), y, 0);
			x = rand()%1024;
			alfa = rand()%3;
			y = 0;
			speed = 2 + rand()%20;
		}
		al_flip_display();	
	}
	al_destroy_bitmap(razzo);
	al_destroy_bitmap(sfondo);
	al_destroy_bitmap(boom);
	return 0;
}



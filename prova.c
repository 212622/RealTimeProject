/*--------------------------------------------------------------*/
/*  Patriots																															*/
/*	Real Time Project by Tomas Torricelli and Lorenzo Cuoghi																			*/
/*																																		*/
/*	This project simulate a physical system or the behavior of active agents interacting with each other and with the environment		*/
/*	Simulate a set of Patriot defense missiles that identify enemy targets, predict their trajectories and are launched to catch them 	*/
/*--------------------------------------------------------------*/
#include "lib/Headerfiles.h"
#include "lib/Init.h"
/*--------------------------------------------------------------*/
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
/*--------------------------------------------------------------*/
/*  Periodic task for camera detection   */
/*--------------------------------------------------------------*/
void camera(){
	int x = 0, y = 0, v = 1, c, k, j;
	int grey = makecol(0, 0, 0); 
	while(1){
		rect(screen, x, y + YCAM, XCAM + x, y, makecol(255, 0, 0));
		if(x + XCAM > 1024) v = -1;
		if(x == 0) v = 1;
		x += 2 * v;
		for(k = 0; k < XCAM; k++){
			for(j = 0; j < YCAM; j++){
				c = getpixel(buf, x+k, y+j);
				if(c != grey) printf("TROVATO: %d %d\n",k,j);
			}
		}
		ptask_wait_for_period();
	}
}
/*--------------------------------------------------------------*/
/*  Periodic task for drawing   */
/*--------------------------------------------------------------*/
void draw(){
	while(1){
		blit(buf, screen, 0, 0, 0, 0, buf->w, buf->h);
		ptask_wait_for_period();
	}
}
/*--------------------------------------------------------------*/
/*  Periodic task for background   */
/*--------------------------------------------------------------*/
void background(){
	BITMAP *sfondo = load_bitmap("img/sfondo.bmp", NULL);
	while(1){
		clear_to_color(buf, makecol(0,0,0));
		draw_sprite(buf, sfondo, 0, 600);
		for(int k=0; k<MAXT; k++){
			 if(tid[k]!=0) 
			 	ptask_activate(tid[k]);
		}
		if(mti != 0)
			ptask_activate(mti);

		ptask_wait_for_period();
	}
}
/*--------------------------------------------------------------*/
/*  Periodic task for enemy   */
/*--------------------------------------------------------------*/
void enemy(){
	float speed = 5;
	float x = rand()%1024, y = 0, alfa = rand()%3;
	int a;
	// speed = 2 + rand()%20;
	while(1){
		// alfa usata per decidere se il razzo si sposta a destra sinistra o centrale
		if(alfa == 0)
			a = 1;
		if(alfa == 1)
			a = -1;
		if(alfa == 2)
			a = 0;
		// se il razzo non è arrivato alla citta scende, altrimenti scoppia
		if(x < 1024 && x > 0 && y < 600 - (aereo->h)){
			y = y + speed;
			x = x - (a * speed/5);
			draw_sprite(buf, aereo, x, y);
		}else{
			draw_sprite(buf, boom, x + 65 - (boom->w), y);
			i--;
			break;
		}
		ptask_wait_for_period();
	}
}
/*--------------------------------------------------------------*/
/*  MAIN process   */
/*--------------------------------------------------------------*/
int main(int argc, char const *argv[])
{	
	init();
	int scan;
	mti = 0;
	i = 0;
	tpars params;
	ptask_param_init(params);
	aereo = load_bitmap("img/aereo.bmp", NULL);
	boom = load_bitmap("img/boom.bmp", NULL);	
	buf = create_bitmap(SCREEN_W, SCREEN_H);
	ptask_create_prio(camera, PER, PRIO + 1, NOW);
	ptask_create_prio(background, PER, PRIO, NOW);
	mti = ptask_create_prio(draw, PER, PRIO-MAXT-1, DEFERRED);
	for(int k=0; k<MAXT; k++) tid[k]=0;
	// make_p_b("img/Pow.bmp", "img/aa.bmp");
	do{
		scan = 0;
		if(keypressed()) scan = readkey() >> 8;
		if(scan == KEY_SPACE && i < MAXT){
			ptask_param_period(params, PER, MILLI);
			ptask_param_priority(params, PRIO);
			ptask_param_activation(params, DEFERRED);
			tid[i] = ptask_create_param(enemy, &params);
			printf("Task: %d Priorità: %d\n",tid[i],PRIO);
			i++;
		}
	}while(!key[KEY_ESC]);
	allegro_exit();
	return 0;
}
/*--------------------------------------------------------------*/

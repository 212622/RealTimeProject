void init(void)
{
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_install_mouse();
    al_init_image_addon();
    srand(time(NULL));
    ptask_init(SCHED_FIFO, GLOBAL, PRIO_INHERITANCE);
    display = al_create_display(XWIN, YWIN);
    al_clear_to_color(BLACK);
    al_flip_display();
}

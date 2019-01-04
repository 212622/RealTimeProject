void init(void)
{
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, XWIN, YWIN, 0, 0);
    srand(time(NULL));
    ptask_init(SCHED_FIFO, GLOBAL, PRIO_INHERITANCE);
}

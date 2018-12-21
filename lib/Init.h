void init(void)
{
char s[LEN];
char t[LEN];

    allegro_init();
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, XWIN, YWIN, 0, 0);
    clear_to_color(screen, BKG);
    install_keyboard();
    install_mouse();
    ptask_init(SCHED_FIFO, GLOBAL, PRIO_INHERITANCE);

    sprintf(s, "Press ESC for quit");
    sprintf(t, "Press ENTER for start");
    textout_ex(screen, font, s, 10, 10, 14, BKG);
    textout_ex(screen, font, t, 10, 20, 14, BKG);
}

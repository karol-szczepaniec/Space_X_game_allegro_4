#include "space_x.h"

volatile long timer;
void timer_counter()
{
    timer++;
}
END_OF_FUNCTION(timer_counter);

void init();

volatile bool close_w = FALSE;
void close_the_window()
{
    close_w = TRUE;
}
END_OF_FUNCTION(close_the_window);


enum TYPE_SCR
{
    WELCOMSCREEN,
    MENUSCREEN,
    GAMESCREEN,
    QUITSCREEN,
    ABOUT
};

    LOCK_VARIABLE(timer);
    LOCK_FUNCTION(timer_counter);

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main()
{
    init();

    install_int_ex(timer_counter, MSEC_TO_TIMER(1));

    /// BUFOR BMP:
    BITMAP *bufor = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP *pause = create_bitmap(SCREEN_W,SCREEN_H);

    // WCZYTUJE BITMAPE
    BITMAP *enemy = load_bitmap("enemy.bmp", NULL);
    BITMAP *space_ship = load_bitmap("space_ship.bmp", NULL);
    if(!space_ship && !enemy && !bufor && !pause)
    {
        set_gfx_mode(GFX_TEXT,0,0,0,0);
        allegro_message("nie mozna otworzyc bmp!");
        allegro_exit();
        return 0;
    }

    TYPE_SCR type_screen = GAMESCREEN;

    show_mouse(screen);
    timer=NULL;

    // PASUE
    PALLETE my_palette;
    COLOR_MAP global_trans_table;
    create_trans_table(&global_trans_table, my_palette,128, 128, 128, NULL);
    set_trans_blender(128, 128, 128, 125);

    // VARIABLES:
    bool paused = FALSE;
    int space_ship_x = SCREEN_W/2;
    int space_ship_y = SCREEN_H - 100;
    Shoot_list *lista = Create_shoots_list();
    ENEMY *en = Create_Enemy(enemy,SCREEN_H/2,50);
    int prev_key;
    int timer_FPS = timer -10;

    // FONT:
    FONT *Aharoni = load_font("Aharoni.pcx",NULL,NULL);

    while(!close_w)
    {
        prev_key = key[KEY_ESC];
        poll_keyboard();
        if(key[KEY_ESC] && !prev_key)    {close_the_window();}

        switch (type_screen)
        {
            case WELCOMSCREEN:
            {
                clear_keybuf();
                textout_centre_ex(bufor, Aharoni, "WELCOME in Space-X game",SCREEN_W/2, (SCREEN_H/2)-100,makecol(20,0,255),-1);
                textout_centre_ex(bufor, font, "press ENTER KEY to continue",SCREEN_W/2, (SCREEN_H/2)+200,makecol(255,255,255),-1);
                draw_sprite(screen,bufor,0,0);
                blit(bufor,screen,0,0,0,0, bufor->w, bufor->h);
                readkey();
                if(key[KEY_ENTER])  type_screen = MENUSCREEN;
            } break;

            case MENUSCREEN:
                {
                    clear_keybuf();
                    clear_to_color(bufor,makecol(25,25,112));
                    textout_centre_ex(bufor, Aharoni, "MENU",SCREEN_W/2,50,makecol(112,138,144),-1);

                    rectfill(bufor, 200, 200, 600, 250, makecol(112,138,144));
                    rectfill(bufor, 200, 300, 600, 350, makecol(112,138,144));
                    rectfill(bufor, 200, 400, 600, 450, makecol(112,138,144));

                    // NEW GAME
                    if((mouse_x > 200 && mouse_x < 600) && (mouse_y > 200 && mouse_y < 250 ))
                    {
                        rectfill(bufor, 200, 200, 600, 250, makecol(135,206,250));

                        if(mouse_b == 1) type_screen = GAMESCREEN;
                    }
                    // ABOUT:
                    else if((mouse_x > 200 && mouse_x < 600) && (mouse_y > 300 && mouse_y < 350 ))
                    {
                        rectfill(bufor, 200, 300, 600, 350, makecol(135,206,250));

                        if(mouse_b == 1) type_screen = ABOUT;
                    }
                    // EXIT
                    else if((mouse_x > 200 && mouse_x < 600) && (mouse_y > 400 && mouse_y < 450 ))
                    {
                        rectfill(bufor, 200, 400, 600, 450, makecol(135,206,250));

                        if(mouse_b == 1) type_screen = QUITSCREEN;
                    }


                    textout_centre_ex(bufor, font, "NEW GAME",SCREEN_W/2,225,makecol(25,25,112),-1);
                    textout_centre_ex(bufor, font, "ABOUT",SCREEN_W/2,325,makecol(25,25,112),-1);
                    textout_centre_ex(bufor, font, "EXIT",SCREEN_W/2,425,makecol(25,25,112),-1);
                    blit(bufor,screen,0,0,0,0, bufor->w, bufor->h);
                } break;

            case GAMESCREEN:
                {
                    clear_keybuf();
                    while(!key[KEY_ESC] && !close_w)
                    {
                    if(timer - timer_FPS > 10)
                    {
                    clear_to_color(bufor,makecol(25,25,112));
                    masked_blit(space_ship,bufor,0,0,space_ship_x,space_ship_y, space_ship->w, space_ship->h);
                    prev_key = key[KEY_SPACE];
                    poll_keyboard();

                    if(key[KEY_ESC]) type_screen = MENUSCREEN;

                    if(key[KEY_UP] && space_ship_y > 450)                                               space_ship_y-=7;
                    else if(key[KEY_DOWN] && space_ship_y < SCREEN_H - space_ship -> h)                 space_ship_y+=7;
                    else if(key[KEY_RIGHT] && space_ship_x < SCREEN_W - space_ship ->w)                 space_ship_x+=7;
                    else if(key[KEY_LEFT] && space_ship_x > 0)                                          space_ship_x-=7;

                        if(key[KEY_SPACE] && !prev_key)
                        {
                            Insert_shoots(lista,(space_ship_x + space_ship->w - 22),space_ship_y);

                        }
                    Update_shoots_list(lista);
                    Draw_shoot_list(lista,bufor);
                    Draw_Enemy(bufor,en);
                    Update_Enemy(en);

                    rectfill(bufor, 0, 0, SCREEN_W, 20, makecol(0,0,0));
                    textprintf_ex(bufor,font, 10, 5, makecol(255,255,255),-1, "TIMER: %d i timer FPS: %d",timer, time(NULL));
                    textprintf_right_ex(bufor,font, SCREEN_W- 10, 5, makecol(255,255,255),-1, "PRESS ESC to END and back to MENU");

                    blit(bufor,screen,0,0,0,0, bufor->w, bufor->h);
                    vsync();
                    //timer--;
                    timer_FPS = timer; // counter reset to 0.
                    }
                    }

                } break;

            case QUITSCREEN:
                {
                    clear_keybuf();
                    clear_to_color(bufor,makecol(25,25,112));
                    rectfill(bufor, 100, 300, 375, 350, makecol(112,138,144));
                    rectfill(bufor, 400, 300, 700, 350, makecol(112,138,144));
                    textout_centre_ex(bufor, font, "Do you wanna quit the game?",SCREEN_W/2,200,makecol(112,138,144),-1);

                    if((mouse_x > 100 && mouse_x < 375) && (mouse_y > 300 && mouse_y < 350 ))
                    {
                        rectfill(bufor, 100, 300, 375, 350, makecol(135,206,250));

                        if(mouse_b == 1)    close_the_window();
                    }

                    else if((mouse_x > 400 && mouse_x < 700) && (mouse_y > 300 && mouse_y < 350 ))
                    {
                        rectfill(bufor, 400, 300, 700, 350, makecol(135,206,250));

                        if(mouse_b == 1)    {type_screen = MENUSCREEN; mouse_b = 0;}
                    }

                    textout_centre_ex(bufor, font, "Y / YES",238,325,makecol(25,25,112),-1);
                    textout_centre_ex(bufor, font, "N / NO",538,325,makecol(25,25,112),-1);
                    blit(bufor,screen,0,0,0,0, bufor->w, bufor->h);

                    if(key[KEY_Y]) close_the_window();
                    else if(key[KEY_N]) type_screen = MENUSCREEN;

                } break;

            case ABOUT:
                {
                    clear_keybuf();
                    clear_to_color(bufor,makecol(25,25,112));
                    textout_centre_ex(bufor, font, "Game created by: Karol Szczepaniec",SCREEN_W/2,SCREEN_H/2,makecol(112,138,144),-1);
                    textout_centre_ex(bufor, font, "press ENTER KEY to MENU",SCREEN_W/2, (SCREEN_H/2)+200,makecol(255,255,255),-1);
                    blit(bufor,screen,0,0,0,0, bufor->w, bufor->h);
                    if(key[KEY_ENTER]) type_screen = MENUSCREEN;

                }break;

            default: ;

        }

    // ZRZUT EKRANU:
    //save_bitmap("zrzut.bmp",screen,default_palette);
    }

    // SPRZATAM:
    destroy_bitmap(bufor);
    destroy_bitmap(pause);
    destroy_bitmap(space_ship);
    Destroy_shhot_list(lista);
    Destroy_Enemy(en);

 // ZAMYKAM
 allegro_exit();
 return 0;
}
END_OF_MAIN();

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void init()
{
    // INICJALIZACJA EKRANU:
    allegro_init();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0);
    clear_to_color(screen,makecol(128,128,128));

    set_close_button_callback(close_the_window);
    set_window_title("Space X- game");

    LOCK_VARIABLE(close_w);
    LOCK_FUNCTION(close_the_window);

    // INSTALACJA SPRZETOW
    install_keyboard();
    install_timer();
    install_mouse();

}
END_OF_FUNCTION(init);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


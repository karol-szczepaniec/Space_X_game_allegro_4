#ifndef SPACE_X_H_INCLUDED
#define SPACE_X_H_INCLUDED

#include <allegro.h>

////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct SHOOTS
{
    int x;
    int y;
    int speed;
    int active;

}Shoots;

Shoots *Create_shoots(int x, int y);
void Update_shoots(Shoots *s);
void Draw_shoots(Shoots *s, BITMAP *bufor);
void Destroy_shoots (Shoots *s);
/////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct NO_SHOOTS
{
    Shoots *s;
    struct NO_SHOOTS *next;
}No_shoots;
///////////////////////////////////////////////////////////////////////////////////////////////

typedef struct SHOOT_LIST
{
    No_shoots *inite;
}Shoot_list;

Shoot_list *Create_shoots_list();
void Insert_shoots(Shoot_list *l, int x, int y);
void Update_shoots_list(Shoot_list *l);
void Draw_shoot_list(Shoot_list *l, BITMAP *bufor);
void Destroy_shhot_list(Shoot_list *l);
//////////////////////////////////////////////////////////////////////////////////////////

///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct ENEMY
{
    int e_x;
    int e_y;
    BITMAP *e_bmp;

    bool hit;
}Enemy;

Enemy *Create_Enemy(BITMAP *_bmp, int x, int y);
void Update_Enemy(Enemy *e);
void Draw_Enemy(BITMAP *_buf, Enemy *e);
void Destroy_Enemy(Enemy *e);

#endif // SPACE_X_H_INCLUDED


















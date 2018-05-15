#include "space_x.h"

Shoots *Create_shoots(int x, int y)
{
    Shoots *s = (Shoots*) malloc(sizeof(Shoots));

    s->active = TRUE;
    s->x = x;
    s->y = y;
    s->speed = 10;
    return s;
}
////////////////////////////////////////////////////////

void Update_shoots(Shoots *s)
{
    s->y -= s->speed;

    if(s->y < -10)  s->active = FALSE;

}
///////////////////////////////////////////////////////

void Draw_shoots(Shoots *s, BITMAP *bufor)
{
    circlefill(bufor, s->x, s->y, 3, makecol(255,0,0));
}
/////////////////////////////////////////////////////

void Destroy_shoots (Shoots *s)
{
    free(s);
}
////////////////////////////////////////////////////

/// ================================================================================================


Shoot_list *Create_shoots_list()
{
    Shoot_list *l = (Shoot_list*) malloc(sizeof(Shoot_list));

    l->inite = NULL;

    return l;
}
/////////////////////////////////////////////////////////////////////

void Insert_shoots(Shoot_list *l, int x, int y)
{
    No_shoots *news = (No_shoots*) malloc(sizeof(No_shoots));
    news->s = Create_shoots(x,y);
    news->next = l->inite;
    l->inite = news;
}
/////////////////////////////////////////////////////////////////

void Update_shoots_list(Shoot_list *l)
{
    No_shoots *aux = l->inite;
    No_shoots *aux2 = l->inite;


    while(aux != NULL)
    {
        // if shoot still active
       if(aux->s->active)
       {
           Update_shoots(aux->s);
           aux2 = aux;
           aux = aux->next;
       }

       // else inactive remove from list
       else
       {
           // top of the list
           if(aux == aux2)
           {
               l->inite = l->inite ->next;
               Destroy_shoots(aux->s);
               free(aux);
               aux2 = aux = l->inite;
           }
           // Any position (aux2 will be the previous to aux)
           else
           {
               aux2 ->next = aux->next;
               Destroy_shoots(aux->s);
               free(aux);
               aux = aux2->next;
           }
       }
    }
}
/////////////////////////////////////////////////////////////////

void Draw_shoot_list(Shoot_list *l, BITMAP *bufor)
{
    No_shoots *aux = l->inite;

    while(aux != NULL)
    {
    Draw_shoots(aux->s, bufor);
    aux = aux->next;
    }
}

////////////////////////////////////////////////////////////////

void Destroy_shhot_list(Shoot_list *l)
{
    while(l->inite != NULL)
    {
        No_shoots *aux = l->inite;
        l->inite = l->inite->next;
        Destroy_shoots(aux->s);
        free(aux);
    }
}
///////////////////////////////////////////////////////////////

///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Enemy *Create_Enemy( BITMAP *_bmp, int x, int y)
{
    Enemy *e = (Enemy*) malloc(sizeof(Enemy));

    e->e_bmp = _bmp;
    e->e_x = x;
    e->e_y = y;
    e->hit = FALSE;

    return e;
}
////////////////////////////////////////////////////////////////

void Update_Enemy(Enemy *e)
{
    e->e_x-=5;
    if(e->e_x == -5)
    {
        e->e_x=SCREEN_W+5;
    }
}
///////////////////////////////////////////////////////////////

void Draw_Enemy(BITMAP *_buf, Enemy *e)
{
    masked_blit(e->e_bmp, _buf, 0,0,e->e_x, e->e_y, e->e_bmp->w, e->e_bmp->h);
}
/////////////////////////////////////////////////////////////

void Destroy_Enemy(Enemy *e)
{
    free(e);
}
//////////////////////////////////////////////////////////////



















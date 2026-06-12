#include <form.h>
#include <menu.h>
#include <panel.h>
#include "api.h"
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4


char *trim(char *str);

int form_login(shm_privada *shm_p, char * usuario, char * contra,  int *my_id);
void draw_password(FIELD *field, WINDOW *win);

int form_register(shm_privada * shm_p);
int form_update_user(shm_privada *shm_p, Usuario_t usuario);

int hp_menu();

int menu_my_habits(shm_privada *shm_p, Habito *habitos, int count, int * ids, int *selected_count, int my_id);

int menu_available_habits(Habito *habitos, int count, int * ids, int *selected_count);

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

int log_menu();
int admin_menu();
int menu_administrar_usuarios(Usuario_t *usuarios, int count, int *selected_id);
int menu_administrar_habitos(shm_privada *shm_p, Habito *habitos, int count, int *selected_id);
int menu_my_progress(RegistroVista *registros, int count);
int form_register_habit(shm_privada *shm_p, WINDOW *root);
int form_nota(shm_privada *shm_p, WINDOW *root, char *nota);
int form_show_user(Usuario_t usuario, int y, int x);
void actualizar_registros_habito(RegistroVista *registros,int count_registros,int habito_id,WINDOW *reg_win,MENU **menu,ITEM ***items);
void menu_my_registros(WINDOW *win,
                       RegistroVista *registros,
                       int count);
int build_registros_menu(RegistroVista *registros, int count,int alto_ventana, int ancho_ventana,WINDOW **out_win, MENU **out_menu);
void draw_registros_win(WINDOW *win, MENU *my_menu, RegistroVista *registros, int ancho_der, int win_h);
void free_registros_menu(WINDOW *win, MENU *menu);

bool esCorreoValido(const char *str);


#include <form.h>
#include <menu.h>
#include "api.h"
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4


char *trim(char *str);

int form_login(shm_privada *shm_p, char * usuario, char * contra);

int form_register(shm_privada * shm_p);

int hp_menu();

int menu_my_habits(Habito *habitos, int count, int * ids, int *selected_count);

int menu_available_habits(Habito *habitos, int count, int * ids, int *selected_count);

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

int log_menu();
int admin_menu();
int menu_administrar_usuarios(Usuario_t *usuarios, int count, int *selected_id);
int menu_administrar_habitos(Habito *habitos, int count, int *selected_id);

int form_register_habit(shm_privada *shm_p);

bool esCorreoValido(const char *str);


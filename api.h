#include "cJSON.h"

#include "modelo.h"
#include <semaphore.h>
#include <sys/stat.h>
#include <pthread.h>
#include <fcntl.h>
#include<stdlib.h>
#include <sys/mman.h>

#include<sys/types.h>
#include<string.h>


int api_login(shm_privada *shm_p, char * username, char * contra,  char *msg, int *my_id);
int api_register(shm_privada *shm_p, Usuario_t usuario, char *msg, int rol);
int api_get_usuarios(shm_privada *shm_p, Usuario_t *usuarios, int *count);
int api_get_all_habits(shm_privada *shm_p, Habito * habitos, int *count);
int api_get_usuario(shm_privada *shm_p, int id, Usuario_t *usuario);

int api_register_usuariohabitos(shm_privada *shm_p, int *ids, int selected_count);
int api_get_user_habits(shm_privada *shm_p,Habito *habitos,int *count);
int api_register_habit(shm_privada * shm_p, char * nombre);

int api_update_user(shm_privada *shm_p, Usuario_t usuario, char *msg);

/*


int api_get_habits(shm_privada *shm_p, Habito * habitos, int *count);

*/ 
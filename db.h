#include "cJSON.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "modelo.h"
#include <time.h>

#ifndef DB_H
#define DB_H

//estructuras de bases de datos con el mutex

typedef struct{
    Usuario_t usuarios[50];

    int count;

    pthread_mutex_t mutex;
}db_usuarios_t;

typedef struct{

    Habito habitos[50];

    int count;

    pthread_mutex_t mutex;

}db_habit_t;

typedef struct{
    UsuarioHabito usuariohabitos[100];
    int count;

    pthread_mutex_t mutex;
    
}db_usuariohabito_t;

typedef struct{
    Registro_t registros[100];
    int count;

    pthread_mutex_t mutex;
}db_registro_t;



int db_usuarios_init(const char *filename);
int db_usuarios_get_all(Usuario_t *usuarios, int *count);
int db_usuarios_get_usuario_by_username( Usuario_t *usuario, const char*username);
int db_usuarios_get_usuario_by_id(int id, Usuario_t *usuario);

int db_habitos_init(const char* filename);
int db_habitos_insert(Habito habito);
int db_habits_get(Habito * habitos, int * count);

int db_init(char*filename, char * db_name);
int db_user_register(Usuario_t usuario);

int db_usuariohabito_init(const char *filename);
int db_usuariohabito_insert(UsuarioHabito usuariohabito);
int db_usuariohabito_get_by_id(int id, UsuarioHabito * usuariohabito);


static int db_user_exist(const char *usuario);
int db_usuarios_existe_usuario(const char *username);
int db_update_user(Usuario_t usuario);

int db_habitos_get_by_usuario_id(int usuario_id,Habito *habitos,int *count);

cJSON * db_get_auth_info();

int db_registros_init(const char * filename);
int db_registros_get_all(Registro_t * registros, int *count);
//int db_registros_insert(Registro_t registro);
int db_registro_insert(int habito_id, int usuario_id);
int db_get_registros_by_usuario_id(int usuario_id, Registro_t *registros, int *count);
UsuarioHabito get_usuario_habito(int usuario_id, int habito_id);
int db_get_registros_usuario(int usuario_id, RegistroVista *salida, int *count);


//Manejo de archivos
char * get_data(const char * filename);
int file_db_save(const char * filename, const char * data);

#endif
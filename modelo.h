#ifndef MODELO_H
#define MODELO_H

#include <string.h>

#include "cJSON.h"

#include <semaphore.h>

#define ACTION_LOGIN 1
#define ACTION_REGISTER 2
#define ACTION_GET_HABITS 3
#define ACTION_ADD_USER_HABITS 4
#define ACTION_GET_USER_HABITS 5
#define ACTION_GET_USUARIOS 6
#define ACTION_REGISTER_HABIT 7



typedef struct{
    char id;
    char nombre[50];
}Habito;

Habito json_to_habito(cJSON * json);
cJSON * habito_to_json(Habito habito);


typedef struct{
    int id;
    int habito_id;
    int usuario_id;
    int activo;
}UsuarioHabito;

UsuarioHabito json_to_usuariohabito(cJSON * json);
cJSON * usuariohabito_to_json(UsuarioHabito usuariohabito);

typedef struct{
    int id;
    char username[25];
    char contra[25];
    char nombre[25];
    char apellido[25];
    char correo[50];
    int peso;
    int activo;
    int rol;
}Usuario_t;

Usuario_t usuario_from_json(cJSON * usuario_json);
cJSON * usuario_to_json(Usuario_t usuario);


typedef struct{
    int estatus;
    char msg[100];
    char data[409600];
}Respuesta_t;

Respuesta_t crear_respuesta(int estatus, const char *msg, char *data);

void respuesta_to_json(Respuesta_t respuesta, cJSON * respuesta_json);

typedef struct{
    int action;
    char data[800];

}Solicitud_t;

Solicitud_t crear_solicitud(int action,const char *data);


typedef struct{
    sem_t solicitud_lista;
    sem_t respuesta_lista;
    Solicitud_t solicitud;
    Respuesta_t respuesta;
} shm_privada;


typedef struct{

    int usuario_id;
    int usuario_rol;
    int autenticado;

    shm_privada *shm;

} cliente_contexto;


typedef enum {
    SCREEN_LOGIN_MENU,
    SCREEN_LOGIN,
    SCREEN_REGISTER,
    SCREEN_HOME,
    SCREEN_HABITS,
    SCREEN_EXIT,
    SCREEN_MY_HABITS,
    SCREEN_MY_PROGRESS,
    SCREEN_SETTINGS,
    SCREEN_ADMIN_MENU,
    SCREEN_ADMIN_USUARIOS,
    SCREEN_ADMIN_HABITOS
} Screen;
#endif
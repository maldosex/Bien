#include "api.h"



int api_login(shm_privada *shm_p, char * username, char * contra,  char *msg, int *my_id){

    cJSON *req_json = cJSON_CreateObject();

    cJSON_AddStringToObject(req_json, "username", username);
    cJSON_AddStringToObject(req_json, "contra",   contra);

    char *req_str =cJSON_PrintUnformatted(req_json);

    Solicitud_t solicitud = crear_solicitud(ACTION_LOGIN,req_str);

    shm_p->solicitud = solicitud;

    sem_post(&shm_p->solicitud_lista);

    sem_wait(&shm_p->respuesta_lista);

    strcpy(msg, shm_p->respuesta.msg);

    //Estatus 10 indica admin
    int estatus = shm_p->respuesta.estatus;
    if ((estatus == 0 || estatus == 10) &&
    strlen(shm_p->respuesta.data) > 0)
{
    cJSON *json = cJSON_Parse(shm_p->respuesta.data);

    if (json != NULL) {
        cJSON *id = cJSON_GetObjectItem(json, "id");

        if (cJSON_IsNumber(id))
            *my_id = id->valueint;

        cJSON_Delete(json);
    }
}

    free(req_str);

    cJSON_Delete(req_json);

    return estatus;
}


int api_register(shm_privada *shm_p, Usuario_t usuario, char *msg, int rol){
    
    usuario.rol = rol;
    char * req_str = cJSON_PrintUnformatted(usuario_to_json(usuario));

    Solicitud_t solicitud = crear_solicitud(ACTION_REGISTER, req_str);

    shm_p->solicitud = solicitud;

    sem_post(&shm_p->solicitud_lista);
    sem_wait(&shm_p->respuesta_lista);

    //Copiar el mensaje del servidor para imprimir en el formulario
    strcpy(msg, shm_p->respuesta.msg);

    int estatus = shm_p->respuesta.estatus;

    free(req_str);
    return estatus;
}

int api_get_usuarios(shm_privada *shm_p, Usuario_t *usuarios, int *count){

    Solicitud_t solicitud = crear_solicitud(ACTION_GET_USUARIOS, NULL);

    shm_p->solicitud = solicitud;

    sem_post(&shm_p->solicitud_lista);
    sem_wait(&shm_p->respuesta_lista);

    if(shm_p->respuesta.estatus != 0){
        *count = 0;
        return shm_p->respuesta.estatus;
    }

    cJSON *json_usuarios = cJSON_Parse(shm_p->respuesta.data);

    if(json_usuarios == NULL || !cJSON_IsArray(json_usuarios)){
        *count = 0;
        return -1;
    }

    int i = 0;
    cJSON *usuario_json = NULL;

    cJSON_ArrayForEach(usuario_json, json_usuarios){
        usuarios[i] = usuario_from_json(usuario_json);
        i++;
    }

    *count = i;
    cJSON_Delete(json_usuarios);
    return 0;
}

//paso el id devuelve el usuario
int api_get_usuario(shm_privada *shm_p, int id, Usuario_t *usuario){

    cJSON *req_json = cJSON_CreateObject();

    cJSON_AddNumberToObject(req_json, "id", id);

    char *req_str = cJSON_PrintUnformatted(req_json);

    Solicitud_t solicitud = crear_solicitud(ACTION_GET_USER, req_str);

    shm_p->solicitud = solicitud;

    sem_post(&shm_p->solicitud_lista);

    sem_wait(&shm_p->respuesta_lista);

    //Ya respondio el server

    int status = shm_p->respuesta.estatus;

    cJSON *res_json = cJSON_Parse(shm_p->respuesta.data);

    if(status == 0 && res_json != NULL){
        *usuario = usuario_from_json(res_json);
    }

    if(res_json != NULL){
        cJSON_Delete(res_json);
    }
    free(req_str);
    cJSON_Delete(req_json);
    
    return status;
}

int api_update_user(shm_privada *shm_p, Usuario_t usuario, char *msg)
{
    char *req_str = cJSON_PrintUnformatted(usuario_to_json(usuario));

    Solicitud_t solicitud =
        crear_solicitud(ACTION_UPDATE_USER, req_str);

    shm_p->solicitud = solicitud;

    sem_post(&shm_p->solicitud_lista);
    sem_wait(&shm_p->respuesta_lista);

    // Copiar mensaje devuelto por el servidor
    strcpy(msg, shm_p->respuesta.msg);

    int status = shm_p->respuesta.estatus;

    free(req_str);

    return status;
}

int api_get_all_habits(shm_privada *shm_p, Habito * habitos, int *count){


    Solicitud_t solicitud = crear_solicitud(ACTION_GET_HABITS, NULL);

    shm_p->solicitud = solicitud;

    sem_post(&shm_p->solicitud_lista);
    sem_wait(&shm_p->respuesta_lista);


    
    // Validar respuesta servidor
    if(shm_p->respuesta.estatus != 0){

        *count = 0;

        return shm_p->respuesta.estatus;
    }
    cJSON *json_habitos = cJSON_Parse(shm_p->respuesta.data);

    if(json_habitos == NULL){

        *count = 0;

        return -1;
    }

        // Validar que sea arreglo
    if(!cJSON_IsArray(json_habitos)){

        cJSON_Delete(json_habitos);

        *count = 0;

        return -1;
    }

    int i = 0;

    cJSON *habito_json = NULL;

    cJSON_ArrayForEach(habito_json, json_habitos){

        habitos[i] = json_to_habito(habito_json);

        i++;
    }

    *count = i;
    cJSON_Delete(json_habitos);
    return 0;
}


int api_register_usuariohabitos(shm_privada *shm_p, int *ids, int selected_count){

    cJSON *json_ids = cJSON_CreateArray();

    for(int i = 0; i < selected_count; i++){

        cJSON_AddItemToArray(json_ids,cJSON_CreateNumber(ids[i]));
    }

    char *data = cJSON_PrintUnformatted(json_ids);

    Solicitud_t solicitud = crear_solicitud(ACTION_ADD_USER_HABITS, data);

    shm_p->solicitud = solicitud;

    sem_post(&shm_p->solicitud_lista);

    sem_wait(&shm_p->respuesta_lista);

    int status = shm_p->respuesta.estatus;

    free(data);

    cJSON_Delete(json_ids);

    return status;
}

int api_get_user_habits(shm_privada *shm_p,Habito *habitos,int *count){

    Solicitud_t solicitud = crear_solicitud(ACTION_GET_USER_HABITS,NULL);

    shm_p->solicitud = solicitud;

    sem_post(&shm_p->solicitud_lista);

    sem_wait(&shm_p->respuesta_lista);

    if(shm_p->respuesta.estatus != 0){

        *count = 0;

        return shm_p->respuesta.estatus;
    }

    cJSON *json_habitos =
        cJSON_Parse(shm_p->respuesta.data);

    if(json_habitos == NULL ||
       !cJSON_IsArray(json_habitos)){

        *count = 0;

        return -1;
    }

    int i = 0;

    cJSON *habito_json = NULL;

    cJSON_ArrayForEach(habito_json, json_habitos){

        habitos[i] = json_to_habito(habito_json);

        i++;
    }

    *count = i;

    cJSON_Delete(json_habitos);

    return 0;
}


int api_register_habit(shm_privada * shm_p, char * nombre){

    cJSON * habit_json = cJSON_CreateObject();    
    cJSON_AddStringToObject(habit_json,"nombre", nombre);

    char *json_str = cJSON_PrintUnformatted(habit_json);

    Solicitud_t solicitud = crear_solicitud(ACTION_REGISTER_HABIT, json_str);

    shm_p->solicitud = solicitud;

    sem_post(&shm_p->solicitud_lista);
    sem_wait(&shm_p->respuesta_lista);

    free(json_str);
    cJSON_Delete(habit_json);
    return shm_p->respuesta.estatus;
    
}

int api_insert_registro(shm_privada *shm_p, int usuario_id, int habito_id, char * nota, char *msg){
    cJSON * req_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(req_json, "usuario_id", usuario_id);
    cJSON_AddNumberToObject(req_json, "habito_id", habito_id);
    cJSON_AddStringToObject(req_json, "nota", nota);


    char *req_str = cJSON_PrintUnformatted(req_json);
    Solicitud_t solicitud = crear_solicitud(ACTION_INSERT_REGISTRO, req_str);

    shm_p->solicitud = solicitud;
    sem_post(&shm_p->solicitud_lista);
    sem_wait(&shm_p->respuesta_lista);

    strcpy(msg, shm_p->respuesta.msg);

    int estatus = shm_p->respuesta.estatus;

    free(req_str);
    return estatus;
}

int api_get_registros_usuario(
    shm_privada *shm_p,
    RegistroVista *registros,
    int *count
){
    Solicitud_t solicitud =
        crear_solicitud(ACTION_GET_REGISTROS_USUARIO, NULL);

    shm_p->solicitud = solicitud;

    sem_post(&shm_p->solicitud_lista);

    sem_wait(&shm_p->respuesta_lista);

    if(shm_p->respuesta.estatus != 0){

        *count = 0;

        return shm_p->respuesta.estatus;
    }

    cJSON *json_registros =
        cJSON_Parse(shm_p->respuesta.data);

    if(json_registros == NULL ||
       !cJSON_IsArray(json_registros)){

        *count = 0;

        if(json_registros != NULL)
            cJSON_Delete(json_registros);

        return -1;
    }

    int i = 0;

    cJSON *registro_json = NULL;

    cJSON_ArrayForEach(registro_json, json_registros){

        registros[i] =
            registrovista_from_json(registro_json);

        i++;
    }

    *count = i;

    cJSON_Delete(json_registros);

    return 0;
}

    /*



/*
int api_add_user_habits(shm_privada * shm_p, Habito * habitos, int count){
    cJSON * req = cJSON_CreateObject();

     if(req == NULL){
        return -1;
    }

    //Se agregan los campos a la peticion
    cJSON_AddStringToObject(req, "action", "add_user_habit");

    cJSON * habits_array = cJSON_CreateArray();

    if(habits_array == NULL){
        cJSON_Delete(req);
        return -1;
    }


     for(int i = 0; i < count; i++){

        cJSON *habit_obj = cJSON_CreateObject();

        if(habit_obj == NULL){
            cJSON_Delete(req);
            return -1;
        }

        cJSON_AddNumberToObject(habit_obj, "id", habitos[i].id);

        cJSON_AddItemToArray(habits_array, habit_obj);
    }
    
    cJSON_AddItemToObject(req, "habits", habits_array);

    //La peticioon se convierte en string
    char  * req_str = cJSON_PrintUnformatted(req);

    if (req_str == NULL){
        cJSON_Delete(req);
        return -1;
    }

    //Se hace la comunicacion con el servidor 
        //Se escribe la peticion en la memoria compartida
        snprintf(shm_p->solicitud, sizeof(shm_p->solicitud), "%s",req_str);
        //Se avisa al servidor que la solicitud esta lista despertandolo
        sem_post(&shm_p->solicitud_lista);
        //Se espera la respuesta del servidor
        sem_wait(&shm_p->respuesta_lista);

    //Se procesa la respuesta del sertvidor 
    cJSON * respuesta_json = cJSON_Parse(shm_p->respuesta);
    int estatus = (cJSON_GetObjectItem(respuesta_json, "estatus"))->valueint;
    free(req_str);
    return estatus;
}


*/

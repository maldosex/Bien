#include "api.h"



int api_login(shm_privada *shm_p, char * username, char * contra,  char *msg){

    cJSON *req_json = cJSON_CreateObject();

    cJSON_AddStringToObject(req_json, "username", username);
    cJSON_AddStringToObject(req_json, "contra",   contra);

    char *req_str =cJSON_PrintUnformatted(req_json);

    Solicitud_t solicitud = crear_solicitud(ACTION_LOGIN,req_str);

    shm_p->solicitud = solicitud;

    sem_post(&shm_p->solicitud_lista);

    sem_wait(&shm_p->respuesta_lista);

    strcpy(msg, shm_p->respuesta.msg);

    int estatus = shm_p->respuesta.estatus;

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
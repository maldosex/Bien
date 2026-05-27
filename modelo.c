#include "modelo.h"

Habito json_to_habito(cJSON * json){
    Habito habito;
    cJSON * id_json = cJSON_GetObjectItem(json, "id");
    cJSON * nombre_json = cJSON_GetObjectItem(json, "nombre");

    strcpy(habito.nombre, nombre_json->valuestring);
    habito.id = id_json->valueint;
    return  habito;
}

cJSON * habito_to_json(Habito habito){
    cJSON * habito_json = cJSON_CreateObject();

    cJSON_AddNumberToObject(habito_json, "id", habito.id);
    cJSON_AddStringToObject(habito_json, "nombre", habito.nombre);

    return habito_json;
}

UsuarioHabito json_to_usuariohabito(cJSON * json){
    UsuarioHabito usuariohabito;
    cJSON * id_json = cJSON_GetObjectItem(json, "id");
    cJSON * usuario_id_json = cJSON_GetObjectItem(json, "usuario_id");
    cJSON * habito_id_json = cJSON_GetObjectItem(json, "habito_id");
    cJSON * activo = cJSON_GetObjectItem(json, "activo");

    usuariohabito.id = id_json->valueint;
    usuariohabito.usuario_id = usuario_id_json->valueint;
    usuariohabito.habito_id = habito_id_json->valueint;
    usuariohabito.activo = activo->valueint;
    return usuariohabito;
    
}


cJSON * usuariohabito_to_json(UsuarioHabito usuariohabito){
    cJSON * usuariohabito_json = cJSON_CreateObject();

    cJSON_AddNumberToObject(usuariohabito_json, "id", usuariohabito.id);
    cJSON_AddNumberToObject(usuariohabito_json, "usuario_id", usuariohabito.usuario_id);
    cJSON_AddNumberToObject(usuariohabito_json, "habito_id", usuariohabito.habito_id);
    cJSON_AddNumberToObject(usuariohabito_json, "activo", usuariohabito.activo);
    return usuariohabito_json;
}

cJSON * usuario_to_json(Usuario_t usuario){

    cJSON * usuario_json = cJSON_CreateObject();

    cJSON_AddNumberToObject(usuario_json,"id",usuario.id);

    cJSON_AddStringToObject(usuario_json,"usuario",usuario.username);

    cJSON_AddStringToObject(usuario_json,"contra",usuario.contra);

    return usuario_json;
}

Usuario_t usuario_from_json(cJSON *usuario_json){

    Usuario_t usuario;

    memset(&usuario,0,sizeof(Usuario_t));

    cJSON *id_json =cJSON_GetObjectItem(usuario_json,"id");

    cJSON *username_json =cJSON_GetObjectItem(usuario_json,"usuario");

    cJSON *contra_json =cJSON_GetObjectItem(usuario_json,"contra");

    if(cJSON_IsNumber(id_json)){
        usuario.id =id_json->valueint;
    }

    if(cJSON_IsString(username_json)){

        strcpy(usuario.username,username_json->valuestring);
    }

    if(cJSON_IsString(contra_json)){

        strcpy(usuario.contra,contra_json->valuestring);
    }

    return usuario;
}

Solicitud_t crear_solicitud(int action,const char *data){

    Solicitud_t solicitud;

    solicitud.action = action;

    if(data != NULL){

        strncpy(solicitud.data,data,sizeof(solicitud.data) - 1);

        solicitud.data[sizeof(solicitud.data) - 1] = '\0';

    }else{

        solicitud.data[0] = '\0';
    }

    return solicitud;
}


Respuesta_t crear_respuesta(int estatus, const char *msg, char *data){
    Respuesta_t respuesta;

    respuesta.estatus = estatus;
    strcpy(respuesta.msg, msg);
    respuesta.msg[
        sizeof(respuesta.msg) - 1
    ] = '\0';

    if(data != NULL){

        strncpy(respuesta.data,data,sizeof(respuesta.data) - 1);

        respuesta.data[sizeof(respuesta.data) - 1] = '\0';

    }else{

        respuesta.data[0] = '\0';
    }

    return respuesta;
}

void respuesta_to_json(Respuesta_t respuesta, cJSON * respuesta_json){


    cJSON_AddNumberToObject(respuesta_json,"estatus",respuesta.estatus);

    cJSON_AddStringToObject(respuesta_json,"msg",respuesta.msg);

    if(respuesta.data != NULL){


        cJSON_AddStringToObject(respuesta_json,"data",respuesta.data);
    }
}
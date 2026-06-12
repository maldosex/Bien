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
    cJSON_AddStringToObject(usuario_json,"username",usuario.username);
    cJSON_AddStringToObject(usuario_json,"contra",usuario.contra);
    cJSON_AddStringToObject(usuario_json,"nombre",usuario.nombre);
    cJSON_AddStringToObject(usuario_json,"apellido",usuario.apellido);
    cJSON_AddStringToObject(usuario_json,"correo",usuario.correo);
    cJSON_AddNumberToObject(usuario_json,"peso",usuario.peso);
    cJSON_AddNumberToObject(usuario_json,"activo",usuario.activo);
    cJSON_AddNumberToObject(usuario_json,"rol",usuario.rol);
    
    return usuario_json;
}
Usuario_t usuario_from_json(cJSON *usuario_json){

    Usuario_t usuario;

    memset(&usuario,0,sizeof(Usuario_t));

    cJSON *id_json = cJSON_GetObjectItem(usuario_json,"id");
    cJSON *username_json =cJSON_GetObjectItem(usuario_json,"username");
    cJSON *contra_json = cJSON_GetObjectItem(usuario_json,"contra");
    cJSON *nombre_json = cJSON_GetObjectItem(usuario_json, "nombre");

    cJSON *apellido_json = cJSON_GetObjectItem(usuario_json,"apellido");
    cJSON *correo_json = cJSON_GetObjectItem(usuario_json,"correo");
    cJSON *peso_json = cJSON_GetObjectItem(usuario_json,"peso");
    cJSON *activo_json = cJSON_GetObjectItem(usuario_json,"activo");
    cJSON *rol_json = cJSON_GetObjectItem(usuario_json,"rol");


    if(cJSON_IsNumber(id_json)){
        usuario.id = id_json->valueint;
    }

    if(cJSON_IsString(username_json)){
        strcpy(usuario.username,username_json->valuestring);
    }

    if(cJSON_IsString(contra_json)){
        strcpy(usuario.contra,contra_json->valuestring);
    }

    if(cJSON_IsString(nombre_json)){
        strcpy(usuario.nombre, username_json->valuestring);
    }

    if(cJSON_IsString(apellido_json)){
        strcpy(usuario.apellido,apellido_json->valuestring);
    }

    if(cJSON_IsString(correo_json)){
        strcpy(usuario.correo ,correo_json->valuestring);
    }

    if(cJSON_IsNumber(peso_json)){
        usuario.peso = peso_json->valueint;
    }

    if(cJSON_IsNumber(activo_json)){
        usuario.activo = activo_json->valueint;
    }
    if(cJSON_IsNumber(rol_json)){
        usuario.rol = rol_json->valueint;
    }

    return usuario;
}


cJSON * registro_to_json(Registro_t registro){
    cJSON * registro_json  = cJSON_CreateObject();

    cJSON_AddNumberToObject(registro_json, "id", registro.id);
    cJSON_AddNumberToObject(registro_json, "usuariohabito_id", registro.usuariohabito_id);
    cJSON_AddStringToObject(registro_json, "fecha", registro.fecha);
    cJSON_AddStringToObject(registro_json, "nota", registro.nota);
    return registro_json;
}
Registro_t registro_from_json(cJSON * registro_json){

    Registro_t registro;

    memset(&registro,0,sizeof(Registro_t));

    cJSON * id_json = cJSON_GetObjectItem(registro_json, "id");
    cJSON * usuariohabito_id_json = cJSON_GetObjectItem(registro_json, "usuariohabito_id");
    cJSON * fecha_json = cJSON_GetObjectItem(registro_json, "fecha");
    cJSON * nota_json = cJSON_GetObjectItem(registro_json, "nota");

    if(cJSON_IsNumber(id_json)){
        registro.id = id_json->valueint;
    }

    if(cJSON_IsNumber(usuariohabito_id_json)){
        registro.usuariohabito_id = usuariohabito_id_json->valueint;
    }

    if(cJSON_IsString(fecha_json)){
        strcpy(registro.fecha, fecha_json->valuestring);
    }
    if(cJSON_IsString(nota_json)){
        strcpy(registro.nota, nota_json->valuestring);
    }

    return registro;
}

RegistroVista registrovista_from_json(cJSON * registrovista_json){
    RegistroVista registrovista;
    memset(&registrovista, 0, sizeof(RegistroVista));
    cJSON * id_json = cJSON_GetObjectItem(registrovista_json, "id");
    cJSON * habito_id_json = cJSON_GetObjectItem(registrovista_json, "habito_id");
    cJSON * nombre_habito_json = cJSON_GetObjectItem(registrovista_json, "nombre_habito");
    cJSON * fecha_json = cJSON_GetObjectItem(registrovista_json, "fecha");
    cJSON * nota_json = cJSON_GetObjectItem(registrovista_json, "nota");

    if(cJSON_IsNumber(id_json)){
        registrovista.id = id_json->valueint;
    }
    if(cJSON_IsNumber(habito_id_json)){
        registrovista.habito_id = habito_id_json->valueint;
    }
    if(cJSON_IsString(nombre_habito_json)){
        strcpy(registrovista.nombre_habito, nombre_habito_json->valuestring);
    }
    if(cJSON_IsString(fecha_json)){
        strcpy(registrovista.fecha, fecha_json->valuestring);
    }
    if(cJSON_IsString(nota_json)){
        strcpy(registrovista.nota, nota_json->valuestring);
    }
    return registrovista;
}
cJSON * registrovista_to_json(RegistroVista registrovista){
    cJSON * registrovista_json = cJSON_CreateObject();

    cJSON_AddNumberToObject(registrovista_json, "id", registrovista.id);

    cJSON_AddNumberToObject(registrovista_json, "habito_id", registrovista.habito_id);
    cJSON_AddStringToObject(registrovista_json, "nombre_habito", registrovista.nombre_habito);
    cJSON_AddStringToObject(registrovista_json, "fecha", registrovista.fecha);
    cJSON_AddStringToObject(registrovista_json, "nota", registrovista.nota);
    return registrovista_json;
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
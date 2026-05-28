#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include<sys/types.h>
#include<unistd.h>


#include "servidor.h"
#include "db.h"


#include "modelo.h"

#include "cJSON.h"
#include "stdio.h"


typedef struct
{
    pid_t pid;

}shm_general;

void cifrar(char *str) {
    if (str == NULL) return;

    while (*str != '\0') {
        // Obtenemos el valor actual y le sumamos 1
        // Nota: Si el caracter es el 255 (límite de ASCII extendido), 
        // volverá a 0 automáticamente al ser un char.
        *str = *str + 1; 
        
        str++; // Siguiente posición de memoria
    }
}

void * atender_cliente(void * shmem);

int main(){
    printf("Soy el servidor\n");

    //Crear Semaforo
    sem_t * mutex_general, *solicitud, *respuesta;


    //Borrar semaforos preexistentes
    sem_unlink("/mutex_general");
    sem_unlink("/solicitud");
    sem_unlink("/respuesta");


    //Abrir semaforos generales
    mutex_general = sem_open("/mutex_general", O_CREAT, 0666, 1);
    solicitud = sem_open("/solicitud", O_CREAT, 0666, 0);
    respuesta = sem_open("/respuesta", O_CREAT, 0666, 0);

    //Memoria compartida general escucha servidor
    int shm_fd = shm_open("/shm_general", O_CREAT|O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(shm_general));
    shm_general * shm_g = mmap(NULL, sizeof(shm_general), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);

    db_usuarios_init("datos.json");
    db_habitos_init("habitos.json");
    db_usuariohabito_init("usuariohabitos.json");

    Habito habitos[50];
    int habitos_count;
    db_habits_get(habitos, &habitos_count);
    printf("Habito 1: %s\n", habitos[0].nombre);

    printf("%s\n", get_data("datos.json"));



    //Iniciar escucha de peticiones
    while(1){
        printf("A esperar clientes\n");
        sem_wait(solicitud);
        printf("Llego cliente: %d\n", shm_g->pid);
        printf("Lo atiendo...\n");
        
        //Crear memoria privada

        char name_shmem[64];
        sprintf(name_shmem,"/shm_%d",shm_g->pid);


        int shmp_fd = shm_open(name_shmem, O_CREAT|O_RDWR, 0666);
        ftruncate(shmp_fd, sizeof(shm_privada));
        shm_privada * shm_p = mmap(NULL, sizeof(shm_privada), PROT_READ|PROT_WRITE, MAP_SHARED, shmp_fd, 0);

        sem_init(&shm_p->respuesta_lista, 1, 0);
        sem_init(&shm_p->solicitud_lista, 1, 0);

        //Crear hilo para el cliente
        pthread_t thread;
        pthread_create(&thread, NULL, atender_cliente, shm_p);
        sem_post(respuesta);
        
        
    }
    return 0;
}

void *atender_cliente(void *shmem){

    pthread_detach(pthread_self());

    printf("Hilo para cliente\n");

    cliente_contexto cliente_ctxt = {
        .usuario_id = -1,
        .autenticado = 0,
        .shm = (shm_privada *)shmem
    };


    
    while(1){

        sem_wait(&cliente_ctxt.shm->solicitud_lista);

        printf("Accion recibida: %d\n",cliente_ctxt.shm->solicitud.action);

        printf("Data recibida: %s\n", cliente_ctxt.shm->solicitud.data);

        route_request(&cliente_ctxt);

        printf("Respuesta status: %d\n", cliente_ctxt.shm->respuesta.estatus);

        printf("Respuesta msg: %s\n", cliente_ctxt.shm->respuesta.msg);

        printf("Respuesta data: %s\n", cliente_ctxt.shm->respuesta.data);

        sem_post(&cliente_ctxt.shm->respuesta_lista);
    }

    return NULL;
}




//HANDLERS_______________________________
int handle_log(cliente_contexto * cliente_ctx, char *username, char *contra){

    Usuario_t usuario_db;

    int existe = db_usuarios_get_usuario_by_username(&usuario_db, username);

    int contra_correcta = 0;

    if(existe){
        cifrar(contra);
        contra_correcta = (strcmp(usuario_db.contra, contra) == 0);
    }

    if(!existe || !contra_correcta){

        cliente_ctx->shm->respuesta = crear_respuesta(1, "usuario o contrasena incorrectos", NULL);

        return 1;
    }
    cliente_ctx->usuario_id = usuario_db.id;
    cliente_ctx->usuario_rol = usuario_db.rol;
    cliente_ctx->autenticado = 1;


    //Si el usuario es admin se devuel estatus 10
    if(usuario_db.rol == 0){
        cliente_ctx->shm->respuesta = crear_respuesta(10,"Login Admin",NULL);
        return 10;
    }
    //Si el usuario es admin se devuelve estatus 00
    cliente_ctx->shm->respuesta = crear_respuesta(0,"login correcto",NULL);
    return 0;
}
int handle_reg(Usuario_t usuario_a_registrar, Respuesta_t *respuesta){
    //Se intenta el registro con la base de datos
    cifrar(usuario_a_registrar.contra);
    int registro_estatus = db_user_register(usuario_a_registrar);
    char msg[50];

    //Estatus exitoso
    if(registro_estatus == 0){
        strcpy(msg, "Registro exitoso, haga login");
    }

    //El registro no fue posible, ya existe el usuario
    else if (registro_estatus == 1){
        strcpy(msg, "Registro incorrecto, el usuario ya existe");
    }


    *respuesta = crear_respuesta(registro_estatus, msg, NULL);
    return registro_estatus;

}
int handle_get_users(cliente_contexto *cliente_ctx){
    Usuario_t usuarios[50];
    int count = 0;

    db_usuarios_get_all(usuarios, &count);

    cJSON *arreglo = cJSON_CreateArray();
    for(int i = 0; i < count; i++){
        cJSON_AddItemToArray(arreglo, usuario_to_json(usuarios[i]));
    }

    char *data = cJSON_PrintUnformatted(arreglo);
    cliente_ctx->shm->respuesta = crear_respuesta(0, "usuarios obtenidos", data);
    free(data);
    cJSON_Delete(arreglo);
    return 0;
}
int handle_get_habits(cliente_contexto * cliente_ctx){

    Habito habitos[50];
    int count;

    db_habits_get(habitos, &count);
    cJSON * arreglo = cJSON_CreateArray();

    cliente_ctx->shm->respuesta.estatus = 0;

    for (int i = 0; i < count; i++)
    {
        cJSON_AddItemToArray(arreglo, habito_to_json(habitos[i]));

    }

    char *data = cJSON_PrintUnformatted(arreglo);

    cliente_ctx->shm->respuesta = crear_respuesta(0, "Datos obtenidos", data);
    free(data);
    cJSON_Delete(arreglo);
    return 0;
}

int handle_registrar_habito(Habito habito, Respuesta_t *respuesta);

int handle_add_user_habits(cliente_contexto *cliente_ctx,int *ids,int count){
    for(int i = 0; i < count; i++){

        UsuarioHabito uh;

        uh.usuario_id =
            cliente_ctx->usuario_id;

        uh.habito_id = ids[i];

        uh.activo = 1;

        db_usuariohabito_insert(uh);
    }

    cliente_ctx->shm->respuesta =
        crear_respuesta(
            0,
            "habitos agregados",
            NULL
        );

    return 0;
}

int handle_get_user_habits(cliente_contexto *cliente_ctx){

    if(!cliente_ctx->autenticado){

        cliente_ctx->shm->respuesta = crear_respuesta(1,"usuario no autenticado",NULL);

        return 1;
    }

    Habito habitos[50];

    int count = 0;

    db_habitos_get_by_usuario_id(cliente_ctx->usuario_id,habitos,&count);

    cJSON *arreglo = cJSON_CreateArray();

    for(int i = 0; i < count; i++){

        cJSON_AddItemToArray(arreglo,habito_to_json(habitos[i]));
    }

    char *data = cJSON_PrintUnformatted(arreglo);

    cliente_ctx->shm->respuesta = crear_respuesta(0,"habitos obtenidos",data);

    free(data);

    cJSON_Delete(arreglo);

    return 0;
}

//___________________________________Router________________________________________________

int route_request(cliente_contexto *cliente_ctx){

    printf("Accion recibida: %d\n", cliente_ctx->shm->solicitud.action);
    //Se identifica la accion que solicita el cliente
    switch(cliente_ctx->shm->solicitud.action){

        case ACTION_LOGIN:{

            cJSON * data_json = cJSON_Parse(cliente_ctx->shm->solicitud.data);

            //Se valida el contenido de la solicitud
            if(data_json == NULL){
                cliente_ctx->shm->respuesta = crear_respuesta(-1,"json invalido",NULL);
                return -1;
            }
            cJSON * data_username_json = cJSON_GetObjectItem(data_json, "username");
            cJSON * data_contra_json = cJSON_GetObjectItem(data_json, "contra");
            char username[25], contra[25];
            strncpy(username, data_username_json->valuestring, 24);
            strncpy(contra,   data_contra_json->valuestring,   24);
            username[24] = '\0';
            contra[24]   = '\0';

            cJSON_Delete(data_json);

            return handle_log(cliente_ctx, username, contra);
        }

        case ACTION_REGISTER:{
            printf("El cliente solicita Registrar usuario\n");
            cJSON * usuario_solicitud_json = cJSON_Parse(cliente_ctx->shm->solicitud.data);

            //Se valida el contenido de la solicitud
            if(usuario_solicitud_json == NULL){

                cliente_ctx->shm->respuesta = crear_respuesta(-1,"json invalido",NULL);
                return -1;
            }
            Usuario_t usuario_a_registrar = usuario_from_json(usuario_solicitud_json);

            cJSON_Delete(usuario_solicitud_json);

            return handle_reg(usuario_a_registrar, &cliente_ctx->shm->respuesta);
        }

        case ACTION_GET_HABITS:{
            printf("El cliente solicita obtener la lista de habitos\n");

            handle_get_habits(cliente_ctx);
            break;
        }
        case ACTION_ADD_USER_HABITS: {

            cJSON *ids_json = cJSON_Parse(cliente_ctx->shm->solicitud.data);
            
            if(ids_json == NULL){
            
                cliente_ctx->shm->respuesta =crear_respuesta(-1,"json invalido",NULL);
                return -1;
            }
        
            if(!cJSON_IsArray(ids_json)){
            
                cJSON_Delete(ids_json);
                cliente_ctx->shm->respuesta = crear_respuesta(-1,"se esperaba arreglo",NULL); 
                return -1;
            }
        
            int ids[50];
        
            int count = 0;
        
            cJSON *id_json = NULL;
        
            cJSON_ArrayForEach(id_json, ids_json){
            
                if(cJSON_IsNumber(id_json)){
                
                    ids[count] = id_json->valueint;
                
                    count++;
                }
            }
        
            cJSON_Delete(ids_json);
        
            return handle_add_user_habits(cliente_ctx, ids, count);
        }

        case ACTION_GET_USER_HABITS:{
            return handle_get_user_habits(cliente_ctx);
        }
        case ACTION_GET_USUARIOS:{
            return handle_get_users(cliente_ctx);
        }


/*
            return handle_register(solicitud,respuesta);

        case ACTION_GET_HABITS:

            printf("Recibo get habits\n");

            return get_habits(solicitud,respuesta);
            */

        default:

            cliente_ctx->shm->respuesta = crear_respuesta(-1,"accion invalida",NULL);
            return -1;
    }
}
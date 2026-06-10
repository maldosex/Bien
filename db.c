#include "db.h"



static pthread_mutex_t db_mutex = PTHREAD_MUTEX_INITIALIZER;


//Base catalogo de habitos::
static db_usuarios_t db_usuarios;

static db_habit_t db_habitos;

static db_usuariohabito_t db_usuariohabitos;


static db_registro_t db_registros;

int db_usuarios_init(const char* filename){
    cJSON *usuarios_json = cJSON_Parse(get_data(filename));

    pthread_mutex_init(&db_usuarios.mutex, NULL);

    db_usuarios.count = 0;

    cJSON *usuario_json = NULL;

    cJSON_ArrayForEach(usuario_json, usuarios_json){
        int index = db_usuarios.count;

        db_usuarios.usuarios[index] = usuario_from_json(usuario_json);

        db_usuarios.count++;
    }

    cJSON_Delete(usuarios_json);
    return 0;
}

int db_usuarios_get_all(Usuario_t *usuarios, int *count){
    pthread_mutex_lock(&db_usuarios.mutex);

    int n = db_usuarios.count;
    memcpy(usuarios, db_usuarios.usuarios, sizeof(Usuario_t) * n);
    *count = n;

    pthread_mutex_unlock(&db_usuarios.mutex);
    return 0;
}
int db_usuarios_get_usuario_by_username( Usuario_t *usuario, const char*username){
    int existe = 0;
    pthread_mutex_lock(&db_usuarios.mutex);
        for(int i = 0; i<db_usuarios.count; i++){
            if(strcmp(db_usuarios.usuarios[i].username, username)==0){
                *usuario = db_usuarios.usuarios[i];
                existe = 1;
                break;
            }
        }
    pthread_mutex_unlock(&db_usuarios.mutex);
    return existe;
}
int db_usuarios_get_usuario_by_id(int id, Usuario_t *usuario){
    pthread_mutex_lock(&db_usuarios.mutex);

    for(int i = 0; i < db_usuarios.count; i++){
        if(db_usuarios.usuarios[i].id == id){
            *usuario = db_usuarios.usuarios[i];

            strcpy(usuario->contra, "");

            pthread_mutex_unlock(&db_usuarios.mutex);
            return 0;
        }
    }

    pthread_mutex_unlock(&db_usuarios.mutex);
    return 1;   // no encontrado
}



int db_habitos_init(const char* filename){
    cJSON *habitos = cJSON_Parse(get_data(filename));

    pthread_mutex_init(&db_habitos.mutex, NULL);

    db_habitos.count = 0;

    cJSON *habito = NULL;


    cJSON_ArrayForEach(habito, habitos){
        int index = db_habitos.count;

        cJSON *id_json = cJSON_GetObjectItem(habito, "id");

        cJSON *nombre_json = cJSON_GetObjectItem(habito, "nombre");

        db_habitos.habitos[index].id = id_json->valueint;

        strncpy(db_habitos.habitos[index].nombre, nombre_json->valuestring, sizeof(db_habitos.habitos[index].nombre));

        db_habitos.count++;
    }

    cJSON_Delete(habitos);

    return 0;
}

Habito *db_habito_get_by_id(int id);
int db_habitos_insert(Habito habito){
    pthread_mutex_lock(&db_habitos.mutex);

    // Verificar duplicado por nombre
    for(int i = 0; i < db_habitos.count; i++){
        if(strcmp(db_habitos.habitos[i].nombre, habito.nombre) == 0){
            pthread_mutex_unlock(&db_habitos.mutex);
            return 1;
        }
    }

    // Asignar ID y agregar
    habito.id = db_habitos.count + 1;
    db_habitos.habitos[db_habitos.count] = habito;
    db_habitos.count++;

    // Serializar y guardar
    cJSON *arreglo = cJSON_CreateArray();
    for(int i = 0; i < db_habitos.count; i++){
        cJSON_AddItemToArray(arreglo, habito_to_json(db_habitos.habitos[i]));
    }

    char *json_str = cJSON_Print(arreglo);
    file_db_save("habitos.json", json_str);

    free(json_str);
    cJSON_Delete(arreglo);
    pthread_mutex_unlock(&db_habitos.mutex);
    return 0;
}

int db_habits_get(Habito * habitos, int  *count){

    pthread_mutex_lock(&db_habitos.mutex);

    int n = db_habitos.count;

    memcpy(habitos, db_habitos.habitos, sizeof(Habito) * n);
    *count = n;

    pthread_mutex_unlock(&db_habitos.mutex);

    return 0;
}

//Regisrtrar el usuario nuevo
int db_user_register(Usuario_t usuario){

    pthread_mutex_lock(&db_usuarios.mutex);

    //Se verifica que no exista el usuario
    for(int i = 0; i < db_usuarios.count; i++){

        //Si existe se devuelve 1
        if(strcmp(db_usuarios.usuarios[i].username,usuario.username) == 0){
            pthread_mutex_unlock(&db_usuarios.mutex);
            return 1;
        }
    }

    //Si no existe se registra en la RAM:
    //Se le asigna el siguiente id
    usuario.id = db_usuarios.count + 1;
    int index = db_usuarios.count;
    //Se agrega al final del arreglo
    db_usuarios.usuarios[index] = usuario;

    db_usuarios.count++;

    //Se formatea con json
    cJSON *usuarios_json = cJSON_CreateArray();
    for(int i = 0; i < db_usuarios.count; i++){
        cJSON_AddItemToArray(usuarios_json,usuario_to_json(db_usuarios.usuarios[i]));
    }

    //Se convierte en String
    char *json_str = cJSON_Print(usuarios_json);

    //Se guarda en el archivo
    file_db_save("datos.json",json_str);

    //Se libera memoria
    free(json_str);
    cJSON_Delete(usuarios_json);

    //Se libera la memoria compartida
    pthread_mutex_unlock(&db_usuarios.mutex);

    //Se devuelve cero, guardado en la base
    return 0;
}

int db_usuariohabito_init(const char *filename){
    cJSON *usuariohabitos = cJSON_Parse(get_data(filename));

    pthread_mutex_init(&db_usuariohabitos.mutex, NULL);

    db_usuariohabitos.count = 0;

    cJSON *usuariohabito = NULL;


    cJSON_ArrayForEach(usuariohabito, usuariohabitos){
        int index = db_usuariohabitos.count;

        db_usuariohabitos.usuariohabitos[index] = json_to_usuariohabito(usuariohabito);

        db_usuariohabitos.count++;
    }

    cJSON_Delete(usuariohabitos);

    return 0;
}

int db_usuariohabito_insert(UsuarioHabito usuariohabito){

    pthread_mutex_lock(&db_usuariohabitos.mutex);

    // Verificar duplicados
    for(int i = 0; i < db_usuariohabitos.count; i++){

        UsuarioHabito actual =
            db_usuariohabitos.usuariohabitos[i];

        if(
            actual.usuario_id == usuariohabito.usuario_id &&
            actual.habito_id == usuariohabito.habito_id
        ){

            pthread_mutex_unlock(
                &db_usuariohabitos.mutex
            );

            return 1;
        }
    }

    // Asignar ID
    usuariohabito.id = db_usuariohabitos.count + 1;

    usuariohabito.activo = 1;

    // Insertar en RAM
    int index = db_usuariohabitos.count;

    db_usuariohabitos.usuariohabitos[index] = usuariohabito;

    db_usuariohabitos.count++;

    // Serializar arreglo completo
    cJSON *json_array = cJSON_CreateArray();

    for(int i = 0;i < db_usuariohabitos.count;i++){

        cJSON_AddItemToArray(json_array,usuariohabito_to_json(db_usuariohabitos.usuariohabitos[i]));
    }

    // Convertir a string
    char *json_str = cJSON_Print(json_array);

    // Guardar archivo
    file_db_save("usuariohabitos.json",json_str);

    // Liberar memoria
    free(json_str);

    cJSON_Delete(json_array);

    pthread_mutex_unlock( &db_usuariohabitos.mutex);

    return 0;
}

int db_usuariohabito_get_by_id(int id, UsuarioHabito * usuariohabito){
    pthread_mutex_lock(&db_usuariohabitos.mutex);

    for(int i = 0; i < db_usuariohabitos.count; i++){
        if(db_usuariohabitos.usuariohabitos[i].id == id){
            *usuariohabito = db_usuariohabitos.usuariohabitos[i];

            pthread_mutex_unlock(&db_usuariohabitos.mutex);
            return 0;
        }
    }

    pthread_mutex_unlock(&db_usuariohabitos.mutex);
    return 1; 
}

UsuarioHabito get_usuario_habito(int usuario_id, int habito_id)
{
    UsuarioHabito resultado;
    memset(&resultado, 0, sizeof(resultado));

    pthread_mutex_lock(&db_usuariohabitos.mutex);

    for (int i = 0; i < db_usuariohabitos.count; i++)
    {
        if (db_usuariohabitos.usuariohabitos[i].usuario_id == usuario_id &&
            db_usuariohabitos.usuariohabitos[i].habito_id == habito_id)
        {
            resultado = db_usuariohabitos.usuariohabitos[i];
            break;
        }
    }

    pthread_mutex_unlock(&db_usuariohabitos.mutex);

    printf("usuario_id=%d habito_id=%d -> encontrado id=%d\n",
           usuario_id,
           habito_id,
           resultado.id);

    return resultado;
}

int db_habitos_get_by_usuario_id(int usuario_id,Habito *habitos,int *count){

    pthread_mutex_lock(&db_usuariohabitos.mutex);
    pthread_mutex_lock(&db_habitos.mutex);

    int encontrados = 0;

    for(int i = 0; i < db_usuariohabitos.count; i++){

        UsuarioHabito uh =
            db_usuariohabitos.usuariohabitos[i];

        if(uh.usuario_id == usuario_id &&
           uh.activo == 1){

            for(int j = 0; j < db_habitos.count; j++){

                Habito h = db_habitos.habitos[j];

                if(h.id == uh.habito_id){

                    habitos[encontrados] = h;

                    encontrados++;

                    break;
                }
            }
        }
    }

    *count = encontrados;

    pthread_mutex_unlock(&db_habitos.mutex);
    pthread_mutex_unlock(&db_usuariohabitos.mutex);

    return 0;
}

int db_registros_init(const char * filename){
    cJSON * registros = cJSON_Parse(get_data(filename));

    pthread_mutex_init(&db_registros.mutex, NULL);

    db_registros.count = 0;

    cJSON *registro = NULL;

    cJSON_ArrayForEach(registro, registros){
        int index = db_registros.count;
        db_registros.registros[index] = registro_from_json(registro);

        db_registros.count++;
    }
    cJSON_Delete(registros);
    return 0;
}


int db_registros_get_all(Registro_t * registros, int *count){
    pthread_mutex_lock(&db_registros.mutex);

    int n = db_registros.count;
    memcpy(registros, db_registros.registros, sizeof(Registro_t) * n);
    *count = n;

    pthread_mutex_unlock(&db_registros.mutex);
    return 0;
}
/*
int db_registros_insert(Registro_t registro){
    pthread_mutex_lock(&db_registros.mutex);

    registro.id = db_registros.count + 1;

    int index = db_registros.count;

    db_registros.registros[index] = registro;

    db_registros.count ++;

    cJSON * registros_array_json = cJSON_CreateArray();

    for (int i = 0; i < db_registros.count; i++){
        cJSON_AddItemToArray(registros_array_json, registro_to_json(db_registros.registros[i]));
    }

    char * registros_array_str = cJSON_Print(registros_array_json);

    file_db_save("registros.json", registros_array_str);

    free(registros_array_str);

    cJSON_Delete(registros_array_json);

    pthread_mutex_unlock(&db_registros.mutex);

    return 0;
    
}
*/

int db_registro_insert(int habito_id, int usuario_id){
    UsuarioHabito usuariohabito = get_usuario_habito(usuario_id, habito_id);
    Registro_t registro;
    


    if (usuariohabito.id == 0) {
        printf("ERROR: no existe UsuarioHabito para usuario=%d habito=%d\n",
               usuario_id, habito_id);
        return 1;
    }
    pthread_mutex_lock(&db_registros.mutex);
    registro.id = db_registros.count +1;

    int index = db_registros.count;

    registro.usuariohabito_id = usuariohabito.id;
    

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(registro.fecha, sizeof(registro.fecha), "%Y-%m-%d", tm_info);
    
    db_registros.registros[index] = registro;

    db_registros.count ++;

    cJSON * registros_array_json = cJSON_CreateArray();

    for (int i = 0; i < db_registros.count; i++){
        cJSON_AddItemToArray(registros_array_json, registro_to_json(db_registros.registros[i]));
    }

    char * registros_array_str = cJSON_Print(registros_array_json);

    file_db_save("registros.json", registros_array_str);

    free(registros_array_str);

    cJSON_Delete(registros_array_json);

    pthread_mutex_unlock(&db_registros.mutex);

    return 0;

}

int db_get_registros_usuario(int usuario_id, RegistroVista *salida, int *count){
    int encontrados = 0;

    pthread_mutex_lock(&db_mutex);

    for(int i = 0; i < db_registros.count; i++){

        Registro_t reg = db_registros.registros[i];

        // Buscar el UsuarioHabito correspondiente
        UsuarioHabito *uh = NULL;

        for(int j = 0; j < db_usuariohabitos.count; j++){

            if(db_usuariohabitos.usuariohabitos[j].id ==
               reg.usuariohabito_id){

                uh = &db_usuariohabitos.usuariohabitos[j];
                break;
            }
        }

        // No existe la relación
        if(uh == NULL)
            continue;

        // No pertenece al usuario solicitado
        if(uh->usuario_id != usuario_id)
            continue;

        // Buscar el hábito
        Habito *hab = NULL;

        for(int j = 0; j < db_habitos.count; j++){

            if(db_habitos.habitos[j].id ==
               uh->habito_id){

                hab = &db_habitos.habitos[j];
                break;
            }
        }

        // No existe el hábito
        if(hab == NULL)
            continue;

        // Llenar estructura de salida
        salida[encontrados].id = reg.id;

        strcpy(salida[encontrados].nombre_habito,
               hab->nombre);

        strcpy(salida[encontrados].fecha,
               reg.fecha);

        encontrados++;
    }

    *count = encontrados;

    pthread_mutex_unlock(&db_mutex);

    return 0;
}

/*
int db_usuariohabito_get(UsuarioHabito * usuariohabito, int *count){
    pthread_mutex_lock(&db_usuariohabito.mutex)
}

int db_get_usuarioHabitos_by_usuario_id(UsuarioHabito * usuariohabito, int *count, ){
    pthread_mutex_lock(&db_usuariohabito.mutex)
}
    */



//Manejon de archivos:

int db_update_user(Usuario_t usuario){
    int i;
    pthread_mutex_lock(&db_usuarios.mutex);
    for(i = 0; i<db_usuarios.count; i++){
        if(db_usuarios.usuarios[i].id == usuario.id){
            break;
        }
    }
    strcpy(db_usuarios.usuarios[i].nombre, usuario.nombre);
    strcpy(db_usuarios.usuarios[i].apellido, usuario.apellido);
    strcpy(db_usuarios.usuarios[i].correo, usuario.correo);
    db_usuarios.usuarios[i].peso = usuario.peso;
    cJSON *usuarios_json = cJSON_CreateArray();

    for(int i = 0; i < db_usuarios.count; i++){
        cJSON_AddItemToArray(
            usuarios_json,
            usuario_to_json(db_usuarios.usuarios[i])
        );
    }

    char *json_str = cJSON_Print(usuarios_json);

    file_db_save("datos.json", json_str);

    free(json_str);
    cJSON_Delete(usuarios_json);

    pthread_mutex_unlock(&db_usuarios.mutex);

    return 0;
}



char * get_data(const char * filename){
    FILE *file = fopen(filename, "r");
    
    if(file == NULL){
        return(NULL);
        printf("Error al encontrar el archivo\n");
    }
    

    //Llevar a la posicion final el archivo
    fseek(file, 0, SEEK_END);

    //Obtener el tamaño en numero de posiciones;
    long file_size = ftell(file);

    //Regresar la  posicion al inicio
    rewind(file);

    //Asignar memoria del tamaño del archivo
    char * buffer = (char*)malloc(file_size + 1);
    //Si fallla la signacion
    if(buffer == NULL){
        fclose(file);
        return NULL;
    }

    //Leer el archivo completo
    fread(buffer, sizeof(char), file_size, file);
    buffer[file_size] = '\0';

    //Cerrar el archivo
    fclose(file);

    return buffer;
}

int file_db_save(const char * filename, const char * data){

    FILE * file = fopen(filename, "w");
    if(file == NULL) return 1;
    fputs(data, file);
    fclose(file);
    return 0; 
}
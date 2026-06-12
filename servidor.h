#include "cJSON.h"
#include "db.h"
#include <stdbool.h>



//HANDLER___________________________________________________________
int handle_log(cliente_contexto * cliente_ctx, char *username, char *contra);
int handle_reg(Usuario_t usuario_login, Respuesta_t *respuesta);

int handle_register_habit(cliente_contexto * cliente_ctx);

int handle_get_habits(cliente_contexto * cliente_ctx);
int handle_add_user_habits(cliente_contexto *cliente_ctx,int *ids,int count);
int handle_get_user_habits(cliente_contexto *cliente_ctx);

int handle_user_set_status();

int handle_user_set_password(cliente_contexto *cliente_ctx);

int handle_user_update(Usuario_t usuario, Respuesta_t * respuesta);

int handle_registro_insert(int habito_id, int usuario_id, char* nota,  Respuesta_t * respuesta);



//ROUTER____________________________________________________________
int route_request(cliente_contexto * cliente_ctxt);
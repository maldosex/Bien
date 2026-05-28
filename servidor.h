#include "cJSON.h"
#include "db.h"
#include <stdbool.h>



//HANDLER___________________________________________________________
int handle_log(cliente_contexto * cliente_ctx, char *username, char *contra);
int handle_reg(Usuario_t usuario_login, Respuesta_t *respuesta);



int handle_get_habits(cliente_contexto * cliente_ctx);
int handle_add_user_habits(cliente_contexto *cliente_ctx,int *ids,int count);
int handle_get_user_habits(cliente_contexto *cliente_ctx);



//ROUTER____________________________________________________________
int route_request(cliente_contexto * cliente_ctxt);
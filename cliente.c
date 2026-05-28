#include <semaphore.h>
#include <sys/stat.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include<sys/types.h>
#include<unistd.h>
#include "cJSON.h"

#include "api.h"
#include "gui.h"
#include <time.h>    
#include <errno.h>


typedef struct
{
    pid_t pid;

}shm_general;



static int sem_wait_timeout(sem_t *sem, int segundos) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    ts.tv_sec += segundos;
    return sem_timedwait(sem, &ts);
}


int main(){
    sem_t * mutex_general, *solicitud, *respuesta;

    //Abrir los semaforos  generales:
    mutex_general = sem_open("/mutex_general", 0);
    solicitud = sem_open("/solicitud", 0);
    respuesta = sem_open("/respuesta", 0);


    //Mandar solicitud
    printf("Soy el proceso %d\n", getpid());
    printf("Intento contactar al servidor\n");
    //
    if(sem_wait_timeout(mutex_general, 5) != 0){
        printf("Servidor no disponible. Presione Enter para salir.\n");
        getchar();
        return 1;
    }

    int shm_fd = shm_open("/shm_general", O_RDWR, 0666);
    shm_general * shm_g = mmap(NULL, sizeof(shm_general), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);

    shm_g->pid = getpid();
    sem_post(solicitud);
    printf("Solicito region de memoria compartida\n");
    if(sem_wait_timeout(respuesta, 5) != 0){
        printf("Servidor no disponible. Presione Enter para salir.\n");
        sem_post(mutex_general);
        getchar();
        return 1;
    }


    char name_shmem[64];
    sprintf(name_shmem,"/shm_%d",shm_g->pid);



    printf("Mando mi solicitud...\n");


    printf("Respuesta recibida\n");
    sem_post(mutex_general);

    int shmp_fd = shm_open(name_shmem, O_CREAT|O_RDWR, 0666);        
    shm_privada * shm_p = mmap(NULL, sizeof(shm_privada), PROT_READ|PROT_WRITE, MAP_SHARED, shmp_fd, 0);
    int i = 0;
    char str_usuario[100];
    char str_contra[100];
    

    initscr();
    start_color();
	curs_set(1);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    Screen current = SCREEN_LOGIN_MENU;

    while (current != SCREEN_EXIT) {

        switch(current) {

            case SCREEN_LOGIN_MENU: {
                int op = log_menu();

                if (op == 0)
                    current = SCREEN_LOGIN;
                else if (op == 1)
                    current = SCREEN_REGISTER;
                else
                    current = SCREEN_EXIT;

                break;
            }

            case SCREEN_LOGIN: {

                int status = form_login(shm_p, str_usuario, str_contra);

                if(status == -1)
                    current = SCREEN_LOGIN_MENU;

                if (status == 0)
                    current = SCREEN_HOME;
                else if (status == 10)
                {
                    current = SCREEN_ADMIN_MENU;
                }
                

                break;
            }

            case SCREEN_REGISTER: {

                int op = form_register(shm_p);

                if (op == 1)
                    current = SCREEN_LOGIN;  /* Registro exitoso → ir a login */
                else
                    current = SCREEN_LOGIN_MENU; /* F1 o error → menú principal */

                break;
            }

            case SCREEN_HOME: {

                int op = hp_menu();

                if (op == 0)
                    current = SCREEN_HABITS;

                else if(op == 1)
                    current = SCREEN_MY_HABITS;

                else if(op == 2)
                    current = SCREEN_MY_PROGRESS;

                else if(op == 3)
                    current = SCREEN_SETTINGS;

                else
                    current = SCREEN_LOGIN_MENU;

                break;
            }           

            case SCREEN_HABITS: {

                Habito habitos[50];
                int count = 0;
                int status = api_get_all_habits(shm_p, habitos, &count);

                int habitos_ids[50];
                int selected_count = 0;

                if(status == 0){
                    int result = menu_available_habits(habitos, count, habitos_ids, &selected_count);

                    if(result == 1 && selected_count > 0){ 
                        api_register_usuariohabitos(shm_p, habitos_ids, selected_count);
                    }
                }
                else{
                
                    clear();
                
                    mvprintw(LINES / 2,(COLS - 30) / 2,"Error obteniendo habitos");
                
                    refresh();
                
                    getch();
                }
            
                current = SCREEN_HOME;
            
                break;
            }

            case SCREEN_MY_HABITS: {

                Habito habitos[50];
                int count = 0;
                int habito_ids[50];
                int selected_count = 0;

                int status = api_get_user_habits(shm_p, habitos, &count);

                if(status != 0 || count == 0){
                    clear();
                    attron(COLOR_PAIR(1));
                    mvprintw(1, (COLS - 10) / 2, "Habit FLOW");
                    attroff(COLOR_PAIR(1));
                    mvprintw(LINES/2, (COLS - 38) / 2, "Aun no tienes habitos registrados.");
                    mvprintw(LINES/2 + 1, (COLS - 44) / 2, "Ve a 'Habit Store' para agregar habitos.");
                    mvprintw(LINES - 2, 0, "Presione cualquier tecla para volver");
                    refresh();
                    getch();
                    clear();
                    refresh();
                } else {
                    menu_my_habits(habitos, count, habito_ids, &selected_count);
                }

                current = SCREEN_HOME;
                break;
        }

            case SCREEN_MY_PROGRESS: {
                clear();
                attron(COLOR_PAIR(1));
                mvprintw(1, (COLS - 10) / 2, "Habit FLOW");
                attroff(COLOR_PAIR(1));
                mvprintw(LINES/2 - 1, (COLS - 14) / 2, "Mi Progreso");
                mvprintw(LINES/2,     (COLS - 38) / 2, "Esta seccion estara disponible pronto.");
                mvprintw(LINES - 2, 0, "Presione cualquier tecla para volver");
                refresh();
                getch();
                clear();
                refresh();
                current = SCREEN_HOME;
                break;
            }

            case SCREEN_SETTINGS: {
                clear();
                attron(COLOR_PAIR(1));
                mvprintw(1, (COLS - 10) / 2, "Habit FLOW");
                attroff(COLOR_PAIR(1));
                mvprintw(LINES/2 - 1, (COLS - 16) / 2, "Configuracion");
                mvprintw(LINES/2,     (COLS - 38) / 2, "Esta seccion estara disponible pronto.");
                mvprintw(LINES - 2, 0, "Presione cualquier tecla para volver");
                refresh();
                getch();
                clear();
                refresh();
                current = SCREEN_HOME;
                break;
            }

            case SCREEN_ADMIN_MENU: {
                int op = admin_menu();
                if (op == 0){
                    current = SCREEN_ADMIN_USUARIOS;
                    break;
                }
                break;
            }
            case SCREEN_ADMIN_USUARIOS: {
                Usuario_t usuarios[50];
                int count = 0;
                int selected_id = -1;
                        
                int status = api_get_usuarios(shm_p, usuarios, &count);
                        
                if(status != 0 || count == 0){
                    clear();
                    mvprintw(LINES/2, (COLS - 30) / 2, "No hay usuarios disponibles");
                    mvprintw(LINES - 2, 0, "Presione cualquier tecla para volver");
                    refresh();
                    getch();
                    clear();
                    refresh();
                } else {
                    int op = menu_administrar_usuarios(usuarios, count, &selected_id);
                    // aqui puedes usar selected_id para lo que necesites
                }

    current = SCREEN_HOME;
    break;
}

    }

    }
    endwin();

    return 0;
}
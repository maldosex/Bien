#include "gui.h"


char *trim(char *str) {
    char *end;

    while(*str == ' ') str++;

    if(*str == 0) return str;

    end = str + strlen(str) - 1;
    while(end > str && *end == ' ') end--;

    *(end + 1) = '\0';

    return str;
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}
#include <stdio.h>

void print_logotipe(WINDOW *win, int starty, int startx, int width, chtype color)
{	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;
    
    length = strlen("| |_| |/ _` | '_ \\| | __| | |_  | |/ _ \\ \\ /\\ / /\n");
	temp = (width - length)/ 2;
	x = startx + (int)temp;

	wattron(win, color);
	mvwprintw(win, y,   x, " _   _       _     _ _     _____ _               \n");
	mvwprintw(win, y+1, x, "| | | | __ _| |__ (_) |_  |  ___| |              \n");
	mvwprintw(win, y+2, x, "| |_| |/ _` | '_ \\| | __| | |_  | |/ _ \\ \\ /\\ / /\n");
	mvwprintw(win, y+3, x, "|  _  | (_| | |_) | | |_  |  _| | | (_) \\ V  V / \n");
	mvwprintw(win, y+4, x, "|_| |_|\\__,_|_.__/|_|\\__| |_|   |_|\\___/ \\_/\\_/  \n");
	wattroff(win, color);
	refresh();                  
}




//------------MENUS____________________
char *hp_menu_choices[] = {
    "Habitos",
    "Mis Habitos",
    "Progreso",
    "Configuracion",
    (char *)NULL,
};
char *login_menu_choices[] = {
    "Log In",
    "Register",
    "Exit",
    (char *)NULL,
};
char *admin_menu_choices[] = {
    "Usuarios",
    "Habitos",
    "Reportes",
    "Configuracion",
    (char *)NULL,
};

char *admin_actions_menu_choices[] = {
    "Crear",
    "Ver",
    "Rep",
    "Configuracion",
    (char *)NULL,
};


int log_menu()
{	ITEM **my_items;
	int c;				
	MENU *my_menu;
    WINDOW *my_menu_win;
    int n_choices, i;
    
    int ancho_ventana = getmaxx(stdscr); 
    int ancho_logotipo = 40; 
    int start_x = (ancho_ventana - ancho_logotipo) / 2;
    int centro = ancho_ventana/2;

	
	init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

	/* Create items */
        n_choices = ARRAY_SIZE(login_menu_choices);
        my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i)
                my_items[i] = new_item(login_menu_choices[i], " ");

	/* Crate menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Create the window to be associated with the menu */
        int cols = 40;
        my_menu_win = newwin(10, cols, 10, centro - cols/2);
        keypad(my_menu_win, TRUE);
     
	/* Set main window and sub window */
        set_menu_win(my_menu, my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));

	/* Set menu mark to the string " * " */
        set_menu_mark(my_menu, " * ");
        

	/* Print a border around the main window and print a title */
        box(my_menu_win, 0, 0);
	print_in_middle(my_menu_win, 1, 0, 40, "Menu", COLOR_PAIR(2));
    

    print_logotipe(stdscr, 1, start_x, ancho_logotipo, COLOR_PAIR(2));

	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
	mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
	mvprintw(LINES - 2, 0, "F1 to exit");
    attron(COLOR_PAIR(1));
    attron(A_BLINK );
    attroff(A_BLINK);
    attroff(COLOR_PAIR(1));
	refresh();
        

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	int option = -1;
	while((c = wgetch(my_menu_win)) != KEY_F(1)){
		switch(c){	
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
        	case 10:
            	mvprintw(LINES-2, 3, "selected: ");
				option = item_index(current_item(my_menu));
            	break;
		}
        wrefresh(my_menu_win);
		if (option != -1) {
            break;
        }
	}	

	/* Unpost and free all the memory taken up */
        unpost_menu(my_menu);
        free_menu(my_menu);
		
        for(i = 0; i < n_choices; ++i)
                free_item(my_items[i]);
	delwin(my_menu_win);
	clear();
    refresh();
	return option;
}

int hp_menu()
{	ITEM **my_items;
	int c;				
	MENU *my_menu;
    WINDOW *my_menu_win;
    int n_choices, i;
    int ancho_ventana = getmaxx(stdscr); 
    int ancho_logotipo = 40; 
    int start_x = (ancho_ventana - ancho_logotipo) / 2;
    int centro = ancho_ventana/2;
	
    
	/* Initialize curses */
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);

	/* Create items */
        n_choices = ARRAY_SIZE(hp_menu_choices);
        my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i)
                my_items[i] = new_item(hp_menu_choices[i], " ");

	/* Crate menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Create the window to be associated with the menu */
        int cols = 40;
        my_menu_win = newwin(10, cols, 10, centro - cols/2);
        keypad(my_menu_win, TRUE);
     
	/* Set main window and sub window */
        set_menu_win(my_menu, my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));

	/* Set menu mark to the string " * " */
        set_menu_mark(my_menu, " * ");

	/* Print a border around the main window and print a title */
        box(my_menu_win, 0, 0);
	print_in_middle(my_menu_win, 1, 0, 40, "Modulos", COLOR_PAIR(1));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
	mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
	mvprintw(LINES - 2, 0, "F1 to exit");
    attron(COLOR_PAIR(1));
    print_logotipe(stdscr, 1, start_x, ancho_logotipo, COLOR_PAIR(2));
    attroff(COLOR_PAIR(1));
	refresh();
        

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	int option = -1;
	while((c = wgetch(my_menu_win)) != KEY_F(1)){
		switch(c){	
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
        	case 10:
            	mvprintw(LINES-2, 3, "selected: ");
				option = item_index(current_item(my_menu));
            	break;
		}
        wrefresh(my_menu_win);
		//Si eligio una opcion entonces sale del ciclo del menu
		if (option != -1) {
            break;
        }
	}	

	/* Unpost and free all the memory taken up */
        unpost_menu(my_menu);
        free_menu(my_menu);
		
        for(i = 0; i < n_choices; ++i){
                free_item(my_items[i]);
			}
    delwin(my_menu_win);
	clear();
    refresh();
	return option;
}

//(Habito *habitos, int count, int * ids, int *selected_count)

int menu_my_habits(shm_privada * shm_p, Habito *habitos, int count, int * ids, int *selected_count, int my_id){

    int i;
    char choices[30][100];

    int ancho_ventana = getmaxx(stdscr); 
    int alto_ventana = getmaxy(stdscr);
    int ancho_logotipo = 40; 
    int start_x = (ancho_ventana - ancho_logotipo) / 2;
    int centro = ancho_ventana/2;

    init_pair(1, COLOR_GREEN, COLOR_BLACK);

    for(i = 0; i < count; i++){

        snprintf(choices[i], sizeof(choices[i]), "%d - %s", habitos[i].id,  habitos[i].nombre);
    }


    ITEM **my_items;
	int c;				
	MENU *my_menu;
    WINDOW *my_menu_win;
    int n_choices;

    WINDOW *win_reg;

    win_reg = newwin(
        alto_ventana - 5 - 9,
        50,
        9,
        45);

    box(win_reg, 0, 0);
    wrefresh(win_reg);



    RegistroVista registros[100];
    int count_registros = 0;

    api_get_registros_usuario(shm_p,registros, &count_registros);
	


	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);

	/* Create items */
        n_choices = count +1;
        my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
        for(i = 0; i < count; ++i){
                my_items[i] = new_item(choices[i], "");
        }
        my_items[count] = NULL;

	/* Crate menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Create the window to be associated with the menu */
    int cols = 40;
        my_menu_win = newwin(alto_ventana-5-9, cols, 9, 2);
        keypad(my_menu_win, TRUE);

        set_menu_win(my_menu, my_menu_win);
        int alto_ventana_habitos = getmaxy(my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win, alto_ventana_habitos-4, cols-2, 3, 1));

	/* Set menu mark to the string " * " */
        set_menu_mark(my_menu, " * ");

	/* Print a border around the main window and print a title */
        box(my_menu_win, 0, 0);
	print_in_middle(my_menu_win, 1, 0, 40, "-----Mis habitos-----", COLOR_PAIR(1));
    
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
	mvwaddch(my_menu_win, 2, 39, ACS_RTEE);




    mvprintw(LINES - 2, 0, "[F1] salir\t[R] Agregar Registro\t");
    attron(COLOR_PAIR(1));
    print_logotipe(stdscr, 1, start_x, ancho_logotipo, COLOR_PAIR(2));
    attroff(COLOR_PAIR(1));

	menu_opts_off(my_menu, O_ONEVALUE);
	refresh();
        

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	int option = -1;
	while((c = wgetch(my_menu_win)) != KEY_F(1)){
		switch(c){	
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			case ' ':
				menu_driver(my_menu, REQ_TOGGLE_ITEM);
				break;
        	case 10: {

                if(ids != NULL && selected_count != NULL){
                    ITEM **items = menu_items(my_menu);
                    int seleccionados = 0;
                    for(i = 0; i < item_count(my_menu); i++){
                        if(item_value(items[i]) == TRUE){
                            ids[seleccionados] = habitos[i].id;
                            seleccionados++;
                        }
                    }
                    *selected_count = seleccionados;
                }
                option = 1;
                break;
            }
            case 'r':{
                ITEM *current = current_item(my_menu);

                int index = item_index(current);

                char msg[100];
                char nota[50];
                form_nota(shm_p, my_menu_win, nota);

                api_insert_registro(shm_p, my_id,habitos[index].id, nota, msg);
                int h, w;
                getmaxyx(my_menu_win, h, w);

                mvwprintw(my_menu_win, h - 2, 2, "%s", msg);
                wrefresh(my_menu_win);
                break;
                
            }
		}
         api_get_registros_usuario(shm_p,registros, &count_registros);
        ITEM *current = current_item(my_menu);

        if(current != NULL)
        {
            int index = item_index(current);
        
            RegistroVista registroshabito[50];
            int count_registroshabito = 0;
        
            
            for(int k = 0; k < count_registros; k++)
            {
                if(registros[k].habito_id == habitos[index].id)
                {
                    registroshabito[count_registroshabito++] =registros[k];
                }
            }
            wrefresh(stdscr);
            menu_my_registros(win_reg,registroshabito,count_registroshabito);}

        wrefresh(my_menu_win);
		if (option != -1) {
            break;
        }
	}	

	/* Unpost and free all the memory taken up */
        unpost_menu(my_menu);
        free_menu(my_menu);
		
        for(i = 0; i < count; ++i)
                free_item(my_items[i]);

    
	clear();
    refresh();
	return option;
}

int form_nota(shm_privada *shm_p, WINDOW *root, char *nota)
{
    FIELD *field[2];
    FORM *my_form;
    WINDOW *win;
    WINDOW *sub;

    int ch;
    int rows, cols;
    int respuesta_form = -1;

    init_pair(1, COLOR_CYAN,  COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    init_pair(3, COLOR_BLACK, COLOR_WHITE);
    init_pair(4, COLOR_RED,   COLOR_BLACK);
    init_pair(5, COLOR_GREEN, COLOR_BLACK);

    /*------------------*/
    /* Crear formulario */
    /*------------------*/

    field[0] = new_field(1, 25, 0, 0, 0, 0);
    field[1] = NULL;

    set_field_back(field[0], COLOR_PAIR(2));
    set_field_fore(field[0], COLOR_PAIR(3));
    field_opts_off(field[0], O_AUTOSKIP);

    my_form = new_form(field);

    scale_form(my_form, &rows, &cols);

    /*------------------*/
    /* Crear popup      */
    /*------------------*/

    int h = 9;
    int w = 45;

    int y = getbegy(root) + (getmaxy(root) - h) / 2;
    int x = getbegx(root) + (getmaxx(root) - w) / 2;

    touchwin(root);
    wrefresh(root);

    win = newwin(h, w, y, x);
    keypad(win, TRUE);

    box(win, 0, 0);

    wattron(win, COLOR_PAIR(1));
    mvwprintw(win, 1, (w - 13) / 2, "Nuevo Registro");
    wattroff(win, COLOR_PAIR(1));

    mvwprintw(win, 3, 2, "Nota:");
    mvwprintw(win, 6, 2, "[Enter] Guardar\t[Esc] Cancelar");

    sub = derwin(win, rows, cols, 3, 11);

    set_form_win(my_form, win);
    set_form_sub(my_form, sub);

    post_form(my_form);

    set_current_field(my_form, field[0]);
    pos_form_cursor(my_form);

    wrefresh(win);

    /*------------------*/
    /* Loop principal   */
    /*------------------*/

    while ((ch = wgetch(win)))
    {
        switch (ch)
        {
            case 27:      /* ESC */
                respuesta_form = -1;
                goto cleanup;

            case KEY_BACKSPACE:
            case 127:
            case 8:
                form_driver(my_form, REQ_DEL_PREV);
                break;

            case KEY_LEFT:
                form_driver(my_form, REQ_PREV_CHAR);
                break;

            case KEY_RIGHT:
                form_driver(my_form, REQ_NEXT_CHAR);
                break;

            case KEY_DC:
                form_driver(my_form, REQ_DEL_CHAR);
                break;

            case KEY_HOME:
                form_driver(my_form, REQ_BEG_LINE);
                break;

            case KEY_END:
                form_driver(my_form, REQ_END_LINE);
                break;

            case 10:      /* ENTER */
            {
                form_driver(my_form, REQ_VALIDATION);

                strcpy(nota, trim(field_buffer(field[0], 0)));

                /* Limpiar únicamente la línea de mensajes */
                mvwhline(win, 7, 2, ' ', w - 4);

                goto cleanup;;
                wrefresh(win);
                break;
            }

            default:
                form_driver(my_form, ch);
                break;
        }

        pos_form_cursor(my_form);
        wrefresh(win);
    }

cleanup:

    unpost_form(my_form);

    free_form(my_form);

    free_field(field[0]);

    delwin(sub);

    werase(win);
    wrefresh(win);
    delwin(win);

    /* Redibujar completamente la ventana padre */
    touchwin(root);
    redrawwin(root);
    wrefresh(root);

    return respuesta_form;
}

void actualizar_registros_habito(RegistroVista *registros,int count_registros,int habito_id,WINDOW *reg_win,MENU **menu,ITEM ***items){
    if (*menu != NULL)
    {
        unpost_menu(*menu);
        free_menu(*menu);

        if (*items != NULL)
        {
            for (int i = 0; (*items)[i] != NULL; i++)
                free_item((*items)[i]);

            free(*items);
        }
    }

    int cantidad = 0;

    for (int i = 0; i < count_registros; i++)
    {
        if (registros[i].habito_id == habito_id)
            cantidad++;
    }

    *items = calloc(cantidad + 1, sizeof(ITEM *));

    char texto[100][120];

    int k = 0;

    for (int i = 0; i < count_registros; i++)
    {
        if (registros[i].habito_id != habito_id)
            continue;

        snprintf(
            texto[k],
            sizeof(texto[k]),
            "%s | %s",
            registros[i].fecha,
            registros[i].nota);

        (*items)[k] = new_item(texto[k], "");

        k++;
    }

    (*items)[cantidad] = NULL;

    *menu = new_menu(*items);

    set_menu_win(*menu, reg_win);

    set_menu_sub(
        *menu,
        derwin(
            reg_win,
            getmaxy(reg_win) - 4,
            getmaxx(reg_win) - 2,
            3,
            1));

    set_menu_mark(*menu, "");

    post_menu(*menu);

    box(reg_win, 0, 0);

    print_in_middle(
        reg_win,
        1,
        0,
        getmaxx(reg_win),
        "----- Registros -----",
        COLOR_PAIR(1));

    mvwaddch(reg_win, 2, 0, ACS_LTEE);
    mvwhline(reg_win, 2, 1, ACS_HLINE, getmaxx(reg_win) - 2);
    mvwaddch(reg_win, 2, getmaxx(reg_win) - 1, ACS_RTEE);

    wrefresh(reg_win);
}

int menu_available_habits(Habito *habitos, int count, int * ids, int *selected_count){
    int i;
    char choices[30][100];
    init_pair(1, COLOR_RED, COLOR_BLACK);
    for(i = 0; i < count; i++){
        snprintf(choices[i], sizeof(choices[i]), "%d - %s", habitos[i].id, habitos[i].nombre);
    }

    ITEM **my_items;
    int c;              
    MENU *my_menu;
    WINDOW *my_menu_win;
    int n_choices;

    init_pair(1, COLOR_RED, COLOR_BLACK);

    n_choices = count + 1;
    my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
    for(i = 0; i < count; ++i){
        my_items[i] = new_item(choices[i], "");
    }
    my_items[count] = NULL;

    my_menu = new_menu((ITEM **)my_items);

    my_menu_win = newwin(10, 40, 4, 20);
    keypad(my_menu_win, TRUE);
    set_menu_win(my_menu, my_menu_win);
    set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));
    set_menu_format(my_menu, 5, 1); 

    set_menu_mark(my_menu, " * ");

    box(my_menu_win, 0, 0);
    print_in_middle(my_menu_win, 1, 0, 40, "-----Habitos disponibles-----", COLOR_PAIR(1));
    mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
    mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
    mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
    mvprintw(LINES - 5, 0, "Espacio para seleccionar");
    mvprintw(LINES - 4, 0, "Enter para aceptar y agregar");
    mvprintw(LINES - 4, 0, "Flechas para desplazar");
    mvprintw(LINES - 2, 0, "F1 to exit | PgUp/PgDn para desplazar una pagina");
    attron(COLOR_PAIR(1));
    mvprintw(1, 20, "Habit FLOW");
    attroff(COLOR_PAIR(1));
    menu_opts_off(my_menu, O_ONEVALUE);
    refresh();

    post_menu(my_menu);
    wrefresh(my_menu_win);

    int option = -1;
    while((c = wgetch(my_menu_win)) != KEY_F(1)){
        switch(c){  
            case KEY_DOWN:
                menu_driver(my_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(my_menu, REQ_UP_ITEM);
                break;
            case KEY_NPAGE:                         
                menu_driver(my_menu, REQ_SCR_DPAGE);
                break;
            case KEY_PPAGE:                          
                menu_driver(my_menu, REQ_SCR_UPAGE);
                break;
            case ' ':
                menu_driver(my_menu, REQ_TOGGLE_ITEM);
                break;
            case 10: {
                ITEM **items = menu_items(my_menu);
                int seleccionados = 0;
                for(i = 0; i < item_count(my_menu); i++){
                    if(item_value(items[i]) == TRUE){
                        ids[seleccionados] = habitos[i].id;
                        seleccionados++;
                    }
                }
                *selected_count = seleccionados;
                option = 1;
                break;
            }
        }
        wrefresh(my_menu_win);
        if (option != -1) break;
    }   

    unpost_menu(my_menu);
    free_menu(my_menu);
    for(i = 0; i < count; ++i)
        free_item(my_items[i]);
    clear();
    refresh();
    return option;
}


void menu_my_registros(WINDOW *win,
                       RegistroVista *registros,
                       int count)
{
    int i;
    char texto[120];

    werase(win);
    box(win, 0, 0);

   

    mvwaddch(win, 2, 0, ACS_LTEE);
    mvwhline(win, 2, 1, ACS_HLINE, getmaxx(win) - 2);
    mvwaddch(win, 2, getmaxx(win) - 1, ACS_RTEE);
     print_in_middle(win,1,0,getmaxx(win),registros[0].nombre_habito,COLOR_PAIR(1));

    for(i = 0; i < count && i < getmaxy(win) - 5; i++)
    {
        snprintf(texto,sizeof(texto),"%s - %s",registros[i].fecha,registros[i].nota);

        mvwprintw(win, 3 + i, 1, "%s", texto);
    }

    wrefresh(win);
}


int menu_my_progress(RegistroVista *registros, int count){

    int i;
    int c;

    char choices[100][120];

    init_pair(1, COLOR_RED, COLOR_BLACK);

    for(i = 0; i < count; i++){

        snprintf(choices[i], sizeof(choices[i]), "%s - %s ", registros[i].fecha, registros[i].nombre_habito);
    }

    ITEM **my_items;
    MENU *my_menu;
    WINDOW *my_menu_win;

    my_items = (ITEM **)calloc(count + 1, sizeof(ITEM *));

    for(i = 0; i < count; i++){

        my_items[i] =
            new_item(
                choices[i],
                ""
            );
    }

    my_items[count] = NULL;

    my_menu = new_menu(my_items);

    my_menu_win = newwin(16, 50, 4, 20);

    keypad(my_menu_win, TRUE);

    set_menu_win(my_menu, my_menu_win);

    set_menu_sub( my_menu, derwin( my_menu_win, 9, 0, 3, 1));

    set_menu_format( my_menu, 8, 1);

    set_menu_mark(my_menu, " ");

    box(my_menu_win, 0, 0);

    print_in_middle( my_menu_win, 1, 0, 50, "----- Mi progreso -----", COLOR_PAIR(1));

    mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
    mvwhline(my_menu_win, 2, 1, ACS_HLINE, 48);
    mvwaddch(my_menu_win, 2, 49, ACS_RTEE);

    mvwaddch(my_menu_win, 13, 0, ACS_LTEE);
    mvwhline(my_menu_win, 13, 1, ACS_HLINE, 48);

    mvprintw(LINES - 2,0,"F1 para volver | PgUp/PgDn para desplazarse");

    refresh();

    post_menu(my_menu);

    wrefresh(my_menu_win);

    while((c = wgetch(my_menu_win)) != KEY_F(1)){

        switch(c){

            case KEY_DOWN:
                menu_driver( my_menu, REQ_DOWN_ITEM);
                break;

            case KEY_UP:
                menu_driver(my_menu,REQ_UP_ITEM);
                break;

            case KEY_NPAGE:
                menu_driver(my_menu, REQ_SCR_DPAGE);
                break;

            case KEY_PPAGE:
                menu_driver(my_menu, REQ_SCR_UPAGE);
                break;
        }

        ITEM *cur = current_item(my_menu);
        int idx = item_index(cur);
        mvwprintw(my_menu_win, 14, 1, "                           ");
        mvwprintw(my_menu_win, 14, 1, "Nota: %s", registros[idx].nota);
        refresh();
        wrefresh(my_menu_win);
    }

    unpost_menu(my_menu);

    free_menu(my_menu);

    for(i = 0; i < count; i++){

        free_item(my_items[i]);
    }

    free(my_items);

    delwin(my_menu_win);

    clear();
    refresh();

    return 0;
}
//---------------------_____________________________


//-----------FORMULARIOS-------------------------

int form_login(shm_privada *shm_p, char * usuario, char * contra, int *my_id){
    FIELD *field[3];
    FORM  *my_form;
    WINDOW *my_form_win;
    int ch, rows, cols;


    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);

    // Definición de campos
    field[0] = new_field(1, 20, 2, 14, 0, 0); // Usuario
    field[1] = new_field(1, 20, 4, 14, 0, 0); // Contraseña
    field[2] = NULL;

    set_field_back(field[0], A_UNDERLINE);
    set_field_back(field[1], A_UNDERLINE);
    
    // Ocultar caracteres en el campo de contraseña
    field_opts_off(field[1], O_PUBLIC); 

    my_form = new_form(field);
    scale_form(my_form, &rows, &cols);

    // Crear ventana centrada
    my_form_win = newwin(10, 40, (LINES-10)/2, (COLS-40)/2);
    keypad(my_form_win, TRUE);

	wattron(my_form_win, COLOR_PAIR(3));
    box(my_form_win, 0, 0);
	wattroff(my_form_win, COLOR_PAIR(3));

    set_form_win(my_form, my_form_win);
    set_form_sub(my_form, derwin(my_form_win, rows, cols, 2, 2));

    print_in_middle(my_form_win, 1, 0, 40, " LOGIN", COLOR_PAIR(1));
    
    post_form(my_form);

    
    // Etiquetas de texto
    mvwprintw(my_form_win, 4, 3, "Usuario:");
    mvwprintw(my_form_win, 6, 3, "Contrasea:");

	set_current_field(my_form, field[0]);
	pos_form_cursor(my_form);

	mvprintw(LINES-2, 2, "Presione F1 para salir");
	refresh();
    
    wrefresh(my_form_win);
    

    int respuesta_form = 0;
    char msg[50];

    Usuario_t usuario_login;
    //Se mantendra en en ciclo del formulario hasta que logra hacer login o decida salir

    while((ch = wgetch(my_form_win))) {
        
        switch(ch) {
            case KEY_F(1):
                respuesta_form = -1;
                goto cleanup;
                break;
			case KEY_BACKSPACE:
            case 127:
            case 8:
                // Le ordena al formulario borrar el carácter a la izquierda del cursor
                form_driver(my_form, REQ_DEL_PREV);
                break;
            case KEY_DOWN:
            case KEY_UP:
                form_driver(my_form, REQ_NEXT_FIELD);
                form_driver(my_form, REQ_END_LINE);
                break;
            case 10: // Enter para aceptar
                form_driver(my_form, REQ_VALIDATION);
                char *username = trim(field_buffer(field[0], 0));
                char *contra = trim(field_buffer(field[1], 0));
                //Se mantendra en en ciclo del formulario hasta que logra hacer login o decida salir
                int status = api_login(shm_p, username, contra, msg, my_id); 
                if(status == 1) {
					mvwprintw(my_form_win, 8, 2, "Usuario o contrasena incorrectos");
                    wrefresh(my_form_win);
                }
                if(status == 0 || status == 10){
                    respuesta_form = status;
                    goto cleanup;
                }
                //Si la llamada a la api de login fue exitosa, se saldra del formulario para continuar con el siguiente           
                break;        
            default:            
                form_driver(my_form, ch);            
                break;    
            }
    }
    cleanup:
    unpost_form(my_form);
    free_form(my_form);
    free_field(field[0]);
    free_field(field[1]);
    delwin(my_form_win); // Importante: eliminar ventana creada
    clear();
    refresh();
    return respuesta_form; 
    // Devolvemos 0 si es usuario , -1 si canceló con F1, 10 si es admin
}


int form_register(shm_privada * shm_p){
	FIELD *field[7];
    FORM  *my_form;
    WINDOW *my_form_win;
    int ch, rows, cols;
    Usuario_t usuario;


	init_pair(1, COLOR_CYAN, COLOR_BLACK);    // titulo
	init_pair(2, COLOR_RED, COLOR_BLUE);   // campo activo
	init_pair(3, COLOR_WHITE, COLOR_BLUE);   // texto
	init_pair(4, COLOR_RED, COLOR_BLACK);     // borde

    // Definición de campos
    field[0] = new_field(1, 20, 2, 14, 0, 0); // Usuario
    field[1] = new_field(1, 20, 5, 14, 0, 0); // Contraseña
    field[2] = new_field(1, 20, 8, 14, 0, 0); // Nombre
    field[3] = new_field(1, 20, 11, 14, 0, 0);// Apellido
    field[4] = new_field(1, 20, 14, 14, 0, 0); // Correo
    field[5] = new_field(1, 20, 17, 14, 0, 0); // Peso
    field[6] = NULL;

    set_field_type(field[5], TYPE_INTEGER, 0, 0, 500);

	for(int i = 0; i<6; i++){
		set_field_back(field[i], COLOR_PAIR(2));
		set_field_fore(field[i], COLOR_PAIR(3));
		field_opts_off(field[i], O_AUTOSKIP);
	}
    
    // Ocultar caracteres en el campo de contraseña
    field_opts_off(field[1], O_PUBLIC); 

    my_form = new_form(field);
    scale_form(my_form, &rows, &cols);

    // Crear ventana centrada
    my_form_win = newwin(22, 50, (LINES-24)/2, (COLS-50)/2);
    keypad(my_form_win, TRUE);

	wattron(my_form_win, COLOR_PAIR(3));
    box(my_form_win, 0, 0);
	wattroff(my_form_win, COLOR_PAIR(3));

    set_form_win(my_form, my_form_win);
    set_form_sub(my_form, derwin(my_form_win, rows, cols, 2, 2));

    print_in_middle(my_form_win, 1, 0, 50, "Registrar Usuario", COLOR_PAIR(1));
    
    post_form(my_form);

    
    // Etiquetas de texto
    mvwprintw(my_form_win, 4, 4, "Usuario:");
    mvwprintw(my_form_win, 7, 4, "Contrasea:");
    mvwprintw(my_form_win, 10, 4, "Nombre:");
    mvwprintw(my_form_win, 13, 4, "Apellido:");
    mvwprintw(my_form_win, 16, 4, "Correo:");
    mvwprintw(my_form_win, 19, 4, "Peso:");

	set_current_field(my_form, field[0]);
	pos_form_cursor(my_form);

	mvprintw(LINES-2, 2, "ENTER = Registrar   F1 = Salir");
	refresh();
    
    wrefresh(my_form_win);
    

	int respuesta_form = 0;

    char msg[50];
    while((ch = wgetch(my_form_win))) {
        switch(ch) {
            case KEY_F(1):
                respuesta_form = -1;
                goto fin;
                break;
			case KEY_BACKSPACE:
            case 127:
            case 8:
                // Le ordena al formulario borrar el carácter a la izquierda del cursor
                form_driver(my_form, REQ_DEL_PREV);
                break;
            case KEY_DOWN:
				form_driver(my_form, REQ_NEXT_FIELD);
                form_driver(my_form, REQ_END_LINE);
				break;
            case KEY_UP:
                form_driver(my_form, REQ_PREV_FIELD);
                form_driver(my_form, REQ_END_LINE);
                break;
            case 10:{
                form_driver(my_form, REQ_VALIDATION);

                int lleno = 1;
                for(int k = 0; k < 5; k++){
                    if(strcmp(trim(field_buffer(field[k], 0)), "") == 0){
                        mvwprintw(my_form_win, 20, 4, "No deje espacios vacios        ");
                        wrefresh(my_form_win);
                        wgetch(my_form_win);
                        wmove(my_form_win, 20, 4);
                        wclrtoeol(my_form_win);
                        wattron(my_form_win, COLOR_PAIR(3));
                        box(my_form_win, 0, 0);
	                    wattroff(my_form_win, COLOR_PAIR(3));
                        set_current_field(my_form, field[k]);
                        lleno = 0;
                        break;
                    }
                }
            
                if(!lleno) break;  // salir si hay campos vacíos
            
                if(!esCorreoValido(trim(field_buffer(field[4], 0)))){
                    mvwprintw(my_form_win, 18, 4, "Vaya, eso no parece un correo  ");
                    wrefresh(my_form_win);
                    set_current_field(my_form, field[4]);
                    break;  //
                }


                usuario.peso = atoi(trim(field_buffer(field[5], 0)));
                // Solo llega aquí si todo está lleno Y correo válido
                strcpy(usuario.username, trim(field_buffer(field[0], 0)));
                strcpy(usuario.contra,   trim(field_buffer(field[1], 0)));
                if(strlen(usuario.contra)<8){
                    mvwprintw(my_form_win, 20, 4, "La contrasena es muy corta  ");
                    wrefresh(my_form_win);
                    set_current_field(my_form, field[1]);
                    break;  //
                }
                strcpy(usuario.nombre,   trim(field_buffer(field[2], 0)));
                strcpy(usuario.apellido,   trim(field_buffer(field[3], 0)));
                strcpy(usuario.correo,   trim(field_buffer(field[4], 0)));
                usuario.peso = atoi(trim(field_buffer(field[5], 0)));
                usuario.activo = 1;
            
                //1Para registrar no admin
                int status = api_register(shm_p, usuario, msg, 1);
                if(status == 0){
                    respuesta_form = 1;
                    mvwprintw(my_form_win, 20, 4, "Usuario registrado, haga login ");
                    wrefresh(my_form_win);
                    getch();
                    goto fin;
                } else {
                    mvwprintw(my_form_win, 20, 4, "%s", msg);
                    wrefresh(my_form_win);
                }
                break;
            }
            default:
                form_driver(my_form, ch);
                break;
        }
    }
	fin:
        // Limpieza de ncurses
        unpost_form(my_form);
        free_form(my_form);
        free_field(field[0]);
        free_field(field[1]);
        clear();
        refresh();
        return respuesta_form; // Devolvemos 1 si registro exitoso, -1 si canceló con F1
}



int form_update_user(shm_privada *shm_p, Usuario_t usuario)
{
    FIELD *field[6];
    FORM *my_form;
    WINDOW *my_form_win;
    int ch, rows, cols;

    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);

    field[0] = new_field(1, 20, 2, 14, 0, 0);   // Usuario
    field[1] = new_field(1, 20, 5, 14, 0, 0);   // Nombre
    field[2] = new_field(1, 20, 8, 14, 0, 0);   // Apellido
    field[3] = new_field(1, 20, 11, 14, 0, 0);  // Correo
    field[4] = new_field(1, 20, 14, 14, 0, 0);  // Peso
    field[5] = NULL;

    set_field_type(field[4], TYPE_INTEGER, 0, 0, 500);

    for (int i = 0; i < 5; i++) {
        set_field_back(field[i], COLOR_PAIR(2));
        set_field_fore(field[i], COLOR_PAIR(3));
        field_opts_off(field[i], O_AUTOSKIP);
    }

    /* Precargar datos */

    char peso_str[16];
    sprintf(peso_str, "%d", usuario.peso);

    set_field_buffer(field[0], 0, usuario.username);
    set_field_buffer(field[1], 0, usuario.nombre);
    set_field_buffer(field[2], 0, usuario.apellido);
    set_field_buffer(field[3], 0, usuario.correo);
    set_field_buffer(field[4], 0, peso_str);

    my_form = new_form(field);
    scale_form(my_form, &rows, &cols);

    my_form_win = newwin(20, 50, (LINES - 20) / 2, (COLS - 50) / 2);
    keypad(my_form_win, TRUE);

    box(my_form_win, 0, 0);

    set_form_win(my_form, my_form_win);
    set_form_sub(my_form, derwin(my_form_win, rows, cols, 2, 2));

    print_in_middle(my_form_win, 1, 0, 50,
                    "Editar usuario", COLOR_PAIR(1));

    post_form(my_form);

    mvwprintw(my_form_win, 4, 4,  "Usuario:");
    mvwprintw(my_form_win, 7, 4,  "Nombre:");
    mvwprintw(my_form_win, 10, 4, "Apellido:");
    mvwprintw(my_form_win, 13, 4, "Correo:");
    mvwprintw(my_form_win, 16, 4, "Peso:");

    set_current_field(my_form, field[0]);
    pos_form_cursor(my_form);

    mvprintw(LINES - 2, 2,
             "ENTER = Guardar   F1 = Cancelar");

    refresh();
    wrefresh(my_form_win);

    int respuesta_form = -1;
    char msg[100];

    while ((ch = wgetch(my_form_win))) {

        switch (ch) {

            case KEY_F(1):
                goto fin;

            case KEY_BACKSPACE:
            case 127:
            case 8:
                form_driver(my_form, REQ_DEL_PREV);
                break;

            case KEY_DOWN:
                form_driver(my_form, REQ_NEXT_FIELD);
                form_driver(my_form, REQ_END_LINE);
                break;

            case KEY_UP:
                form_driver(my_form, REQ_PREV_FIELD);
                form_driver(my_form, REQ_END_LINE);
                break;

            case 10:

                form_driver(my_form, REQ_VALIDATION);

                /* Verificar campos vacíos */
                for (int i = 0; i < 5; i++) {

                    if (strcmp(trim(field_buffer(field[i], 0)), "") == 0) {

                        mvwprintw(my_form_win, 18, 2,
                                  "No deje campos vacios            ");
                        wrefresh(my_form_win);
                        goto continuar;
                    }
                }

                /* Validar correo */
                if (!esCorreoValido(trim(field_buffer(field[3], 0)))) {

                    mvwprintw(my_form_win, 18, 2,
                              "Correo invalido                  ");
                    wrefresh(my_form_win);
                    goto continuar;
                }

                strcpy(usuario.username,
                       trim(field_buffer(field[0], 0)));

                strcpy(usuario.nombre,
                       trim(field_buffer(field[1], 0)));

                strcpy(usuario.apellido,
                       trim(field_buffer(field[2], 0)));

                strcpy(usuario.correo,
                       trim(field_buffer(field[3], 0)));

                usuario.peso =
                    atoi(trim(field_buffer(field[4], 0)));

                if (api_update_user(shm_p, usuario, msg) == 0) {

                    mvwprintw(my_form_win, 18, 2,
                              "Usuario actualizado correctamente");
                    wrefresh(my_form_win);

                    wgetch(my_form_win);

                    respuesta_form = 1;
                    goto fin;
                }

                mvwprintw(my_form_win, 18, 2,
                          "%-46s", msg);
                wrefresh(my_form_win);

continuar:
                break;

            default:
                form_driver(my_form, ch);
                break;
        }
    }

fin:

    unpost_form(my_form);
    free_form(my_form);

    for (int i = 0; i < 5; i++) {
        free_field(field[i]);
    }

    delwin(my_form_win);

    clear();
    refresh();

    return respuesta_form;
}


int form_show_user(Usuario_t usuario, int y, int x){
    FORM *my_form;
    WINDOW *detalles_win;
    int ch, rows, cols;

    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);


    /* Precargar datos */

    char peso_str[16];
    sprintf(peso_str, "%d", usuario.peso);



    detalles_win = newwin(20, 50, y, x);
    keypad(detalles_win, TRUE);

    box(detalles_win, 0, 0);


    print_in_middle(detalles_win, 1, 0, 50,
                    "Detalles", COLOR_PAIR(1));


    mvwaddch(detalles_win, 2, 0, ACS_LTEE);
    mvwhline(detalles_win, 2, 1, ACS_HLINE, 49);
    mvwaddch(detalles_win, 2, 50, ACS_RTEE);

    mvwprintw(detalles_win, 4, 4,  "Usuario: %s", usuario.username);
    mvwprintw(detalles_win, 7, 4,  "Nombre: %s", usuario.nombre);
    mvwprintw(detalles_win, 10, 4, "Apellido: %s", usuario.apellido);
    mvwprintw(detalles_win, 13, 4, "Correo: %s", usuario.correo);
    mvwprintw(detalles_win, 16, 4, "Peso: %d", usuario.peso);


    refresh();
    wrefresh(detalles_win);

    int respuesta_form = -1;
    char msg[100];

    

    return 0;
}






bool esCorreoValido(const char *str) {
    const char *arroba = strchr(str, '@');
    const char *punto = strrchr(str, '.');

    //'@' y '.', y el '.' debe estar después de la '@'
    if (arroba != NULL && punto != NULL && punto > arroba) {
        return true;
    }
    return false;
}

//___________Modo ADMIN________________________
int admin_menu()
{	ITEM **my_items;
	int c;				
	MENU *my_menu;
    WINDOW *my_menu_win;
    int n_choices, i;
	
    
	/* Initialize curses */
	init_pair(1, COLOR_RED, COLOR_BLACK);

	/* Create items */
        n_choices = ARRAY_SIZE(admin_menu_choices);
        my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i)
                my_items[i] = new_item(admin_menu_choices[i], " ");

	/* Crate menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Create the window to be associated with the menu */
        my_menu_win = newwin(10, 40, 4, 20);
        keypad(my_menu_win, TRUE);
     
	/* Set main window and sub window */
        set_menu_win(my_menu, my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));

	/* Set menu mark to the string " * " */
        set_menu_mark(my_menu, " * ");

	/* Print a border around the main window and print a title */
        box(my_menu_win, 0, 0);
	print_in_middle(my_menu_win, 1, 0, 40, "Modulos Admin", COLOR_PAIR(1));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
	mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
	mvprintw(LINES - 2, 0, "F1 to exit");
    attron(COLOR_PAIR(1));
    mvprintw(1, 35, "-------ADMIN-------");
    attroff(COLOR_PAIR(1));
	refresh();
        

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	int option = -1;
	while((c = wgetch(my_menu_win)) != KEY_F(1)){
		switch(c){	
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
        	case 10:
            	mvprintw(LINES-2, 3, "selected: ");
				option = item_index(current_item(my_menu));
            	break;
		}
        wrefresh(my_menu_win);
		//Si eligio una opcion entonces sale del ciclo del menu
		if (option != -1) {
            break;
        }
	}	

	/* Unpost and free all the memory taken up */
        unpost_menu(my_menu);
        free_menu(my_menu);
		
        for(i = 0; i < n_choices; ++i){
                free_item(my_items[i]);
			}
    delwin(my_menu_win);
	clear();
    refresh();
	return option;
}

int menu_administrar_usuarios(Usuario_t *usuarios, int count, int *selected_id){

    int i;
    char choices[50][100];

    init_pair(1, COLOR_RED, COLOR_BLACK);

    for(i = 0; i < count; i++){
        snprintf(choices[i], sizeof(choices[i]), "%d - %s", usuarios[i].id, usuarios[i].username);
    }

    ITEM **my_items;
    int c;
    MENU *my_menu;
    WINDOW *my_menu_win;
    int n_choices;

    n_choices = count + 1;
    my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
    for(i = 0; i < count; ++i){
        my_items[i] = new_item(choices[i], "");
    }
    my_items[count] = NULL;

    my_menu = new_menu((ITEM **)my_items);

    my_menu_win = newwin(20, 40, 4, 5);
    keypad(my_menu_win, TRUE);
    set_menu_win(my_menu, my_menu_win);
    set_menu_sub(my_menu, derwin(my_menu_win, 14, 38, 3, 1));
    set_menu_format(my_menu, 12, 1);
    set_menu_mark(my_menu, " * ");

    box(my_menu_win, 0, 0);
    print_in_middle(my_menu_win, 1, 0, 40, "---Usuarios---", COLOR_PAIR(1));
    mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
    mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
    mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
    mvprintw(LINES - 2, 0, "F1 to exit");
    attron(COLOR_PAIR(1));
    mvprintw(1, 20, "Habit FLOW");
    attroff(COLOR_PAIR(1));

    menu_opts_off(my_menu, O_ONEVALUE);
    refresh();
    post_menu(my_menu);
    wrefresh(my_menu_win);

    int option = -1;
    while((c = wgetch(my_menu_win)) != KEY_F(1)){

        
        
        switch(c){
            case KEY_DOWN:
                menu_driver(my_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(my_menu, REQ_UP_ITEM);
                break;
            case KEY_NPAGE:
                menu_driver(my_menu, REQ_SCR_DPAGE);
                break;
            case KEY_PPAGE:
                menu_driver(my_menu, REQ_SCR_UPAGE);
                break;
            case 10: {
                if(selected_id != NULL){
                    int idx = item_index(current_item(my_menu));
                    *selected_id = usuarios[idx].id;
                }
                option = 1;
                break;
            }
        }
        ITEM *cur = current_item(my_menu);
        int idx = item_index(cur);
        form_show_user(usuarios[idx], 4, 45);
        refresh();
        wrefresh(my_menu_win);
        wrefresh(my_menu_win);
        if(option != -1) break;
    }

    unpost_menu(my_menu);
    free_menu(my_menu);
    for(i = 0; i < count; ++i)
        free_item(my_items[i]);
    clear();
    refresh();
    return option;
}

int menu_administrar_habitos(shm_privada *shm_p, Habito *habitos, int count, int *selected_id){

    int i;
    char choices[50][100];

    init_pair(1, COLOR_RED, COLOR_BLACK);

    for(i = 0; i < count; i++){
        snprintf(choices[i], sizeof(choices[i]), "%d - %s", habitos[i].id, habitos[i].nombre);
    }

    ITEM **my_items;
    int c;
    MENU *my_menu;
    WINDOW *my_menu_win;
    int n_choices;

    n_choices = count + 1;
    my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
    for(i = 0; i < count; ++i){
        my_items[i] = new_item(choices[i], "");
    }
    my_items[count] = NULL;

    my_menu = new_menu((ITEM **)my_items);

    my_menu_win = newwin(18, 60, 2, 5);
    keypad(my_menu_win, TRUE);

    set_menu_win(my_menu, my_menu_win);

    set_menu_sub(my_menu,derwin(my_menu_win, 14, 56, 3, 2));

    /* 12 filas visibles, 1 columna */
    set_menu_format(my_menu, 14, 1);

    set_menu_mark(my_menu, " * ");

    box(my_menu_win, 0, 0);
    print_in_middle(my_menu_win, 1, 0, 40, "---Administrar Habitos---", COLOR_PAIR(1));
    mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
    mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
    mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
    mvprintw(LINES - 3, 0, "[N] Agregar \t[L] Reporte");
    mvprintw(LINES - 2, 0, "F1 to exit | PgUp/PgDn para desplazarse");
    attron(COLOR_PAIR(1));
    mvprintw(1, 20, "Habit FLOW");
    attroff(COLOR_PAIR(1));

    menu_opts_off(my_menu, O_ONEVALUE);
    refresh();
    post_menu(my_menu);
    wrefresh(my_menu_win);

    int option = -1;
    while((c = wgetch(my_menu_win)) != KEY_F(1)){
        switch(c){
            case KEY_DOWN:
                menu_driver(my_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(my_menu, REQ_UP_ITEM);
                break;
            case KEY_NPAGE:
                menu_driver(my_menu, REQ_SCR_DPAGE);
                break;
            case KEY_PPAGE:
                menu_driver(my_menu, REQ_SCR_UPAGE);
                break;
            case 10: {
                if(selected_id != NULL){
                    int idx = item_index(current_item(my_menu));
                    *selected_id = habitos[idx].id;
                }
                option = 1;
                break;
            }
            //si presiona n, regresamos 0, osea agregar nuevo
            case 'n':
            case 'N': {
            
                form_register_habit(shm_p, my_menu_win);

                touchwin(my_menu_win);
                wrefresh(my_menu_win);
                break;
            }
        }
        wrefresh(my_menu_win);
        if(option != -1) break;
    }

    unpost_menu(my_menu);
    free_menu(my_menu);
    for(i = 0; i < count; ++i)
        free_item(my_items[i]);
    clear();
    refresh();
    return option;
}

int form_register_habit(shm_privada *shm_p, WINDOW *root)
{
    FIELD *field[2];
    FORM *my_form;
    WINDOW *win;
    WINDOW *sub;

    int ch;
    int rows, cols;
    int respuesta_form = -1;

    init_pair(1, COLOR_CYAN,  COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_RED,   COLOR_BLACK);
    init_pair(5, COLOR_GREEN, COLOR_BLACK);

    /*------------------*/
    /* Crear formulario */
    /*------------------*/

    field[0] = new_field(1, 25, 0, 0, 0, 0);
    field[1] = NULL;

    set_field_back(field[0], COLOR_PAIR(2));
    set_field_fore(field[0], COLOR_PAIR(3));
    field_opts_off(field[0], O_AUTOSKIP);

    my_form = new_form(field);

    scale_form(my_form, &rows, &cols);

    /*------------------*/
    /* Crear popup      */
    /*------------------*/

    int h = 9;
    int w = 45;

    int y = getbegy(root) + (getmaxy(root) - h) / 2;
    int x = getbegx(root) + (getmaxx(root) - w) / 2;

    touchwin(root);
    wrefresh(root);

    win = newwin(h, w, y, x);
    keypad(win, TRUE);

    box(win, 0, 0);

    wattron(win, COLOR_PAIR(1));
    mvwprintw(win, 1, (w - 13) / 2, "Nuevo Habito");
    wattroff(win, COLOR_PAIR(1));

    mvwprintw(win, 3, 2, "Nombre:");
    mvwprintw(win, 6, 2, "Enter: Guardar   Esc: Cancelar");

    sub = derwin(win, rows, cols, 3, 11);

    set_form_win(my_form, win);
    set_form_sub(my_form, sub);

    post_form(my_form);

    set_current_field(my_form, field[0]);
    pos_form_cursor(my_form);

    wrefresh(win);

    /*------------------*/
    /* Loop principal   */
    /*------------------*/

    while ((ch = wgetch(win)))
    {
        switch (ch)
        {
            case 27:      /* ESC */
                respuesta_form = -1;
                goto cleanup;

            case KEY_BACKSPACE:
            case 127:
            case 8:
                form_driver(my_form, REQ_DEL_PREV);
                break;

            case KEY_LEFT:
                form_driver(my_form, REQ_PREV_CHAR);
                break;

            case KEY_RIGHT:
                form_driver(my_form, REQ_NEXT_CHAR);
                break;

            case KEY_DC:
                form_driver(my_form, REQ_DEL_CHAR);
                break;

            case KEY_HOME:
                form_driver(my_form, REQ_BEG_LINE);
                break;

            case KEY_END:
                form_driver(my_form, REQ_END_LINE);
                break;

            case 10:      /* ENTER */
            {
                form_driver(my_form, REQ_VALIDATION);

                char *nombre = trim(field_buffer(field[0], 0));

                /* Limpiar únicamente la línea de mensajes */
                mvwhline(win, 7, 2, ' ', w - 4);

                if (nombre == NULL || strlen(nombre) == 0)
                {
                    wattron(win, COLOR_PAIR(4));
                    mvwprintw(win, 7, 2,
                              "Nombre no puede estar vacio");
                    wattroff(win, COLOR_PAIR(4));

                    wrefresh(win);
                    break;
                }

                int status = api_register_habit(shm_p, nombre);

                if (status == 0)
                {
                    wattron(win, COLOR_PAIR(5));
                    mvwprintw(win, 7, 2,
                              "Habito registrado correctamente");
                    wattroff(win, COLOR_PAIR(5));

                    respuesta_form = 1;

                    wrefresh(win);
                    wgetch(win);

                    goto cleanup;
                }
                else if (status == 1)
                {
                    wattron(win, COLOR_PAIR(4));
                    mvwprintw(win, 7, 2,
                              "El habito ya existe");
                    wattroff(win, COLOR_PAIR(4));
                }
                else
                {
                    wattron(win, COLOR_PAIR(4));
                    mvwprintw(win, 7, 2,
                              "Error al registrar habito");
                    wattroff(win, COLOR_PAIR(4));
                }

                wrefresh(win);
                break;
            }

            default:
                form_driver(my_form, ch);
                break;
        }

        pos_form_cursor(my_form);
        wrefresh(win);
    }

cleanup:

    unpost_form(my_form);

    free_form(my_form);

    free_field(field[0]);

    delwin(sub);

    werase(win);
    wrefresh(win);
    delwin(win);

    /* Redibujar completamente la ventana padre */
    touchwin(root);
    redrawwin(root);
    wrefresh(root);

    return respuesta_form;
}


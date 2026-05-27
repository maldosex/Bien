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


//------------MENUS____________________
char *hp_menu_choices[] = {
                        "Habit Store",
                        "My Habits",
                        "My progress",
                        "Settings",
                        (char *)NULL,
                  };
char *login_menu_choices[] = {
                        "Log In",
                        "Register",
                        "Exit",
                        (char *)NULL,
                  };


int log_menu()
{	ITEM **my_items;
	int c;				
	MENU *my_menu;
    WINDOW *my_menu_win;
    int n_choices, i;
	
	init_pair(1, COLOR_RED, COLOR_BLACK);

	/* Create items */
        n_choices = ARRAY_SIZE(login_menu_choices);
        my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i)
                my_items[i] = new_item(login_menu_choices[i], " ");

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
	print_in_middle(my_menu_win, 1, 0, 40, "Menu", COLOR_PAIR(1));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
	mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
	mvprintw(LINES - 2, 0, "F1 to exit");
    attron(COLOR_PAIR(1));
    attron(A_BLINK );
    mvprintw(1, 20, "Habit FLOW");
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
	
	/* Initialize curses */
	init_pair(1, COLOR_RED, COLOR_BLACK);

	/* Create items */
        n_choices = ARRAY_SIZE(hp_menu_choices);
        my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i)
                my_items[i] = new_item(hp_menu_choices[i], " ");

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
	print_in_middle(my_menu_win, 1, 0, 40, "Habit Flow", COLOR_PAIR(1));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
	mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
	mvprintw(LINES - 2, 0, "F1 to exit");
    attron(COLOR_PAIR(1));
    attron(A_BLINK );
    mvprintw(1, 20, "Habit FLOW");
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

int menu_my_habits(Habito *habitos, int count, int * ids, int *selected_count){

    int i;
    char choices[30][100];

    init_pair(1, COLOR_RED, COLOR_BLACK);

    for(i = 0; i < count; i++){

        snprintf(choices[i], sizeof(choices[i]), "%d - %s", habitos[i].id,  habitos[i].nombre);
    }


    ITEM **my_items;
	int c;				
	MENU *my_menu;
    WINDOW *my_menu_win;
    int n_choices;
	
	init_pair(1, COLOR_RED, COLOR_BLACK);

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
        my_menu_win = newwin(10, 40, 4, 20);
        keypad(my_menu_win, TRUE);

        set_menu_win(my_menu, my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));

	/* Set menu mark to the string " * " */
        set_menu_mark(my_menu, " * ");

	/* Print a border around the main window and print a title */
        box(my_menu_win, 0, 0);
	print_in_middle(my_menu_win, 1, 0, 40, "-----Mis habitos-----", COLOR_PAIR(1));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
	mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
	mvprintw(LINES - 2, 0, "F1 to exit");
    attron(COLOR_PAIR(1));
    mvprintw(1, 20, "Habit FLOW");
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
		}
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
//---------------------_____________________________


//-----------FORMULARIOS-------------------------

int form_login(shm_privada *shm_p, char * usuario, char * contra){
    FIELD *field[3];
    FORM  *my_form;
    WINDOW *my_form_win;
    int ch, rows, cols;


    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
	init_pair(3, COLOR_RED, COLOR_BLACK);

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
                strcpy(usuario_login.username, trim(field_buffer(field[0], 0)));
                strcpy(    usuario_login.contra,    trim(field_buffer(field[1], 0)));
                //Se mantendra en en ciclo del formulario hasta que logra hacer login o decida salir
                int status = api_login(shm_p,usuario_login,msg); 
                if(status == 1) {
					mvwprintw(my_form_win, 8, 2, "Usuario o contrasena incorrectos");
                    wrefresh(my_form_win);
                }
                if(status == 0){
                    respuesta_form = 1;
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
    // Devolvemos 1 si quiere loguearse, 0 si canceló con F1
}


int form_register(shm_privada * shm_p){
	FIELD *field[6];
    FORM  *my_form;
    WINDOW *my_form_win;
    int ch, rows, cols;
    Usuario_t usuario;


	init_pair(1, COLOR_CYAN, COLOR_BLACK);    // titulo
	init_pair(2, COLOR_BLACK, COLOR_WHITE);   // campo activo
	init_pair(3, COLOR_WHITE, COLOR_BLACK);   // texto
	init_pair(4, COLOR_RED, COLOR_BLACK);     // borde

    // Definición de campos
    field[0] = new_field(1, 20, 2, 14, 0, 0); // Usuario
    field[1] = new_field(1, 20, 5, 14, 0, 0); // Contraseña
    field[2] = new_field(1, 20, 8, 14, 0, 0); // Contraseña
    field[3] = new_field(1, 20, 11, 14, 0, 0);
    field[4] = new_field(1, 20, 14, 14, 0, 0);
    field[5] = NULL;

	for(int i = 0; i<5; i++){
		set_field_back(field[i], COLOR_PAIR(2));
		set_field_fore(field[i], COLOR_PAIR(3));
		field_opts_off(field[i], O_AUTOSKIP);
	}
    
    // Ocultar caracteres en el campo de contraseña
    field_opts_off(field[1], O_PUBLIC); 

    my_form = new_form(field);
    scale_form(my_form, &rows, &cols);

    // Crear ventana centrada
    my_form_win = newwin(20, 50, (LINES-20)/2, (COLS-50)/2);
    keypad(my_form_win, TRUE);

	wattron(my_form_win, COLOR_PAIR(3));
    box(my_form_win, 0, 0);
	wattroff(my_form_win, COLOR_PAIR(3));

    set_form_win(my_form, my_form_win);
    set_form_sub(my_form, derwin(my_form_win, rows, cols, 2, 2));

    print_in_middle(my_form_win, 1, 0, 50, "Register", COLOR_PAIR(1));
    
    post_form(my_form);

    
    // Etiquetas de texto
    mvwprintw(my_form_win, 4, 4, "Usuario:");
    mvwprintw(my_form_win, 7, 4, "Contrasea:");
    mvwprintw(my_form_win, 10, 4, "Nombre:");
    mvwprintw(my_form_win, 13, 4, "Apellido:");
    mvwprintw(my_form_win, 16, 4, "Correo:");

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
            case 10: // Enter para aceptar
                form_driver(my_form, REQ_VALIDATION);
				if(strcmp(trim(field_buffer(field[0], 0)), "") == 0){
					mvwprintw(my_form_win, 18, 4, "No deje espacios vacios");
					break;
				}

                strcpy(usuario.username, trim(field_buffer(field[0], 0)));
				strcpy(usuario.contra, trim(field_buffer(field[1], 0)));
                /*
				strcpy(usuario.nombre, trim(field_buffer(field[2], 0)));
				strcpy(data.apellido, trim(field_buffer(field[3], 0)));
				strcpy(data.correo, trim(field_buffer(field[4], 0)));

                */

                int status = api_register(shm_p, usuario, msg);
                //Si se logro el registro:
                if(status == 0) {
                    respuesta_form = 1;
					mvwprintw(my_form_win, 18, 4, "Usuario Registrado, ahora haga login");
                    wrefresh(my_form_win);
                    getch(); /* Pausa para que el usuario lea el mensaje */
                    goto fin;
                }
                else{
                    mvwprintw(my_form_win,  18, 3, "%s", msg);
                    wrefresh(my_form_win);
                    /* No salimos: el usuario puede corregir y volver a intentar */
                }

                break;
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


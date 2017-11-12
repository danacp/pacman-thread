#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <ncurses.h>
#include <unistd.h>
// creando un tread
// void *work( void *ptr )
// {
// 	printf("Trabajo que realiza este thread \n");
// 	pthread_exit(0);
// }

//-------------- Lo basico de ncurses-------//
 // initscr();
 // noecho();
 // curs_set(FALSE);

 // mvprintw(0, 0, "Hello, world!");
 // refresh();

 // sleep(1);

 // endwin();
//----------------------------------------------


int alto, ancho;
char *nombreArchivo;

char MAP[99][99];

struct coord {
  int x;
  int y;
  char pos; //posicion para checkear si fantasma esta en ' ' o '.'
}pacman, fantasma[4];


void IniciarVentana(){
    initscr();
    noecho();
    curs_set(FALSE);
}

void leer_mapa ()
{
    FILE* in;
    int i,j;
    char prueba= 'X';
    char c;
    if (  (in = fopen( nombreArchivo, "rt" )) == NULL )
    {
        perror ("Ocurrio el siguiente error");
        exit (-1);
    }

    for ( i = 0; i < alto; i++){
      for ( j = 0; j < ancho; j++){
          fscanf ( in, "%c", &c );
          MAP[i][j] = c;
          
          if(c == 'P'){
            pacman.x = i;
            pacman.y = j;
          }else if( c == '1'){
            fantasma[0].x = i;
            fantasma[0].y = j;
          }else if( c == '2'){
            fantasma[1].x = i;
            fantasma[1].y = j;
          }else if( c == '3'){
            fantasma[2].x = i;
            fantasma[2].y = j;
          }else if( c == '4'){
            fantasma[3].x = i;
            fantasma[3].y = j;
          }     

      }
    }
    fclose (in);
    printf("\n");
}

void mostrarMapa ()
{
    int i,j;

    for ( i = 0; i < alto; i++)
    {
        for ( j = 0; j < ancho; j++){
	         mvaddch(i,j,MAP[i][j]); //imprime en una posicion de la pantalla
            
            refresh();

        }

    }
   printw ("\n") ;

}



int sePuedeMoverPacman(char dir){
  int boolean = 0;

  if ( dir == 'w'){   // arriba
    if( MAP[pacman.x-1][pacman.y] == ' ' || MAP[pacman.x-1][pacman.y] == '.'){
      boolean = 1;
    }
	if (MAP[pacman.x-1][pacman.y] == '1' || MAP[pacman.x-1][pacman.y] == '2' || MAP[pacman.x-1][pacman.y] == '3' || MAP[pacman.x-1][pacman.y] == '4' ){
		printf("¡Ha sido atrapado por un fantasma!");
		
		
	}
  }
  else if ( dir == 's'){   // abajo
    if( MAP[pacman.x+1][pacman.y] == ' ' || MAP[pacman.x+1][pacman.y] == '.'){
      boolean = 1;
    }
	if (MAP[pacman.x+1][pacman.y] == '1' || MAP[pacman.x+1][pacman.y] == '2' || MAP[pacman.x+1][pacman.y] == '3' || MAP[pacman.x+1][pacman.y] == '4' ){
		printf("¡Ha sido atrapado por un fantasma!");
		
	}
  }
  else if ( dir == 'a'){   // izquierda
    if( MAP[pacman.x][pacman.y-1] == ' ' || MAP[pacman.x][pacman.y-1] == '.'){
      boolean = 1;
    }
	if (MAP[pacman.x][pacman.y-1] == '1' || MAP[pacman.x][pacman.y-1] == '2' || MAP[pacman.x][pacman.y-1] == '3' || MAP[pacman.x][pacman.y-1] == '4' ){
		printf("¡Ha sido atrapado por un fantasma!");
		
	}
  }
  else if ( dir == 'd'){   // derecha
    if( MAP[pacman.x][pacman.y+1] == ' ' || MAP[pacman.x][pacman.y+1] == '.'){
      boolean = 1;
    }
	if (MAP[pacman.x][pacman.y+1] == '1' || MAP[pacman.x][pacman.y+1] == '2' || MAP[pacman.x][pacman.y+1] == '3' || MAP[pacman.x][pacman.y+1] == '4' ){
		printf("¡Ha sido atrapado por un fantasma!");
		
	}
  }

  return boolean;

}

void moverPacman(char dir){

  
  if(dir == 'w'){  // arriba

    if(sePuedeMoverPacman(dir) == 1){

      MAP[pacman.x-1][pacman.y] = 'P';
      MAP[pacman.x][pacman.y] = ' ';
      pacman.x--;

    }

  }
  else if ( dir == 's'){ // abajo

    if(sePuedeMoverPacman(dir) == 1){
      MAP[pacman.x+1][pacman.y] = 'P';
      MAP[pacman.x][pacman.y] = ' ';
      pacman.x++;
    }

  }
  else if ( dir == 'a'){  // izquierda

    if(sePuedeMoverPacman(dir) == 1){
      MAP[pacman.x][pacman.y-1] = 'P';
      MAP[pacman.x][pacman.y] = ' ';
      pacman.y--;
    }
  }
  else if ( dir == 'd'){ // derecha

    if(sePuedeMoverPacman(dir) == 1){
      MAP[pacman.x][pacman.y+1] = 'P';
      MAP[pacman.x][pacman.y] = ' ';
      pacman.y++;
    }
  }


}

void *capturar(){
  char dir;

  while(true){
    
    scanf("%c", &dir);
  
    moverPacman(dir);

    mostrarMapa();

    refresh();
  }
}

void *roamFantasma(void* n){
	int m= *((int*)n);
	fantasma[m].pos=' '; //inicializa fantasma parado en ' '
	char ghost;
	switch (m){
	case 0 :
		ghost= '1'; break; 	
	case 1 :
		ghost= '2'; break;
	case 2 :
		ghost= '3'; break;
	case 3 :
		ghost= '4'; break;
	}
	while (true){ //secuencia de movimiento up->left->down->right
		sleep(3);	
	
		if( MAP[fantasma[m].x-1][fantasma[m].y] == ' '){ //up
			MAP[fantasma[m].x-1][fantasma[m].y] = ghost; 
			MAP[fantasma[m].x][fantasma[m].y] = fantasma[m].pos ; 	
			fantasma[m].pos= ' '; 	//nueva pos del fantsma	
     			fantasma[m].x--;
		}else if ( MAP[fantasma[m].x-1][fantasma[m].y] == '.'){ //up
			MAP[fantasma[m].x-1][fantasma[m].y] = ghost;
			MAP[fantasma[m].x][fantasma[m].y] = fantasma[m].pos ;		
			fantasma[m].pos= '.'; 		
     			fantasma[m].x--;		
		}else if( MAP[fantasma[m].x][fantasma[m].y-1] == ' '){ //left
			MAP[fantasma[m].x][fantasma[m].y-1] = ghost;
			MAP[fantasma[m].x][fantasma[m].y] = fantasma[m].pos ;		
			fantasma[m].pos= ' '; 		
     			fantasma[m].y--;
		}else if ( MAP[fantasma[m].x][fantasma[m].y-1] == '.'){ //left
			MAP[fantasma[m].x][fantasma[m].y-1] = ghost;
			MAP[fantasma[m].x][fantasma[m].y] = fantasma[m].pos ;		
			fantasma[m].pos= '.'; 		
     			fantasma[m].y--;		
		}else if( MAP[fantasma[m].x+1][fantasma[m].y] == ' '){ //down
			MAP[fantasma[m].x+1][fantasma[m].y] = ghost;
			MAP[fantasma[m].x][fantasma[m].y] = fantasma[m].pos ;		
			fantasma[m].pos= ' '; 		
     			fantasma[m].x++;
		}else if ( MAP[fantasma[m].x+1][fantasma[m].y] == '.'){ //down
			MAP[fantasma[m].x+1][fantasma[m].y] = ghost;
			MAP[fantasma[m].x][fantasma[m].y] = fantasma[m].pos ;		
			fantasma[m].pos= '.'; 		
     			fantasma[m].x++;		
		}else if( MAP[fantasma[m].x][fantasma[m].y+1] == ' '){ //right
			MAP[fantasma[m].x][fantasma[m].y+1] = ghost;
			MAP[fantasma[m].x][fantasma[m].y] = fantasma[m].pos ;		
			fantasma[m].pos= ' '; 		
     			fantasma[m].y++;
		}else if ( MAP[fantasma[m].x][fantasma[m].y+1] == '.'){ //right
			MAP[fantasma[m].x][fantasma[m].y+1] = ghost;
			MAP[fantasma[m].x][fantasma[m].y] = fantasma[m].pos ;		
			fantasma[m].pos= '.'; 		
     			fantasma[m].y++;		
		}
		
	}
}

int main(int argc, char *argv[])
{
  if( argc == 4){
	int index[4];
  	char *c;
	int i;
  	c = (char *) calloc(1, sizeof(char));

  	alto = atoi(argv[1]);
  	ancho = atoi(argv[2]);
  	nombreArchivo = argv[3];

    leer_mapa();

    IniciarVentana();

    mostrarMapa();
	
	pthread_t thread_pacman;
	pthread_t thread_fantasma[4];
	pthread_create(&thread_pacman, NULL, &capturar, NULL);	

	for(i=0;i<4;i++) {
		index[i]=i;
		pthread_create(&thread_fantasma[i], NULL, &roamFantasma, (void*)(index + i));
	}
	for(i=0;i<4 ;i++){
		pthread_join(thread_fantasma[i],NULL);
	}
	pthread_join(thread_pacman,NULL);
    
	endwin();


    printf("\n");

  }
  else{
    printf("Error en los argumentos\n");
  }

  
	return 0;
}

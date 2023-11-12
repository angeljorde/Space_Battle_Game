#include<stdio.h>
#include<windows.h>
#include<conio.h> //La utilizaremos para poder mover el objeto
#include<stdlib.h> //La utilizaremos para poder utilizar la funcion rand y cls.
#include<list> //Nos permite hacer listas
#include<iostream> //Nos permite hacer la entrada de datos

using namespace std;

//Esto es para poder mover el objeto con las flechas, cada flecha tiene un numero asociado, ahoora podremos llamar a la flecha por su nombre y no por su numero
#define ARRIBA 72
#define	IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80
#define ESPACIO 32
#define ENTER 13

void gotoxy (int x, int y){ //Con  esta función lo que hago es posicionar el objeto en la pantal
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	
	SetConsoleCursorPosition(hCon, dwPos);
}

void OcultarCursor(){ //La utilizo paa poder ocultar el curs
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 2; //Sirve para controlar el  tamaño del cursor
	cci.bVisible = FALSE; //Sirve para controlar la visibilidad del cursor. Si es FALSE no se mostrará el cursor y si es TRUE sí. Se tiene que poner en mayúsculas
	
	SetConsoleCursorInfo(hCon, &cci);
}

void pintar_limites(){
	//Lineas horizontales
	for(int i = 2; i<78; i++){
		gotoxy (i, 3);
		printf ("%c",205);
		gotoxy (i, 33);
		printf ("%c",205);
	}
	
	//Lineas verticales
	for(int i = 4; i<33; i++){
		gotoxy (2, i);
		printf ("%c",186);
		gotoxy (77, i);
		printf ("%c",186);
	}
	
	//Esquinas
	gotoxy(2,3);
	printf ("%c",201);
	gotoxy(2,33);
	printf ("%c",200);
	gotoxy(77,3);
	printf ("%c",187);
	gotoxy(77,33);
	printf ("%c",188);
}


class NAVE{
	int x,y; //Estos datos son privados, es decir, no podemos acceder a ellos fuera de la estructura, por ello los puedo llamar igual que otras variables
	int corazones;
	
	//Pero podemos hacer una parte publica:
	public:
		int vidas;
		//Todas las clases tienen que tener un constructor, lo defino
		NAVE(int _x,int _y, int _corazones, int _vidas): x(_x), y(_y), corazones(_corazones), vidas(_vidas){}  //El constructor se tiene que llamar igual que la clase. Cada variable será igual a su parametro correspondiente
		
		int X(){ return x; }
		int Y(){ return y; }
		int VID(){return vidas;}
		//Creo las siguientes funciones que seran necesarias para el objeto y son publicas
		void COR(){corazones--;}
		void pintar();
		void borrar();
		void mover();
		void pintar_corazones();
		void morir();
};

//Defino las funciones de NAVE
void NAVE::pintar(){
	gotoxy (x,y); 
	printf ("  %c",65);//Primera linea del avion
	gotoxy (x, y+1);
	printf (" %c%c%c",47,177,92);//Segunda linea. Pongo 3 veces %c porque cada uno es para un simbolo del codigo ASCC
	gotoxy (x, y+2);
	printf ("%c%c %c%c",47,177,177,92);//Tercera linea
	gotoxy (x, y+3);
	printf ("%c%c%c%c%c",201,205,203,205,187);//Cuarta linea
	
}

void NAVE::borrar(){
	gotoxy(x,y);   printf ("      "); //Dejo en blanco 6 huecos para que los borre, 6 porque son 6 caracteres como maximo por linea
	gotoxy(x,y+1); printf ("      ");
	gotoxy(x,y+2); printf ("      ");
	gotoxy(x,y+3); printf ("      ");
}

void NAVE::mover(){
	
	if(kbhit()){//La función kbhit va con la biblioteca conio.h y detecta si se ha presionado alguna tecla
			char tecla = getch();//La funcion getch va con la biblioteca conio.h y lo que hace es guardar la tecla que se presiona
			borrar();
			
			//Aunque tecla sea una variable de tipo char al definir la palabra al numero podriamos poner el numero en vez de la palabra y funcionaria igual
			if(tecla == IZQUIERDA && x > 3) x--; //Pongo la segunda condicion para fijar un limite
			if(tecla == DERECHA && x+6 < 77) x++; //Pongo x+6 porque mi nave tiene 5 caracteres y el primero empieza por la izquierda
			if(tecla == ARRIBA && y > 4) y--;
			if(tecla == ABAJO && y+4 < 33) y++;
						
			pintar();
			pintar_corazones();
		}
}

void NAVE::pintar_corazones(){
	
	gotoxy(50,2);
	printf("Vidas: %d", vidas);
	gotoxy(64,2);
	printf("Salud: ");
	gotoxy(70,2);
	printf("       ");
	
	for (int i = 0; i < corazones; i++){
		gotoxy(70 +i, 2);
		printf ("%c", 3);
	}
}

void NAVE::morir(){
	if (corazones == 0){
		borrar();
		gotoxy(x,y);
		printf("  **   ");
		gotoxy(x,y+1);
		printf(" ****  ");
		gotoxy(x,y+2);
		printf("  **   ");
		Sleep(200);
		
		borrar();
		gotoxy(x,y);
		printf("* ** *");
		gotoxy(x,y+1);
		printf(" **** ");
		gotoxy(x,y+2);
		printf("* ** *");
		Sleep(200);
		
		borrar();
		vidas--;
		corazones = 3;
		pintar_corazones();
		pintar();
	}
}

//Aquí creo la clase que define a los asteroides

class AST{
	int x, y;
	public: 
	AST(int _x, int _y): x(_x), y(_y){}
	void pintar();
	void mover();
	void choque(class NAVE &N);
	int X(){return x;};
	int Y(){return y;}
	
};

void AST::pintar(){
	gotoxy (x,y);
	printf ("%c", 254);
}

void AST::mover(){
	gotoxy(x,y); 
	printf(" ");

	y++;
	if (y > 32){
		x = rand()%71 + 4;	//Esta función nos da un numero al azar entre 0 y 71, pero le sumo 4 porque la x tiene que estar entre 4 y 75, no ocupa toda la pantalla
		y = 4;
	}
	pintar(); //Esta funcion pintar es el de la clase AST no el de NAVE.
}

void AST::choque(class NAVE &N){
	if (x >= N.X() && x < N.X() + 6 && y >= N.Y() && y <= N.Y() + 2) {//Esto será cuando choquen nave y asteroide. La primera x es la del asteroide porque estoy en la funcion de la clase asteroide. N.X es de la clase nave
		N.COR();
		N.borrar();
		N.pintar();
		N.pintar_corazones();
		x = rand()%71 + 4;	
		y = 4;
	}
}

//Creamos otra clase para la bala

class BALA{
	int x,y;
	public: 
	BALA (int _x, int _y): x(_x),y (_y){}
	int X(){return x;}
	int Y(){return y;}
	
	void mover();
	bool fuera();
};

void BALA::mover(){
	gotoxy(x,y);
	printf(" ");
	y--;
	
	gotoxy(x,y);
	printf("%c", 94);
}

bool BALA::fuera(){
	if (y == 4)	return true;
	return false;
}

class PRES{
	int x,y;
	public:
		PRES(int _x, int _y): x(_x),y(_y){}
		int max;
		int Y(){return y;}
		int X(){return x;}
		void opciones();
		void elegir();
};

void PRES::opciones(){
	gotoxy(5,0);
	printf("Jugar");
	gotoxy(5,2);
	printf("Color de fondo");
	gotoxy(5,4);
	printf("Como jugar");
	gotoxy(5,6);
	printf("Acerca de");
	gotoxy(5,8);
	printf("Fin");
	gotoxy(2,0);
	printf("%c", 26);
}

void PRES::elegir(){
	bool opcion = false;
	x = 2;
	y = 0;

	while(!opcion){
		if(kbhit()){
			char tecla = getch();
			if(tecla == ARRIBA && y!=0){
				gotoxy(2,Y());
				printf(" ");
				y = y - 2;
				gotoxy(2,Y());
				printf("%c", 26);
			}
			if (tecla == ABAJO && y !=max){
				gotoxy(2,Y());
				printf(" ");
				y = y + 2;
				gotoxy(2,Y());
				printf("%c", 26);
			}
			if (tecla == ENTER){
				opcion = true;
			}
			
	gotoxy(0,30);
		}		
	}
	
}
int main (){
	int puntos;
	int cont;
	int cont2;
	system("cls");
	system ("color f0");
	
	//Necesitamos crear una lista para la bala
	list<BALA*> B;
	list<BALA*>::iterator it;//Creo el puntero it. Un puntero lo que guarda es una... 
	//Creo una lista de asteroides
	list<AST*> A;
	list<AST*>::iterator itA;
	//Vamos a crear objetos de la clase nave.
	NAVE N(37,29,3,3); //Los dos primeros valores hacen referencia a las coordenadas y el tercero al numero de corazones. N es el objeto de NAVE
	
	bool repetir = false;
	bool empezar = false;
	bool game_over = false;
	bool volverint = false;
	
	while (!empezar){
	cont = 0;
	repetir = false;
    empezar = false;
	game_over = false;
    volverint = false;
	system("cls");
	
	OcultarCursor();
	PRES P(0,0);
	P.opciones();
	P.max = 8;
	P.elegir();
	
		switch(P.Y()){
			case 0:
			
			A.clear();//Elimino todos  los elementos de la lista asteroides
			B.clear();//Elimino todos  los elementos de la lista bala
			
			N.vidas = 3;
			system("cls");
			
			pintar_limites();
			
			N.pintar();//Podemos llamar estos metodos en la funcion main porque esta en la parte de la clase que es publica
			//Por ejemplo no podria hacer N.x++ porque el atributo x esta dentro de las declaraciones privadas. No puedo cambiarlo fuera de los metodos de la clase
			N.pintar_corazones();
			
			puntos = 0;
			cont = 0;
			cont2 = 0;//Este contador será el numero exacto de asteoides
			while(!game_over){ //Se ejecutara mientras no sea verdadero
				cont ++;
				gotoxy(4,2);
				printf("Puntos %d", puntos);
				
				if (cont == 40 && cont2 <= 6){
					A.push_back(new AST(rand()%75 + 3, rand()%5 + 4));	//Sirve para  generar asteoides
					cont = 0;
					cont2++;
				}
				
				if(kbhit()){
					char teclass = getch();
					if (teclass == ESPACIO)//Si pulso la tecla espacio me va a generar una bala nueva
						B.push_back(new BALA(N.X()+2, N.Y() - 1 ));	
				}
		
				for(it = B.begin(); it != B.end(); it++){//Establcemos el it al primer elemento de la lista. 
					(*it) ->mover();
					if((*it) ->fuera()){//Si la bala llega al límite
						gotoxy((*it) ->X(), (*it) ->Y()); 
						printf (" ");
						//Para eliminar un elemennto de la lista:
						delete(*it);//Quito el iterador
						it = B.erase(it);//Creo uno nuevo
				
					}
				}
	
				for(itA = A.begin(); itA !=A.end(); itA++){
					(*itA) -> mover();
					(*itA) -> choque(N); 
				}
		
				for(itA = A.begin(); itA != A.end(); itA++){//Recorre los asteroides
					for(it = B.begin(); it != B.end(); it++){//Recorre las balas
						if((*itA) -> X() == (*it) -> X() && ((*itA) -> Y() + 1 ==  (*it) -> Y() || (*itA) -> Y() + 1 ==  (*it) -> Y() )){
							gotoxy((*it) -> X(), (*it) -> Y());
							printf(" ");
							delete(*it);
							it = B.erase(it);
					
							A.push_back(new AST(rand()%74 + 3,  4));
							gotoxy((*itA) -> X(), (*itA) -> Y());
							printf(" ");
							delete(*itA);
							itA = A.erase(itA);
					
							puntos++;
						}
					}
				}
		
				if (N.VID() == 0){
					Sleep(1000);
					
					system("cls");
						
					game_over = true;
					
					gotoxy (25,10);
					printf ("Game Over \n");
					gotoxy (25,12);
					printf ("Total puntos: %d", puntos);
					gotoxy (25,20);
					printf ("Pulse ENTER para volver al menu.");
				}	
			
			N.morir();
			N.mover();
			Sleep(30);
			//Detener el programa 30 milisegundos
			}
				while (!volverint){
					if (kbhit()){
						char teclat = getch();
						if (teclat == ENTER){
							volverint = true;
						}
					}	
				}
			break;
			
			case 2:
				gotoxy(2,2);
				printf(" ");
				
				gotoxy(2,0);
				printf("%c", 26);
				
				gotoxy(5,0);
				printf("Negro             ");
				gotoxy(5,2);
				printf("Blanco            ");
				gotoxy(5,4) ;
				printf("Verde             ");
				gotoxy(5,6);
				printf("Rojo              ");
				gotoxy(5,8);
				printf("Purpura           ");
				gotoxy(5,10);
				printf("Amarillo          ");
				gotoxy(5,12);
				printf("Azul              ");
				P.max = 12;
				P.elegir();
				
				switch(P.Y()){
					case 1:
						system ("color 0F");
						break;
					case 2:
						system ("color 7");
						break;
					case 3:
						system ("color 4D");
						break;
					case 4:
						system ("color 2F");
						break;
					case 5:
						system ("color 5F");
						break;
					case 6:
						system ("color 6F");
						break;
					case 7:
						system ("color 1F");
						break;
				}
				
				
				
				break;
			
			case 4:
				system("cls");
				gotoxy(0,0);
				cout<<"Hola! Bienvenido al emocionante mundo de Space Battle. \nTe explico brevemente como vas a poder jugar.\n";
				cout<<"Apareceras como una nave espacial en la parte inferior de la pantalla.\nEl juego consiste en esquivar los asteroides que iran cayendo.\n";
				cout<<"Tus controles seran los siguientes:\n";
				cout<<"En primer lugar podras utilizar las flechas para moverte hacia todos los lados.\nY en segundo lugar podras utilizar la barra espaciadora para poder disparar misiles contra los asteroides,\npero te lo advierto, tendras que apuntar bien!!\n";
				cout<<"Disfruta! \n";
				cout<<"Presiona ENTER para volver al menu.";
			
				while (!repetir){
					if (kbhit()){
						char teclas = getch();
						if (teclas == ENTER){
							repetir = true;
						}
					}	
				}
					
				break;
				
				case 6:
				system("cls");
				gotoxy(0,2);
				cout<<"Este juego ha sido creado por Angel Jorde entre el 17-04-2020 y el 20-04-2020.";
				while (!repetir){
					if (kbhit()){
						char teclas = getch();
						if (teclas == ENTER){
							repetir = true;
						}
					}	
				}
				break;
				
				case 8:
				empezar = true;
				break;		
		}
	}
	

	return 0;
}
	

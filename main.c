/*
GameJam BCN 2014
 
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"

 
// Constantes que molan
const int ScreenWidth = 1280;
const int ScreenHeight = 720;
const int CameraWidth = 1080;
const int CameraHeight = 720;
int ScreenColumns = 0;
const int ScreenRows = 5;
const int NumDeEnemics = 50;
const int CapVariation = 5;
const int Cap = 200;
int ForwardSpeed = 15; //Cambiar nirvana si aquest valor varía
const Vector2 PosicioCamera = {200, 0};
 
 //TYPEDEFS
typedef enum GameScreen { LOGO, MENU, GAMEPLAY, ENDING } GameScreen;
typedef enum {noEnemy, ZigZag, Straight, ZigZagReal, StraightReal} EnemyTypes;
typedef enum {Arriba, Abajo} Directions;
//Estructuras

typedef struct Player{
        Vector2 position;
		int speed;
		int cardiaca;
		int cardiacaCont;
		int cont;
		bool nirvana;
		bool dead;
		Rectangle recAnimation;
		int frame;
		int column;
		Texture2D sprite;
		int drawCount;
		bool firsttime;
		bool updown;
		int frameSpeed;
		int almas;
		//bool player.auxiliar;
	   
} Player;
 
 
typedef struct Camera2D{
    Rectangle pov;
    Texture2D bg;
} Camera2D;
 
//Struct que determina les dades relacionades amb els enemics
typedef struct Enemy{
	Vector2 position;
	//Temporalment sera un Rectangle en comptes d'un sprite
	Texture2D sprite;
	int frame;
	Rectangle recAnimation;
	bool vivo;
	EnemyTypes tipo;
	bool haXocat;
	//Only for ZIGZAG
	Directions direccio;
	int cap;
	int current_cap;
} Enemy;

 //VARIABLES GLOBALS
int framesCount=0;

GameScreen screen = LOGO;

int **enemy_matrix;
Enemy *enemy_list;
int NumDeEnemicsVius;
int vida[4] = {10, 12, 7, 9};
Player player;
bool eyes;
int Enemy_DrawCount = 0;

int cosas = 0;

bool damage=0;

bool FinalFeliz;



//CAPCELERES DE FUNCIONS
void Enemy_LoadEnemies(char * path);
void Enemy_SpawnEnemies(int columna);
void Enemy_InitEnemyList();
void Enemy_AfegirEnemic(EnemyTypes tipo, int columna);
void Enemy_DrawEnemies();
void Enemy_UpdateEnemies();
void CheckGlobalCollisions();

int main() { 
	int i = 0;
	int j = 0;
	float timeAlmas=0;
    // Inicialitzem la finestra a les mides declarades sense possibilitat de maximitzar-la
	
    InitWindowEx(ScreenWidth, ScreenHeight, "Lightless", false, "");
	InitAudioDevice();
	
	Texture2D bg1 = LoadTexture("resources/bg1.png");     
	Texture2D bg2 = LoadTexture("resources/bg2.png");   
	Texture2D bg3 = LoadTexture("resources/bg3.png");   
	Texture2D bg4 = LoadTexture("resources/bg4.png");   
	Texture2D bg5 = LoadTexture("resources/bg5.png");  
	Texture2D bg6 = LoadTexture("resources/bg6.png");
	
	Texture2D logoteam = LoadTexture("resources/logoteam.png");
	Texture2D logoraylib = LoadTexture("resources/logoraylib.png");
	Texture2D gametitle = LoadTexture("resources/gametitle.png");
	
	Texture2D hud = LoadTexture("resources/hud.png");
	
	Sound Shit = LoadSound("resources/hit.wav");
	Sound Seyes = LoadSound("resources/eyes.wav");
	Sound Sintro = LoadSound("resources/intro.wav");
	Sound Sstart = LoadSound("resources/start.wav");
	Sound Splay = LoadSound("resources/play.wav");
	
	int xbg1 = 0;
	int xbg2; 
	int xbg3; 
	int xbg4; 
	int xbg5; 
	int xbg6;
	
	Color myblack={10, 10, 10, 220};
	Color alphero=WHITE;
	
	
	//init player
	player.position.x = 200;
	player.position.y = 720/2;
	player.speed = 6;
	player.cardiaca = 80;
	player.cardiacaCont = 0;
	player.cont = 0;
	player.nirvana = false;
	player.dead = false;
	Rectangle rec = {0, 0, 128, 128};
	player.recAnimation = rec;
	player.frame = 0;
	player.column = 0;
	player.sprite = LoadTexture("resources/sprite.png");
	player.drawCount = 0;
	player.firsttime = false;
	player.updown = false;
	player.frameSpeed = 8;
	player.almas = 0;

    //DECLARACIONS/INITS
    //Camera2D camera = {{0,0,CameraWidth, CameraHeight}, LoadTexture("resources/bg_mario.png")};
	// Posicio player (falta centrar player (texture))
	
	Enemy_LoadEnemies("resources/enemy");
	enemy_list = (Enemy *) malloc ( NumDeEnemics * sizeof(Enemy));
	Enemy_InitEnemyList();

	int contador_columnas = 0;
	int columna = 0;
	
	int toogleLogos=0;
	int toogleStarter=false;
	
    //Camera2D camera = {{0,0,CameraWidth, CameraHeight}, LoadTexture("resources/bg_mario.gif")}    ;
    //int frame = 0;
    // Main game loop
        SetTargetFPS(60);
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
         switch(screen) 
       {
			case LOGO: 
           {
				if (!IsPlaying(Sintro))	PlaySound(Sintro);
				framesCount++;
				if (toogleLogos == 0){
					if (framesCount<270) alphero.a++;
					if (alphero.a>=254)	alphero.a=254;
					if (framesCount>270) 
					{
						framesCount=0;
						toogleLogos = 1;
					}
				}
				if (toogleLogos == 1){
					if (framesCount<270) alphero.a--;
					if (alphero.a<=2)	alphero.a=2;
					if (framesCount>270) 
					{
						framesCount=0;
						toogleLogos = 2;
					}
				}
				if (toogleLogos == 2){
					if (framesCount<270) alphero.a++;
					if (alphero.a>=254)	alphero.a=254;
					if (framesCount>270) 
					{
						framesCount=0;
						toogleLogos = 3;
					}
				}
				if (toogleLogos == 3){
					if (framesCount<270) alphero.a--;
					if (alphero.a<=2)	alphero.a=2;
					if (framesCount>270) 
					{
						framesCount=0;
						toogleLogos = 0;
						screen = MENU;
					}
				}
		   } break;
			case MENU: 
           {
				framesCount++;
				if (framesCount<270) alphero.a++;
				if (alphero.a>=254)	alphero.a=254;
				if (framesCount>270) 
				{
					framesCount=0;
					toogleStarter = true;
				}
				if ((IsKeyPressed(KEY_ENTER)) && toogleStarter) screen = GAMEPLAY;
			
		   } break;
			case GAMEPLAY: 
           {
		   //INPUTS
               
                //accio = LlegirInputs();
                
                //LOGICA
				
				
				
				Enemy_DrawCount++;
				
				if (framesCount==60)	framesCount=0;
				framesCount++;

				if (player.cont==10) player.cont=0;			
				player.cont++;
						
				player.drawCount++;
				
				//Moviment up/down player

				
				
				if (IsKeyDown(KEY_UP))	
				{
					player.position.y-=player.speed;
					//ANIMACIO
					if(eyes){
						if (player.firsttime) 
						{
							player.frame=0;
							player.firsttime=0;
						}
						player.column=10;
						player.recAnimation.x=player.recAnimation.width*player.frame;
						player.recAnimation.y=player.recAnimation.height*player.column;
								
						if (player.frame==2)	player.updown=0;
						else if (player.frame==0)	player.updown=1;
						if (player.drawCount==player.frameSpeed)
						{
							player.drawCount=0;
							if (player.updown)	player.frame++;
							else if (!player.updown)	player.frame--;
						}
					}
					else{
						if (player.firsttime) 
						{
							player.frame=0;
							player.firsttime=0;
						}
						player.column=2;
						player.recAnimation.x=player.recAnimation.width*player.frame;
						player.recAnimation.y=player.recAnimation.height*player.column;
								
						if (player.frame==2)	player.updown=0;
						else if (player.frame==0)	player.updown=1;
						if (player.drawCount==player.frameSpeed)
						{
							player.drawCount=0;
							if (player.updown)	player.frame++;
							else if (!player.updown)	player.frame--;
						}						
					}
				}

				if (IsKeyDown(KEY_LEFT))
				{
					player.position.x-=player.speed;
					//ANIMACIO
				}
				
				if (IsKeyDown(KEY_RIGHT))
				{
					player.position.x+=player.speed;
				
				}
					
				if (IsKeyDown(KEY_DOWN))	
				{
					player.position.y+=player.speed;
					//ANIMACIO
					if(eyes){
						if (player.firsttime) 
						{
							player.frame=0;
							player.firsttime=0;
						}
						player.column=9;
						player.recAnimation.x=player.recAnimation.width*player.frame;
						player.recAnimation.y=player.recAnimation.height*player.column;
								
						if (player.frame==2)	player.updown=0;
						else if (player.frame==0)	player.updown=1;
						if (player.drawCount==player.frameSpeed)
						{
							player.drawCount=0;
							if (player.updown)	player.frame++;
							else if (!player.updown)	player.frame--;
						}
					}
					else{				
						if (player.firsttime) 
						{
							player.frame=0;
							player.firsttime=0;
						}
						player.column=1;
						player.recAnimation.x=player.recAnimation.width*player.frame;
						player.recAnimation.y=player.recAnimation.height*player.column;
						
						if (player.frame==2)	player.updown=0;
						else if (player.frame==0)	player.updown=1;
						if (player.drawCount==player.frameSpeed)
						{
							player.drawCount=0;
							if (player.updown)	player.frame++;
							else if (!player.updown)	player.frame--;
						}
						
					}
				}
					//ANIMACIO damage taken
				
				if (damage)
				{
				
					if (player.firsttime) 
					{
						player.frame=0;
						player.firsttime=0;
					}										
					player.column=3;
					player.recAnimation.x=player.recAnimation.width*player.frame;
					player.recAnimation.y=player.recAnimation.height*player.column;
					
					if (player.frame==2)	player.updown=0;
					else if (player.frame==0)	player.updown=1;
					if (player.drawCount==player.frameSpeed)
					{
						player.drawCount=0;
						if (player.updown)
						{
							player.frame++;
							if (player.frame==1)	PlaySound(Shit);
						}
						else if (!player.updown)
						{	
							player.frame--;
							if (player.frame==0)	damage=0;
						}
					
					}
				}
			//	if (framesCount==50)	damage=0;
					
					else if (player.nirvana)
					{
							if (player.firsttime) 
						{
							player.frame=0;
							player.firsttime=0;
						}										
						player.column=7;
						player.recAnimation.x=player.recAnimation.width*player.frame;
						player.recAnimation.y=player.recAnimation.height*player.column;
						
						if (player.frame==3)	player.updown=0;
						else if (player.frame==0)	player.updown=1;
						if (player.drawCount==player.frameSpeed)
						{
							player.drawCount=0;
							if (player.updown)
							{
								player.frame++;
							}
							else if (!player.updown)
							{	
								player.frame--;
							}
						
						}
					}	
					
					
					
					
					else if ((!IsKeyDown(KEY_UP))&&(!IsKeyDown(KEY_DOWN)))
				{
					player.firsttime=1;
					if (eyes)
					{
						player.column=8;
						player.recAnimation.x=player.recAnimation.width*player.frame;
						player.recAnimation.y=player.recAnimation.height*player.column;
						if (player.drawCount==player.frameSpeed)
						{
							player.frame++;
							player.drawCount=0;
						}
						if (player.frame==5)	player.frame=0;
					}
					
					
				
					//ANIMACIO
				else 
				{
					player.column=0;
					player.recAnimation.x=player.recAnimation.width*player.frame;
					player.recAnimation.y=player.recAnimation.height*player.column;
					if (player.drawCount==player.frameSpeed)
					{
						player.frame++;
						player.drawCount=0;
					}
					if (player.frame==5)	player.frame=0;
				}
				//Limits up/down screen				
				if (player.position.y<0)	player.position.y=0;
				else if (player.position.y>ScreenHeight-128/*player Height*/)	player.position.y=ScreenHeight-128/*player Height*/;

				if (player.position.x<200)	player.position.x=200;
				else if (player.position.x>ScreenWidth-128)	player.position.x=ScreenWidth-128;
				
			}	
				
				//Recurs tancar ulls, inmunitat (falta inmunitat)
				if (IsKeyPressed(KEY_SPACE))	
				{
					PlaySound(Seyes);
					eyes=!eyes;
				}
				
				if ((eyes)&&(player.cont==10)||(player.cont==0))	player.cardiaca++;
				
				//NirvanaOP-------------------------PENDIENTE dE TESTEOH
				
				if ((player.almas>0)&&(IsKeyPressed('N'))&&(player.cardiaca<150)) 
				{		
					ForwardSpeed=5;
					timeAlmas+=0.6;
					player.almas--;
					player.nirvana=1;
				
				}
				
				if (player.nirvana==1)
				{
					if ((timeAlmas>0)) 
					{
						timeAlmas-=0.01;
					}		
					
					if (timeAlmas<=0)
					{
						ForwardSpeed=10;
						timeAlmas=0;
						player.nirvana=0;
					}
				}
				

				//-------------------------------------------------
				
				
				
				
				//Cardiaca
				
				if (framesCount==60)	player.cardiacaCont++;
				if ((player.cardiacaCont>0)&&(player.cardiaca>60)&&(player.cont==10))	player.cardiaca--;
				if (eyes)	player.cardiacaCont=0;
	
				if (player.cardiaca>209) 
				{
					player.dead=1;
					eyes = false;
					screen=ENDING;
				}
				if (player.position.x >= xbg6+bg6.width-1600 && player.dead == false){
					eyes=false;
					screen=ENDING;
				}
				
		
				
			} break;
			case ENDING: 
           
				player.drawCount++;
				
				if (player.dead)
				{
					if (player.frame<6)
					{				
			   
					if (player.firsttime) 
						{
							player.frame=0;
							player.firsttime=0;
						}
					player.column=4;
					player.recAnimation.x=player.recAnimation.width*player.frame;
					player.recAnimation.y=player.recAnimation.height*player.column;
					if (player.drawCount==player.frameSpeed+8)
						{
							player.frame++;
							player.drawCount=0;
						}
					}
				}
				else
				{
						if (player.firsttime) 
					{
						player.frame=0;
						player.firsttime=0;
					}										
					player.column=7;
					player.recAnimation.x=player.recAnimation.width*player.frame;
					player.recAnimation.y=player.recAnimation.height*player.column;
					
					if (player.frame==3)	player.updown=0;
					else if (player.frame==0)	player.updown=1;
					if (player.drawCount==player.frameSpeed)
					{
						player.drawCount=0;
						if (player.updown)
						{
							player.frame++;
						}
						else if (!player.updown)
						{	
							player.frame--;
						}
					
					}
				}	
				break;
           default: break;
       
		}		

		if(!FinalFeliz){
				contador_columnas+=ForwardSpeed;
			if(contador_columnas >= 150){
				Enemy_SpawnEnemies(columna);
				contador_columnas = 0;
				columna++;
				if(columna == ScreenColumns) FinalFeliz = true;
			}
		}
		Enemy_UpdateEnemies();
		CheckGlobalCollisions();
			
				
		
		//LOGICA DIBUIX	

				xbg1-= 2;
				xbg2 = xbg1 + bg1.width;
				xbg3 = xbg2 + bg2.width;
				xbg4 = xbg3 + bg3.width;
				xbg5 = xbg4 + bg4.width;		
				xbg6 = xbg5 + bg5.width;
		
		
       // camera.pov.x+=10;    
        //DIBUIX
        BeginDrawing();
       
            ClearBackground(RAYWHITE);
            switch(screen) 
        	{
	            case LOGO: 
	            {
					DrawTexture(bg1, 0, 0, WHITE);
					if ((toogleLogos == 0) || (toogleLogos == 1)) DrawTexture (logoteam, ScreenWidth/2-logoteam.width/2, ScreenHeight/2-logoteam.height/2, alphero);
					if ((toogleLogos == 2) || (toogleLogos == 3)) DrawTexture (logoraylib, ScreenWidth/2-logoteam.width/2, ScreenHeight/2-logoteam.height/2, alphero);
				} break;
				case MENU: 
	            {
					DrawTexture(bg1, 0, 0, WHITE);
					 DrawTexture (gametitle, ScreenWidth/2-logoteam.width/2, ScreenHeight/2-logoteam.height/2, alphero);
					if (toogleStarter) DrawText("PRESS [ENTER] TO START PLAYING", ScreenWidth-300, ScreenHeight-200, 20, RED);
				} break;
				case GAMEPLAY: 
	            {
	               // DrawTextureRec(camera.bg, camera.pov, PosicioCamera, WHITE);
					
					DrawTexture(bg1, xbg1, 0, WHITE);
					DrawTexture(bg2, xbg2, 0, WHITE);
					DrawTexture(bg3, xbg3, 0, WHITE);
					DrawTexture(bg4, xbg4, 0, WHITE);
					DrawTexture(bg5, xbg5, 0, WHITE);
					DrawTexture(bg6, xbg6, 0, WHITE);
					
					
					if (eyes)	DrawRectangle(200, 0, ScreenWidth-200, ScreenHeight, myblack);
					
					Enemy_DrawEnemies();
					DrawTextureRec(player.sprite, player.recAnimation, player.position, WHITE);		

					DrawTexture(hud, 0, 0, WHITE);
				
					DrawText(FormatText("%i", player.cardiaca), 80, 40, 40, BLACK);
					DrawText(FormatText("%i", player.almas), 80, 100, 40, BLACK);
					DrawText(FormatText("%.2f", timeAlmas), 80, 160, 40, BLACK);
					
					
					break;
				}
				case ENDING:
				{
					DrawTexture(bg6, 0, 0, WHITE);
					
					//DrawTextureRec(camera.bg, camera.pov, PosicioCamera, WHITE);
					
					if (player.dead)	DrawText("GAME OVERT, YOU LOSE!", 100, 100, 60, BLACK) ;
					if (!player.dead)	DrawText("GAME OVERT, YOU WIN!", 100, 100, 60, BLACK) ;
					
					DrawTextureRec(player.sprite, player.recAnimation, player.position, WHITE);		
					DrawText(FormatText("%i", player.cardiaca), 80, 40, 40, BLACK);
				}
			}
                   
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
 
    // De-Initialization
	
	//->	UnloadTexture(explosion);
	//UnloadTexture(camera.bg);
	UnloadTexture(player.sprite);
	UnloadTexture(bg1);
	UnloadTexture(bg2);
	UnloadTexture(bg3);
	UnloadTexture(bg4);
	UnloadTexture(bg5);
	UnloadTexture(bg6);
	
	UnloadSound(Seyes);
	UnloadSound(Shit);
	
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
   
    return 0;
}

//Carrega la matriu d'enemics del joc
void Enemy_LoadEnemies(char * path){
	FILE * file = fopen(path, "r");
	
	int i = 0;
	int j = 0;
	
	fscanf(file, "%d", &ScreenColumns);
	
	enemy_matrix= (int**)malloc ( ScreenRows * sizeof(int*) );
	for(i = 0; i < ScreenRows; i++) {
		enemy_matrix[i] = (int*)malloc ( ScreenColumns * sizeof(int) );
	}
	
	for(j = 0; j < ScreenColumns; j++){
		for(i = 0; i < ScreenRows; i++){
			fscanf(file, "%d", &enemy_matrix[i][j]);
		}
	}
	fclose(file);
}

//Fem apareixe l'enemic al seu lloc
void Enemy_SpawnEnemies(int columna){
	int i = 0;
	for(i = 0; i < ScreenRows; i++){
		switch(enemy_matrix[i][columna]){
			case ZigZag:
			case Straight:
			case StraightReal:
			case ZigZagReal:
				Enemy_AfegirEnemic(enemy_matrix[i][columna], i);
				NumDeEnemicsVius ++;
				break;
		}
	}
}

//Inicialitzem tot el vector
void Enemy_InitEnemyList(){
	int i = 0;
	Vector2 vec = {0, 0};
	Rectangle rec = {0, 0, 0, 0};
	for(i = 0; i < NumDeEnemics; i++){
	
		enemy_list[i].position = vec;
		enemy_list[i].recAnimation = rec;
		enemy_list[i].vivo = false;
		enemy_list[i].tipo = noEnemy;
		enemy_list[i].direccio = Arriba;
		enemy_list[i].cap = 0;
		enemy_list[i].current_cap = 0;
		enemy_list[i].haXocat = false;
		enemy_list[i].frame = 0;
	
	}
	
	//DEBUG
	/*
	for(i = 0; i < NumDeEnemics; i++){
		printf("Enemic numero %d:\n", i);
		printf("Posició--> x: %d y: %d\n", enemy_list[i].position.x, enemy_list[i].position.y);
		printf("Rectangle --> x: %d y: %d width: %d height: %d\n", enemy_list[i].rec.x, enemy_list[i].rec.y, enemy_list[i].rec.width, enemy_list[i].rec.height);
		printf("Vivo: %d\n", enemy_list[i].vivo);
		printf("Tipo: %d\n", enemy_list[i].tipo);
		printf("Direccio: %d\n", enemy_list[i].direccio);
		printf("Cap: %d\n", enemy_list[i].cap);
		printf("Current_cap: %d\n", enemy_list[i].current_cap);
	
	}
	*/
}

void Enemy_AfegirEnemic(EnemyTypes tipo, int fila){
	bool trobat = false;
	int count = 0;
	Rectangle rec = {0, 0, 128, 128};
	Vector2 vec = {ScreenWidth, (ScreenHeight/ScreenRows) * fila};
	while(!trobat && count < NumDeEnemics){
		if(enemy_list[count].vivo == false){
			enemy_list[count].tipo = tipo;
			enemy_list[count].haXocat = false;
			enemy_list[count].position = vec;
			enemy_list[count].vivo = true;
			enemy_list[count].recAnimation = rec;
			
			switch(tipo){
				case ZigZag:
				case ZigZagReal:
					//printf("Niu enemi in mai escrin\n With da following values:\n");
					enemy_list[count].direccio = Arriba;
					enemy_list[count].cap = Cap;
					enemy_list[count].current_cap = CapVariation;
					enemy_list[count].sprite = LoadTexture("resources/enemy1.png");
					break;
				case Straight:
				case StraightReal:
					enemy_list[count].sprite = LoadTexture("resources/enemy2.png");
					break;
			}
			trobat = true;
		}
		count++;
	}
}

//Dibuixo tots els enemics vius de la llista
// TODO: canviar Rectangle per Sprite
void Enemy_DrawEnemies(){
	int i = 0;
	int contador_vivos = 0;
	for(i = 0; i < NumDeEnemics; i++){
		if(enemy_list[i].vivo){
			contador_vivos++;
			switch(enemy_list[i].tipo){
				case Straight:
				case ZigZag:
					if(!eyes){
						DrawTextureRec(enemy_list[i].sprite, enemy_list[i].recAnimation, enemy_list[i].position, WHITE);
					}
					break;
				default:
					DrawTextureRec(enemy_list[i].sprite, enemy_list[i].recAnimation, enemy_list[i].position, WHITE);
					break;
			}
			if(contador_vivos == NumDeEnemicsVius) break;
		}
	}
}

//Funcio que actualitza tots els enemics vius de la llista
void Enemy_UpdateEnemies(){
	int i = 0;
	for(i = 0; i < NumDeEnemics; i++){
		if(enemy_list[i].vivo){
			if(Enemy_DrawCount >= 15){
				enemy_list[i].recAnimation.x = enemy_list[i].recAnimation.width * enemy_list[i].frame;
				enemy_list[i].frame++;
			}
			enemy_list[i].position.x-=ForwardSpeed;
			switch(enemy_list[i].tipo){
				case ZigZag:
				case ZigZagReal:
					switch(enemy_list[i].direccio) {
						case Arriba:
							enemy_list[i].position.y -= CapVariation;
							enemy_list[i].current_cap-=CapVariation;
							break;
						case Abajo:
							enemy_list[i].position.y += CapVariation;
							enemy_list[i].current_cap+=CapVariation;
							break;
					}
					if(fabs(enemy_list[i].current_cap) >= enemy_list[i].cap){
						if(enemy_list[i].direccio == Arriba) enemy_list[i].direccio = Abajo;
						else enemy_list[i].direccio = Arriba;
					}
					break;
					if(enemy_list[i].frame > 1) enemy_list[i].frame = 0;
					
				case Straight:
					if(enemy_list[i].frame > 6) enemy_list[i].frame = 0;
			} //switch(enemy_list[i].tipo)
			
			if(enemy_list[i].position.x + 128 <= 0){
				enemy_list[i].vivo = false;
				NumDeEnemicsVius--;
			}
		} //if(enemy_list[i].vivo)
	} //for(i = 0; i < NumDeEnemics; i++)
	if(Enemy_DrawCount >= 15) Enemy_DrawCount = 0;
}

void CheckGlobalCollisions(){
	Vector2 offset = {20, 30};
	Rectangle col_player = {player.position.x + offset.x, player.position.y + offset.y, player.recAnimation.width - 2*offset.x, player.recAnimation.height - 2*offset.y};
	Rectangle col_enemy = {0, 0, 0, 0};
	int contador_enemigos = 0;
	int i =0;
	int centre_y, centre_x;
	float radi = 20.0;
	for(i = 0; i < NumDeEnemics; i++){
		if(enemy_list[i].vivo){
			switch(enemy_list[i].tipo){
			/* 
			bool CheckCollisionRecs(Rectangle rec1, Rectangle rec2);                                           
			bool CheckCollisionCircleRec(Vector2 center, float radius, Rectangle rec);                         
			*/
				case ZigZag:
					if(!eyes){
						
						centre_x = enemy_list[i].position.x + enemy_list[i].recAnimation.width/2;
						centre_y = enemy_list[i].position.y + enemy_list[i].recAnimation.height/2;
						
						radi = 20.0;					
						
						if( CheckCollisionCircleRec( (Vector2){centre_x, centre_y}, radi, col_player) && enemy_list[i].haXocat == false ){
							
							player.cardiaca+=vida[0];
							damage=1;
							
							//bool col = CheckCollisionCircleRec( centre, radi, col_player);
							//printf("Hem xocat\n");
							//printf("%d\n", col);
							//printf("%d\n", enemy_list[i].haXocat);
							//getchar();
							player.cardiacaCont=0;
							enemy_list[i].haXocat = true;

							
						}
					}
					break;
				
				case ZigZagReal:
					centre_x = enemy_list[i].position.x + enemy_list[i].recAnimation.width/2;
					centre_y = enemy_list[i].position.y + enemy_list[i].recAnimation.height/2;
					
					radi = 20.0;					
					
					if( CheckCollisionCircleRec( (Vector2){centre_x, centre_y}, radi, col_player) && enemy_list[i].haXocat == false ){
						
						if(eyes){
							player.almas++;
						}
						else{
							//TODO
							player.cardiaca+=vida[1];
							player.cardiacaCont=0;
							damage=1;
						}
						
						enemy_list[i].haXocat = true;
						
					}
					break;
				
				case Straight:
					if(!eyes){
					
						col_enemy.x = enemy_list[i].position.x + offset.x;
						col_enemy.y = enemy_list[i].position.y + offset.y;
						col_enemy.width = enemy_list[i].recAnimation.width - 2*offset.x;
						col_enemy.height = enemy_list[i].recAnimation.height - 2*offset.y;
						
						if(CheckCollisionRecs(col_enemy, col_player)&&(enemy_list[i].haXocat==false)){
							enemy_list[i].haXocat = true;
							player.cardiaca+=vida[2];
							player.cardiacaCont=0;
							damage=1;
						}
					}	
					break;
				
				case StraightReal:
					col_enemy.x = enemy_list[i].position.x + offset.x;
					col_enemy.y = enemy_list[i].position.y + offset.y;
					col_enemy.width = enemy_list[i].recAnimation.width - 2*offset.x;
					col_enemy.height = enemy_list[i].recAnimation.height - 2*offset.y;

					if(CheckCollisionRecs(col_enemy, col_player)&&(enemy_list[i].haXocat==false)){
						enemy_list[i].haXocat = true;
						
						if (eyes)	player.almas++;
						else	
						{
							player.cardiaca+=vida[3];
							player.cardiacaCont=0;
							damage=1;
						}
					}
					break;
			}
		}
	}
}
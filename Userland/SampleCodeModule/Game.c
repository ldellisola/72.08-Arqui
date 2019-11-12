#include "./include/Game.h"
#include "../Include/Time.h"
#include "../Include/Curses.h"



#define LIVESi                      3//cantidad de vidas al iniciar el juego    

#define SCREEN_HEIGHT               600//completar
#define SCREEN_WIDTH                600//completar

#define BAR_LENGTH                  (17*SCREEN_WIDTH/100)
#define BAR_HEIGHT                  (4*SCREEN_HEIGHT/100)                
#define BAR_YPOS                    (78*SCREEN_HEIGHT/100)

#define BLOCK_WIDTH                 ((2*SCREEN_WIDTH/3) / C_BLOCKS)
#define BLOCK_XSEPARATION           ((SCREEN_WIDTH/3) / C_BLOCKS)
#define BLOCK_HEIGHT                ( (SCREEN_HEIGHT/2) / ( R_BLOCKS + 2) )
#define BLOCK_YSEPARATION           ( (2*BLOCK_HEIGHT ) / (R_BLOCKS +2 ))
#define BALL_RADIO                  (1*SCREEN_HEIGHT/200) 

#define bar_vel                     (2*SCREEN_WIDTH/100)

//COLORES
    #define BLACK                       0x000000
    #define WHITE                       0xFFFFFF
    #define BLUE                        0x1
    #define GREEN                       0x2
    #define AQUA                        0x3
    #define RED                         0x4
    #define PURPLE                      0x5
    #define YELLOW                      0x6
    #define LightBlue                   0x9
    #define LightGreen                  0xA
    #define LightAqua                   0xB
    #define LightRed                    0xC
    #define LightPurple                 0xD
    #define LightYellow                 0xE   


#define X                           0
#define Y                           1

#define NO_BLOCK                   -1

#define LEFT_ARROW                 'a'
#define RIGHT_ARROW                'd' 


static int lives;                                          //cantidad de vidas que tiene

static int ball_pos[2];                                    //pelota en el medio de ls pantalla
static int ball_vel;                                       //la velocidad cuenta de a cuantos cuadraditos se mueve
static ballDirec ball_dir;

static int bar_pos[2];

static int blocks[R_BLOCKS][C_BLOCKS];                     //matriz de los bloques
static int blocks_left; 

//TIEMPO
    //tiempo total
    static int time_past;
    //tiempo de inicio de juego
    int relative_startTime[6];
    //va de 0 a 15 segundos
    int relative_time;
    int start_time[6];


//DECLARACION DE FUNCIONES
    void printObjects();
    void print_time();


//para inicializar el juego
int runGame(void){
    time_past=0;
    lives = LIVESi;
    blocks_left= R_BLOCKS*C_BLOCKS;                            
    ball_pos[X]=SCREEN_WIDTH/2;
    ball_pos[Y]=SCREEN_HEIGHT/2;      
    ball_vel=5;

    bar_pos[X]=SCREEN_WIDTH/2;
    bar_pos[Y]=BAR_YPOS; 
    ball_dir = D; //la variable se llama igual al tipo, entonces le cambio el nombre al tipo por dir y declaro aca

    //pongo la matriz de bloques todos en uno, (osea que estan)
    for(int i = 0; i < C_BLOCKS ; i++){
        for(int j = 0; j < R_BLOCKS; j++){
            blocks[j][i]= 1;
        }
    }

    /*comienza el juego  */
    startGame();      
    return 0;
}

//cuando quiero retomar el juego
int startGame(){
    setRelativeStartTime();
    start_time[0]=relative_startTime[0];
    start_time[1]=relative_startTime[1];
    start_time[2]=relative_startTime[2];
    start_time[3]=relative_startTime[3];
    start_time[4]=relative_startTime[4];
    start_time[5]=relative_startTime[5];
    startGameRec();
    return 0;
}



 //juega recursivamente

int startGameRec(void){ 
     
    relative_time=(GetSeconds()- relative_startTime[4]) + (GetMinutes()-relative_startTime[3]) *60 + (GetHours() - relative_startTime[2]) * 60 *60 + (GetDayOfMonth()- relative_startTime[1]) *60*60*24 + (GetYear() - relative_startTime[0])*60*60*24*365; 
    if(stopKeyPressed()){ 
        time_past += past_time();
        //COMPLETAR!!! TIENE QUE PASAR ALGO
        return 0;
    }
    if(lives == 0  || blocks_left == 0 ){
        time_past=past_time();
        finishGame(time_past);
        return 0;        
    }
   
    
    if(relative_time >= 15){
        ball_vel++;
        setRelativeStartTime();
    }
    

    int curr_BallPos[]={ball_pos[X], ball_pos[Y]};
    int curr_BarPos[]={bar_pos[X], bar_pos[Y]};
    /*MOVIMIENTO DE LA BARRA*/
    handleBarMov();
    /*MOVIMIENTO DE LA PELOTA*/
    handleBallMov();
    //modificar velocidad de 

    printObjects(curr_BallPos, curr_BarPos);
    startGameRec();
    return 1; //no tendria que llegar aca, es para evitar el warning, esta mal asi?
}

void printObjects(int * curr_BallPos, int * curr_BarPos){
    print_ball(curr_BallPos,BLACK );
    print_bar(curr_BarPos, BLACK); 
    print_ball(ball_pos, WHITE );
    print_blocks(blocks);
    print_bar(bar_pos, WHITE);
    print_time();
}

void handleBarMov(){
        //barHitWall devuelve un int que representa que pared esta chocando (enum walls)
    if(left_arrow_pressed()){
       if(!(barHitWall() == LEFT)){      
             bar_pos[X]  -= bar_vel;                     //muevo la barra para la izquierda
       }
    }
    if(right_arrow_pressed()){
        if(!(barHitWall()== RIGHT)){
            bar_pos[X] += bar_vel;                     //muevo la barra para la derecha
        }
    }

}
void handleBallMov(void){
    //si pega contra una pared
    walls wall;
    barSides bar_side;
    int block[3];
    ballHitBlock(block);
    if( (wall = ballHitWall()) ){   //NONE = 0 entonces devuelve FALSE
        switch(wall){
            case FLOOR:
                lives -=1; 
                ball_pos[0]=SCREEN_WIDTH/2;
                ball_pos[0]=SCREEN_HEIGHT/2;
                ball_dir= D;
                bar_pos[X] = SCREEN_WIDTH/2;
                return;
            break;
            case LEFT:    
            case RIGHT:
            case UPPER:
                invertDirection(wall);
            break;
            case URCORNER:
                ball_dir = LD;
            break;
            case ULCORNER:
                ball_dir = RD;
            break;
            case NONE:
            case LRCORNER:
            case LLCORNER:
            break;
        }
    }
    //si pega contra un bloque
    else if(block[0] != NO_BLOCK){    
        blocks[block[0]][block[1]]=0;
        invertDirection(block[2]); //acordarse que si pega en la derecha tiene que devolver wall = LEFT
    }
    //Si pega en la barra
    else if( (bar_side = ballHitBar()) ){
        ballHitBarChangeDireccion(bar_side);
    }
    ballMove();
}

/*typedef enum barSides{N = 0, L, R, UL, UM, UR} barSides;      
             ____________________________________ bar_Ycord[0]
            |___________|_____-______|___________| <-bar      __ bar_Ycord[1]
     bar_Xcord[0]  bar_Xcord[1]  bar_Xcord[2]  bar_Xcord[3]
                               ^
                               |
                            bar_pos
*/
barSides ballHitBar(){
    int bar_Xcord[]={bar_pos[X] - BAR_LENGTH/2, bar_pos[X] - BAR_LENGTH/6, bar_pos[X] + BAR_LENGTH/6, bar_pos[X] + BAR_LENGTH/2};
    int bar_Ycord[] = {bar_pos[Y] - BAR_HEIGHT / 2, bar_pos[Y] + BAR_HEIGHT / 2 };
    int nextPos[2];
    ballNextPos(nextPos);
    int LLSquare[2];
    int URSquare[2];
    
    makeSquare(LLSquare, bar_Xcord[0], bar_Ycord[1]);  
    makeSquare(URSquare, bar_Xcord[3], bar_Ycord[0]);

    if(!insideSquare(nextPos, LLSquare , URSquare)){
        return N;
    }
    else{
        //dentro de la primer parte de la barra
        
        makeSquare(LLSquare, bar_Xcord[0], bar_Ycord[1]);  
        makeSquare(URSquare, bar_Xcord[1], bar_Ycord[0]);

        if(insideSquare(nextPos, LLSquare, URSquare)){
            if(ballBetween(ball_pos[Y], bar_Ycord[0], bar_Ycord[1])){
                return L;
            }
            return UL;
        }
        //dentro de la segunda parte de la barra

        makeSquare(LLSquare, bar_Xcord[1], bar_Ycord[1]);  
        makeSquare(URSquare, bar_Xcord[2], bar_Ycord[0]);

        if(insideSquare(nextPos, LLSquare, URSquare)){
            return UM;
        }
        //dentro de la TERCER parte de la barra
        
        makeSquare(LLSquare, bar_Xcord[2], bar_Ycord[1]);  
        makeSquare(URSquare, bar_Xcord[3], bar_Ycord[0]);

        if(insideSquare(nextPos, LLSquare, URSquare)){
            if(ballBetween(ball_pos[Y], bar_Ycord[0], bar_Ycord[1])){
                return R;
            }
            return UR;
        }
    }
    return N;
}

void makeSquare(int * square, int x, int y){
    square[X] = x;
    square[Y] = y;
    return;
}

int ballBetween(int auxPos, int y1, int y2){
    if(auxPos < y2 && auxPos > y1){
        return 1;
    }
    return 0;
}

//la pos de la pelota, punto de abajo a la izquierda y arriba a la derecha del cuadrado
int insideSquare(int * auxPos, int * LLSquare, int * URSquare){
    if(ballBetween(auxPos[X],LLSquare[X], URSquare[X]) && ballBetween(auxPos[Y], URSquare[Y], LLSquare[Y])){
        return 1;
    }
    return 0;
}

void print_blocks(int blocks[R_BLOCKS][C_BLOCKS]){
    int x;
    int y;
    for(int i = 0; i < R_BLOCKS ; i++){
        for(int j = 0; j <C_BLOCKS ; j++){
                x = (j * BLOCK_WIDTH) + BLOCK_XSEPARATION*(j+1) ;
                y =  (i * BLOCK_HEIGHT) + BLOCK_YSEPARATION*(i+1) ;
            if( blocks[i][j] == 1){
                print_block( x ,y,WHITE);
            }
            else
                print_block( x , y,BLACK);
        }
    }
}

void ballHitBarChangeDireccion(barSides side){
    //enum ballDirec{LU, U, RU, RD,D, LD}ballDirec
    switch(side){
        case L:
            ball_dir = LD;
            break;
        case R:
            ball_dir = RU;
            break;
        case UL:
            ball_dir = LU;
            break;
        case UM:
            ball_dir = U;
            break;
        case UR:
            ball_dir = RU;
            break;
    }
}

void ballMove(){
    ballNextPos(ball_pos);
}

void ballNextPos(int * auxPos){
    auxPos[X] =ball_pos[X];
    auxPos[Y] = ball_pos[Y]; 
    switch(ball_dir){
        case LU:
            auxPos[X] -= ( ball_vel * 0.7071); 
            auxPos[Y] -= ( ball_vel * 0.7071);
            break;
        case U:
            auxPos[Y] -= ball_vel;  
            break;
        case RU:
            auxPos[X] += ( ball_vel * 0.7071); 
            auxPos[Y] -= ( ball_vel * 0.7071);
            break;
        case RD:
            auxPos[X] += ( ball_vel * 0.7071); 
            auxPos[Y] += ( ball_vel * 0.7071);
            break;
        case D: 
            auxPos[Y] += ball_vel * 0.7071;
            break;
        case LD:
            auxPos[X] -= ( ball_vel * 0.7071); 
            auxPos[Y] += ( ball_vel * 0.7071);
            break;
    }
    return;
}
 

void invertDirection(walls wall){
    switch(wall){
        case ULCORNER:
            ball_dir = RD;
        break;
        case URCORNER:
            ball_dir = LD;
        break;
        case LLCORNER:
            ball_dir = RU;
        break;
        case LRCORNER:
            ball_dir = LU;
        break;
        case LEFT:
            if(ball_dir == LU){
                    ball_dir = RU;
            }
            else if( ball_dir == LD){
                ball_dir = RD;
            }
        break;
        case RIGHT:
            if(ball_dir == RU){
                ball_dir = LU;
            }else if(ball_dir == RD){
                ball_dir = LD;
            }
        break;
        case UPPER:
            switch(ball_dir){
                case LU:
                    ball_dir = LD;
                break;
                case RU:
                    ball_dir = RD;
                break;
                case U:
                    ball_dir = D;
                break;
            }
        break;
        //FLOOR SOLO PASA CON LOS BLOQUES(la parte de arriba)
        case FLOOR:
            switch(ball_dir){
                case LD:
                    ball_dir = LU;
                break;
                case RD:
                    ball_dir = RU;
                break;
                case D:
                    ball_dir = U;
                break;
            }
        break;

    }
}



//implementar CORNERS
walls ballHitWall(){
    int auxPos[2];
    ballNextPos(auxPos);
    if(auxPos[X] + BALL_RADIO >= SCREEN_WIDTH ){
        return RIGHT;
    }else if(auxPos[X] - BALL_RADIO <= 0){
        return LEFT;
    }else if(auxPos[Y] + BALL_RADIO >= SCREEN_HEIGHT){
        return FLOOR;
    }else if(auxPos[Y] - BALL_RADIO <= 0 ){
        return UPPER;
    }
    return NONE;
}

walls barHitWall(){
    if( ( bar_pos[X]+ bar_vel + (BAR_LENGTH /2) ) >= SCREEN_WIDTH){
        return RIGHT;
    }else if(bar_pos[X] -bar_vel- BAR_LENGTH/2 <= 0){
        return LEFT;
    }
    return NONE;
}

void ballHitBlock(int * block){
    walls auxWall;
    for(int i = 0; i < C_BLOCKS ; i++){
        for(int j = 0; j < R_BLOCKS; j++){
            auxWall = ballTouchingWall(i, j);
            if(auxWall){
                block[0]=i;
                block[1]=j;
                block[2]=auxWall;
                return;
            }       
        }
    }
    block[0]= NO_BLOCK;
    return;
}


walls ballTouchingWall(int c, int r){
    
    int nextPos[2];
    ballNextPos(nextPos);
    

    if(ballBetweenXSides(nextPos, c, r) && ballBetweenYSides(nextPos, c, r)){
        blocks_left -=1;
        
        if( ballBetweenXSides(ball_pos, c, r) ){
            
            if(ball_dir == U || ball_dir == LU || ball_dir == RU){
                
                return UPPER;//en verdad es la parte de abajo del bloque pero se comporta como la pared de arriba
            }
            if(ball_dir == D || ball_dir == LD || ball_dir == RD){
                return FLOOR; //en verdad esta tocando la parte de arriba pero se comporta como piso
            }
        }
        if(ballBetweenYSides(ball_pos, c, r)){
            
            if(ball_dir == LU || ball_dir == LD){
                return LEFT;
            }
            if(ball_dir == RU || ball_dir == RD){
                return RIGHT; 
            }
        }
        if( !ballBetweenYSides(ball_pos, c, r) && !ballBetweenXSides(ball_pos, c, r)){
            
            switch(ball_dir){
                case LU:
                    return ULCORNER;    
                break;
                case RU:
                    return URCORNER;
                break;
                case LD:
                    return  LLCORNER;
                break;
                case RD:
                    return LRCORNER;
                break;
            
            }
        }
    }
    return NONE;
}

int ballBetweenXSides(int * auxPos, int c, int r){
    int x1=(c* BLOCK_WIDTH + (c+1)* BLOCK_XSEPARATION);
    int x2=( (c+1)* BLOCK_WIDTH + (c+1)* BLOCK_XSEPARATION );

    return (ballBetween(auxPos[X], x1, x2 ))? 1:0;
}

int ballBetweenYSides(int * auxPos, int c, int r){
    int lowerSideOfBlock = (r+1) * BLOCK_HEIGHT + (r+1) * BLOCK_YSEPARATION - BLOCK_HEIGHT/2 ;
    int upperSideOfBlock =  r * BLOCK_HEIGHT + (r+1) * BLOCK_YSEPARATION;

    return   (ballBetween(auxPos[Y], upperSideOfBlock, lowerSideOfBlock))? 1:0; 
}


int finishGame(int time_past){
    if(blocks_left == 0){
       // printf("congratulations you've won!! it took you %d seconds", time_past);
    }else{
        //printf("better luck next time! time: %d seconds", time_past);
    }
    return 0;
}

int past_time(){
    return (relative_time + relative_startTime[0] - start_time[0] + relative_startTime[1] - start_time[1] + relative_startTime[2] - start_time[2] + relative_startTime[3] - start_time[3] + relative_startTime[4] - start_time[4]);
}


void setRelativeStartTime(){
    relative_startTime[0]=GetYear();
    relative_startTime[1]=GetMonth();
    relative_startTime[2]= GetDayOfMonth();
    relative_startTime[3]= GetHours();
    relative_startTime[4]= GetMinutes();
    relative_startTime[5]= GetSeconds();
}
void print_ball(int * ball_pos,int color){

    printOnScreen(ball_pos,BALL_RADIO*2,BALL_RADIO*2,color);
}

void print_bar(int * bar_pos,int color){
    printOnScreen(bar_pos,BAR_LENGTH,BAR_HEIGHT,color);
}
void print_block(int x,int y,int color){
    int pos[]= {x, y};
    printOnScreen(pos,BLOCK_WIDTH,BLOCK_HEIGHT,color);
}

int stopKeyPressed(){
    char key = readKey();
    if(key == 'q'){
        return 1;
    }
    return 0;
}
int left_arrow_pressed(){
    char key = readKey();
    if(key == LEFT_ARROW){
        return 1;
    }
    return 0;
}
int right_arrow_pressed(){
    char key = readKey();
    if(key == RIGHT_ARROW){
        return 1;
    }
    return 0;
}

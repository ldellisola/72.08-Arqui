#include "./include/Game.h"
#include "../Include/Time.h"

#define LIVESi  3                                     //cantidad de vidas al iniciar el juego    

#define BAR_LENGTH

#define BLOCK_WIDTH            //COMPLETAR
#define BLOCK_XSEPARATION      //COMPLETAR
#define BLOCK_HEIGHT         //COMPLETAR
#define BLOCK_YSEPARATION      //COMPLETAR


int X = 0;
int Y = 1;

int WON=1;
int LOST =0;

int lives;                                          //cantidad de vidas que tiene

int ball_pos[2];                                    //pelota en el medio de ls pantalla
int ball_vel;                                       //la velocidad cuenta de a cuantos cuadraditos se mueve
ballDirec ball_dir;
int bar_vel= 1;                                     //velocidad de la barra 
int bar_pos;

int blocks[R_BLOCKS][C_BLOCKS];                     //matriz de los bloques
int NO_BLOCK[]={-1,-1,-1};
int blocks_left=R_BLOCKS*C_BLOCKS;

int time_past;
int relative_startTime[6];
int relative_time;
int start_time[6];

int SCREEN_HEIGHT; 
int SCREEN_WIDTH;


//para inicializar el juego
int runGame(void){
    time_past=0;
    lives = LIVESi;                            
    ball_pos[0]=SCREEN_WIDTH/2;
    ball_pos[1]=SCREEN_HEIGHT/2;      
    ball_vel=1;                         
    bar_pos=SCREEN_WIDTH/2; 
    ball_dir = D; //la variable se llama igual al tipo, entonces le cambio el nombre al tipo por dir y declaro aca

    //pongo la matriz de bloques todos en uno, (osea que estan)
    for(int i = 0; i < C_BLOCKS ; i++){
        for(int j = 0; j < R_BLOCKS; j++){
            blocks[i][j]= 1;
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
        //mainMenu(); esto va?
        return 0;
    }
    if(lives == 0){
        time_past=past_time();
        finishGame(time_past, LOST);
        return 0;        
    }
    if(blocks_left == 0){
        time_past=past_time();
        finishGame(time_past, WON);
        return 0;
    }

    // block[0]=posX de bloque que choco, block[1]=posY, block[2]=lado que choco del bloque;                       
    print_ball(ball_pos);
    print_blocks(blocks);
    print_bar(bar_pos);

    /*MOVIMIENTO DE LA BARRA*/
    handleBarMov();
    /*MOVIMIENTO DE LA PELOTA*/
    handleBallMov();
    //modificar velocidad de 

    if(relative_time >= 15){
        ball_vel++;
        setRelativeStartTime();
    }


    startGameRec();
    return 1; //no tendria que llegar aca, es para evitar el warning, esta mal asi?
}

void handleBarMov(){
        //barHitWall devuelve un int que representa que pared esta chocando (enum walls)
    if(left_arrow_pressed()){
       if(!(barHitWall() == LEFT)){      
             bar_pos  -= bar_vel;                     //muevo la barra para la izquierda
       }
    }
    if(right_arrow_pressed()){
        if(!(barHitWall()== RIGHT)){
            bar_pos += bar_vel;                     //muevo la barra para la derecha
        }
    }

}
void handleBallMov(void){
    //si pega contra una pared
    walls wall;
    barSides bar_side;
    int block[3];
    if(wall = ballHitWall()){   //NONE = 0 entonces devuelve FALSE
        switch(wall){
            case FLOOR:
                lives -=1; 
                ball_pos[0]=SCREEN_WIDTH/2;
                ball_pos[0]=SCREEN_HEIGHT/2;
                ball_dir= D;
                bar_pos = SCREEN_WIDTH/2;
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
        }
    }
    //si pega contra un bloque
    else if((block = ballHitBlock()) != NO_BLOCK){    
        blocks[block[0]][block[1]]=0;
        invertDirection(block[2]); //acordarse que si pega en la derecha tiene que devolver wall = LEFT
    }
    //Si pega en la barra
    else if(bar_side = ballHitBar()){
        ballHitBarChangeDireccion(bar_side);
    }
    ballMove();
}

void print_blocks(int blocks[R_BLOCKS][C_BLOCKS]){
    for(int i = 0; i < C_BLOCKS ; i++){
        for(int j = 0; j < R_BLOCKS; j++){
            if( blocks[i][j] == 1){
                print_block(i * BLOCK_WIDTH + BLOCK_XSEPARATION, j * BLOCK_HEIGHT + BLOCK_YSEPARATION);
            }
        }
    }
}

void ballHitBarChangeDireccion(barSide side){
    //enum ballDirec{LU, U, RU, RD,D, LD}ballDirec
    switch(side){
        case L:
            ball_dir = LD;
            break;
        case R:
            ball_dir = RD;
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
            auxPos[X] -= ( ball_vel * 0,7071); 
            auxPos[Y] += ( ball_vel * 0,7071);
            break;
        case U:
            auxPos[Y] += ball_vel;  
            break;
        case RU:
            auxPos[X] += ( ball_vel * 0,7071); 
            auxPos[Y] += ( ball_vel * 0,7071);
            break;
        case RD:
            auxPos[X] += ( ball_vel * 0,7071); 
            auxPos[Y] -= ( ball_vel * 0,7071);
            break;
        case D: 
            auxPos[Y] += ball_vel * 0,7071;
            break;
        case LD:
            auxPos[X] -= ( ball_vel * 0,7071); 
            auxPos[Y] -= ( ball_vel * 0,7071);
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
            ball-direc = LD;
        break;
        case LLCORNER:
            ball-direc = RU;
        break;
        case LRCORNER:
            ball-direc = LU;
        break;
        case LEFT:
            switch(ball_dir){
                case LU:
                    ball_dir = RU;
                break;
                case LD:
                    ball_dir = RD;
                break;
            }
        break;
        case RIGHT:
            switch(ball_dir){
                case RU:
                    ball_dir = LU;
                break;
                case RD:
                    ball_dir = LD;
                break;
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
    int auxPos[] = ballNextPos();
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
    if(bar_pos+ bar_vel + BAR_LENGTH/2 >= SCREEN_WIDTH){
        return RIGHT;
    }else if(bar_pos -bar_vel- BAR_LENGTH/2 <= 0){
        return LEFT;
    }
    return NONE;
}

int * ballHitBlock(){
    walls auxWall;
    for(int i = 0; i < C_BLOCKS ; i++){
        for(int j = 0; j < R_BLOCKS; j++){
            auxWall = ballTouchingWall(i, j);
            if(auxWall){
                return {i,j, auxWall};
            }       
        }
    }
    return NO_BLOCK;
}


wall ballTouchingWall(int c, int r){
    int auxPos[]=ballNextPos();
    if(ballBetweenXSides(auxPos, c, r) && ballBetweenXSides(ball_pos, c, r) && ballBetweenYSides(auxPos, c, r)){
        blocks_left -=1;
        if(ball_dir == U || ball_dir == LU || ball_dir == RU){
            return UPPER;//en verdad es la parte de abajo del bloque pero se comporta como la pared de arriba
        }
        if(ball_dir == D || ball_dir == LD || ball_dir == RD){
            return FLOOR; //en verdad esta tocando la parte de arriba pero se comporta como piso
        }
    }
    if(ballBetweenYSides(auxPos, c, r) && ballBetweenYSides(ball_pos) && ballBetweenXSides(auxPos)){
        blocks_left -=1;
        if(ball_dir == LU || ball_dir == LD){
            return LEFT;
        }
        if(ball_dir == RU || ball_dir == RD){
            return RIGHT; 
        }
    }if(ballBetweenXSides(auxPos, c, r) && ballBetweenYSides(auxPos) && !ballBetweenYSides(ball_pos, c, r) && !ballBetweenXSides(ball_pos, c, r)){
        blocks_left -=1;
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
    return NONE;
}

int ballBetweenXSides(int * auxPos, int c, int r){
    if(((auxPos[x] - BALL_RADIO) < ((c+1)* BLOCK_WIDTH + (c+1)* BLOCK_XSEPARATION)) && ((auxPos[X] + BALL_RADIO) > (c* BLOCK_WIDTH + (c+1)* BLOCK_XSEPARATION))){
        return 1;
    }
    return 0;
}

int ballBetweenYSides(int * auxPos, int c, int r){
    return  (auxPos[Y] - BALL_RADIO) < ((r+1) * BLOCK_HEIGHT + (r+1) * BLOCK_YSEPARATION - BLOCK_HEIGHT/2) && (auxPos[Y] + BALL_RADIO) > ( r * BLOCK_HEIGHT + (r+1) * BLOCK_YSEPARATION );
}


int finishGame(int time_past, int result){
    if(result == WON){
        printf("congratulations you've won!! it took you %d seconds", time_past);
    }else{
        printf("better luck next time! time: %d seconds", time_past);
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



/* funciones a hacer=
    .printf();
    .bool stopKeyPressed()
    .bool left_arrow_pressed()
    .bool right_arrow_pressed()
    .print_ball(ball_pos)
    .print_bar(bar_pos) 
    .mainMenu()                             seria la funcion que se corre para mostrar si elegir la terminal o el juego

*/

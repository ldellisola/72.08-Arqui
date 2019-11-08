#ifndef GAME_H
#define GAME_H

#include <stdio.h>
typedef enum ballDirec{LU, U, RU, RD, D, LD} ball_dir;                                                         //Left up, up, right up, right down, down, left down
typedef enum walls{NONE = 0, LEFT, RIGHT, UPPER, FLOOR, URCORNER, ULCORNER, LRCORNER, LLCORNER } wall;           //los bordes de la pantalla
typedef enum barSides{N, L, R, UL, UM, UR} bar_side;                                                             //none, left, right, upperLeft, UpperMiddle, UpperRight

#define R_BLOCKS  4                                   //Cantidad de filas de bloques
#define C_BLOCKS  5                                   //Cantidad de columnas de bloques

//Inicialiizar el juego desde cero
int runGame(void);


//para volver si fue pausado
int startGame(void);
int startGameRec(void);


void print_blocks(int blocks[R_BLOCKS][C_BLOCKS]);

int past_time();

void handleBallMov(int * block);

void handleBarMov();
/*Le cambia la direccion a la pelota dependiendo exactamente donde cae en la barra
    Si cae en la parte de arriba la barra se divide en tres:
        A la izquierda: va a salir 135 grados(LU)
        En el medio a 90 grados(arriba, U)
        A la derecha a 45gados(RU)
    si cae en uno de los lados:
        derecha: sale a -45 grados(RD)
        izquierda: sale a 225 grados(RD)
*/
void ballHitBarChangeDireccion(bar_side side);

//Le cambia la posicion a la pelota dependiendo su pos y direccion y vel
void ballMove();


//hace que la pelotita cambie de direccion si choca con una pared o bloque
void invertDirection(wall wall);

//devuelve que pared le esta pegando y false(osea 0) si no le pega a ninguna
wall ballHitWall();

//devuelve la pared que esta tocando y false(osea 0) si no esta tocando
wall barHitWall();

//Devuelve la pos en la mariz del bloque que choco y que borde del bloque, sino choco devuelve falso
int * ballHitBlock();


//devuelve que borde del bloque esta chocando la pelota, se devuelve la pared que representa ese borde(ya que actua de la misma forma)
wall ballTouchingWall(int c, int r);

//Se fija si la pelota esta o no en el rango y del bloque correspondiente a {c,r} (column , row)
int ballBetweenYSides(int * auxPos, int c, int r);

//Se fija si la pelota esta o no en el rango x del bloque correspondiente a {c,r} (column , row)
int ballBetweenXSides(int * auxPos, int c, int r);

int finishGame(int time_past, int result);
#endif
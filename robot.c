/*
* Robot.c
* Gruppe A422
* Autonom Robot
* *
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
 
/* Size of Room*/
#define WIDTH 30
#define HEIGHT 17
 
/* ASCII Objects */
#define WALL 178
#define SOLID 177
#define OPEN 176
#define ROBOT 1
 
#define CLEANED 'X'
 
/* Defines movement directions */
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
/* Defines the struct for our tiles */
struct field{
    int type;
    int isCleaned;
    int isRobot;
    int visits;
  };
 
/*Makes a room with height and width values*/
void InitRoom(struct field room[HEIGHT][WIDTH]);
/* Prints Room to Console */
void PrintRoom(struct field room[HEIGHT][WIDTH]);
/* Makes a robot in tile x,y */
void SpawnRobot(int x, int y, struct field room[HEIGHT][WIDTH]);
/* Moves the Robot one tile in a given direction */
int MoveRobot(int dir, int (*y), int (*x), struct field room[HEIGHT][WIDTH]);
/* Handles the robots thinking */
int CalcNextMove(int (*dir), int *y, int *x, int *prev_wall, struct field room[HEIGHT][WIDTH]);
int FindLowestCost(int *CostArray);
 
 
 
int main (void)
{
  struct field room[HEIGHT][WIDTH];
 
  int x = 1, y = 1, i = 0, dir = 3, direction = 0, previous = 0;
 
  InitRoom(room);
  SpawnRobot(x,y,room);
  
 
  for(i=0; i < 600; i++)
  {
    system("cls");
    PrintRoom(room);
    direction = (CalcNextMove(&dir, &y, &x, &previous, room));
    MoveRobot(direction, &y,&x, room);
    if(i % 5 == 0)
      getchar();
  }
 
 
 
 
  printf("\nDone\n");
 
   return 0;
}
 
void InitRoom(struct field room[HEIGHT][WIDTH])
{
  int j = 0, i = 0;
  for(j = 0; j < HEIGHT; ++j) /* Height */
  {
    for(i = 0; i < WIDTH ; ++i) /* Width */
    {
      room[j][i].isRobot = 0;
      room[j][i].isCleaned = 0;
      
     
      if(j == 0 || j == HEIGHT-1 || i == 0 || i == WIDTH-1)
      {
         room[j][i].visits = 10;
         room[j][i].type = 178;
      }
      else
      {
         room[j][i].visits = 1;
         room[j][i].type = 176;
      }
    }
  }
  
  room[10][1].type = SOLID;
  room[10][2].type = SOLID;
  room[10][3].type = SOLID;
  room[10][4].type = SOLID;
  room[10][5].type = SOLID;
  room[10][6].type = SOLID;
  
  
  
}
 
void PrintRoom(struct field room[HEIGHT][WIDTH])
{
  int j = 0, i = 0;
  for(j = 0; j < HEIGHT; ++j) /* Height */
  {
    for(i = 0; i < WIDTH; ++i) /* Width */
    {
      if(room[j][i].isRobot)
        printf("%c",ROBOT);
      else if (room[j][i].isCleaned)
        printf("%c", CLEANED);
      else
      printf("%c",room[j][i].type);
    }
      printf("\n");
  }
  printf("\n");
}
 
void SpawnRobot(int x, int y, struct field room[HEIGHT][WIDTH])
{
  if(room[y][x].type == WALL || room[y][x].type == SOLID)
    printf("ERROR: Cannot Spawn robot at position as it is either solid or a wall\n");
  else
    room[y][x].isRobot = 1;
}
 
int MoveRobot(int dir, int (*y), int (*x), struct field room[HEIGHT][WIDTH])
{
  int tempx = (*x);
  int tempy = (*y);
  int error = 0;
 
  if(dir == UP && (*y)-1 >= 0 && (*y)-1 <= HEIGHT && room[(*y)-1][(*x)].type == OPEN)
    (*y) -= 1;
  else if(dir == RIGHT && (*x)+1 >= 0 && (*x)+1 <= WIDTH && room[(*y)][(*x)+1].type == OPEN)
    (*x) += 1;
  else if(dir == DOWN && (*y)+1 >= 0 && (*y)+1 <= HEIGHT && room[(*y)+1][(*x)].type == OPEN)
    (*y) += 1;
  else if (dir == LEFT && (*x)-1 >= 0 && (*x)-1 <= WIDTH && room[(*y)][(*x)-1].type == OPEN)
    (*x) -= 1;
  else
  {
    printf("Collision\n");
    error = 1;
    if(dir == UP)
      room[(*y)-1][*x].visits += 1;
    else if(dir == RIGHT)
      room[*y][(*x)+1].visits += 1;
    else if(dir == DOWN)
      room[(*y)+1][*x].visits += 1;
    else if(dir == LEFT)
      room[*y][(*x)-1].visits += 1;
  }
  
  if(!error)
  {
    room[(*y)][(*x)].visits += 3;
    room[tempy][tempx].isRobot = 0;
    room[tempy][tempx].isCleaned = 1;
    room[(*y)][(*x)].isRobot = 1;
  }
  
  return error;
}
 
 
int CalcNextMove(int (*dir), int *y, int *x, int *prev_wall, struct field room[HEIGHT][WIDTH])
{
  int cost[4];
  /*int CostUp = 1, CostRight = 2, CostLeft = 2, CostDown = 3;*/
  int cost_fwd = 1, cost_side = 2, cost_back = 3;
 
  
  
  
  cost[*dir] = cost_fwd;
  cost[(*dir+1) % 4] = cost_side;
  cost[(*dir+2) % 4] = cost_back;
  cost[(*dir+3) % 4] = cost_side;
  
  cost[0] *= room[(*y)-1][(*x)-0].visits * (room[(*y)-1][(*x)-0].type - 170);
  cost[1] *= room[(*y)-0][(*x)+1].visits * (room[(*y)-0][(*x)+1].type - 170);
  cost[2] *= room[(*y)+1][(*x)-0].visits * (room[(*y)+1][(*x)-0].type - 170);
  cost[3] *= room[(*y)-0][(*x)-1].visits * (room[(*y)-0][(*x)-1].type - 170);
  
  
  
  
  (*dir) = FindLowestCost(cost);
  
  printf("UP:\t%d\nRIGHT:\t%d\nDOWN:\t%d\nLEFT:\t%d\n", cost[0], cost[1], cost[2], cost[3]);
  return (*dir);
}
 
int FindLowestCost(int *CostArray){
  int i=0, lowest=9000, entry=0;;
 
  for(i=0;i<=3; i++){
    if(CostArray[i]<lowest){
      lowest = CostArray[i];
      entry = i;
    }
  }
  return entry;
}
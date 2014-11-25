/*
* Robot.c
* Gruppe A422
* Autonom Robot
* *
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Size of Room*/
#define WIDTH 16
#define HEIGHT 8
 
/* ASCII Objects */
#define WALL 178
#define SOLID 177
#define OPEN 176
#define ROBOT 1

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
  };
 
/*Makes a room with height and width values*/
void InitRoom(struct field room[HEIGHT][WIDTH]);
/* Prints Room to Console */
void PrintRoom(struct field room[HEIGHT][WIDTH]);
/* Makes a robot in tile x,y */
void SpawnRobot(int x, int y, struct field room[HEIGHT][WIDTH]);
/* Moves the Robot one tile in a given direction */
void MoveRobot(enum moveDirection dir, int *y, int *x, struct field room[HEIGHT][WIDTH]);
 
 
int main (void)
{
  struct field room[HEIGHT][WIDTH];

  int x = 1, y = 1;
  
  InitRoom(room);
  SpawnRobot(x,y,room);
  PrintRoom(room);
  MoveRobot(UP, y,x, room);
  
  
 
  printf("Done\n");
 
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
        room[j][i].type = 178;
      else
        room[j][i].type = 176;
    }
  }
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

void MoveRobot(int dir, int *y, int *x, struct field room[HEIGHT][WIDTH])
{
  int tempx = *x;
  int tempy = *y;
  int error = 0;
  
  if(dir == UP && *y-1 >= 0 && *y-1 <= HEIGHT && room[*y-1][*x].type == OPEN)
    *y -= 1;
  else if(dir == RIGHT && *x+1 >= 0 && *x+1 <= WIDTH && room[*y][*x+1].type == OPEN)
    *x += 1;
  else if(dir == DOWN && *y+1 >= 0 && *y+1 <= HEIGHT && room[*y+1][*x].type == OPEN)
    *y += 1;
  else if (dir == LEFT && *x-1 >= 0 && *x-1 <= HEIGHT && room[*y][*x-1].type == OPEN)
    *x -= 1;
  else
  {
    printf("ERROR MOVING ROBOT");
    error = 1;
  }
  
  if(!error)
  {
    room[tempy][tempx].isRobot = 0;
    room[*y][*x].isRobot = 1;
  }
}
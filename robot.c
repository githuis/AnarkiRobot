/*
* Robot.c
* Gruppe A422
* Autonom Robot
*
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
#define WIDTH 16
#define HEIGHT 8
 
/* ASCII Objects */
#define WALL 178
#define SOLID 177
#define OPEN 176
#define ROBOT 1
 
struct field{
    int x;
    int y;
    int type;
    int isCleaned;
    int isRobot;
  };  
 
/*Makes a room with height and width values*/
void InitRoom(struct field room[HEIGHT][WIDTH]);
/* Prints Room to Console */
void PrintRoom(struct field room[HEIGHT][WIDTH]);
/* Defines the struct for our tiles */
 
 
int main (void)
{
 
  struct field room[HEIGHT][WIDTH];
 
  InitRoom(room);
  PrintRoom(room);
  
 
 
  printf("lol");
 
   return 0;
}
 
void InitRoom(struct field room[HEIGHT][WIDTH])
{
  int j = 0, i = 0;
  for(j = 0; j < HEIGHT; ++j) /* Height */
  {
    for(i = 0; i < WIDTH ; ++i) /* Width */
    {
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
      printf("%c",room[j][i].type);
    }
      printf("\n");
  }
}
void SpawnRobot(int x, int y, struct field room[HEIGHT][WIDTH])
{
  if(room[y][x].type == WALL || room[y][x].type == SOLID)
    printf("ERROR: Cannot Spawn robot at position as it is either solid or a wall\n");
  else
    ;
}
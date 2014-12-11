/*********************
* Program   Robot.c  *
* Gruppe:   A422     *
* Autonom Robot      *
**********************/
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
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

/* How many steps does the robot take */
#define NUMBER_OF_STEPS 550

/* Defines the struct for our tiles */
struct field{
    int type;
    int is_cleaned;
    int is_robot;
    int visits;
    int time;
  };

/* User dialogue for selecting algorithm */
int set_algorithm();
/*Makes a room with height and width values*/
void init_room(struct field room[HEIGHT][WIDTH]);
/* Prints Room to Console */
void print_room(struct field room[HEIGHT][WIDTH]);
/* Makes a robot in tile x,y */
void spawn_robot(int x, int y, struct field room[HEIGHT][WIDTH]);
/* Moves the Robot one tile in a given direction */
int move_robot(int dir, int *y, int *x, struct field room[HEIGHT][WIDTH]);
/* Handles the robots' 'thinking'*/
int calc_next_move(int *dir, int *y, int *x, int *prev_wall, int *cur_wall, int *time, struct field room[HEIGHT][WIDTH]);
/* Finds  the lowest cost in a 4-long array*/
int find_lowest_cost(int *cost_array);
/* Finds a solid */
int return_solid(int *y, int *x, struct field room[HEIGHT][WIDTH]);
 
int steps = 0, algorithm = 0; 

int main (void)
{
  struct field room[HEIGHT][WIDTH];
  int x = 1, y = 1, i = 0, dir = 0, direction = 0, previous = 0, current, time = 0;
  
  algorithm = set_algorithm();
  if(algorithm == 1) time = 800;
  
  init_room(room);
  spawn_robot(x,y,room);
  
  for(i=0; i < NUMBER_OF_STEPS; i++)
  {
    print_room(room);
    direction = calc_next_move(&dir, &y, &x, &previous, &current, &time, room);
    move_robot(direction, &y,&x, room);
    steps++;
    getchar();
  }
 
  return 0;
}

int set_algorithm()
{
  int rtn = 0;
  printf("Please select which algorithm you wish to run\n\t0. Cost and memorybased\n\t1. Cost and timebased\n\t2. Costbased with awareness\n\n> ");
  scanf(" %d", &rtn);
  if(rtn > -1 && rtn < 3)
    return rtn;
  else
  {
    printf("Error selecting algorithm");
    exit(0);
  }
  return 0;
}
 
void init_room(struct field room[HEIGHT][WIDTH])
{
  int j = 0, i = 0;
  for(j = 0; j < HEIGHT; ++j) /* Height */
  {
    for(i = 0; i < WIDTH ; ++i) /* Width */
    {
      room[j][i].is_robot = 0;
      room[j][i].is_cleaned = 0;
      room[j][i].time = 1;
      
      if(j == 0 || j == HEIGHT-1 || i == 0 || i == WIDTH-1)
      {
         room[j][i].visits = 10;
         room[j][i].type = WALL;
      }
      else
      {
         room[j][i].visits = 1;
         room[j][i].type = OPEN;
      }
    }
  }
    
  for(i=1;i<=8;i++)
  {
    room[8][i].type = SOLID;
    room[8][i].visits = 10;
    room[8][i].time = SOLID;
    
    room[12][i+5].type = SOLID;
    room[12][i+5].visits = 10;
    room[12][i+5].time = SOLID;
    
    room[i][12].type = SOLID;
    room[i][12].visits = 10;
    room[i][12].time = SOLID;
    
    room[i+7][24].type = SOLID;
    room[i+7][24].visits = 10;
    room[i+7][24].time = SOLID;
  }
}
 
void print_room(struct field room[HEIGHT][WIDTH])
{
  int j = 0, i = 0;
  for(j = 0; j < HEIGHT; ++j) /* Height */
  {
    for(i = 0; i < WIDTH; ++i) /* Width */
    {
      if(room[j][i].is_robot)
        printf("%c",ROBOT);
      else if (room[j][i].is_cleaned)
        printf("%c", CLEANED);
      else
      printf("%c",room[j][i].type);
    }
      printf("\n");
  }
  
}
 
void spawn_robot(int x, int y, struct field room[HEIGHT][WIDTH])
{
  if(room[y][x].type == WALL || room[y][x].type == SOLID)
    printf("ERROR: Cannot Spawn robot at position as it is either solid or a wall\n");
  else
    room[y][x].is_robot = 1;
}
 
int move_robot(int dir, int *y, int *x, struct field room[HEIGHT][WIDTH])
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
  else if (dir == LEFT && *x-1 >= 0 && *x-1 <= WIDTH && room[*y][*x-1].type == OPEN)
    *x -= 1;
  else
  {
    printf("Collision\n");
    error = 1;
    if(dir == UP)
      room[*y-1][*x].visits += 3;
    else if(dir == RIGHT)
      room[*y][*x+1].visits += 3;
    else if(dir == DOWN)
      room[*y+1][*x].visits += 3;
    else if(dir == LEFT)
      room[*y][*x-1].visits += 3;
  }
  
  if(!error)
  {
    room[*y][*x].visits += 3;
    room[tempy][tempx].is_robot = 0;
    room[tempy][tempx].is_cleaned = 1;
    room[*y][*x].is_robot = 1;
  }
  
  return error;
}
 
int calc_next_move(int (*dir), int *y, int *x, int *prev_wall, int *cur_wall, int *time, struct field room[HEIGHT][WIDTH])
{
  int cost[4];
  int cost_fwd = 1, cost_side = 2, cost_back = 3;
  *prev_wall = *cur_wall;
  *cur_wall = 5;
  
  *cur_wall = return_solid(y,x, room);
  
  cost[*dir] = cost_fwd;
  cost[(*dir+1) % 4] = cost_side;
  cost[(*dir+2) % 4] = cost_back;
  cost[(*dir+3) % 4] = cost_side;
  
  cost[0] *= room[(*y)-1][(*x)-0].visits * (room[(*y)-1][(*x)-0].type - 170) + (room[(*y)-1][(*x)-0].time);
  cost[1] *= room[(*y)-0][(*x)+1].visits * (room[(*y)-0][(*x)+1].type - 170) + (room[(*y)-0][(*x)+1].time);
  cost[2] *= room[(*y)+1][(*x)-0].visits * (room[(*y)+1][(*x)-0].type - 170) + (room[(*y)+1][(*x)-0].time);
  cost[3] *= room[(*y)-0][(*x)-1].visits * (room[(*y)-0][(*x)-1].type - 170) + (room[(*y)-0][(*x)-1].time);
  
  if(*prev_wall != 5 && *cur_wall == 5 && algorithm == 0)
  {
    cost[*prev_wall] = cost[*prev_wall] /5;
  }
  
  if(algorithm == 1)
  {
    room[*y][*x].time = (*time);
    (*time)--;
  }
  
  
  (*dir) = find_lowest_cost(cost);
  
  printf("Prev:\t%d\tCur:\t%d\tSteps:\t%d\nUP:\t%d\nRIGHT:\t%d\nDOWN:\t%d\nLEFT:\t%d\n",*prev_wall, *cur_wall, steps, cost[0], cost[1], cost[2], cost[3]);
  return (*dir);
}
 
int find_lowest_cost(int *cost_array){
  int i = 0, lowest = 900000000, entry = 0;
  
  if(algorithm != 1)
    for(i = 0; i <= 3; i++)
    {
      if(cost_array[i]<lowest){
        lowest = cost_array[i];
        entry = i;
      }
    }
  else
    for(i = 3; i >= 0; i--)
    {
      if(cost_array[i]<lowest){
        lowest = cost_array[i];
        entry = i;
      }
    }
    
  return entry;
}

int return_solid(int *y, int *x, struct field room[HEIGHT][WIDTH])
{
  if(room[*y-1][*x].type == SOLID )    
    return UP;                           
  else if (room[*y][*x+1].type == SOLID )
    return RIGHT;                        
  else if (room[*y+1][*x].type == SOLID)
    return DOWN;                         
  else if (room[*y][*x-1].type == SOLID)
    return LEFT;
    
  return 5;
}
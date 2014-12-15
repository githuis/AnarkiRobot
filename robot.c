/*********************
* Program   Robot.c  *
* Gruppe:   A422     *
* Autonom Robot      *
**********************/
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
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
#define NUMBER_OF_STEPS 700

/* Defines the struct for our tiles */
struct field{
    int type;
    int is_cleaned;
    int is_robot;
    int weight;
    double time;
  };

/* User dialogue for selecting algorithm */
int set_algorithm();
/* User dialogue for selection the robots starting position */
void set_start_position(int *x, int *y);
/*Makes a room with height and width values*/
void init_room(struct field room[HEIGHT][WIDTH]);
/* Prints Room to Console */
void print_room(struct field room[HEIGHT][WIDTH]);
/* Makes a robot in tile x,y */
void spawn_robot(int x, int y, struct field room[HEIGHT][WIDTH]);
/* Moves the Robot one tile in a given direction */
int move_robot(int dir, int *y, int *x, struct field room[HEIGHT][WIDTH]);
/* Handles the robots' 'thinking'*/
int calc_next_move(int *dir, int *y, int *x, int *prev_wall, int *cur_wall, double *time, struct field room[HEIGHT][WIDTH]);
/* Finds  the lowest cost in a 4-long array*/
int find_lowest_cost(int *cost_array);
/* Finds a solid */
int return_solid(int *y, int *x, struct field room[HEIGHT][WIDTH]);
 
int steps = 0, algorithm = 0; 

int main (void)
{
  struct field room[HEIGHT][WIDTH];
  int x = 1, y = 1, i = 0, dir = 3, direction = 0, previous = 0, current = 0;
  double time_spent = 0;
  
  srand(time(NULL));
  
  algorithm = set_algorithm();
  if(algorithm == 1) 
    time_spent = 8;
  
  set_start_position(&x,&y);
  init_room(room);
  spawn_robot(x,y,room);
  
  for(i = 0; i < NUMBER_OF_STEPS; i++)
  {
    getchar();
    system("cls");
    print_room(room);
    direction = calc_next_move(&dir, &y, &x, &previous, &current, &time_spent, room);
    move_robot(direction, &y,&x, room);
    steps++;
  }
 
  return 0;
}

int set_algorithm()
{
  int rtn = 0;
  printf("Please select which algorithm you wish to run\n\t0. Cost and memorybased\n\t1. Cost and timebased\n\t2. Costbased with awareness\n\t3. For the random algorithm\n\n> ");
  scanf(" %d", &rtn);
  if(rtn > -1 && rtn < 4)
    return rtn;
  else
  {
    printf("Error selecting algorithm, Quitting\n");
    exit(0);
  }
  return 0;
}

void set_start_position(int *x, int *y)
{
  int sel;
  printf("Please select a starting position\n");
  printf("\t0. Top Left corner\n\t1. Top Right corner\n\t2. Bottom Right corner\n\t3. Bottom Left corner\n\t4. Middle of room\n");
  scanf(" %d", &sel);
  switch(sel)
  {
    case(0):
      (*x) = 1;
      (*y) = 1;
      break;
    case(1):
      (*x) = 28;
      (*y) = 1;
      break;
    case(2):
      (*x) = 28;
      (*y) = 15;
      break;
    case(3):
      (*x) = 1;
      (*y) = 15;
      break;
    case(4):
      (*x) = 15;
      (*y) = 8;
      break;
    default:
      printf("Invalid selection, starting in position (1,1)");
      break;
  }
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
         room[j][i].weight = 30;
         room[j][i].type = WALL;
      }
      else
      {
         room[j][i].weight = 1;
         room[j][i].type = OPEN;
      }
    }
  }
    
  for(i=1;i<=8;i++)
  {
    room[8][i].type = SOLID;
    room[8][i].weight = 30;
    
    room[12][i+5].type = SOLID;
    room[12][i+5].weight = 30;
    
    room[i][12].type = SOLID;
    room[i][12].weight = 30;
    
    room[i+7][24].type = SOLID;
    room[i+7][24].weight = 30;
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
  {
    room[y][x].is_robot = 1;
    room[y][x].weight += 3;
  }
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
      room[*y-1][*x].weight += 3;
    else if(dir == RIGHT)
      room[*y][*x+1].weight += 3;
    else if(dir == DOWN)
      room[*y+1][*x].weight += 3;
    else if(dir == LEFT)
      room[*y][*x-1].weight += 3;
  }
  
  if(!error)
  {
    room[*y][*x].weight += 3;
    room[tempy][tempx].is_robot = 0;
    room[tempy][tempx].is_cleaned = 1;
    room[*y][*x].is_robot = 1;
  }
  
  return error;
}
 
int calc_next_move(int (*dir), int *y, int *x, int *prev_wall, int *cur_wall, double *time, struct field room[HEIGHT][WIDTH])
{
  int cost[4], cost_fwd = 1, cost_side = 2, cost_back = 3, s_check = 1, temp = 0;
  int wall_up = 0, wall_down = 0, wall_right = 0, wall_left = 0;
  
  if(algorithm == 3)
  {
    temp = *dir;
    (*dir) = rand()%4;
    while(temp == (*dir+2)%4)
      (*dir) = rand()%4;
    return (*dir);
  }
  
  *prev_wall = *cur_wall;
  *cur_wall = 5;
  
  *cur_wall = return_solid(y,x, room);
  
  cost[*dir] = cost_fwd;
  cost[(*dir+1) % 4] = cost_side;
  cost[(*dir+2) % 4] = cost_back;
  cost[(*dir+3) % 4] = cost_side;
  
  cost[0] *= room[(*y)-1][(*x)-0].weight *(room[(*y)-1][(*x)-0].type - 170) + (room[(*y)-1][(*x)-0].time);
  cost[1] *= room[(*y)-0][(*x)+1].weight *(room[(*y)-0][(*x)+1].type - 170) + (room[(*y)-0][(*x)+1].time);
  cost[2] *= room[(*y)+1][(*x)-0].weight *(room[(*y)+1][(*x)-0].type - 170) + (room[(*y)+1][(*x)-0].time);
  cost[3] *= room[(*y)-0][(*x)-1].weight *(room[(*y)-0][(*x)-1].type - 170) + (room[(*y)-0][(*x)-1].time);
  
  if(*prev_wall != 5 && *cur_wall == 5 && algorithm == 0)
    cost[*prev_wall] = cost[*prev_wall] /5;
  
  if(algorithm == 1) 
  {
    room[*y][*x].time = (*time);
    (*time) -= 0.01;
  }
  
  if(algorithm == 2)
  {
    cost[0] = room[(*y)-1][(*x)-0].weight * 10 + 2; 
    cost[1] = room[(*y)-0][(*x)+1].weight * 10 + 1;
    cost[2] = room[(*y)+1][(*x)-0].weight * 10 + 4;
    cost[3] = room[(*y)-0][(*x)-1].weight * 10 + 3; 
  
    if(room[(*y)-1][(*x)-0].weight > 1 && room[(*y)-0][(*x)+1].weight > 1 && room[(*y)+1][(*x)-0].weight > 1 && room[(*y)-0][(*x)-1].weight > 1)
    {
      printf("Scanning for uncleaned space\n");
      while(1)
      {
        if(room[(*y)-s_check][(*x)-0].type == WALL || room[(*y)-s_check][(*x)-0].type == SOLID)
          wall_up = 1;
        if(room[(*y)-0][(*x)+s_check].type == WALL || room[(*y)-0][(*x)+s_check].type == SOLID)
          wall_right = 1;
        if(room[(*y)+s_check][(*x)-0].type == WALL || room[(*y)+s_check][(*x)-0].type == SOLID)
          wall_down = 1;
        if(room[(*y)-0][(*x)-s_check].type == WALL || room[(*y)-0][(*x)-s_check].type == SOLID)
          wall_left = 1;

        if(room[(*y)-s_check][(*x)-0].weight == 1 && wall_up == 0)
          return (UP);
        if(room[(*y)-0][(*x)+s_check].weight == 1 && wall_right == 0)
          return (RIGHT);
        if(room[(*y)+s_check][(*x)-0].weight == 1 && wall_down == 0)
          return (DOWN);
        if(room[(*y)-0][(*x)-s_check].weight == 1 && wall_left == 0)
          return (LEFT);
        
        if(wall_up==1 && wall_down==1 && wall_right==1 && wall_left==1)
          break;
        s_check++;
      }
    }
  }
  
  (*dir) = find_lowest_cost(cost);
  
  printf("Prev:\t%d\tCur:\t%d\tSteps:\t%d\nUP:\t%d\nRIGHT:\t%d\nDOWN:\t%d\nLEFT:\t%d\n",*prev_wall, *cur_wall, steps, cost[0], cost[1], cost[2], cost[3]);
  return (*dir);
}
 
int find_lowest_cost(int *cost_array){
  int i = 0, lowest = 90000, entry = 0;
  
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
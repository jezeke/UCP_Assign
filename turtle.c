/* turtle.c
#AUTHOR: Jhi Morris (19173632)
#MODIFIED: 18-10-18
#PURPOSE: Manages the parsing of strings for execution of turtle commands.
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "turtle.h"
#include "effects.h"
#include "fileio.h"

/*to add additional commands add their text value to cmpArray in checkCommand(),
  increment NUMCOMMANDS, make a #define for their respective index value in
  cmpArray; then create another CASE in doTurtle using the constant*/
#define NUMCOMMANDS 6
#define CMD_ROTATE 0
#define CMD_MOVE 1
#define CMD_DRAW 2
#define CMD_FG 3
#define CMD_BG 4
#define CMD_PATTERN 5


#define LOGMAX 80 /*maximum log entry size*/
#define PI acos(-1.0) /*bit of a hack*/

typedef int (CommandFunc)(TurtleState *turtle, char* string);

/* parseCommands
#RETURN: int (error code- returns -2 if drawing cursor is put into an invalid
  location. in strict mode this returns 0 if successful and -1 if unsuccesful,
  in non-strict mode this returns the number of skipped lines)
#ARGUMENTS: LinkedList (list of strings containing lines of commands),
  int (strict - true or false determining behaviour regarding invalid lines)
#PURPOSE: pumps the metal and makes this program actually do something*/
int parseCommands(LinkedList *list, int strict)
{
  CommandFunc *command;
  LinkedListNode *currNode = list->head;
  int errCode = 0;
  int lastErr;
  int done = 0; /*false*/

  TurtleState turtle;
  turtle.angle = 0.0; /*initial values*/
  turtle.x = 0.0;
  turtle.y = 0.0;
  turtle.pattern = '+';

  #ifdef SIMPLE
  setFgColour(0); /*white on black*/
  setBgColour(7);
  #else
  setFgColour(7); /*black on white*/
  setBgColour(0);
  #endif

  clearScreen();

  while(currNode != NULL && !done)
  { /*iterate through linked list and parse + process*/
    lastErr = 0; /*reset flag*/

    /*could replace this switch with an array of func pointers. gonna keep this*/
    switch(checkCommand((char*)currNode->value))
    { /*check command to be executed in next step*/
      case CMD_ROTATE:
        command = &rotate;
        break;
      case CMD_MOVE:
        command = &move;
        break;
      case CMD_DRAW:
        command = &draw;
        break;
      case CMD_FG:
        command = &fg;
        break;
      case CMD_BG:
        command = &bg;
        break;
      case CMD_PATTERN:
        command = &pattern;
        break;
      default:
        if(strict)
        {
          done = 1;
          errCode = -1;
        }
        else
        {
          errCode++;
          lastErr = 1; /*so the execution step is skipped*/
        }
        break;
    }

    if(!done && !lastErr)
    { /*execution step*/
      if(command(&turtle, (char*)currNode->value))
      {
        if(strict)
        { /*if its in strict mode, the function stops where it is*/
          done = 1;
          errCode = -1;
        }
        else
        { /*otherwise just increment the number of lines ignored*/
          errCode++;
        }
      }
    }

    if(turtle.x <= -0.5 || turtle.y <= -0.5)
    {
      done = 1;
      errCode = -2;
    }

    currNode = currNode->next;
  }

  penDown();
  setFgColour(7); /*set colors back to normal again*/
  setBgColour(0);
  printf("\n");

  return errCode;
}

/* checkCommand
#RETURN: int (value assocated with the detected command. -1 for not found)
#ARGUMENTS: char* (string containing the command. anything after whitespace
  will be disregarded)
#PURPOSE: Creates a copy of the string and then compares it against an
  array of constants relating to known commands.*/
int checkCommand(char *string)
{
  char tempString[LINELENGTH];
  int i;
  int command = -1; /*default. stays unchanged if no match found*/
  const char *cmpArray[] = {"ROTATE", "MOVE", "DRAW", "FG", "BG", "PATTERN"};

  sscanf(string, "%s", tempString); /*seperate command from arguments, store in buffer*/
  toUpper(tempString);

  i = 0;
  while(i < NUMCOMMANDS && command == -1)
  { /*go through the array and check string against each*/
    if(!strcmp(tempString, cmpArray[i]))
    {
      command = i;
    }

    i++;
  }

  return command;
}

/* rotate
#RETURN: int (0 if all OK)
#ARGUMENTS: TurtleState* (struct containing turtle info), char* (args)
#PURPOSE: Gets double from command argument, executes rotate command
  updating turtle state.*/
int rotate(TurtleState *turtle, char *string)
{
  double newAngle;
  int invalid = 0;

  if(sscanf(string, "%*s %lf", &newAngle) == 1)
  {
    turtle->angle = fixAngle(newAngle + turtle->angle);
  }
  else
  {
    invalid = 1;
  }

  return invalid;
}

/* move
#RETURN: int (0 if all OK)
#ARGUMENTS: TurtleState* (struct containing turtle info), char* (args)
#PURPOSE: Gets double from command argument, executes move command
  updating turtle state.*/
int move(TurtleState *turtle, char *string)
{
  double startx, starty, magnitude;
  char logString[LOGMAX];
  int invalid = 0;

  if(sscanf(string, "%*s %lf", &magnitude) == 1)
  {
    startx = turtle->x;
    starty = turtle->y;
    updateCoords(&turtle->x, &turtle->y, turtle->angle, magnitude);

    sprintf(logString, "MOVE ( %4.3f, %4.3f)=( %4.3f, %4.3f)\n",
      startx, starty, turtle->x, turtle->y);
    makeLog(logString);
    #ifdef DEBUG
    fprintf(stderr, logString);
    #endif
  }
  else
  {
    invalid = 1;
  }

  return invalid;
}

/* draw
#RETURN: int (0 if all OK)
#ARGUMENTS: TurtleState* (struct containing turtle info), char* (args)
#PURPOSE: Gets double from command argument, executes draw command
  updating turtle state and drawing to the screen.*/
int draw(TurtleState *turtle, char *string)
{
  double startx, starty, magnitude;
  char logString[LOGMAX];
  int invalid = 0;


  if(sscanf(string, "%*s %lf", &magnitude) == 1)
  { /*TODO add error checking for DRAW seeing integers, FG seeing floats etc*/
    startx = turtle->x;
    starty = turtle->y;
    updateCoords(&turtle->x, &turtle->y, turtle->angle, magnitude-1);
      /*minus 1 to avoid drawing over previous pos*/

    line(roundDouble(startx), roundDouble(starty), roundDouble(turtle->x),
      roundDouble(turtle->y), &plotting, &turtle->pattern);
    updateCoords(&turtle->x, &turtle->y, turtle->angle, 1); /*add the missing 1*/

    sprintf(logString, "DRAW ( %4.3f, %4.3f)=( %4.3f, %4.3f)\n",
      startx, starty, turtle->x, turtle->y);
    makeLog(logString);
    #ifdef DEBUG
    fprintf(stderr, logString);
    #endif
  }
  else
  {
    invalid = 1;
  }

  return invalid;
}

/* fg
#RETURN: int (0 if all OK)
#ARGUMENTS: TurtleState* (struct containing turtle info), char* (args)
#PURPOSE: Gets int from command argument, executes FG command if not simple.*/
int fg(TurtleState *turtle, char *string)
{
  int invalid;

  #ifndef SIMPLE /*this is used to ignore FG commands during simple mode*/
    int fgColor;
    invalid = 0;
    if(sscanf(string, "%*s %d", &fgColor) == 1)
    {
      if(fgColor >= 0 || fgColor <= 15) /*valid range of FG colors*/
      { /*check if in valid range*/
        setFgColour(fgColor);
      }
      else
      {
        invalid = 1;
      }
    }
    else
    {
      invalid = 1;
    }
  #else
    invalid = 0;
  #endif

  return invalid;
}

/* bg
#RETURN: int (0 if all OK)
#ARGUMENTS: TurtleState* (struct containing turtle info), char* (args)
#PURPOSE: Gets int from command argument, executes BG command if not simple.*/
int bg(TurtleState *turtle, char *string)
{
  int invalid;

  #ifndef SIMPLE /*this is used to ignore BG commands during simple mode*/
    int bgColor;
    invalid = 0;

    if(sscanf(string, "%*s %d", &bgColor) == 1)
    {
      if(bgColor >= 0 && bgColor <= 7)
      { /*check if in valid range*/
        setBgColour(bgColor);
      }
      else
      {
        invalid = 1;
      }
    }
    else
    {
      invalid = 1;
    }
  #else
    invalid = 0;
  #endif

  return invalid;
}

/* pattern
#RETURN: int (0 if all OK)
#ARGUMENTS: TurtleState* (struct containing turtle info), char* (args)
#PURPOSE: Gets char from command argument, updates turtle state*/
int pattern(TurtleState *turtle, char *string)
{
  int invalid = 0;

  if(sscanf(string, "%*s %c", &turtle->pattern) != 1)
  {
    invalid = 1;
  }

  return invalid;
}

/* toUpper
#RETURN: none
#ARGUMENTS: char* (string)
#PURPOSE: Converts all lower-case letters in the string to upper-case.*/
void toUpper(char *string)
{
  int i = 0;
  while(*(string + i) != '\0')
  {
    if (*(string + i) >= 'a' && *(string + i) <= 'z')
    { /*make sure its a lowercase letter first*/
      *(string + i) -= 'a' - 'A'; /*subtract difference between lower and upper case*/
    }
    i++;
  }
}

/* roundDouble
#RETURN: int
#ARGUMENTS: double
#PURPOSE: Rounds a double to the nearest integer. */
int roundDouble(double const num)
{
  int out;

  if(num - floor(num) > 0.5)
  {
    out = ceil(num);
  }
  else
  {
    out = floor(num);
  }

  return out;
}

/* fixAngle
#RETURN: double (fixed angle)
#ARGUMENTS: double (angle)
#PURPOSE: Wraps an angle to be between 0 and 360 degrees*/
double fixAngle(double const angle)
{
  double angleOut;

  if(angle < 0)
  { /*fix to positive, get remainder of, then get complementary angle*/
    angleOut = 360 - fmod((angle * -1), 360.0);
  }
  else
  {
    angleOut = fmod(angle, 360.0); /*get remainder*/
  }

  return angleOut;
}

/* updateCoords
#RETURN: none
#ARGUMENTS: double* (x to be updated), double* (y to be updated),
  double (angle), double (magnitude)
#PURPOSE: Updates the x/y coordinates according to the angle of direction
  and magnitude of movement.*/
void updateCoords(double *x, double *y, const double angle, const double magnitude)
{ /*angle is converted to radians during calculations*/
  *x += magnitude*cos(PI / 180.0 * angle);
  *y -= magnitude*sin(PI / 180.0 * angle);
}

/* plotting
#RETURN: none
#ARGUMENTS: void* (pointer to character)
#PURPOSE: Prints the handed character to the screen at the cursor.*/
void plotting(void *c)
{
  char pattern = *((char*)c);
  printf("%c", pattern);
}

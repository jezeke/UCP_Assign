#ifndef TURTLE_H
#define TURTlE_H
#include "linked_list.h"

typedef struct TurtleState
{
  double angle;
  double x;
  double y;
  char pattern;
} TurtleState;

int checkCommand(char *string);

int parseCommands(LinkedList *list, int strict);

double fixAngle(double angle);

int rotate(TurtleState *turtle, char *string);

int draw(TurtleState *turtle, char *string);

int fg(TurtleState *turtle, char *string);

int bg(TurtleState *turtle, char *string);

int pattern(TurtleState *turtle, char *string);

int move(TurtleState *turtle, char *string);

void toUpper(char *string);

int roundDouble(double num);

void updateCoords(double *x, double *y, const double angle, const double magnitude);

void plotting(void *c);

#endif

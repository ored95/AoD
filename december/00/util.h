#ifndef __UTIL_H__
#define __UTIL_H__
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define MAX_TST 7
#define m       41
#define n       81

double approximate(double x, double x1, double x2, double y1, double y2);
double getBeliefCoef(double x, double x1, double x2);
double MSE(double test[n], double given[n]);

#endif
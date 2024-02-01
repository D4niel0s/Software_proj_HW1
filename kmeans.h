#ifndef KMEANS_H
#define KMEANS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*Included fot DBL_MAX*/
#include <float.h> 

typedef struct point{
    double *coords; /*Coordinates of the vector will be inserted here. (memory allocation in main)*/
    int dim; /*Dimension.*/

    int cluster; /*Cluster the point belongs to.*/
}Point;


int main(int, char **);
void KMeans(int, int, int, int,Point *, Point *);
int FindClosestCentroid(Point, Point *, int);
double dist(Point, Point);
void ADD(Point, Point);
void MULT(Point, double);


#endif
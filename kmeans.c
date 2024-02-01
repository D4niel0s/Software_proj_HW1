#include "KMeans.h"

int main(int argc, char **argv){
    printf("argc: %d, argv[1]: %s\n", argc, argv[1]);
    
    Point x;
    Point y;
    Point cents[] = {x,y};
    Point target;
    int clust;

    x.coords = (double *)malloc(sizeof(double) * 2);
    y.coords = (double *)malloc(sizeof(double) * 2);
    x.coords[0] = 1;
    x.coords[1] = 2;
    y.coords[0] = 4;
    y.coords[1] = 1;
    x.dim = 2;
    y.dim = 2;
    
    target.coords = (double *)malloc(sizeof(double) * 2);
    target.coords[0] = 2;
    target.coords[1] = 3;
    target.dim = 2;

    clust = FindClosestCentroid(target, cents, 2);

    printf("Closest centroid to point: (%d,%d) is: %d", target.coords[0], target.coords[1], clust);

    return 1;
}

/*K clusters, N points, d dimension, iter iterations, data - the points, centroids - the centroids.
//Assumes valid and allocated inputs.
//Updates centroids to be correct centroids for clusters.
//Note: If we want the clustering, we can check for each point which cluster is the closest (outside the function)*/
void KMeans(int K, int N, int d, int iter, Point *data, Point *centroids){
    int i; int j; int iterations = 0;
    double Epsilon = 0.001;
    short END_FLAG = 0;

    Point *KMEANS = (Point *)malloc(sizeof(Point) * K); /*The mean (each cluster is represented by index)*/
    Point *PREV_Centroids = (Point *)malloc(sizeof(Point) * K); /*The previous mean (each cluster is represented by index)*/
    double *Delta_vector = (double *)malloc(sizeof(double) * K); /*the differences between previous and current centroids.*/
    int *PtCounter = (int *)malloc(sizeof(int) * K); /*Number of points (each cluster is represented by index)*/

    /*Init KMEANS and PREV_Centroids*/
    for(i=0; i<K; ++i){
        KMEANS[i].coords = (double *)malloc(sizeof(double) * d);
        PREV_Centroids[i].coords = (double *)malloc(sizeof(double) * d);

        KMEANS[i].dim = d;
        KMEANS[i].cluster = -1;

        for(j=0; j<d; ++j){
            (KMEANS[i].coords)[j] = 0;
        }
    }

    /*Init centroids to be first k datapoints*/
    for(i=0; i<K; ++i){
        for(j=0; j<d; ++j){ /*initialize coords*/
            (centroids[i].coords)[j] = (data[i].coords)[j];
            (PREV_Centroids[i].coords)[j] = (data[i].coords)[j];
        }
        centroids[i].dim = d;
        centroids[i].cluster = -1; /*he centroids aren't a part of a cluster.*/

        PREV_Centroids[i].dim = d;
        PREV_Centroids[i].cluster = -1; /*The centroids aren't a part of a cluster.*/
    }

    do{
        /*Init means and No. of points to zero.*/
        for(i=0; i<K; ++i){
            PtCounter[i] = 0;

            for(j=0; j<d; ++j){
                (KMEANS[i].coords)[j] = 0;
            }
        }

        for(i=0; i<N; ++i){ 
            data[i].cluster = FindClosestCentroid(data[i], centroids, K);/*Categorize points to clusters*/
            ADD(KMEANS[data[i].cluster], data[i]); /*Add points to according sums*/
            PtCounter[data[i].cluster]++; /*Add to counter for according cluster*/
        }
           
        for(i=0; i<K; ++i){
            MULT(KMEANS[i], (1.0/(double)PtCounter[i])); /*Normalize the summations, creating the means*/

            for(j=0; j<d; ++j){ /*Update centroids.*/
                (centroids[i].coords)[j] = (KMEANS[i].coords)[j];
            }
        }

        /*Update delta_vector and PREV_Centroids*/
        for(i=0; i<K; ++i){
            Delta_vector[i] = dist(centroids[i], PREV_Centroids[i]);
            for(j=0; j<d; ++j){
                (PREV_Centroids[i].coords)[j] = (centroids[i].coords)[j];
            }
        }
        /*Check Delta < Epsilon (shouldn't be checked on first iteration)*/
        if(iterations > 0){
            END_FLAG = 1;
            for(i=0; i<K; ++i){
                if(Delta_vector[i] >= Epsilon){
                    END_FLAG = 0;
                }
            }
        }

        iterations++;
    }while(iterations < iter && END_FLAG == 0);
}

/*Finds closest centroid (denoted by cluster number) to a given point*/
int FindClosestCentroid(Point x, Point *centroids, int K){
    int i;
    int assigned;
    double mindist = DBL_MAX;

    for(i=0; i<K; ++i){
        if(dist(x, centroids[i]) < mindist){
            mindist = dist(x, centroids[i]);
            assigned = i;
        }
    }
    return assigned;
}


/*Computes distance between two given points.
//Assumes dimensions match.*/
double dist(Point a,Point b){
    double dist = 0;
    int i;
    
    for(i=0; i<a.dim; ++i){
        dist += pow(a.coords[i] - b.coords[i] , 2);
    }
    dist = sqrt(dist);

    return dist;
}

/*adds b to a (changes a)*/
void ADD(Point a, Point b){
    int i;
    for(i=0; i<a.dim; ++i){
        a.coords[i] += b.coords[i];
    }
}

/*multiplies a by scalar x (changes a)*/
void MULT(Point a, double x){
    int i;
    for(i=0; i<a.dim; ++i){
        a.coords[i] *= x;
    }
}

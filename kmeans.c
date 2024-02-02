#include "kmeans.h"

#define DEF_MAX_ITER 200

int main(int argc, char **argv){
    /*Declarations*/
    int i; int j; /*Loop variables*/
    char *line; /*Buffer to hold line*/
    size_t strSize = 0; /*For getline() syntax*/
    char *lnptr;

    int K; /*Number of clusters*/
    int N; /*Number of data-points*/
    int d; /*Dimension of each point*/
    int max_iter; /*Max iterations of K-means algorithm*/
    Point *centroids; /*Centroids pointer to be updateed by K-means*/
    Point *dataPoints; /*Data points array*/

    /*Check valid number of inpus and assign value to max_iter*/
    switch(argc){
        case 1:
        case 2:
        case 3:
            fprintf(stderr, "An Error Has Occurred\n");
            return 1; /*Break is not needed here since we get out of main*/
        case 4: /*Here max_iter gets default value - not provided*/
            max_iter = DEF_MAX_ITER;
            break;
        case 5:
            if(!(isInt(argv[4])) || atoi(argv[4]) >= 1000 || atoi(argv[4]) <= 1){ /*Check for valid input*/
                fprintf(stderr, "Invalid maximum iteration!\n");
                return 1; /*Break is not needed here since we get out of main*/
            }
            max_iter = atoi(argv[4]);
            break;
        default:
            fprintf(stderr, "An Error Has Occurred\n");
            return 1; /*Break is not needed here since we get out of main*/
    }

    /*Check for valid inputs*/
    if(!(isInt(argv[2])) || atoi(argv[2]) <= 1){
        fprintf(stderr, "Invalid number of points!\n");
        return 1;
    }
    if(!(isInt(argv[1])) || atoi(argv[1]) <= 1 || atoi(argv[1]) >= atoi(argv[2])){
        fprintf(stderr, "Invalid number of clusters!\n");
        return 1;
    }
    if(!(isInt(argv[3]))){
        fprintf(stderr, "Invalid dimension of point!\n");
        return 1;
    }

    /*Assign values to variables and allocate memory to pointers*/
    K = atoi(argv[1]);
    N = atoi(argv[2]);
    d = atoi(argv[3]);
    centroids = (Point *)malloc(sizeof(Point) * K);
    dataPoints = (Point *)malloc(sizeof(Point) * N);
    for(i=0; i<N; ++i){
        dataPoints[i].coords = (double *)malloc(sizeof(double) * d);
    }

    /*Take input from stdin into dataPoints*/
    for(i=0; i<N; ++i){
        getline(&line, &strSize, stdin);
        lnptr = line; /*Save pointer for start of line to free*/
        for(j=0; j<d; ++j){
            (dataPoints[i].coords)[j] = strtod(line, &line);
            line++; /*Skip comma*/
        }
        dataPoints[i].dim = d;
        dataPoints[i].cluster = -1;
        /*Free allocated line*/
        free(lnptr);
        strSize = 0;
    }

    /*Apply K-means algorithm to points*/
    KMeans(K,N,d,max_iter, dataPoints, centroids);

    /*Print new centroids*/
    for(i=0; i<K; ++i){
        for(j=0; j<d; ++j){
            printf("%.4f", (centroids[i].coords)[j]);
            if(j < d-1){
                printf(",");
            }
        }
        printf("\n");
    }
    /*Free allocated memory*/
    for(i=0; i<N; ++i){
        if(i<K){
            free(centroids[i].coords);
        }
        free(dataPoints[i].coords);
    }
    free(dataPoints);
    free(centroids);

    return 0;
}

/*K clusters, N points, d dimension, iter iterations, data - the points, centroids - the centroids.
//Assumes data is valid and allocated.
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

    /*Init KMEANS, centroids and PREV_Centroids*/
    for(i=0; i<K; ++i){
        centroids[i].coords = (double *)malloc(sizeof(double) * d);
        KMEANS[i].coords = (double *)malloc(sizeof(double) * d);
        PREV_Centroids[i].coords = (double *)malloc(sizeof(double) * d);

        KMEANS[i].dim = d;
        centroids[i].dim = d;
        PREV_Centroids[i].dim = d;
        
        KMEANS[i].cluster = -1;
        centroids[i].cluster = -1;
        PREV_Centroids[i].cluster = -1;

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
    /*Free memory allocated for local variables*/
    for(i=0; i<K; ++i){
        free(KMEANS[i].coords);
        free(PREV_Centroids[i].coords);
    }
    free(KMEANS);
    free(PREV_Centroids);
    free(Delta_vector);
    free(PtCounter);
    /*Centroids is not freed since it's the "Output" of the KMeans function*/
}

/*Finds closest centroid (denoted by cluster number) to a given point*/
int FindClosestCentroid(Point x, Point *centroids, int K){
    int i;
    int assigned = 0;
    double mindist = dist(x,centroids[0]);
    double curdist;
    for(i=0; i<K; ++i){
        curdist = dist(x,centroids[i]);
        if(curdist < mindist){
            mindist = curdist;
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

/*Checks if string represent an int*/
int isInt(char *s){
    int i;
    for (i = 0; s[i]!= '\0'; i++){
        if (!(48 <= s[i] && s[i] <= 57)){
            return 0;
        }
    }
    return 1;
}
# This mini-project is an implementation of the K-means clustering algorithm.

## The goal
  Given N data-points, the algorithm's goal is to divide them into K clusters.
  - Each cluster will be represented by it's centroid; At the end of the algorithm, the centroids will represent the centers of the final clustering.
  - Each data-point is a d-dimensional vector (Real number entries only).
  
## The algorithm
```
Init - Assign the initial centroids to be the first K data-points.
   
1. Assign each data-point to it's closest cluster (closest centroid).
2. Compute new centroids for the new clusters, where each centroid is the mean of all members of the cluster.
3. Repeat steps 1-2 until Convergence or until iteration limit is reached.
```
-------------------------------------------------------------------------------------------------------
#### Notes:
- There are variations of this algorithm where the initial choice is centroids is differrent (random etc.).
- The distance in step 1 is a degree of freedom and can be chosen according to the desired output. In this case I needed to implement the algorithm using euclidean distance.
- The mean (step 2) of d-dimensional data-points is identical to a "regular number" mean; (sum of points)/(number of points)
- <em>Convergence</em> - meaning that the change in all centroid's positions was smaller than some small value ε>0. (In my case, ε = 0.001)
- Iteration limit is the maximum iterations the user allows; Another degree of freedom in the algorithm. (In my case, 1<max_iter<1000)

# My implementation
I was required to implement the K-means algorithm in both C and Python. 
Each implementation has it's nuances and uses different tools to reach a similar goal.

## C implementation
  Each point is represented by ```struct``` that includes:
  - Array (pointer) of double type ```coordinates```. (array of length d)
  - The point's ```dimension```. (d)
  - The point's assigned ```cluster```.

  The algorithm recieves the arguments K,N,d, and the max iteration number (optional), as well as the input data-points, and a pre-allocated centroids pointer.<br>
  d-dimensional point arithmetic was implemented using the ADD and MULT functions, which change one of the input points. (This isn't optimal but it's a very convenient approach here)<br>
  All double type values are printed up to 4 decimal places of accuracy. (%.4f flag in printf)<br>
  #### Note:
  The algorithm here is implemented using a ```void``` function, and it's "output" is given in the form of updating the centroids.

  ### Compiling and running
  The mini-project compiles in the ansi-C C90 standard. It is meant to be ran in a linux environment.
  For example, to compile the mini-project using ```GCC```, use the command:
  ```
  gcc -ansi -Wall -Wextra -Werror -pedantic-errors kmeans.c -o kmeans -lm
  ```
  Which creates an executable named "kmeans"

  To run the mini-project, enter the following command:
  ```
  ./kmeans <K> <N> <d> [max_iter] < inpFile.txt
  ```
  Where inpFile.txt is a text file including the data points to be clustered, in the following format:
  - Each row represents a single point.
  - The coordinates are real numbers, separated by commas.
  - No extra whitespaces.

After running, the algorithm will output the final clustering centroids to ```STDIN```.
To check each point's cluster, we can use the ```FindClosestCentroid()``` function which is included.
  
## Python implementation

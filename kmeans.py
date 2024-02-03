import math
import sys

class Point:
    def __init__(self, coords, dim, cluster):
        self.coords = coords
        self.dim = dim
        self.cluster = cluster    

def main():
    #Need to make checks
    K = int(sys.argv[1]) # Number of clusters
    N = int(sys.argv[2]) # Number of points
    d = int(sys.argv[3]) # Dimension of points
    iter = int(sys.argv[4]) # Max iteration number
    
    data = [Point([0]*d,d,-1)]*N # Initialize data array to default values
    file = open(sys.argv[5],'r')

    for i in range(N):
        line = file.readline()
        args = line.split(",")
        data[i]= Point(list(map(float,args)), d, -1) #map function applies float() to each element of args, then turn it to a list using list()

    centroids = KMeans(K, N, d, iter, data)
    
    #Print centroids
    for i in range(K):
        for j in range(d):
            print("{:.4f}".format(centroids[i].coords[j]), end='')
            if(j<d-1):
                print(',',end='')
        print()



#Main logic of kmeans algorithm (Returns new centroids)
def KMeans(K, N, d, iter, data):
    Epsilon = 0.001 #Convergence threshold
    iteration = 0 #Counts iterations
    DeltaVector = [0]*K #Saves distance difference between previous and current centroids

    #Init centroids to be first K dataPoints
    centroids = data[: K].copy()

    while True:
        #Reset means and counters for next iteration
        PtCtr = [0]*K #Counts points in each cluster (each index represents a cluster)
        KMEANS = [Point([0]*d,d,-1)]*K #The K Means (each index represents a cluster)

        #Assign each point to cluster
        for i in range(N):
            data[i].cluster = FindClosestCentroid(data[i], centroids, d)
            PtCtr[data[i].cluster] += 1 #Point assigned to cluster - increase count
            KMEANS[data[i].cluster] = ADD(data[i], KMEANS[data[i].cluster], d) #Add point to according mean's sum

        #Normalize mean for each cluster and compute difference vector
        for i in range(K):
            KMEANS[i] = MULT(KMEANS[i], 1.0/float(PtCtr[i]), d)
            DeltaVector[i] = dist(centroids[i], KMEANS[i], d)

        #Update the centroids
        centroids = KMEANS.copy()

        #Check if converged
        EXIT_FLAG = True
        for i in range(K):
            if(DeltaVector[i] >= Epsilon):
                EXIT_FLAG = False

        iteration += 1
        if(iteration == iter or EXIT_FLAG == True):
            break

    return centroids

#Finds and return closest centroid to given point x (denoted by cluster number)
def FindClosestCentroid(x, centroids, dim):
    assigned = 0
    minDist = dist(x, centroids[0], dim)
    for i in range(len(centroids)):
        curDist = dist(x, centroids[i], dim)
        if(curDist < minDist):
            minDist = curDist
            assigned = i
    
    return assigned

#Returns distance between two points
def dist(x,y, dim):
    dist = 0
    for i in range(dim):
        dist += pow(x.coords[i] - y.coords[i], 2)

    dist = math.sqrt(dist)
    return dist

#Returns a new point that is the sum of x and y
def ADD(x,y,dim):
    res = Point([0]*dim, dim, -1)
    for i in range(dim):
        res.coords[i] = x.coords[i] + y.coords[i]

    return res

#Returns a new point that is x multiplied by the scalar z
def MULT(x,z, dim):
    res = Point([0]*dim, dim, -1)
    for i in range(dim):
        res.coords[i] = x.coords[i] * z

    return res

if __name__ == "__main__":
    main()
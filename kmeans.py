import math

class Point:
    def __init__(self, coords, dim, cluster):
        self.coords = coords
        self.dim = dim
        self.cluster = cluster    


def main():
    x = Point([1,2],2,-1)
    y = Point([3,4],2,-1)
    z = Point([5,6],2,-1)
    w = Point([7,8],2,-1)
    p = Point([9,10],2,-1)
    centroids = KMeans(2,5,2,500,[x,y,z,w,p])
    for i in range(len(centroids)):
        print("{:.4},{:.4}".format(centroids[i].coords[0],centroids[i].coords[1]))


#Main logic of kmeans algorithm
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

        #Normalize mean for each cluster
        for i in range(K):
            KMEANS[i] = MULT(KMEANS[i], 1.0/float(PtCtr[i]), d)

        #Compute difference vector
        for i in range(K):
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
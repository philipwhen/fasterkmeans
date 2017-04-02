#include "dataset.h"
#include "general_functions.h"
#include "hamerly_kmeans.h"
#include "hplus_kmeans.h"
#include "elkan_kmeans.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <string>
#include <map>
#include <ctime>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <vector>

void execute(Kmeans *algorithm, Dataset *x, unsigned short k, unsigned short const *assignment,
    int xcNdx,
    int numthreads,
    int maxIterations,
    std::vector<int> *numItersHistory
    );

int main(int argc, char **argv){
    Dataset *x = NULL;
    unsigned short *assignment = NULL;
    unsigned short k;

    Kmeans *algorithm = NULL;

    std::vector<int> numItersHistory;

    int xcNdx = 0;
    int numthread = 1;
    int maxIterations = std::numeric_limits<int>::max();
    xcNdx++;
    std::string dataFilename;
//    dataFilename = "/home/philip/Desktop/dataset/sdm2010_datasets/mnist_60000_50.txt";
    dataFilename = "/home/philip/Desktop/dataset/sdm2010_datasets/test_data_uniform_1250000_2.txt";

    std::ifstream input(dataFilename.c_str());

    int n = 1250000;
    int d = 2;

    delete x;
    delete [] assignment;
    assignment = NULL;
    x = new Dataset(n,d);

    for (int i = 0; i < n * d; i++){
        input >> x->data[i];
    }
    xcNdx++;
    k = 50;
    std::string method = "kmeansplusplus";
    Dataset *c = NULL;
//    c = init_centers(*x, k);
    c = init_centers_kmeanspp_v2(*x, k);
    delete [] assignment;
    assignment = new unsigned short[x->n];
    for (int i = 0; i < x->n; i++){
        assignment[i] = 0;
    }

    assign(*x, *c, assignment);
    delete c;
    algorithm = new HplusKmeans();
    execute(algorithm, x, k, assignment, xcNdx,numthread, maxIterations, &numItersHistory);
//    std::cout<<(*(algorithm->centers))(1,1);
    delete algorithm;
    Kmeans *algorithm2 = new HamerlyKmeans();

    execute(algorithm2, x, k, assignment, xcNdx,numthread, maxIterations, &numItersHistory);
//    std::cout<<(*(algorithm2->centers))(1,1);
}

rusage get_time() {
    rusage now;
    getrusage(RUSAGE_SELF, &now);
    return now;
}


double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

int timeval_subtract(timeval *result, timeval *x, timeval *y) {
    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_usec < y->tv_usec) {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000) {
        int nsec = (x->tv_usec - y->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec -= nsec;
    }

    /* Compute the time remaining to wait.  tv_usec is certainly positive. */
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;

    /* Return 1 if result
     * is negative. */
    return x->tv_sec < y->tv_sec;
}

double elapsed_time(rusage *start) {
    rusage now;
    timeval diff;
    getrusage(RUSAGE_SELF, &now);
    timeval_subtract(&diff, &now.ru_utime, &start->ru_utime);

    return (double)diff.tv_sec + (double)diff.tv_usec / 1e6;
}

void execute(Kmeans *algorithm, Dataset *x, unsigned short k, unsigned short const *assignment,
    int xcNdx,
    int numthreads,
    int maxIterations,
    std::vector<int> *numItersHistory
    ){
    std::cout<< std::setw(35) << algorithm->getName() << "\t" << std::flush;
    // make a working copy
    unsigned short *workingassignment = new unsigned short[x->n];
    std::copy(assignment, assignment + x->n, workingassignment);
    rusage start_time = get_time();
    double start_wall_time = get_wall_time();
    algorithm->initialize(x,k,workingassignment, numthreads);
    int iterations = algorithm->run(maxIterations);
    double cluster_time = elapsed_time(&start_time);
    double cluster_wall_time = get_wall_time() - start_wall_time;
    std::cout << std::setw(5) << iterations << "\t";
    std::cout << std::setw(10) << numthreads << "\t";
    std::cout << std::setw(10) << cluster_time << "\t";
    std::cout << std::setw(10) << cluster_wall_time << "\t";
    std::cout << std::setw(8) << (getMemoryUsage() / 1024.0) << "\t";
 //   while (numItersHistory->size() <= (size_t)xcNdx){
 //       numItersHistory->push_back(iterations);
 //   }
    #ifdef COUNT_DISTANCES
    std::cout << std::setw(11) << algorithm->numDistances << "\t";
    #endif // COUNT_DISTANCES
    std::cout << std::endl;
    delete [] workingassignment;


    }







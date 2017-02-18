#include "dataset.h"
#include "general_functions.h"
#include "hamerly_kmeans.h"
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

void execute(Kmeans *algorithm, Dataset const *x, unsigned short k, unsigned short const *assignment,
    int numthreads,
    int maxIterations
    );

int main(int argc, char **argv){
    Dataset *x = NULL;
    unsigned short *assignment = NULL;
    unsigned short k;

    Kmeans *algorithm = NULL;

    int numthread = 1;
//    int maxIterations = std::numeric_limits<int>::max();
    int maxIterations = 500;
    std::string dataFilename;
    dataFilename = "";
    std::ifstream input(dataFilename.c_str());

    int n = 1000;
    int d = 3;

    delete x;
    delete [] assignment;
    assignment = NULL;
    x = new Dataset(n,d);

    for (int i = 0; i < n * d; i++){
        input >> x->data[i];
    }

    k = 10;
    std::string method = "kmeansplusplus";
    Dataset *c = NULL;
    c = init_centers(*x, k);

    delete [] assignment;
    assignment = new unsigned short[x->n];
    for (int i = 0; i < x->n; i++){
        assignment[i] = 0;
    }

    assign(*x, *c, assignment);
    delete c;
    algorithm = new HamerlyKmeans();
    execute(algorithm, x, k, assignment, numthread, maxIterations);
    delete algorithm;
    algorithm = NULL;
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

void execute(Kmeans *algorithm, Dataset const *x, unsigned short k, unsigned short const *assignment,
    int numthreads,
    int maxIterations
    ){
    std::cout<< std::setw[35] << algorithm->getName() << "\t" << std::flush;
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
    delete [] workingassignment;


    }






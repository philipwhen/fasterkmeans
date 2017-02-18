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

int main(int argc, char **argv){
    Dataset *x = NULL;
    unsigned short *assignment = NULL;
    unsigned short k;

    Kmeans *algorithm = NULL;

    int numthread = 1;
    int maxIterations = std::numeric_limits<int>::max();

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
    c = init_centers_kmeanspp_v2(*x, k)

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
    double cluster_time = elapsed_time(&start_clustering_time);
    double cluster_wall_time = get_wall_time() - start_wall_time;
    std::cout << std::setw(5) << iterations << "\t";
    std::cout << std::setw(10) << numthreads << "\t";
    std::cout << std::setw(10) << cluster_time << "\t";
    std::cout << std::setw(10) << cluster_wall_time << "\t";
    delete [] workingassignment;


    }







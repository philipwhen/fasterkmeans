#include "hplus_kmeans.h"
#include "general_functions.h"
#include <cmath>


int HplusKmeans::runThread(int threadId, int maxIterations){
    int iterations = 0;

    int startNdx = start(threadId);
    int endNdx = end(threadId);

    while ((iterations < maxIterations) && ! converged){
        ++iterations;

        update_s(threadId);
        synchronizeAllThreads();

        for (int i = startNdx; i < endNdx; ++i){
            unsigned short closest = assignment[i];

            double upper_comparison_bound = std::max(s[closest], lower[i]);

            if (upper[i] <= upper_comparison_bound){
                continue;
            }
            double u2 = pointCenterDist2(i, closest);
            upper[i] = sqrt(u2);

            if (upper[i] <= upper_comparison_bound){
            continue;
            }

            double l2 = std::numeric_limits<double>::max();
            for (int j = 0; j < k; j++){
                if (j == closest){continue;}
                double dist2 = pointCenterDist2(i,j);

                if (dist2 < u2){
                    l2 = u2;
                    u2 = dist2;
                    secondclosest[i] = closest;
                    closest = j;
                }else if (dist2 < l2){
                    l2 = dist2;
                    secondclosest[i] = j;
                }

            }
            lower[i] = sqrt(l2);

            if (assignment[i] != closest){
                upper[i] = sqrt(u2);
                changeAssignment(i, closest, threadId);
            }
        }
        verifyAssignment(iterations, startNdx, endNdx);

        synchronizeAllThreads();
        if (threadId == 0){
            int furthestMovingCenter = move_centers();
            converged = (0.0 == centerMovement[furthestMovingCenter]);
        }

        synchronizeAllThreads();

        if (! converged) {
            update_bounds(startNdx, endNdx);
        }
        synchronizeAllThreads();

    }
    return iterations;

}

void HplusKmeans::update_bounds(int startNdx, int endNdx){
    int furthestMovingCenter = 0;
    double longest = centerMovement[furthestMovingCenter];
    double secondLongest = 0.0;
    for (int j = 0; j < k; ++j){
        if (longest < centerMovement[j]){
            secondLongest = longest;
            longest = centerMovement[j];
            furthestMovingCenter = j;
        }else if (secondLongest < centerMovement[j]){
            secondLongest = centerMovement[j];
        }
    }

    for (int i = startNdx; i < endNdx; ++i){
        upper[i] += centerMovement[assignment[i]];
        double sum = 0.0;
        double sum1 = 0.0;
        for (int dim = 0; dim < d; dim++){
            sum += (*(x->data + i*d + dim) -(*centers)(assignment[i], dim)) * (*cmv)(assignment[i], dim);
            sum1 += (*(x->data + i*d + dim) -(*centers)(secondclosest[i], dim)) * (*cmv)(secondclosest[i], dim);
        }
        if (sum < 0.0){upper[i] += centerMovement[assignment[i]];}

        if (sum1 > 0.0){lower[i] -= centerMovement[secondclosest[i]];}

        lower[i] -= (assignment[i] == furthestMovingCenter) ? secondLongest : longest;

    }

}

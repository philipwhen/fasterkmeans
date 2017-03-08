#ifndef GEO2_KMEANS_H
#define GEO2_KMEANS_H

/* Authors: Greg Hamerly and Jonathan Drake
 * Feedback: hamerly@cs.baylor.edu
 * See: http://cs.baylor.edu/~hamerly/software/kmeans.php
 * Copyright 2014
 *
 * HamerlyKmeans implements Hamerly's k-means algorithm that uses one lower
 * bound per point.
 */

#include "triangle_inequality_base_kmeans.h"

class Geo2Kmeans : public TriangleInequalityBaseKmeans {
    public:
        Geo2Kmeans() { numLowerBounds = 1; }
        virtual ~Geo2Kmeans() { free(); }
        virtual std::string getName() const { return "geo2"; }

    protected:


        // Update the upper and lower bounds for the given range of points.
        void update_bounds(int startNdx, int endNdx);

        virtual int runThread(int threadId, int maxIterations);
        double getupdatefor2d(int j, double r, double lm);
        double getupdateformd(int i, int j, double r, double lm);
};

#endif


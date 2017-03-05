#ifndef HPLUS_KMEANS_H
#define HPLUS_KMEANS_H
#include "triangle_inequality_base_kmeans.h"

class HplusKmeans : public TriangleInequalityBaseKmeans {
    public:
        HplusKmeans() { numLowerBounds = 1;}
        virtual ~HplusKmeans() { free();}
        virtual std::string getName() const { return "hplus";}
//        int executethread(int threadId, int maxIterations, Dataset *x);

    protected:
        void update_bounds(int startNdx, int endNdx);
        virtual int runThread(int threadId, int maxIterations);
//        int executethread(int threadId, int maxIterations, Dataset *x);

};

#endif // H2_KMEANS_H

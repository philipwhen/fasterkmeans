#ifndef HPLUS_KMEANS_H
#define HPLUS_KMEANS_H
#include "triangle_inequality_base_kmeans.h"

class HplusKmeans : public TriangleInequalityBaseKmeans {
    public:
        HplusKmeans() { numLowerBounds = 1;}
        virtual ~HplusKmeans() { free();}
        virtual std::string getName() const { return "hplus";}

    protected:
        void update_bounds(int startNdx, int endNdx);
        virtual int runThread(int threadId, int maxIterations);


};

#endif // H2_KMEANS_H

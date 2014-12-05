#ifndef DENSITY_H

#define DENSITY_H

#include "quality.hpp"
#include "../linkStream.hpp"
#include "../community.hpp"
#include <string>
#include <math.h>


namespace gaumont {
    inline double expectLink(unsigned int sumDeg, unsigned int M){
        double res=(1.0*sumDeg*sumDeg)/(4.0*M);
        return res;
    }
    class PartitionDensity : public Quality {
        double computeQual(const CommunityManager::COM & C)const;

        PartitionDensity();
        PartitionDensity(const PartitionDensity &);
        const PartitionDensity & operator=(const PartitionDensity &);

        // Compute the density inside the com.
        double density(const CommunityManager::COM & C)const;
        double densityWith(const CommunityManager::COM & C,const Link & l)const;
        double densityWithout(const CommunityManager::COM & C,const Link & l)const;
    public:
        static const std::string  name;
        PartitionDensity(const linkStream & S_, const CommunityManager & C_) : Quality("PartitionDensity", S_, C_) {
        };

    };


}

#endif

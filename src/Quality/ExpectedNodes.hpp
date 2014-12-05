#ifndef EXPECTEDNODES_H

#define EXPECTEDNODES_H

#include "quality.hpp"
#include "../linkStream.hpp"
#include "../community.hpp"
#include <string>
#include <limits>
#include <boost/math/distributions/hypergeometric.hpp>

namespace gaumont {

  class ExpectedNodes : public Quality {
    const unsigned int maxCache;
    mutable double * cacheRes;


    double computeComQual(unsigned int communityNodeSize, unsigned int communityLinkSize)const;

    double computeComPosQual(const CommunityManager::COM & C)const {
      return computeComQual(C.nodeSize(), C.linkSize());
    }

    double computeQual(const CommunityManager::COM & C)const {
      double q=0;
      unsigned int AdjaLinkSize= C.countAdjacentNodesAndLink().second;
      q = mixe( C.linkSize() ,computeComPosQual(C) ,AdjaLinkSize , computeComNegFactor(C));
      return q;
    }

    // Compute the difference beetwen the number of adjacent vertex observed versus expected.
    double computeComNegFactor(const CommunityManager::COM & C)const;

    double mixe(double poidsA,double valA,double poidsB,double valB)const{
      return 2*(poidsA*valA+poidsB*valB)/(poidsA+poidsB);
    }


    // Same as above but it consider that C1 and C2 are merged.
    double computeComNegFactor(const CommunityManager::COM & C1, const CommunityManager::COM & C2)const;

    ExpectedNodes();
    ExpectedNodes(const ExpectedNodes &);
    const ExpectedNodes & operator=(const ExpectedNodes &);
  public:
    static const std::string  name;
    ExpectedNodes(const linkStream & S_, const CommunityManager & C_) : Quality("ExpectedNodes", S_, C_), maxCache(2 * S.getNbLinks() + 1) {
      cacheRes = new double[maxCache];
      for (unsigned int i = 0; i < maxCache; ++i) {
        cacheRes[i] = std::numeric_limits<double>::min();
      }
    };

    ~ExpectedNodes() {
      delete(cacheRes);
    }
  };
}

#endif

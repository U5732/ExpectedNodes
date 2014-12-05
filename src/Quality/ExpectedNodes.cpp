#include "ExpectedNodes.hpp"

using namespace gaumont;
const std::string  ExpectedNodes::name="LINKEXPECTATION";
double ExpectedNodes::computeComQual(unsigned int communityNodeSize, unsigned int communityLinkSize)const {

  unsigned int comDegreeSize = 2 * communityLinkSize;
  if (communityNodeSize == 0 || comDegreeSize == 0)
    return 0;
    double expectedNodeSize = 0L;
    double res = 0L;

    assert(maxCache > comDegreeSize); // make sure cacheRes could be assed by comDegreeSize.
    if (cacheRes[comDegreeSize] != std::numeric_limits<double>::min()) {
      // A result have already been stored for this value
      expectedNodeSize = cacheRes[comDegreeSize];
    } else {
      linkStream::const_NodesIterator it = S.begin(),
      ite = S.end();
      unsigned int totalDegree = 2 * S.getNbLinks(); // # of links in l.

      //Iterate on each node
      for (; it != ite; ++it) {

        //Make sur the result is not obvious.
        if (totalDegree > (comDegreeSize + it->second.degree())) {
          boost::math::hypergeometric_distribution<double>H(comDegreeSize, it->second.degree(), totalDegree);
          expectedNodeSize += 1 - boost::math::pdf(H, 0);
        } else {
          expectedNodeSize += 1;
        }
      }
      cacheRes[comDegreeSize] = expectedNodeSize; // store result for next time
    }

    res = 1.0 * (expectedNodeSize - communityNodeSize) / (1.0 * expectedNodeSize);

    return res;
  }

  double ExpectedNodes::computeComNegFactor(const CommunityManager::COM & C)const {
    // C.display("Com "+toStr(C.comID)+" :");

    double res = 0L;
    double expectedNodeSize = 0L;
    std::pair<unsigned int, unsigned int> counter = C.countAdjacentNodesAndLink();
    unsigned int cIncidentLink = counter.second;
    unsigned int cIncidentNode = counter.first;

    unsigned int totalDegree = 2 * S.getNbLinks() - 2 * C.linkSize(); // # total degree without the link of C.

    const nodeSet nodesIncom = C.nodesInCom();
    ;
    nodeSet::const_iterator notFound = nodesIncom.end();

    linkStream::const_NodesIterator it = S.begin(),
    ite = S.end();
    for (; it != ite; ++it) {
      unsigned int degree = it->second.degree();
      // if "it" is in nodesIncom, its degree has to be updated.

      Node * n = const_cast<Node *> (&(it->second));
      if (nodesIncom.find(n) != notFound) {
        degree = C.countExterneEdge(it);
      }
      if (degree != 0) {

        //Make sur the result is not obvious.
        if (totalDegree > (cIncidentLink + degree)) {
          boost::math::hypergeometric_distribution<double>H(cIncidentLink, degree, totalDegree);
          expectedNodeSize += 1 - boost::math::pdf(H, 0);
        } else {
          expectedNodeSize += 1;
        }
      }
    }

    res = (1.0 * cIncidentNode - expectedNodeSize) / expectedNodeSize;
    res = std::min(0.0, res);
    return res;
  }

  double ExpectedNodes::computeComNegFactor(const CommunityManager::COM & C1, const CommunityManager::COM & C2)const {
    double res = 0L;
    double expectedNodeSize = 0L;
    std::pair<unsigned int, unsigned int> counter = C1.countAdjacentNodesAndLink(C2);
    unsigned int cIncidentLink = counter.second;
    unsigned int cIncidentNode = counter.first;

    unsigned int totalDegree = 2 * S.getNbLinks() - 2 * C1.linkSize() - 2 * C2.linkSize(); // # total edge without the link of C.

    nodeSet nodesIncom;
    C1.unionNodes(C2, nodesIncom);

    nodeSet::iterator notFound = nodesIncom.end();

    linkStream::const_NodesIterator it = S.begin(),
    ite = S.end();
    for (; it != ite; ++it) {
      unsigned int degree = it->second.degree();
      // if "it" is in nodesIncom, its degree has to be updated.
      if (nodesIncom.find(const_cast<Node *> (&(it->second))) != notFound) {
        degree = C1.countExterneEdge(it, C2);
      }
      if (degree != 0) {

        //Make sur the result is not obvious.
        if (totalDegree > (cIncidentLink + degree)) {
          boost::math::hypergeometric_distribution<double>H(cIncidentLink, degree, totalDegree);
          expectedNodeSize += 1 - boost::math::pdf(H, 0);
        } else {
          expectedNodeSize += 1;
        }
      }
    }
    res = (cIncidentNode - expectedNodeSize) / expectedNodeSize;
    res = std::min(0.0, res);
    return res;
  }

#include "Evans.hpp"

using namespace gaumont;
const std::string  Evans1::name="E1";
const std::string  Evans2::name="E2";
const std::string  Evans3::name="E3";

std::pair<unsigned int, unsigned int> Evans1::computeIncidenceAndWeight(const CommunityManager::COM & C)const {

    unsigned int incidence = 0;
    const nodeSet & internNode = C.nodesInCom();
    nodeSet::const_iterator it = internNode.begin(), ite = internNode.end();

    for (; it != ite; ++it) {
        const std::deque<Link * > & links = (*it)->linksNeighbor();
        unsigned int counter = 0;
        for (unsigned int i = 0; i < links.size(); ++i) {
            if (links[i]->com == C.comID) {
                ++counter;
            }
        }
        incidence += (counter)*(counter - 1);
    }


    CommunityManager::COM::const_iterator first = C.begin(), end = C.end();
    unsigned int totalComDegree=0;
    for (; first != end; ++first) {
        unsigned int lDegree = (*first)->sumDegree();
        totalComDegree += lDegree;
    }

    return std::make_pair(incidence, totalComDegree);
}

double Evans1::computeQual(const CommunityManager::COM & C1)const {
    double res = 0.0;
    std::pair<unsigned int, unsigned int> p = computeIncidenceAndWeight(C1);
    unsigned int incidence = p.first,
            comWeight = p.second,
            totDegree = edgeDegreeSum;
    res = wrapperComputeQual(incidence, comWeight, totDegree);
    return res;
}

double Evans1::wrapperComputeQual(unsigned int incidence, unsigned int comWeight, unsigned int totWeight)const {
    return (1.0 * incidence) / (1.0 * totWeight)- (1.0 * comWeight * comWeight) / (1.0 * totWeight * totWeight);
}








/*
*
* EVANS2
*
*/
std::pair<double, double> Evans2::computeIncidenceAndWeight(const CommunityManager::COM & C)const {

    double weightedIncidence = 0;
    double totalComDegree=0;
    CommunityManager::COM::const_iterator it=C.begin(), ite=C.end();
    for (;it!=ite;++it){
        totalComDegree+= deg(**it);
    }

    CommunityManager::COM::const_nodeIterator nit = C.beginNode(), nite = C.endNode();
    unsigned int curDegIncom=0;
    for (; nit != nite; ++nit) {
        curDegIncom=(*nit)->degreeInCom(C.comID);
        if((*nit)->degree()>1)
            weightedIncidence+=  static_cast<double>(curDegIncom*(curDegIncom-1))/((*nit)->degree()-1);
    }
    return std::make_pair(weightedIncidence, totalComDegree);
}

double Evans2::computeQual(const CommunityManager::COM & C1)const {
    double res = 0.0;
    std::pair<double, double> p = computeIncidenceAndWeight(C1);
    double incidence = p.first,
            comWeight = p.second,
            totDegree = edgeDegreeSum;
    res = wrapperComputeQual(incidence, comWeight, totDegree);
    return res;
}

double Evans2::wrapperComputeQual(double incidence, double comWeight, unsigned int totWeight)const {
    return (1.0 * incidence) / (1.0 * totWeight)- (1.0 * comWeight * comWeight) / (1.0 * totWeight * totWeight);
}


/*
*
* EVANS3
*
*/
std::pair<double, double> Evans3::computeIncidenceAndWeight(const CommunityManager::COM & C)const {
    double weightedIncidence = 0;
    double totalComDegree=2*C.linkSize();
    double leftd=0,rightd=0;
    CommunityManager::COM::const_iterator it=C.begin(), ite=C.end();

    for (;it!=ite;++it){
        const Node & leftN=(*it)->left();
        const Node & rightN=(*it)->right();
        // std::cout<<"Lien: ("<<leftN<<","<<rightN<<")"<<std::endl;
        leftd=gatherIncidence(C,(**it),leftN);
        rightd=gatherIncidence(C,(**it),rightN);
        weightedIncidence+=rightd+leftd;
    }

    return std::make_pair(weightedIncidence, totalComDegree);
}

double Evans3::gatherIncidence(const CommunityManager::COM & C,const Link & l, const Node & n)const{
    double res=0;
    const std::deque<Link * > & links=n.linksNeighbor();
    unsigned int degN=links.size();
    std::deque<Link * >::const_iterator it=links.begin(),ite=links.end();
    for(;it!=ite;++it){
        const Node & otherNode= (*it)->otherNode(n);
        const std::deque<Link * > & neighbor=otherNode.linksNeighbor();
        std::deque<Link * >::const_iterator neighborIt=neighbor.begin(),neighborIte=neighbor.end();
        for(;neighborIt!=neighborIte;++neighborIt){
            if( (*neighborIt)->com == C.comID  ){
                unsigned int otherD= otherNode.degree();
                double tmp=1/(static_cast<double>(degN*otherD));
                res+=tmp;
            }
        }
    }
    return res;
}

double Evans3::computeQual(const CommunityManager::COM & C1)const {
    double res = 0.0;
    std::pair<double, double> p = computeIncidenceAndWeight(C1);
    double incidence = p.first,
            comWeight = p.second,
            totDegree = edgeDegreeSum;
    res = wrapperComputeQual(incidence, comWeight, totDegree);
    return res;
}

double Evans3::wrapperComputeQual(double incidence, double comWeight, unsigned int totWeight)const {
    return (1.0 * incidence) / (1.0 * totWeight)- (1.0 * comWeight * comWeight) / (1.0 * totWeight * totWeight);
}

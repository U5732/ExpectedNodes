#include "density.hpp"

using namespace gaumont;
const std::string  PartitionDensity::name="PARTIONDENSITY";


double PartitionDensity::computeQual(const CommunityManager::COM & C1)const {
    double res=density(C1);
    return res;
}

double PartitionDensity::density(const CommunityManager::COM & C)const{
    unsigned int M=S.getNbLinks();
    unsigned int deg=C.degree();
    double expectl=expectLink(deg,M);
    double expectd=2.0*expectl/(1.0*C.nodeSize()* (C.nodeSize()-1) );
    double reald=2.0*C.linkSize()/(1.0*C.nodeSize()* (C.nodeSize()-1));
    return (1.0 * reald - expectd) / expectd;
}

double PartitionDensity::densityWith(const CommunityManager::COM & C,const Link & l)const{
    if (l.com == C.comID){ // l is already inside C
        return density(C);
    }
    Node & leftn=l.left();
    unsigned int nodeSi=C.nodeSize();
    unsigned int allDeg=C.degree();
    // std::cout<<"Old+ "<<allDeg;
    if( !leftn.inCom(C.comID) ){ // leftn was'nt inside. So node is increase;
        ++nodeSi;
        allDeg+=leftn.degree();
    }
    Node & rightn=l.right();
    if( !rightn.inCom(C.comID) ) {
        ++nodeSi;
        allDeg+=rightn.degree();
    }
    // std::cout<<", new "<<allDeg<<std::endl;
    double reald=2.0*(C.linkSize()+1)/(1.0*nodeSi* (nodeSi-1));
    double expectd=(2.0*expectLink(allDeg,S.getNbLinks()))/(1.0*nodeSi*(nodeSi-1) );
    return (1.0 * reald - expectd) / expectd;
}

double PartitionDensity::densityWithout(const CommunityManager::COM & C,const Link & l)const{
    if (l.com != C.comID){ // l is already not inside C
        return density(C);
    }
    if(C.linkSize()==1){ //l is the last link inside C.
        return 0;
    }
    Node & leftn=l.left();
    unsigned int nodeSi=C.nodeSize();
    unsigned int allDeg=C.degree();
    // std::cout<<"Old- "<<allDeg;
    if(leftn.degreeInCom(C.comID)==1){ // l was the last link of the community C connected to leftn.
        --nodeSi;
        allDeg-=leftn.degree();
    }
    Node & rightn=l.right();
    if(rightn.degreeInCom(C.comID)==1){
        --nodeSi;
        allDeg-=rightn.degree();
    }
    // std::cout<<", new "<<allDeg<<std::endl;
    double reald=2.0*(C.linkSize()-1)/(1.0*nodeSi* (nodeSi-1));
    double expectd=(2.0*expectLink(allDeg,S.getNbLinks()))/(1.0*nodeSi*(nodeSi-1) );
    return (1.0 * reald - expectd) / expectd;
}

#include "community.hpp"


using namespace gaumont;

void CommunityManager::COM::remove(Link & l) {
    // have to removed the extremity of lid if not attached to the community anymore.
    unsigned int erased = com.erase(&l);
    upToDateAdja = false;
    upToDateQual = false;
    if (erased == 1) {
        l.com = -1;
        Node & left = l.left(), & right = l.right();
        const std::deque<Link * > & leftN = left.linksNeighbor(),
                & rightN = right.linksNeighbor();
        unsigned int i = 0;
        bool afaire = true;
        for (; i < leftN.size(); ++i) {
            if (leftN[i]->com == comID) {
                afaire = false;
                break;
            }
        }
        if (afaire) { // lid was the last link beetwen left and the com
            nodes.erase(&left);
        }
        afaire = true;

        i = 0;
        for (; i < rightN.size(); ++i) {
            if (rightN[i]->com == comID) {
                afaire = false;
                break;
            }
        }
        if (afaire) { // lid was the last link beetwen right and the com
            nodes.erase(&right);
        }
    }
}

void CommunityManager::COM::unionNodes(const CommunityManager::COM & C2, nodeSet & res)const {
    const nodeSet & nodesC2 = C2.nodesInCom();
    res.clear();
    res.insert(nodesC2.begin(), nodesC2.end());
    res.insert(nodes.begin(), nodes.end());
}

unsigned int CommunityManager::COM::countUnionNode(const CommunityManager::COM & C2)const {
    nodeSet res;
    unionNodes(C2, res);
    return res.size();

}

void CommunityManager::COM::merge(COM & c) {
    upToDateAdja = false;
    upToDateQual = false;
    COM::iterator cur = begin(),
            it = c.begin(),
            ite = c.end();
    for (; it != ite; ++it) {
        (*it)->com = comID; //Update the community from c
        cur = add(cur, *(*it));
    }
    const nodeSet & uniqueNode = c.nodesInCom();
    ;
    nodes.insert(uniqueNode.begin(), uniqueNode.end());
    c.clear();
}

void CommunityManager::COM::display(std::string m)const {
    if (m != "")
        std::cout << m << std::endl;
    const_iterator it = begin(),
            ite = end();
    for (; it != ite; ++it) {
        std::cout << "\t" << *(*it) << std::endl;
    }
}

void CommunityManager::COM::displayNode(std::string m)const {
    if (m != "")
        std::cout << m << std::endl;
    nodeSet::const_iterator it = nodes.begin(),
            ite = nodes.end();
    for (; it != ite; ++it) {
        std::cout << "\t" << *(*it) << std::endl;
    }
    std::cout << "Taille :" << nodes.size() << std::endl;
}

void CommunityManager::COM::displayStat(std::string m)const {
    if (m != "")
        std::cout << m << std::endl;
    std::cout << "\tNodeSize :" << nodeSize() << std::endl;
    std::cout << "\tLinkSize :" << linkSize() << std::endl;
}

unsigned int CommunityManager::COM::degree()const{
    unsigned int res=0;
    nodeSet::const_iterator it = nodes.begin(),
            ite = nodes.end();
    for (; it != ite; ++it) {
       res+=(*it)->degree();
    }
    return res;
}
unsigned int CommunityManager::COM::unionDegree(const COM & C2)const{
    nodeSet unionN;
    unsigned int res=0;
    unionNodes(C2,unionN);
    nodeSet::const_iterator it = unionN.begin(),
            ite = unionN.end();
    for (; it != ite; ++it) {
       res+=(*it)->degree();
    }
    return res;
}

const nodeSet & CommunityManager::COM::nodesInCom()const {
    return nodes;
}

unsigned int CommunityManager::COM::nodeSizeWithExtraLink(const Link & l)const {
    unsigned int res = nodeSize();
    nodeSet::const_iterator notFound = nodes.end();
    if (nodes.find(&l.left()) == notFound) {
        ++res;
    }
    if (nodes.find(&l.right()) == notFound) {
        ++res;
    }
    return res;
}

unsigned int CommunityManager::COM::nodeSizeWithoutLink(const Link & l)const {


    if (l.com != comID)
        return nodes.size();

    unsigned int linksSize = com.size();
    if (linksSize == 1) { // Last link in the community
        return 0;
    } else if (linksSize == 2) { // There would only one link if l is removed
        return 2;
    }
    const Node & left = l.left();
    const std::deque<Link * >& leftN = left.linksNeighbor();
    unsigned int res = nodes.size();
    unsigned int sameComCount = 0;
    for (unsigned int i = 0; i < leftN.size() && sameComCount < 2; ++i) {
        if (leftN[i]->com == comID) {
            ++sameComCount;
        }
    }
    if (sameComCount < 2)
        --res;

    sameComCount = 0;
    const Node & right = l.right();
    const std::deque<Link * >& rightN = right.linksNeighbor();
    for (unsigned int i = 0; i < rightN.size() && sameComCount < 2; ++i) {
        if (rightN[i]->com == comID) {
            ++sameComCount;
        }
    }
    if (sameComCount < 2)
        --res;
    return res;
    // Code plus rapide pour les très petites communautés mais plus bourrin.
    // nodeSet uniqueNode;
    // linkSet::const_iterator it =com.begin(),ite=com.end();
    // for(;it!=ite;++it){
    // 	if(*it!=&l){
    // 		uniqueNode.insert(&(*it)->left());
    // 		uniqueNode.insert(&(*it)->right());
    // 	}
    // }
}

unsigned int CommunityManager::COM::countExterneEdge(linkStream::const_NodesIterator node)const {
    const std::deque<Link * > & deq(node->second.linksNeighbor());
    std::deque<Link * >::const_iterator it = deq.begin(), ite = deq.end();
    unsigned int res = 0;
    for (; it != ite; ++it) {
        Link & l(*(*it));
        if (l.com != comID)
            ++res;
    }
    return res;
}

unsigned int CommunityManager::COM::countExterneEdge(linkStream::const_NodesIterator node, const COM & C2)const {
    const std::deque<Link * > & deq(node->second.linksNeighbor());
    std::deque<Link * >::const_iterator it = deq.begin(), ite = deq.end();
    unsigned int res = 0;
    for (; it != ite; ++it) {
        Link & l(*(*it));
        if (l.com != comID && l.com != C2.comID)
            ++res;
    }
    return res;
}

std::pair<const nodeSet *, const linkSet *> CommunityManager::COM::adjacentNodesAndLink()const {
    if (upToDateAdja) {
        return std::make_pair(& adjaNodes, & adjaLinks);
    }
    adjaLinks.clear();
    adjaNodes.clear();
    COM::const_nodeIterator nodeNotFound = nodes.end();

    COM::const_nodeIterator it = nodes.begin(), ite = nodes.end();
    //iterate on Node to find adjacent link
    // For each link iterate on its linkNeighbor
    for (; it != ite; ++it) {
        const std::deque<Link * > & N = (*it)->linksNeighbor(); // N Contains all adjacent link to it.
        std::deque<Link * >::const_iterator lit = N.begin(), lite = N.end();
        // For each link in the Neighboor check if it is from another community.
        for (; lit != lite; ++lit) {
            Link & aLink = *(*lit);
            if (comID != aLink.com) { // if aLink is from another com, add the link and its nodes into adjacent.
                bool inserted = adjaLinks.insert(*lit).second;
                if (inserted) {
                    // Before inserting the nodes we have to make sur it's not in the induced node.
                    if (nodes.find(& aLink.left()) == nodeNotFound) { // The node is not in the induced node by the community so it's adjacent.
                        adjaNodes.insert(& aLink.left());
                    }
                    if (nodes.find(&aLink.right()) == nodeNotFound) {
                        adjaNodes.insert(& aLink.right());
                    }
                }
            }
        }
    }
    upToDateAdja = true;
    return std::make_pair(&adjaNodes, &adjaLinks);
}

void CommunityManager::COM::adjacentNodesAndLink(const CommunityManager::COM & C2, nodeSet & resNode, linkSet & resLink)const {
    resNode.clear();
    resLink.clear();

    std::pair<const nodeSet *, const linkSet *> resC2 = C2.adjacentNodesAndLink();
    const linkSet & C2adjacentLink = *resC2.second;
    const nodeSet & C2adjacentNode = *resC2.first;
    // Store all C2's adjacent links (resp Node), wich are not in this, into resLink( resp resNode),
    nodeComparator nComp;
    linkComparator lnComp;
    std::set_difference(C2adjacentLink.begin(), C2adjacentLink.end(), com.begin(), com.end(), std::inserter(resLink, resLink.end()), lnComp);
    std::set_difference(C2adjacentNode.begin(), C2adjacentNode.end(), nodes.begin(), nodes.end(), std::inserter(resNode, resNode.end()), nComp);
    std::pair<const nodeSet *, const linkSet *> resC1 = adjacentNodesAndLink();
    const linkSet & C1adjacentLink = *resC1.second;
    const nodeSet & C1adjacentNode = *resC1.first;
    // Store all this' adjacent links (resp Node), wich are not in C2, into resLink( resp resNode),
    std::set_difference(C1adjacentLink.begin(), C1adjacentLink.end(), C2.begin(), C2.end(), std::inserter(resLink, resLink.end()), lnComp);
    std::set_difference(C1adjacentNode.begin(), C1adjacentNode.end(), C2.beginNode(), C2.endNode(), std::inserter(resNode, resNode.end()), nComp);
}

std::pair<unsigned int, unsigned int> CommunityManager::COM::countAdjacentNodesAndLink()const {
    std::pair<unsigned int, unsigned int> res;
    std::pair<const nodeSet *, const linkSet *> resAdja = adjacentNodesAndLink();
    const linkSet & adjacentLink = *resAdja.second;
    const nodeSet & adjacentNode = *resAdja.first;
    res.first = adjacentNode.size();
    res.second = adjacentLink.size();
    return res;
}

std::pair<unsigned int, unsigned int> CommunityManager::COM::countAdjacentNodesAndLink(const CommunityManager::COM & C2)const {
    std::pair<unsigned int, unsigned int> res;
    linkSet adjacentLink;
    nodeSet adjacentNode;
    adjacentNodesAndLink(C2, adjacentNode, adjacentLink);
    res.first = adjacentNode.size();
    res.second = adjacentLink.size();
    // std::cout<<"Adjacent Node :"<<res.first<<", Link :"<<res.second<<std::endl;
    return res;
}
void CommunityManager::COM::bordureNodes(nodeSet & resNode, bool toClear)const {
    adjacentNodesAndLink();
    //Now adjaNodes and adjaLinks are up to date.
    if(toClear){
        resNode.clear();
    }

    linkSet::const_iterator it=adjaLinks.begin(),
                                ite=adjaLinks.end();
    for(;it!=ite;++it){
        resNode.insert(&(*it)->left());
        resNode.insert(&(*it)->right());
    }    
}
void CommunityManager::COM::bordureNodes(const CommunityManager::COM & C2, nodeSet & resNode)const {
    nodeSet adjacentNode;
    linkSet adjacentLink;
    adjacentNodesAndLink(C2,adjacentNode,adjacentLink);
    
    linkSet::const_iterator it=adjacentLink.begin(),
                                ite=adjacentLink.end();
    for(;it!=ite;++it){
        resNode.insert(&(*it)->left());
        resNode.insert(&(*it)->right());
    }
}
unsigned int CommunityManager::COM::countBordureNodes()const {
    nodeSet adjacentNode;
    bordureNodes(adjacentNode);
    return adjacentNode.size();
}
unsigned int CommunityManager::COM::countBordureNodes(const CommunityManager::COM & C2)const {
    nodeSet adjacentNode;
    bordureNodes(C2,adjacentNode);
    return adjacentNode.size();
}

void CommunityManager::COM::nodeIntersection(const COM & C2, std::deque<Node *> & res)const {
    res.clear();
    res.resize(nodes.size()); // initialize with suficient size.
    std::deque<Node *>::iterator itv;
    nodeComparator nodeComp;
    itv = std::set_intersection(nodes.begin(), nodes.end(), C2.beginNode(), C2.endNode(), res.begin(), nodeComp);
    res.resize(itv - res.begin());
}

CommunityManager::COM & CommunityManager::getCommunity(unsigned int cId, bool createIfNotPresent) {
    std::map<unsigned int, COM >::iterator it;
    it = community.find(cId);
    if (it == community.end()) {
        if (createIfNotPresent) {
            COM comm(S, cId);
            std::pair < std::map<unsigned int, COM>::iterator, bool> res;
            res = community.insert(std::pair<unsigned int, COM>(cId, comm));
            return res.first->second;
        } else {
            std::cerr << "Inexistant community ID :" << cId << std::endl;
            throw std::string("Inexistant community ID . (in getCommunity())");
        }
    }
    return it->second;
}

unsigned int CommunityManager::merge(unsigned int cId1, unsigned int cId2) {

    COM & C1 = getCommunity(cId1), & C2 = getCommunity(cId2);
    C1.merge(C2);
    community.erase(C2.comID);
    return C1.comID;
}

void CommunityManager::moveLinkInto(linkID lId, unsigned int cId) {
    COM & dest = getCommunity(cId);
    COM & source = getCommunity(linkCommunity(lId));
    source.remove(S.link(lId)); // remove lId from source


    //Remove the community if it's emppty.
    if (source.empty()) {
        community.erase(source.comID);
    }

    //Now addind lId in its new community.
    S.link(lId).com = cId;
    dest.add(S.link(lId));
}

void CommunityManager::display(std::string m)const {
    Log::log(2, m);
    CommunityManager::const_iterator it = begin(),
            ite = end();
    for (; it != ite; ++it) {
        it->second.display("Com number :" + toStr(it->first));
        Log::log(2, "");
    }
}

const CommunityManager::COM & CommunityManager::getCommunity(unsigned int cId)const {
    std::map<unsigned int, COM >::const_iterator it;
    it = community.find(cId);
    if (it == community.end()) {
        std::cerr << "There is no community here" << std::endl;
        throw std::string("There is no community here");
    }
    return it->second;
}

void CommunityManager::fillAdjacentComunityToLink(linkID lId, std::set< unsigned int> & res)const {
    res.clear();
    std::deque<Link * > N;
    S.link(lId).linkNeighbor(N); // Contains all adjacent link (including self).
    std::deque<Link * >::const_iterator it = N.begin(), ite = N.end();
    unsigned int refCom = linkCommunity(lId);
    for (; it != ite; ++it) {
        res.insert((*it)->com);
    }
    res.erase(refCom);
}

void CommunityManager::fillAdjacentComunityToCom(const CommunityManager::COM & C, std::set< unsigned int> & res)const {
    res.clear();

    std::pair<const nodeSet *, const linkSet *> resAdja = C.adjacentNodesAndLink();
    const linkSet & adjacentLink = *resAdja.second;
    linkSet::const_iterator it = adjacentLink.begin(), ite = adjacentLink.end();
    for (; it != ite; ++it) {
        res.insert((*it)->com);
    }
}

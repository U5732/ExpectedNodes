#include "linkStream.hpp"

using namespace gaumont;

std::ostream& gaumont::operator<<(std::ostream& os, const Link& l) {
    const char * sep = " ";
    os<< l.left() << sep << l.right() << sep << l.com;
    return os;
}

std::ostream& gaumont::operator<<(std::ostream & os, const Node & n) {
    os << n.id();
    return os;

}

std::ostream& gaumont::operator<<(std::ostream & os, const nodeID & n) {
    os << static_cast<unsigned int> (n);
    return os;
}
void Node::add(Link & l) {
    linkNeighbor.push_back(&l);
}

unsigned int Node::degreeInCom(unsigned int cId)const {
    std::deque<Link * >::const_iterator it = linkNeighbor.begin(), ite = linkNeighbor.end();
    unsigned int res=0;
    for (; it != ite; ++it) {
        if((*it)->com ==cId)
            ++res;
    }
    return res;
}
bool Node::inCom(unsigned int cId)const{
    std::deque<Link * >::const_iterator it = linkNeighbor.begin(), ite = linkNeighbor.end();
    for (; it != ite; ++it) {
        if((*it)->com ==cId)
            return true;
    }
    return false;
}

void Node::displayLinkNeighbor(std::string prefix)const {
    std::deque<Link * >::const_iterator it = linkNeighbor.begin(), ite = linkNeighbor.end();
    std::cout << prefix << ": " << std::endl;
    for (; it != ite; ++it) {
        std::cout << "\t" << *(*it) << std::endl;
    }
}

bool linkComp (const Link* a, const Link* b) {
  return a->id < b->id;
}
void Link::linkNeighbor(std::deque<Link *> & res)const {
    res.clear();
    const std::deque<Link *> & left = this->left().linksNeighbor();
    const std::deque<Link *> & right = this->right().linksNeighbor();
    res.resize(left.size() + right.size()); // make sur res is large enough to store everything.

    std::deque<Link * >::iterator ite;
    ite = std::set_union(left.begin(), left.end(), right.begin(), right.end(), res.begin(),linkComp); // fill res with the union
    //ite points to the end of added values
    res.resize(ite - res.begin()); // As doublon could occurs=> ite!= res.end(). Therefore res has to be resized.
    // The current link is in the dequeu;
}

std::pair<bool, nodeID> Link::commonNode(const Link &l)const {
    std::pair<bool, nodeID > res(false, nodeID(0));
    if (left().id() == l.left().id() || left().id() == l.right().id()) {
        res.first = true;
        res.second = left().id();
    } else if (right().id() == l.left().id() || right().id() == l.right().id()) {
        res.first = true;
        res.second = right().id();
    }
    return res;
}

linkStream::linkStream(const char * inputFile, bool communityPresent) {
    std::ifstream finput(inputFile);
    if (!finput) {
        std::cerr << "The file " << inputFile << " does not exist" << std::endl;
        exit(-1);
    }

    std::string line;
    nodeID ext1, ext2;
    linkID count(0);
    while (std::getline(finput, line)) {
        std::istringstream iss(line);
        if (!(iss >> ext1 >> ext2)) {
            std::size_t found = line.find("source");
            if (found != std::string::npos) {
                Log::log(2, "Header skipped");
                continue;
            } else {
                std::cerr << "There is a problem with the line :" << std::endl << line << std::endl;
                exit(-1);
            }
        }
        unsigned int com = count;

        if (communityPresent) {
            if (!(iss >> com)) {
                std::cerr << "no community readed in the line :" << std::endl << line << std::endl;
                exit(-1);
            }
        }
        tmpLink l(count, ext1, ext2, com);
        count++;
        addLink(l);
    }
    finput.close();
    Log::log(1, "Number of links read : " + toStr(links.size()));
}

void linkStream::addLink(const tmpLink & l) {
    //Create node if it doesn't exist yet;
    Node & left = getCreateNode(l.u);
    Node & right = getCreateNode(l.v);

    // //Update node neighborhod.
    // left.add(right);
    // right.add(left);

    Link realLink(l, left, right);
    links.push_back(realLink); // There is a copy constructor here. That's why we add the link to the node later.
    //Update links neighborhood
    left.add(*links.rbegin());
    if(l.u!=l.v)
      right.add(*links.rbegin());
}


void linkStream::printDegreeList(std::ostream & f)const {
    const_NodesIterator it = begin(),
            ite = end();
    f << it->second.degree();
    ++it;
    for (; it != ite; ++it) {
        f << "," << it->second.degree();
    }
    f << std::endl;

}

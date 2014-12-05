#ifndef LINKSTREAM_H
#define LINKSTREAM_H

#include <map>
#include <deque>
#include <set>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <assert.h>
#include <stdexcept>
#include "log.hpp"
#include <boost/strong_typedef.hpp>
#include <vector>


namespace gaumont {

    BOOST_STRONG_TYPEDEF(unsigned int, nodeID)
    BOOST_STRONG_TYPEDEF(unsigned int, linkID)

    std::ostream& operator<<(std::ostream &, const nodeID &);

    template <typename T>
    static std::string toStr(const T& val) {
        std::stringstream out;
        out << val;
        return out.str();
    }

    template<class ForwardIter> inline
    void displayContainer(ForwardIter begin, ForwardIter end, std::string m = "") {
        if (m != "")
            std::cout << m << std::endl;
        for (; begin != end; ++begin) {
            std::cout << "\t" << (*begin) << std::endl;
        }

    }

    typedef unsigned int timestamp;
    // typedef unsigned int linkID;

    class GreedyMethod;

    class tmpLink {
    public:

        tmpLink(unsigned int id_, nodeID u_, nodeID v_, unsigned int com_ = -1) : id(id_), u(u_), v(v_), com(com_) {
        }
        const linkID id;
        const nodeID u;
        const nodeID v;
        unsigned int com;
    };
    class linkStream;
    class Link;

    class Node {
        friend class linkStream;

        nodeID ID;
        std::deque<Link * > linkNeighbor;

        void add(Link & l);

    public:

        Node(nodeID id_) : ID(id_) {}

        Node() : ID(0) {}

        Node(const Node & n) : ID(n.ID), linkNeighbor(n.linkNeighbor){
        }

        nodeID id()const {
            return ID;
        }

        const std::deque<Link * >& linksNeighbor()const {
            return linkNeighbor;
        }

        unsigned int degree()const {
            return linkNeighbor.size();
        }

        unsigned int degreeInCom(unsigned int cId)const;
        bool inCom(unsigned int cId)const;

        void displayLinkNeighbor(std::string prefix = "")const;

        bool operator==(const Node & n)const{
            return ID==n.id();
        }
        bool operator!=(const Node & n)const{
            return !(n==*this);
        }

    };

    struct nodeComparator {
        bool operator() (const Node* a, const Node* b) const {
            return a->id() < b->id();
        }
    };
    typedef std::set<Node *, nodeComparator> nodeSet;
    std::ostream& operator<<(std::ostream &, const Node &);

    class Link {
        Node * leftNode;
        Node * rightNode;
    public:

        Link(tmpLink t, Node & leftN, Node & rightN) : leftNode(&leftN), rightNode(&rightN), id(t.id), com(t.com) {
        }

        explicit Link(const Link & l) : leftNode(l.leftNode), rightNode(l.rightNode), id(l.id), com(l.com) {
        }
        const linkID id;
        unsigned int com;

        Node & left()const {
            return *leftNode;
        }

        Node & right()const {
            return *rightNode;
        }
        bool in(nodeID id)const {
            return (id == leftNode->id() || id == rightNode->id());
        }

        const Node & otherNode(const Node & n){
            if (!in(n.id())){
                std::cerr<<"The node "<<n<<" is not in the link "<<this->leftNode<<" "<< this->rightNode<<std::endl;
            }
            if( n!= *leftNode){
                return *leftNode;
            }
            return *rightNode;
        }

        unsigned int sumDegree()const {
            return leftNode->degree() + rightNode->degree()-2;
        }

        // Fill in the deque all the link adjacent to this
        // The deque also contains this.
        void linkNeighbor(std::deque<Link *> &)const;

        std::pair<bool, nodeID> commonNode(const Link &l)const;
        bool operator<=(const Link & l)const{
            return id<=l.id;
        }
    };

    struct linkComparator {
        bool operator() (const Link* a, const Link* b) const {
            return a->id < b->id;
        }
    };
    typedef std::set<Link *, linkComparator> linkSet;

    std::ostream& operator<<(std::ostream &, const Link &);

    class linkStream {
        std::map< nodeID, Node > nodes;
        std::deque<Link> links;

        linkStream(const linkStream &); //prevent copy constructor
        linkStream(); //prevent default constructor
        const linkStream & operator=(const linkStream & l); //prevent assignation constructor

        friend class CommunityManager;

        Node & getCreateNode(nodeID nId) {
            std::map< nodeID, Node >::iterator it = nodes.find(nId);
            if (it == nodes.end()) {
                Node & n = nodes[nId]; //creation of the node
                n.ID = nId;
                return n;
            }
            return it->second;
        }
        void addLink(const tmpLink & l);
    public:
        typedef std::deque<Link>::iterator linksIterator;
        typedef std::deque<Link>::const_iterator const_linksIterator;
        typedef std::map< nodeID, Node >::const_iterator const_NodesIterator;

        const_NodesIterator begin()const {
            return nodes.begin();
        }

        const_NodesIterator end()const {
            return nodes.end();
        }

        const_linksIterator beginLinks()const {
            return links.begin();
        }

        const_linksIterator endLinks()const {
            return links.end();
        }

        linksIterator beginLinks() {
            return links.begin();
        }

        linksIterator endLinks() {
            return links.end();
        }


        linkStream(const char *, bool communityPresent);


        Link & link(linkID lId) {
            return links[lId];
        }

        const Link & link(linkID lId)const {
            return links[lId];
        }

        const Node & node(nodeID nId)const {
            std::map< nodeID, Node >::const_iterator it = nodes.find(nId);
            if (it == nodes.end()) {
                std::cerr << "Wrong NodeId in linkNeighbor" << std::endl;
                throw std::string("Wrong NodeId in linkNeighbor");
            }
            return it->second;
        }

        void printDegreeList(std::ostream & f = std::cout)const;

        unsigned int getNbLinks()const {
            return links.size();
        }

        const std::deque<Link *> & eventList(nodeID a, nodeID b)const;

        unsigned int getNbNodes()const {
            return nodes.size();
        }

    };

} //namespace end





#endif

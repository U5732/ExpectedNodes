
#ifndef COMMUNITY_H
#define COMMUNITY_H

#include <iostream>
#include "linkStream.hpp"
#include "log.hpp"
#include <cstdlib>
#include <algorithm>
#include <string>
#include <ctime>
#include <map>
#include <vector>
namespace gaumont {

    class CommunityManager {
        friend class deltaCore;
    public:
        friend class GreedyMethod;

        class COM {
        public:
            typedef linkSet::iterator iterator;
            typedef linkSet::const_iterator const_iterator;
            typedef nodeSet::const_iterator const_nodeIterator;
        private:
            friend class CommunityManager;
            linkSet com; // Store the link inside the community
            nodeSet nodes; //
            linkStream & S; // to be able to read a Link.

            mutable bool upToDateAdja;
            mutable bool upToDateQual;
            mutable double curQual;
            mutable nodeSet adjaNodes;
            mutable linkSet adjaLinks;

            typedef nodeSet::iterator nodeIterator;


            COM();
            const COM & operator=(const COM &); //prevent assignation constructor

            // Merge c into this without updating the community manager.
            void merge(COM & c);



            // insert with hint.

            COM::iterator add(COM::iterator comIt, Link& l) {
                upToDateAdja = false;
                upToDateQual = false;
                return com.insert(comIt, &l);
            }
        public:
            const unsigned int comID;

            COM(linkStream & S_, unsigned int cId) : S(S_), upToDateAdja(false),upToDateQual(false),curQual(-1), comID(cId) {
            }

            void clear() {
                com.clear();
                nodes.clear();
                upToDateAdja = false;
                upToDateQual = false;
            }

            void add(Link & l) {
                l.com = comID;
                // Update the inside of the com
                com.insert(& l);
                nodes.insert(& l.left());
                nodes.insert(& l.right());

                upToDateAdja = false;
                upToDateQual = false;
            }

            void remove(Link & l);

            void display(std::string m = "")const;
            void displayNode(std::string m = "")const;

            void displayStat(std::string m = "")const;

            iterator begin() {
                return com.begin();
            }

            iterator end() {
                return com.end();
            }

            const_iterator begin()const {
                return com.begin();
            }

            const_iterator end()const {
                return com.end();
            }

            const_nodeIterator beginNode()const {
                return nodes.begin();
            }

            const_nodeIterator endNode()const {
                return nodes.end();
            }

            bool empty()const {
                return com.empty();
            }

            unsigned int linkSize()const {
                return com.size();
            }

            // Compute the sum over the nodes inside this of their degree.
            unsigned int degree()const;
            // Compute the sum over the nodes inside tis or C2 of their degree.
            unsigned int unionDegree(const COM & C2)const;

            //Return a nodeSet of all the nodes inside this (V_in)
            const nodeSet & nodesInCom()const;

            // Fil res with pointer of nodes whoch are inside this or C2.
            void unionNodes(const COM & C2, nodeSet & res)const;

            // Count the union of node which are in this or C2
            unsigned int countUnionNode(const COM & C2)const;

            unsigned int nodeSize()const {
                return nodes.size();
            }

            //Compute the nodeSize of this if l were inside this.
            unsigned int nodeSizeWithExtraLink(const Link & l)const;

            //Compute the nodeSize of this if l weren't inside this.
            unsigned int nodeSizeWithoutLink(const Link & l)const;

            // Count the edges linked with "node" but not in the com.
            unsigned int countExterneEdge(linkStream::const_NodesIterator node)const;

            // Count the edges linked with "node" but not in the com nor in C2.
            unsigned int countExterneEdge(linkStream::const_NodesIterator node, const COM & C2)const;

            // Return a nodeSet and a linkSet wich are adjacent to this. (V_out and L_out)
            std::pair<const nodeSet *, const linkSet *> adjacentNodesAndLink()const;
            // fill a nodeSet and a linkSet with nodes and links wich are adjacent to the union of this and C2. (V_out and L_out)
            void adjacentNodesAndLink(const COM & C2, nodeSet & resNode, linkSet & resLink)const;

            std::pair<unsigned int, unsigned int> countAdjacentNodesAndLink()const;

            // Count the number of node and link ouside the community(this+C2).
            std::pair<unsigned int, unsigned int> countAdjacentNodesAndLink(const COM & C2)const;

            // Return a nodeSet and a linkSet wich are at the bordure to this. (V_bordure and L_out)
            void bordureNodes(nodeSet & resNode, bool toClear=true)const;
            // fill a nodeSet and a linkSet with nodes and links wich are adjacent to the union of this and C2. (V_out and L_out)
            void bordureNodes(const COM & C2,nodeSet & resNode)const;
            unsigned int countBordureNodes()const;
            // Count the number of node and link ouside the community(this+C2).
            unsigned int countBordureNodes(const COM & C2)const;

            //Fill res with the node at the intersection between this and C2.
            void nodeIntersection(const COM & C2, std::deque<Node *> & res)const;



            // Store q as the current quality. It's enable computation if the quality function use it.
            void setQuality(double q)const{curQual=q; upToDateQual=true;}
            // Return the current quality of the community along with status (Uptodate or outdated)
            std::pair<bool,double> getQuality()const{return std::pair<bool,double>(upToDateQual,curQual);}

        };
        typedef std::map<unsigned int, COM >::const_iterator const_iterator;

    private:
        linkStream & S;
        std::map<unsigned int, COM > community;


        COM & getCommunity(unsigned int cId, bool createIfNotPresent = false);



        CommunityManager();
        CommunityManager(const CommunityManager &);
        const CommunityManager & operator=(const CommunityManager &); //prevent assignation constructor

        
    public:
        CommunityManager(linkStream & S_,bool single) : S(S_), community() {
            linkStream::linksIterator it = S_.beginLinks(),
                    ite = S_.endLinks();
            COM &comm = getCommunity(1, true);
            for (; it != ite; ++it) {    
                comm.add(*it);
            }
        }
        CommunityManager(linkStream & S_) : S(S_), community() {
            linkStream::linksIterator it = S_.beginLinks(),
                    ite = S_.endLinks();
            for (; it != ite; ++it) {
                COM &comm = getCommunity(it->com, true);
                comm.add(*it);
            }
        }

        // merge cId2 into cId1
        // return the id of the merged community (normally cId1);
        unsigned int merge(unsigned int cId1, unsigned int cId2);

        // Move the link lId into the commuity cId
        void moveLinkInto(linkID lId, unsigned int cId);
        
        //Below const method

        void display(std::string m = "")const;

        template<class Iterator>
        unsigned int comId(Iterator it)const;

        const_iterator begin()const {
            return community.begin();
        }

        const_iterator end()const {
            return community.end();
        }

        unsigned int linkCommunity(linkID lId)const {
            return S.link(lId).com;
        }

        unsigned int size()const {
            return community.size();
        }

        // Get the community corresponding to cId.
        // It thtrows an error if cId is unkown
        const COM & getCommunity(unsigned int cId)const;

        // Fill res so that it contains all the adjacent community of lId execpt the one in whiche lId is.		
        void fillAdjacentComunityToLink(linkID lId, std::set< unsigned int> & res)const;
        void fillAdjacentComunityToCom(const COM & C, std::set< unsigned int> & res)const;

        bool communityExist(unsigned int cId)const {
            std::map<unsigned int, COM >::const_iterator it;
            it = community.find(cId);
            return !(it == community.end());
        }
    };

    template<> inline
    unsigned int CommunityManager::comId<CommunityManager::const_iterator>(CommunityManager::const_iterator it)const {
        return it->first;
    }

    template<> inline
    unsigned int CommunityManager::comId<std::set< unsigned int>::iterator>(std::set< unsigned int>::iterator it)const {
        return *it;
    }


}
#endif

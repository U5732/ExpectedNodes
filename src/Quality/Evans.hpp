#ifndef EVANS_H

#define EVANS_H

#include "quality.hpp"
#include "../linkStream.hpp"
#include "../community.hpp"
#include <string>
#include <math.h>


namespace gaumont {

//  Compute the Evans1 and lambiotte qualite version 1 (without weight)
    class Evans1 : public Quality {
        double computeQual(const CommunityManager::COM & C)const;

        std::pair<unsigned int, unsigned int> computeIncidenceAndWeight(const CommunityManager::COM & C)const;
        double wrapperComputeQual(unsigned int incidence, unsigned int comWeight, unsigned int totWeight)const;

        Evans1();
        Evans1(const Evans1 &);
        const Evans1 & operator=(const Evans1 &);

        unsigned int edgeDegreeSum;

    public:
        static const std::string  name;
        Evans1(const linkStream & S_, const CommunityManager & C_) : Quality("Evans1", S_, C_) {
            linkStream::const_NodesIterator nIt = S.begin(), nIte = S.end();
            edgeDegreeSum = 0;
            for (; nIt != nIte; ++nIt) {
                unsigned int curD = nIt->second.degree();
                edgeDegreeSum += curD*(curD-1);
            }
        };

        double globalQual()const{
            CommunityManager::const_iterator it = C.begin(),
                        ite = C.end();
                double res = 0;
                for (; it != ite; ++it) {
                    double val = comQual(it->second);
                    res += val;
                }
                return res;
        }

    };

    class Evans2 : public Quality {
        Evans2(); /* prevent this*/
        Evans2(const Evans2 &);/* prevent this*/
        const Evans2 & operator=(const Evans2 &);/* prevent this*/

        double computeQual(const CommunityManager::COM & C)const;

        std::pair<double, double> computeIncidenceAndWeight(const CommunityManager::COM & C)const;
        double wrapperComputeQual(double incidence, double comWeight, unsigned int totWeight)const;

        unsigned int deg(const Link & l)const{
            unsigned int res =2;
            if(l.left().degree() ==1)
                res--;
            if(l.right().degree() ==1)
                res--;

            return res;
        }
        unsigned int edgeDegreeSum;

        public:
            static const std::string  name;
            Evans2(const linkStream & S_, const CommunityManager & C_) : Quality("Evans2", S_, C_) {
                linkStream::const_linksIterator lIt = S.beginLinks(), lIte = S.endLinks();
                edgeDegreeSum = 0;
                for (; lIt != lIte; ++lIt) {
                    unsigned int curD = deg(*lIt);
                    edgeDegreeSum += curD;
                }
            };

            double globalQual()const{
            CommunityManager::const_iterator it = C.begin(),ite = C.end();
                double res = 0;
                for (; it != ite; ++it) {
                    double val = comQual(it->second);
                    res += val;
                }
                return res;
        }
    };

    class Evans3 : public Quality {
        Evans3(); /* prevent this*/
        Evans3(const Evans3 &);/* prevent this*/
        const Evans3 & operator=(const Evans3 &);/* prevent this*/

        double computeQual(const CommunityManager::COM & C)const;

        std::pair<double, double> computeIncidenceAndWeight(const CommunityManager::COM & C)const;
        double wrapperComputeQual(double incidence, double comWeight, unsigned int totWeight)const;


        double gatherIncidence(const CommunityManager::COM & C, const Link & l, const Node & n)const;

        unsigned int edgeDegreeSum;

        public:
            static const std::string  name;
            Evans3(const linkStream & S_, const CommunityManager & C_) : Quality("Evans3", S_, C_) {
                edgeDegreeSum = S.getNbLinks()*2;
            };

            double globalQual()const{
            CommunityManager::const_iterator it = C.begin(),ite = C.end();
                double res = 0;
                for (; it != ite; ++it) {
                    double val = comQual(it->second);
                    res += val;
                }
                return res;
        }
    };


}



#endif

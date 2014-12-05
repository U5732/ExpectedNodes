#ifndef _QUALITY_H
#define _QUALITY_H

#include "../linkStream.hpp"
#include "../community.hpp"
#include <string>
#include <iostream>


namespace gaumont {
    class Quality {
    protected:

        const linkStream & S;
        const CommunityManager & C;

    private:
        Quality(const Quality &); //prevent copy constructor
        Quality(); //prevent default constructor
        const Quality & operator=(const Quality &); //prevent assignation constructor


        //Compute the positive side quality of a community.
        virtual double computeQual(const CommunityManager::COM & C)const = 0;

    public:
        const std::string name;

        Quality(const std::string n, const linkStream & S_, const CommunityManager & C_) : S(S_), C(C_), name(n) {
        }

        double comQual(const CommunityManager::COM & C1)const {
            return computeQual(C1);
        }

        virtual double globalQual()const;

        void print()const;

        virtual ~Quality() {
        }
    };



}

#endif

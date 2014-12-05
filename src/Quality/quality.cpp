#include "quality.hpp"
using namespace gaumont;

void Quality::print()const {
    std::cout << name << std::endl;
}

double Quality::globalQual()const {
    CommunityManager::const_iterator it = C.begin(),
            ite = C.end();
    double res = 0;
    for (; it != ite; ++it) {
        double val = comQual(it->second) * it->second.linkSize();
        res += val;
    }
    res = res / (1.0 * S.getNbLinks());
    return res;

}

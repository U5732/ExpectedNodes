#ifndef _LOG_H

#define _LOG_H

#include <iostream>
#include <string>

class Log {
    static unsigned int verboseLvl;
    static std::ostream & out;
public:

    static void setLvl(unsigned int v) {
        verboseLvl = v;
    }

    static unsigned int getLvl() {
        return verboseLvl;
    }

    static void log(unsigned int lvl, const std::string & s) {
        if (lvl <= verboseLvl)
            out << s << std::endl;
    }
};

#endif
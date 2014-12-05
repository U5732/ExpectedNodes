

#include "greedyMethod.hpp"
#include <iostream>
#include <time.h>
#include <iostream>
#include <limits>
#include <iomanip>
#include <exception>
#include <string>
#include "log.hpp"

enum MOUVEMENT {
    ALL, FUSION, MOVE, NOTHING, TEST
};

std::ostream& operator<<(std::ostream & os, const MOUVEMENT & l) {
    if (l == ALL) {
        os << "All";
    } else if (l == MOVE) {
        os << "Move";
    } else if (l == FUSION) {
        os << "Fusion";
    } else if (l == NOTHING) {
        os << "None";
    } else if (l == TEST) {
        os << "Testing";
    } else {
        os << "Unknown";
    }
    return os;
}

MOUVEMENT movefacto(const std::string & s) {
    std::string S = s;
    std::transform(S.begin(), S.end(), S.begin(), toupper);
    if (S == "ALL") {
        return ALL;
    } else if (S == "MOVE") {
        return MOVE;
    } else if (S == "FUSION" || S == "MERGE") {
        return FUSION;
    } else if (S == "TEST"){
        return TEST;   
    } else if (S == "NOTHING" || s == "NONE") {
        return NOTHING;
    }
    std::cerr << "Mouvement type :" << s << " is unknown." << std::endl;
    exit(-1);
}

static void show_usage(std::string name) {
    std::cerr << "Usage: " << name << std::endl
            << "-i|c input_file [-o outfile] [-v lvl] " << std::endl
            << "Options:\n"
            << "\t-c,--community input\t\tfile to read which also stored community 't u v com'\n"
            << "\t-h,--help\t\tShow this help message\n"
            << "\t-i,--input input\t\t file to read 't u v'\n"
            << "\t-m, mouvementType\t\t All, Fusion, Move'\n"
            << "\t-o,--output output\t\t file to write\n"
            << "\t-q quality\t\t quality function used \n"
            << "\t-v,--verose lvl\t\t verbose level to use"
            << std::endl;
    exit(-1);
}

struct Argument {
    char * outputFile;
    char * inputFile;
    bool com;
    unsigned int logLevel;
    std::string quality;
    MOUVEMENT mouvement;
};

static Argument parse_args(int argc, char **argv) {
    Argument res;
    res.outputFile = NULL;
    res.inputFile = NULL;
    res.com = false;
    res.logLevel = 1;
    res.quality = "0";
    res.mouvement = ALL;
    if (argc < 2) {
        show_usage(argv[0]);
        exit(-1);
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            show_usage(argv[0]);
        } else if ((arg == "-i") || (arg == "--input")) {
            if (i + 1 < argc) {
                res.inputFile = argv[++i];
                std::cout << "InputFile = " << res.inputFile << std::endl;
            } else {
                std::cerr << "--input option requires one argument." << std::endl;
                exit(-1);
            }
        } else if ((arg == "-c") || (arg == "--community")) {
            if (i + 1 < argc) {
                res.inputFile = argv[++i];
                res.com = true;
                std::cout << "InputFile with community structure =" << res.inputFile << std::endl;
            } else {
                std::cerr << "--community option requires one argument." << std::endl;
                exit(-1);
            }
        } else if ((arg == "-v") || (arg == "--verbose")) {
            if (i + 1 < argc) {
                res.logLevel = atoi(argv[++i]);
                std::cout << "Log level = " << res.logLevel << std::endl;
            } else {
                res.logLevel = 10;
                std::cout << "Log level = " << res.logLevel << std::endl;
            }
        } else if ((arg == "-m")) {
            if (i + 1 < argc) {
                res.mouvement = movefacto(argv[++i]);
                std::cout << "Mouvement = " << res.mouvement << std::endl;
            } else {
                std::cerr << "-m option requires one argument -> All or Fusion or Move" << std::endl;
                exit(-1);
            }
        } else if ((arg == "-o") || (arg == "--output")) {
            if (i + 1 < argc) {
                res.outputFile = argv[++i];
                std::cout << "OutputFile =" << res.outputFile << std::endl;
            } else {
                std::cerr << "--output option requires one argument." << std::endl;
                exit(-1);
            }
        } else if ((arg == "-q")) {
            if (i + 1 < argc) {
                res.quality = argv[++i];
            } else {
                std::cerr << "-q option requires one argument-> a number representing the quality function." << std::endl;
                exit(-1);
            }
        } else {
            show_usage(argv[0]);
        }
    }
    return res;
}

int main(int argc, char **argv) {

    clock_t t;
    t = clock();

    

    Argument arg = parse_args(argc, argv);
    Log::setLvl(arg.logLevel);

    gaumont::GreedyMethod g(arg.inputFile, arg.com, arg.quality);

    t = clock() - t;

    Log::log(1, "Input file readed." + gaumont::toStr(((float) t) / CLOCKS_PER_SEC) + "s");

    bool ameliorable;
    t = clock();
    if(arg.mouvement == TEST){
        Log::log(1, "Final quality :" + gaumont::toStr(g.curQual()));   
        return 1;
    }
    if (arg.mouvement != NOTHING) {
        do {
            ameliorable = false;
            if (arg.mouvement == ALL || arg.mouvement == FUSION) {
                Log::log(2, "Now try to merge communities together.");
                ameliorable = g.communtyMerging() || ameliorable;
            }
            if (arg.mouvement == ALL || arg.mouvement == MOVE) {
                Log::log(2, "Now try to move one link from its community to another.");
                ameliorable = g.linksMoving() || ameliorable;
            }

        } while (ameliorable);
    } else {
        g.test();
    }

    t = clock() - t;
    Log::log(1, "Final quality :" + gaumont::toStr(g.curQual()));


    Log::log(1, "End of communties detection in " + gaumont::toStr(((float) t) / CLOCKS_PER_SEC) + "s");
    if (arg.outputFile != NULL) {
        std::ofstream fichier2(arg.outputFile, std::ios::out | std::ios::trunc);
        g.displayPartition(fichier2);
        Log::log(1, "OutputFile writed in :" + gaumont::toStr(arg.outputFile));
        fichier2.close();
    }


    return 0;
}

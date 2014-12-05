


#include <iostream>
#include <time.h>
#include <iostream>
#include <limits>
#include <iomanip>
#include <exception>
#include <string>

#include "log.hpp" // choose he level of verbose
#include "linkStream.hpp" // Graph structure
#include "community.hpp" // Communtity Structure
#include "qualityFactory.hpp" //Quality functions factory

static void show_usage(std::string name) {
	std::cerr << "Usage: " << name << std::endl
			<< "-i input_file [-q qualities] " << std::endl
			<< "Options:\n"
			<< "\t-h,--help\t\t Show this help message\n"
			<< "\t-i,--input input\t\t file to read  as edge list'u v com'\n"
			<< "\t-q quality\t\t quality function used (number or text comma separated) \n"
			<< "\t-v verbose\t\t verbose level 0 or 1 (default=1) \n"
			<< std::endl;
	exit(-1);
}

struct Argument {
	char * inputFile;
	std::string quality;
	unsigned int verbose;
};

static Argument parse_args(int argc, char **argv) {
	Argument res;
	res.inputFile = NULL;
	res.quality = "0";
	res.verbose=1;
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
			} else {
				std::cerr << "--input option requires one argument." << std::endl;
				exit(-1);
			}
		}else if ((arg == "-q")) {
			if (i + 1 < argc) {
				res.quality = argv[++i];
			} else {
				std::cerr << "-q option requires one argument-> a comma separated list of quality function id or name.\n Example: -q 1,2,E3" << std::endl;
				exit(-1);
			}
		}else if ((arg == "-v")) {
			if (i + 1 < argc) {
				res.verbose = atoi(argv[++i]);
			} else {
				std::cerr << "-v option requires one argument-> The verbose level" << std::endl;
				exit(-1);
			}
		} else {
			show_usage(argv[0]);
		}
	}
	return res;
}

int main(int argc, char **argv) {

	clock_t t,start;
	start = clock();

	Argument arg = parse_args(argc, argv);
	Log::setLvl(arg.verbose);
	gaumont::linkStream graph(arg.inputFile,true);
	t = clock() - start;
	Log::log(1, "Input file, "+gaumont::toStr(arg.inputFile)+", read in " + gaumont::toStr(((float) t) / CLOCKS_PER_SEC) + "s");

	gaumont::CommunityManager C(graph);
	std::vector<gaumont::Quality*> quals= gaumont::QualityFactory::create(arg.quality,graph,C);

	for(unsigned int i=0; i<quals.size(); ++i){
		if (Log::getLvl()<2)
			std::cout<<quals[i]->name<<"= "<<quals[i]->globalQual()<<std::endl;
		else{
			t = clock();
			double q = quals[i]->globalQual();
			std::cout<<quals[i]->name<<"= "<<q <<" in "<<(static_cast<float>(clock()-t) / CLOCKS_PER_SEC) <<" s."<<std::endl;

		}
	}
	t = clock() - start;
	Log::log(2, "The quality function benchmark only evaluates the implementation and not the quality function itself.");
	Log::log(1, "End of the program in " + gaumont::toStr(((float) t) / CLOCKS_PER_SEC) + "s");

	return 0;
}

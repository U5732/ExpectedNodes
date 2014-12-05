#ifndef _QUALITYFACTORY_H
#define _QUALITYFACTORY_H

#include <string>
#include <sstream>
#include <vector>

#include "linkStream.hpp"
#include "community.hpp"

#include "Quality/quality.hpp"
#include "Quality/Evans.hpp"
#include "Quality/density.hpp"
#include "Quality/ExpectedNodes.hpp"


namespace gaumont{

	class QualityFactory{

	public:
		static Quality* make(const std::string & name, const linkStream & S, const CommunityManager & C){
			std::string NAME = name;
    		std::transform(NAME.begin(), NAME.end(), NAME.begin(), toupper);
    		Quality * qual=NULL;
			if(NAME=="1" || NAME==ExpectedNodes::name){
				qual=new ExpectedNodes(S, C);
			}else if(NAME=="2" || NAME==PartitionDensity::name){
				qual=new PartitionDensity(S, C);
			}else if(NAME=="3" || NAME==Evans1::name){
				qual=new Evans1(S, C);
			}else if(NAME=="4" || NAME==Evans2::name){
				qual=new Evans2(S, C);
			}else if(NAME=="5" || NAME==Evans3::name){
				qual=new Evans3(S, C);
			}
			if (qual !=NULL)
				return qual;
			std::cerr<<"Unknow identifier or quality function name :"<<name<<std::endl;
			exit(-1);
		}

		static std::vector<Quality*> create(const std::string & name, const linkStream & S, const CommunityManager & C){
			std::vector<Quality*> res;
			char delim=',';
			std::stringstream ss(name);
			std::string item;
			while (std::getline(ss, item, delim)) {
	  		if(item!="")
					res.push_back(make(item,S,C));
			}
			return res;
		}

	};



}
#endif

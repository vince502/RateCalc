#include "RateCalc.cc"
#include "TGraph.h"
#include <ROOT/TProcessExecutor.hxx>
#include <thread>
#include <future>
#include <execution>

std::unordered_map<string, std::map<double, double> > result_map;
std::map<string, TGraph*> graps;

void calculateRate(){
//	string n_file = "openHLT_Gmenu_JPsiEMB_NoSPClimit_v1230_v8.root";
	string n_file = "openHLT_Gmenu_JPsi_v1230_v9.root";

	std::vector<string> v_names;
	{
		RateCalc _calc = RateCalc(n_file.c_str());
		_calc.setAllHLT();
		for( auto word : _calc.v_names ){
			v_names.push_back(word.first);
			std::cout << word.first << std::endl;
		}
	}

	std::vector<double> cuts;
	int nCores = 15;
	ROOT::EnableImplicitMT();
	ROOT::TThreadExecutor mpe(nCores);

	for(auto idx : ROOT::TSeqI(11)){
		cuts.push_back(0.5 *(double) idx);
	}
	auto extractRates = [=](int idx){
		RateCalc calc = RateCalc(n_file.c_str());
		calc.setObjectTree(v_names[idx], cuts);
		calc.evalAll();
		return calc.getRates();
	};

	const auto& res = mpe.Map(extractRates, ROOT::TSeqI(v_names.size()));
	for( auto idx : res ){
		for( auto item : idx ){
			result_map[item.first] = item.second;
		}
	}

	for( auto result : result_map ){
		graps[result.first] = new TGraph();
		for( auto key_val : result.second){
			graps[result.first]->AddPoint(key_val.first, key_val.second);
		}
	};

};

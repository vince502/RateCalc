#include "RateCalc.cc"
#include "TGraph.h"
#include <ROOT/TProcessExecutor.hxx>
#include <thread>
#include <future>
#include <execution>

static int UseNCores = 30;

void calculateRate(){
	string n_file = "openHLT_Gmenu_JPsiEMB_NoSPClimit_v1230_v8.root";
//	string n_file = "openHLT_Gmenu_JPsi_v1230_v9.root";

	//String vector for trigger names
	std::vector<string> v_names;
	{
		RateCalc _calc = RateCalc(n_file.c_str());
		_calc.setAllHLT();
		for( auto word : _calc.v_names ){
			v_names.push_back(word.first);
		}
	}

	//Enable MultiThreading, the thread executor creates threads per trigger and make use of the available CPU cores
	ROOT::EnableImplicitMT(UseNCores);
	ROOT::TThreadExecutor mpe(v_names.size());

	//Define cuts
	std::vector<double> cuts;
	for(auto idx : ROOT::TSeqI(31)){
		cuts.push_back(0.5 *(double) idx);
	}

	//Run calculator
	auto extractRates = [=](int idx){
		RateCalc calc = RateCalc(n_file.c_str());
		calc.setObjectTree(v_names[idx], cuts);
		calc.evalAll();
		return calc.getRates();
	};

	//Result container
	std::unordered_map<string, std::map<double, double> > result_map;
	std::map<string, TGraph*> graps;

	//Map results
	const auto& res = mpe.Map(extractRates, ROOT::TSeqI(v_names.size()));
	for( auto idx : res ){
		for( auto item : idx ){
			result_map[item.first] = item.second;
		}
	}

	//Draw graphs
	TFile* output = new TFile("output.root", "recreate");
	output->cd();
	for( auto result : result_map ){
		graps[result.first] = new TGraph();
		for( auto key_val : result.second){
			graps[result.first]->AddPoint(key_val.first, key_val.second);
		}
		graps[result.first]->SetName(result.first.c_str());
		graps[result.first]->GetXaxis()->SetTitle("p_T cut (GeV/c)");
		graps[result.first]->GetYaxis()->SetLimits(0,1);
		graps[result.first]->GetYaxis()->SetRangeUser(0,1);
		graps[result.first]->Write();
	}
	output->Close();
};

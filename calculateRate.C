#include "RateCalc.cc"
#include "TGraph.h"
#include <ROOT/TProcessExecutor.hxx>
#include <thread>
#include <future>
#include <execution>

static int UseNCores = 12;

void calculateRate(){
//	string n_file = "openHLT_Gmenu_JPsiEMB_NoSPClimit_v1230_v8.root";
//	n_file = "openHLT_HIMuon_MBPD2018_v1230_v9.root";
//	string n_file = "openHLT_HIMuon_LegacyTrigger_MBPD2018.root";
//	string name_output.c_str = "output_Rates_MB2018_legacy_v2.root";

//	string n_file = "/home/CMS/soohwan/store/openHLT_data2018MB_NewMenu_v2_7_mod1.root";
//	string name_output= "output_Rates_NewMenu_v2_7_mod1_v2.root";

//	string n_file = "/home/CMS/soohwan/store/openHLT_NewMumenu_JPsi_v2_9.root";
//	string name_output= "output_Rates_NewMenu_JPsi_v2_9.root";

//	string n_file = "/home/CMS/soohwan/store/openHLT_NewMumenu_JPsiEMB_v2_9.root";
//	string name_output= "output_Rates_NewMenu_JPsiEMB_v2_9.root";

//	string n_file = "/home/CMS/soohwan/store/openHLT_NewMumenu_JPsiEMB_pTHat2_v2_11_mod2.root";
//	string name_output= "output_Rates_NewMenu_JPsiEMB_pTHat2_v2_11_mod2.root";

//	string n_file = "/home/CMS/soohwan/store/openHLT_NewMuMenu_DataMBPD2_v2_11.root";
//	string name_output= "output_Rates_NewMenu_DataMBPD2_v2_11_2.root";

//	string n_file = "/home/CMS/soohwan/store/openHLT_data2018MB_NewMenu_v2_15.root";
//	string name_output= "output_Rates_NewMenu_DataMBPD2_v2_15.root";

//	string n_file = "/home/CMS/soohwan/store/openHLT_data2018MB_NewMenu_v2_15_mod2.root";
//	string name_output= "output_Rates_NewMenu_DataMBPD2_v2_15_mod2.root";
	 
//	string n_file = "/home/CMS/soohwan/store/openHLT_NewMumenu_JPsi_pTHat2_v2_17.root";
//	string name_output= "output_Rates_NewMenu_JPsi_v2_17.root";

//	string n_file = "/home/CMS/soohwan/store/openHLT_data2018MB_NewMenu_v2_19.root";
//	string name_output= "output_Rates_NewMenu_DataMBPD2_v2_19.root";

//	string n_file = "/home/CMS/soohwan/store/openHLT_data2018MB_NewMenu_v3_8.root";
//	string name_output= "output_Rates_NewMenu_DataMBPD2_v3_8.root";

//	string n_file = "/home/CMS/soohwan/store/openHLT_NewMumenu_Data_v3_8_mod1.root";
//	string name_output= "output_Rates_NewMenu_DataMBPD2_v3_8_mod1.root";

	string n_file = "/home/CMS/soohwan/store/openHLT_NewMumenu_Data_v1_9.root";
	string name_output= "output_Rates_NewMenu_DataMBPD2_v3_9.root";

	//String vector for trigger names
	std::vector<std::pair<string, string> > v_names;
	std::vector<std::pair<string, std::pair<double, double> > > v_masscuts;
	{
		RateCalc _calc = RateCalc(n_file.c_str());
		_calc.setAllHLT();
		for( auto word : _calc.v_names ){
			v_names.push_back(std::make_pair(word.first, word.first));
			v_masscuts.push_back(std::make_pair(word.first, std::make_pair(-1, 9999.)));
		}
	}
//	v_names = {"HLT_HIL3Mu0NHitQ10_L2Mu0_MAXdR3p5_M1to5_v", "HLT_HIL3Mu2p5NHitQ10_L2Mu2_M7toinf_v", "HLT_HIL1DoubleMuOpen_v", "HLT_HIL2DoubleMuOpen_v", "HLT_HIL3DoubleMuOpen_v"};
	v_names.push_back(std::make_pair("L2DoubleMuOpen_DoHLTCut_Mass","HLT_HIL2DoubleMu0_Open_v"));
	v_names.push_back(std::make_pair("L3DoubleMuOpen_DoHLTCut_Mass","HLT_HIL3DoubleMu0_Open_v"));
	v_masscuts.push_back(std::make_pair("L2DoubleMuOpen_DoHLTCut_Mass",std::make_pair( 1., 6.5 )));
	v_masscuts.push_back(std::make_pair("L3DoubleMuOpen_DoHLTCut_Mass",std::make_pair( 2., 4.0 )));
	v_names.push_back(std::make_pair("L2DoubleMuZero_DoHLTCut_Mass","HLT_HIL2DoubleMu0_Zero_v"));
	v_names.push_back(std::make_pair("L3DoubleMuZero_DoHLTCut_Mass","HLT_HIL3DoubleMu0_Zero_v"));
	v_masscuts.push_back(std::make_pair("L2DoubleMuZero_DoHLTCut_Mass",std::make_pair( 1., 6.5 )));
	v_masscuts.push_back(std::make_pair("L3DoubleMuZero_DoHLTCut_Mass",std::make_pair( 2., 4.0 )));
	v_names.push_back(std::make_pair("L2DoubleMuSQ_DoHLTCut_Mass","HLT_HIL2DoubleMu0_SQ_v"));
	v_names.push_back(std::make_pair("L3DoubleMuSQ_DoHLTCut_Mass","HLT_HIL3DoubleMu0_SQ_v"));
	v_masscuts.push_back(std::make_pair("L2DoubleMuSQ_DoHLTCut_Mass",std::make_pair( 1., 6.5 )));
	v_masscuts.push_back(std::make_pair("L3DoubleMuSQ_DoHLTCut_Mass",std::make_pair( 2., 4.0 )));

//	v_names.push_back(std::make_pair("HLT_HIL2DoubleMu0_M1to6p5Acop1toPi_Open_v1",".HLT_HIL2DoubleMu0_M1to6p5Acop1toPi_Open_v1"));
//	v_names.push_back(std::make_pair("HLT_HIL2DoubleMu0_M1to6p5Acop1toPiAlphamiPito1_Open_v1","HLT_HIL2DoubleMu0_M1to6p5Acop1toPiAlphamiPito1_Open_v1"));
//	v_names.push_back(std::make_pair("HLT_HIL3DoubleMu0_M2to4p5Acop1p2toPiOpen_v1","HLT_HIL3DoubleMu0_M2to4p5Acop1p2toPiOpen_v1"));

	//Enable MultiThreading, the thread executor creates threads per trigger and make use of the available CPU cores
//	ROOT::EnableImplicitMT(UseNCores);
	ROOT::TThreadExecutor mpe(UseNCores);

	//Define cuts
	std::vector<double> cuts;
	for(auto idx : ROOT::TSeqI(31)){
		cuts.push_back(0.5 *(double) idx);
	}
	for(auto idx : ROOT::TSeqI(19)){
		cuts.push_back(2.0 + 0.1 *(idx+1));
	}
	cuts.erase(unique(cuts.begin(),cuts.end()), cuts.end());
	std::sort(cuts.begin(), cuts.end(),
		[](double a, double b){ return a < b; }
	);

	//Run calculator
	int max_events = -1; //Always should be lower than total event in data, -1 for all event
	auto extractRates = [=](int idx){
		RateCalc calc = RateCalc(n_file.c_str());
		calc.setObjectAlias(v_names[idx].first, v_names[idx].second);
		calc.setObjectTree(v_names[idx].first, cuts);
		calc.setObjectMassCut(v_masscuts[idx].first, v_masscuts[idx].second);
		calc.evalAll(max_events);
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
	TFile* output = new TFile(name_output.c_str(), "recreate");
	output->cd();
	for( auto result : result_map ){
		graps[result.first] = new TGraph();
		for( auto key_val : result.second){
			graps[result.first]->AddPoint(key_val.first, key_val.second);
		}
		graps[result.first]->SetName(result.first.c_str());
		graps[result.first]->SetMarkerStyle(kFullCircle);
		graps[result.first]->SetMarkerSize(1.2);
		graps[result.first]->GetXaxis()->SetTitle("p_T cut (GeV/c)");
		graps[result.first]->GetYaxis()->SetTitle("Rate (HLT L(N) / Total Events)");
		graps[result.first]->GetYaxis()->SetLimits(0,1);
		graps[result.first]->GetYaxis()->SetRangeUser(0,1);
		graps[result.first]->Write();
	}
	output->Close();
};

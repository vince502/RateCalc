#pragma once

#include "RateCalc.h"
#include "HltInput.cc"


RateCalc::RateCalc(string name_file) : HltInput(name_file){
};

RateCalc::~RateCalc(){
};

void RateCalc::setAllHLT(){
	auto blist = ((TDirectoryFile*) file_hlt->Get("hltobject"))->GetListOfKeys();
	for( auto nameObj : *blist){
		string name = nameObj->GetName();
		this->setObjectTree(name);
	}
};

void RateCalc::setTriggerCuts(std::vector<double> cuts){
	for( auto names : v_names ){
		map_cuts[names.first] = cuts;
	}
};

void RateCalc::setTriggerCuts(string name_hltobj, std::vector<double> cuts){
	map_cuts[name_hltobj] = cuts;
};

bool RateCalc::setObjectTree(string name_hltobj){
	map_hltobj[name_hltobj] = getObjectTree(name_hltobj);
	v_names[name_hltobj] = true;
	return true;
};

bool RateCalc::setObjectTree(std::vector<string> names){
	for( auto name : names){
		setObjectTree(name);
	}
	return true;
};

bool RateCalc::setObjectTree(string name, std::vector<double> cuts){
	setObjectTree(name);
	setTriggerCuts(name, cuts);
	return true;
};

bool RateCalc::setObjectTree(std::vector<string> names, std::vector<double> cuts){
	for( auto name : names){
		setObjectTree(name);
		setTriggerCuts(name, cuts);
	}
	return true;
};


bool RateCalc::unsetObjectTree(string name_hltobj){
	map_hltobj.erase(name_hltobj);
	v_names.erase(name_hltobj);
	return true;
};

bool RateCalc::unsetObjectTree(std::vector<string> names){
	for( auto name : names){
		unsetObjectTree(name);
	}
	return true;
};

int RateCalc::needMuons(string name_hltobj){
	if( name_hltobj.find("Double") != std::string::npos) return 2;
	return 1;
};

std::unordered_map<string, std::map<double, double> > RateCalc::getRates(){
	int total_evt = tree_hlt->GetEntries();
	std::unordered_map<string, std::map<double, double> > dict;
	for(auto cont : map_cutNpasses){
		for( auto vals : cont.second){
			dict[cont.first][vals.first] = ((double) vals.second ) / ((double) total_evt);
		}
	}
	return dict;
};

std::pair<string, std::map<double, int> > RateCalc::evalTrigger(string name_hltobj, const bool set_prescl = false){
	if( !checkObjectTree(name_hltobj) ) { std::cout << name_hltobj.c_str() << " not set!" << std::endl; return std::make_pair(name_hltobj, std::map<double, int> {}); }

	HltObj obj{}; 
	bool hlt_bit; 
	float hlt_prescl;
	std::map<double, int> map_cutNpass;
	initTuple(map_cutNpass, map_cuts[name_hltobj]);
	int Nrequiremuons = needMuons(name_hltobj);

	map_hltobj[name_hltobj]->SetBranchAddress("pt", &(obj.pt));
	map_hltobj[name_hltobj]->SetBranchAddress("eta", &(obj.eta));
	tree_hlt->SetBranchAddress(Form("%s",name_hltobj.c_str()), &hlt_bit); 
	if( set_prescl ){
		tree_hlt->SetBranchAddress(Form("%s_Prescl",name_hltobj.c_str()), &hlt_bit ); 
	}
	int countx= 0;
	for( auto idx : ROOT::TSeqI(tree_hlt->GetEntries())){
		if( (countx % 1000) == 0 ) std::cout << "Iterating " << countx << std::endl;
		map_hltobj[name_hltobj]->GetEntry(idx);
		tree_hlt->GetEntry(idx);
		fillTuple(map_cutNpass, obj, hlt_bit, Nrequiremuons);
		countx ++;
	}
	return std::make_pair(name_hltobj, map_cutNpass);
};


bool RateCalc::evalAll(bool set_prescl = false){
	for( auto name_ : v_names ){
		string name = name_.first;
		map_cutNpasses[name] = evalTrigger(name).second;
	}
	return true;
};

bool RateCalc::checkObjectTree(string name_hltobj){
	if( map_hltobj[name_hltobj] ) return true;
	else return false;
};

void RateCalc::fillTuple( std::map<double, int>& tuple , HltObj obj, int hlt_bit, int muons){
	std::map<double, int>::reverse_iterator tupleit = tuple.rbegin();
	 while( tupleit != tuple.rend()){
		if(hlt_bit){
			tuple[(*tupleit).first] += ( (int) passCut(obj, (*tupleit).first, muons) );
			if(tuple[(*tupleit).first] !=0 ){ 
				tupleit++;
	 			while( tupleit != tuple.rend()){
					tuple[(*tupleit).first] += 1;
					tupleit++;
				}
				break;
			}
		}
		tupleit++;
	};
};

void RateCalc::initTuple(std::map<double, int>& tuple, std::vector<double> cuts){
	tuple = std::map<double, int>{};
	for( auto cut : cuts ){
		tuple.insert({cut, 0});
	}
};

bool RateCalc::passCut(HltObj obj, double cut, int muons){
	auto pts = *obj.pt;
	auto etas = *obj.eta;
	int num_cands = pts.size();
	int passMuons = 0;
	for( auto idx : ROOT::TSeqI(num_cands) ){
//		std::cout << Form("%.2f, %.2f", etas[idx], pts[idx] ) << std::endl;
		if( fabs(etas[idx]) < 2.4 ){
			if( pts[idx] >= cut ) passMuons ++;
		}
	}
	if ( passMuons >= muons ) { return true; }
	return false;
};










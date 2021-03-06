/*
 * FermiEnergy.cc
 * 
 * Copyright 2013 Jean Diehl <jdiehl@itp.uni-frankfurt.de>
 * 
 */

#include "FermiEnergy.h"

FermiEnergy::FermiEnergy(std::string fn) : FileGeneral::FileGeneral(fn, "scf") {}
FermiEnergy::~FermiEnergy() {}

void FermiEnergy::read(double& EF) {
	std::string line;
	
	if (myfile.is_open()) {
		while ( myfile.good() ) {
			getline (myfile,line);
			
			if (line.length() > 47) {
				
				if (line.substr(0,4).compare(":FER") == 0) {
					std::stringstream s(line.substr(41,7));
					
					s >> EF;
				}
				
				
			}
		}
	}
}

void FermiEnergy::read(double& EF, double& volUnitcell) {
	std::string line;
	
	if (myfile.is_open()) {
		while ( myfile.good() ) {
			getline (myfile,line);
			
			if (line.length() > 47) {
				
				if (line.substr(0,4).compare(":FER") == 0) {
					std::stringstream s(line.substr(41,7));
					
					s >> EF;
				}
			}
            if (line.length() > 39) {
				
				if (line.substr(0,4).compare(":VOL") == 0) {
					std::stringstream s(line.substr(26,line.length()-26));
					
					s >> volUnitcell;
				}
			}
		}
	}
}

/*
 * run_spec.cc
 * 
 * Copyright 2013 Jean Diehl <jdiehl@itp.uni-frankfurt.de>
 * 
 */

#include "WannierProj"

#include <unistd.h> // for getcwd
#include <complex>
#include <iostream>
#include <vector>
#include <fstream>
#include <Eigen/Core>

int main(int argc, char **argv) {

	// get current working directory
	char cwd[256];
	getcwd(cwd, sizeof(cwd));
	std::string path = std::string(cwd);

	unsigned pos = path.find_last_of("/");
	std::string w2kProjectName = path.substr(pos + 1, path.length() - (pos + 1) );
	
	std::vector<int> multiplicities;
	std::vector<std::string> atomNames;
	std::cout << "# Read *.struct file" << std::endl;
	FileStruct fileStruct(w2kProjectName);
	fileStruct.read(multiplicities, atomNames);

	
	std::vector<std::vector<int> > energyIndex;
	std::vector<std::vector<std::vector<std::vector<int> > > > combinedIndex;

	std::vector<int> combinedIndexJAtom;
	std::vector<int> combinedIndexAtom;
	std::vector<int> combinedIndexL;
	std::vector<int> combinedIndexM;

	std::cout << "# Read *.outputproj" << std::endl;
	FileOutputproj fileOutputproj(w2kProjectName);
	fileOutputproj.read(combinedIndex, energyIndex, combinedIndexJAtom, combinedIndexAtom, combinedIndexL, combinedIndexM);
	
	Projector proj;
	proj.initialize(combinedIndex, energyIndex, combinedIndexJAtom, combinedIndexAtom, combinedIndexL, combinedIndexM);

	std::cout << "# Read *.proj" << std::endl;
	FileGeneral projFile(w2kProjectName,"proj");
	projFile.read<Projector>(proj);

	std::vector<std::vector<Eigen::MatrixXcd> > Symm;
	std::vector<std::vector<int> > alpha;
	
	std::cout << "# Read *.symm" << std::endl;
	FileSymm fileSymm(w2kProjectName);
	fileSymm.read(Symm, alpha, atomNames);

	std::vector<std::string> kpath;
    std::vector<int> kpathIndex;
	std::vector<std::vector<double> > energy;
	std::vector<double> weight;
	std::cout << "# Read *.energy file" << std::endl;
	FileEnergy fileEnergy(w2kProjectName);
	fileEnergy.readBands(energy, weight, kpath, kpathIndex);

	double emin, emax, de, eta;
	std::cout << "# Read *.ingf" << std::endl;
	FileIngf fileIngf(w2kProjectName);
	fileIngf.read(emin, emax, de, eta);

	double EF;
	std::cout << "# Read Fermi Energy from *.scf file" << std::endl;
	FermiEnergy fermi(w2kProjectName);
	fermi.read(EF);


	std::vector<Eigen::MatrixXcd> R;
	std::cout << "# Read *.rot file" << std::endl;
	FileRot fileRot(w2kProjectName);
	fileRot.read(R);

	std::vector<Eigen::MatrixXcd> S;
	std::cout << "# Read *.smat file" << std::endl;
	FileSmat fileSmat(w2kProjectName);
	fileSmat.read(S);


	GreensFunction gf;
	std::cout << "# Calculate Greens Function" << std::endl;
	GreensFunctionCalculator gfCalc(emin, emax, de, eta);
	gfCalc.calculate(gf, energy, EF);

	GreensFunction gfProj;
	std::cout << "# Project Greens Function" << std::endl;
	GreensFunctionProjector gfProjector;
	gfProjector.calculate(gf, gfProj, proj);

    SpectralFunction specFunc(kpath, kpathIndex);
    std::cout << "# Calculate Spectral Function" << std::endl;
    SpectralFunctionCalculator specFuncCalc;
    specFuncCalc.calculate(specFunc, gfProj);


	std::cout << "# Write *.specproj" << std::endl;
	FileGeneral overFile(w2kProjectName,"specproj");
	overFile.write<SpectralFunction>(specFunc);   

	return 0;
}




#include "TatortFMPredictor.h"
#include "TatortFMParser.h"
#include "Logger.h"
#include "Tools.h"

#include <sstream>
#include <chrono>
#include <fstream>



using namespace std;





TatortFMPredictor::TatortFMPredictor() {
	_algorithm = "mcmc";

	_iterations = 200;
	_stdev = 0.0;
	_learnRate = 0.0;
	_regulation = "0";
	_logfile = "logfile.log";
	

	parametersToUse(true, true, 8);
}






TatortFMPredictor::~TatortFMPredictor() {

}


void TatortFMPredictor::setLogfile(string logfile) {
	_logfile = logfile;
}


void TatortFMPredictor::parametersToUse(bool w0, bool w, unsigned int numOfLatentFactors) {
	_useW0 = w0;
	_useW = w;
	_numOfLatentFactors = numOfLatentFactors;


	ostringstream os("");

	if(_useW0)
		os << "1";
	os << ",";
	if(_useW)
		os << "1";
	os << ",";
	os << _numOfLatentFactors;

	_dim = os.str();
}


double TatortFMPredictor::trainAndTest(string trainFilename, string testFilename, string predictionFilename, int dataRepresentation) {

	// execute libFM command
	ostringstream libfmCmd("");

	#ifdef _WIN32
		libfmCmd << "libfm.exe ";
	#else
		libfmCmd << "./libFM ";
	#endif
	libfmCmd << "-task r ";
	libfmCmd << "-dim " << _dim << " ";
	libfmCmd << "-iter " << to_string(_iterations) << " ";
	libfmCmd << "-method " << _algorithm << " ";
	libfmCmd << "-init_stdev " << to_string(_stdev) << " ";
	libfmCmd << "-regular " << _regulation << " ";
	libfmCmd << "-learn_rate " << to_string(_learnRate) << " ";
	libfmCmd << "-train " << trainFilename << " ";
	libfmCmd << "-test " << testFilename << " ";
	libfmCmd << "-out " << predictionFilename << " ";
	libfmCmd << "-rlog " << _logfile << " ";

	Logger::getInstance()->log("executing cmd: '"+ libfmCmd.str() +"'", LOG_DEBUG);

	// just for measuring elapsed time
	std::chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
	
	int ret = system(libfmCmd.str().c_str());
	// the rest of measuring elapsed time	
	std::chrono::high_resolution_clock::time_point stop = chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(stop - start);
	double elapsedSeconds = time_span.count();

	// write time to file
	std::fstream file;
	file.open("libfm_elapsed_time", fstream::app);
	file << dataRepresentation << " ::: " << libfmCmd.str() << "::: " << elapsedSeconds << endl;
	file.close();


	Logger::getInstance()->log("libfm returned '"+ to_string(ret) +"'", LOG_DEBUG);


	TatortFMParser fmParser;

	vector<double> predicted = fmParser.readPredictionFromFile(predictionFilename);
	vector<double> targets = fmParser.readPredictionFromFile(testFilename);

	double mae = Tools::getInstance()->computeMAE(predicted, targets);

	return mae;
}







void TatortFMPredictor::setAlgorithm(string algo) {
	_algorithm = algo;
}

void TatortFMPredictor::setIterations(unsigned int iters) {
	_iterations = iters;
}

void TatortFMPredictor::setRegulation(string reg) {
	_regulation = reg;
}

void TatortFMPredictor::setLearningRate(double lr) {
	_learnRate = lr;
}

void TatortFMPredictor::setStdev(double stdev) {
	_stdev = stdev;
}


string TatortFMPredictor::getAlgorithm() {
	return _algorithm;
}

int TatortFMPredictor::getIterations() {
	return _iterations;
}

string TatortFMPredictor::getRegulation() {
	return _regulation;
}

double TatortFMPredictor::getLearningRate() {
	return _learnRate;
}

double TatortFMPredictor::getStdev() {
	return _stdev;
}


string TatortFMPredictor::tuningParamsToString() {
	ostringstream os("");

	os << "algorithm|stdev|iterations|regulation|learnrate" << endl;
	os << _algorithm << "|";
	os << _stdev << "|";
	os << _iterations << "|";
	os << _regulation << "|";
	os << _learnRate;

	return os.str();
}

void TatortFMPredictor::copyFrom(TatortFMPredictor *fmp) {
	this->setAlgorithm(fmp->getAlgorithm());
	this->setStdev(fmp->getStdev());
	this->setIterations(fmp->getIterations());
	this->setRegulation(fmp->getRegulation());
	this->setLearningRate(fmp->getLearningRate());
	this->_useW0 = fmp->_useW0;
	this->_useW = fmp->_useW;
	this->_numOfLatentFactors = fmp->_numOfLatentFactors;
	this->parametersToUse(this->_useW0, this->_useW, this->_numOfLatentFactors);
}



string TatortFMPredictor::getDimension() {
	return _dim;
}

int TatortFMPredictor::getNumOfLatentFactors() {
	return _numOfLatentFactors;
}

void TatortFMPredictor::setNumOfLatentFactors(int num) {
	_numOfLatentFactors = num;
	parametersToUse(_useW0, _useW, num);
}

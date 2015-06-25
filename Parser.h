#ifndef __PARSER_H__
#define __PARSER_H__



#include <string>
#include <vector>
#include <map>


using namespace std;

class Parser {


protected:
	vector<string> _lines;
	int _numOfColumns;
	int _numOfLines;
	int _numOfDatasets;
	vector<string> *_columns;
	vector<string> _headline;

	
	vector<string>* getAllColumns();
	
public:
	Parser();
	virtual ~Parser();
	virtual void clear();

	virtual int parseFile(string filename, string delimiter, bool isHeaderPresent);
	virtual void readDBFile(string filename);

	virtual void divideLinesTrainAndTest(string sourceFilename, bool isHeaderPresent, double trainPercentage, string trainFilename, string testFilename, string predictionTargetFilename);
	virtual int getNumberOfDatasets();
	virtual int getNumberOfLines();
	virtual int getNumberOfColumns();
	vector<string> getColumn(unsigned int index);

};










#endif




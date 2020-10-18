#include "CSVOutPuter.h"
#include<time.h>
#include<sstream>
using namespace std;

string CreateTextName();
CSVOutPuter::CSVOutPuter(int count) {
	_setDataDistance = count;
	_nowCount = 0;
}

void CSVOutPuter::TryAddData(float data) {
	_nowCount++;
	if (_nowCount == _setDataDistance) {
		_nowCount = 0;
		_outData.push_back(data);
	}
}

void CSVOutPuter::OutPutCSV() {
	string textName = CreateTextName();
	ofstream outputFile(textName);

	for (int i = 0; i < _outData.size(); i++) {
		outputFile << _outData[i];
		if (i < _outData.size())outputFile << ",";
	}
	outputFile.close();
}

string CreateTextName() {
	time_t t = time(NULL);
	struct tm* tm = localtime(&t);
	stringstream ss;
	ss  << "_outData_"
		<< tm->tm_year + 1900<<"_"
		<< tm->tm_mon + 1 << "_"
		<< tm->tm_mday << "_"
		<< tm->tm_hour << "_"
		<< tm->tm_min << "_"
		<< tm->tm_sec
		<< ".csv";
	return ss.str();
}
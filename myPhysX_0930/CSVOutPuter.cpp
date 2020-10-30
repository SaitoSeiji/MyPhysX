#pragma once
#include "CSVOutPuter.h"
using namespace std;

string CreateTextName();
CSVOutPuter::CSVOutPuter(string title,int count) {
	_setDataDistance = count;
	_nowCount = 0;
	_title = "_"+title;
}

void CSVOutPuter::TryAddData(float data) {
	_nowCount++;
	if (_nowCount == _setDataDistance) {
		_nowCount = 0;
		_outData.push_back(data);
	}
}

string CSVOutPuter::GetOutPutText() {
	stringstream ss;
	for (int i = 0; i < _outData.size(); i++) {
		ss << _outData[i];
		if (i < _outData.size())ss << ",";
	}
	return ss.str();
}
void CSVOutPuter::OutPutCSV() {
	string textName = CreateTextName();
	ofstream outputFile(_title+textName);
	outputFile << GetOutPutText();
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
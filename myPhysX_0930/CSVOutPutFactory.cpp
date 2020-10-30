#pragma once
#include "CSVOutPutFactory.h"

string CreateTextName2();

void CSVOutPutFactory::AddDataSet(string key, int distance) {
	_keyDatas.push_back(key);
	_csvDatas.push_back(new CSVOutPuter("factory", distance));
}
void CSVOutPutFactory::Update(string key, float data) {
	for (int i = 0; i < _keyDatas.size(); i++) {
		if (key._Equal(_keyDatas[i])) {
			(*_csvDatas[i]).TryAddData(data);
		}
	}
}
void CSVOutPutFactory::OutPutCSV() {
	string textName = CreateTextName2();
	ofstream outputFile(textName);
	for (int i = 0; i < _keyDatas.size(); i++) {
		outputFile << (*_csvDatas[i]).GetOutPutText() << "\n";
	}
	outputFile.close();
}

string CreateTextName2() {
	time_t t = time(NULL);
	struct tm* tm = localtime(&t);
	stringstream ss;
	ss << "_outData_"
		<< tm->tm_year + 1900 << "_"
		<< tm->tm_mon + 1 << "_"
		<< tm->tm_mday << "_"
		<< tm->tm_hour << "_"
		<< tm->tm_min << "_"
		<< tm->tm_sec
		<< ".csv";
	return ss.str();
}

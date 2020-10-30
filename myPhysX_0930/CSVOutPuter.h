#pragma once
#include<vector>
#include<iostream>
#include<fstream>
#include<time.h>
#include<sstream>
using namespace std;
class CSVOutPuter
{
	vector<float> _outData;
	int _setDataDistance;
	int _nowCount;
	string _title;
public:
	CSVOutPuter(string title,int distance);
	void TryAddData(float data);
	string GetOutPutText();
	void OutPutCSV();
};


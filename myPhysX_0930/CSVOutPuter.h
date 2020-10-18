#pragma once
#include<vector>
#include<iostream>
#include<fstream>
using namespace std;
class CSVOutPuter
{
	vector<float> _outData;
	int _setDataDistance;
	int _nowCount;
public:
	CSVOutPuter(int distance);
	void TryAddData(float data);
	void OutPutCSV();
};


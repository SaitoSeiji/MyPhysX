#pragma once
#include <iostream>
#include<chrono>
using namespace std;
using namespace chrono;
class DtCulcurator
{
	system_clock::time_point beforePoint=chrono::system_clock::now();
	bool isInited = false;
public:
	void Start();
	bool IsStarted();
	float CalcDt_sec();
	void Reset();
};


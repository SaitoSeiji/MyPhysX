#pragma once
#include <iostream>
#include <time.h>
class DtCulcurator
{
	long long beforest = -1;
	long long beforent = -1;
	bool isInited = false;
public:
	void Start();
	bool IsStarted();
	float CalcDt_sec();
	void Reset();
};


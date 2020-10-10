#include "DtCulcurator.h"
#include<chrono>
using namespace std;

void DtCulcurator::Start() {
	Reset();
	isInited = true;
}
bool DtCulcurator::IsStarted() {
	return isInited;
}

float DtCulcurator::CalcDt_sec() {
	if (!IsStarted()) {
		cerr << "‰Šú‰»‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ";
		return -1;
	}
	timespec _time;
	if (timespec_get(&_time, TIME_UTC) == 0) {
		cerr << "Žž‚ÌŽæ“¾‚ÉŽ¸”s‚µ‚½";
	}
	long long nowst = _time.tv_sec;
	long long nownt = _time.tv_nsec;

	long long dnt = nownt - beforent;
	beforent = nownt;
	long dst = 0;
	if (nowst > beforest) {
		dst = nowst - beforest;
		dnt += dst * 1000000000.0f;
		beforest = nowst;
	}
	return dnt / 1000000000.0f;
}

void DtCulcurator::Reset() {

	timespec _time;
	if (timespec_get(&_time, TIME_UTC) == 0) {
		cerr << "Žž‚ÌŽæ“¾‚ÉŽ¸”s‚µ‚½";
	}
	long long nowst = _time.tv_sec;
	long long nownt = _time.tv_nsec;
	beforest = nowst;
	beforent = nownt;
}
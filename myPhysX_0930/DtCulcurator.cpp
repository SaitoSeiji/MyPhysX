#include "DtCulcurator.h"

using namespace std;

void DtCulcurator::Init() {

	 timespec _time;
	 if (timespec_get(&_time, TIME_UTC) == 0) {
		 cerr << "Žž‚ÌŽæ“¾‚ÉŽ¸”s‚µ‚½";
	 }
	 long long nowst = _time.tv_sec;
	 long long nownt = _time.tv_nsec;
	 beforest = nowst;
	 beforent = nownt;

	 isInited = true;
}
bool DtCulcurator::IsInited() {
	return isInited;
}

float DtCulcurator::CalcDt() {
	if (!IsInited()) {
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
		//cout << dst << "•bŒo‰ß\n";
		dnt += dst * 1000000000.0f;
		beforest = nowst;
	}
	/*if (dnt < 0) {
		cout << "framemiss "<< dst<<" "<<dnt<<"\n";
		return 0;
	}*/
	return dnt / 100000000.0f;
}
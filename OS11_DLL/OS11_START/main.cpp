#pragma comment(lib, "../x64/debug/OS11_HTAPI.lib")

#include <conio.h>
#include "../OS11_HTAPI/pch.h"
#include "../OS11_HTAPI/HT.h"

using namespace std;

wchar_t* getWC(const char* c);

int main(int argc, char* argv[])
{
	wchar_t* fileName = getWC(argv[1]);

	ht::HtHandle* ht = ht::open(fileName);
	if (ht)
	{
		cout << "==============  HT-Storage Start  ==============" << endl;
		wcout << "filename:\t\t" << ht->fileName << endl;
		cout << "secSnapshotInterval:\t" << ht->secSnapshotInterval << endl;
		cout << "capacity:\t\t" << ht->capacity << endl;
		cout << "maxKeyLength:\t\t" << ht->maxKeyLength << endl;
		cout << "maxPayloadLength:\t" << ht->maxPayloadLength << endl;
		cout << "================================================" << endl;

		while (!kbhit())
			SleepEx(0, TRUE);

		ht::close(ht);
	}
	else
		cout << "-- open: error" << endl;
}

wchar_t* getWC(const char* c)
{
	wchar_t* wc = new wchar_t[strlen(c) + 1];
	mbstowcs(wc, c, strlen(c) + 1);

	return wc;
}

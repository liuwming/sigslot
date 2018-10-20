#include <stdio.h>
#include <iostream>
#include <map>
#include <string>
#include <Windows.h>

#include "sigslot.h"    // 信号与槽 的 库 ，只包含一个头文件。

using namespace std;

std::map<int, string> mapTmp;

class CSender {
public:
    sigslot::signal2<std::map<int, string>, int> m_pfnsigDanger;

    void Panic() 
    {
        static int nVal = 0;
        char szVal[20] = {0};

        sprintf_s(szVal, "help--%d", nVal);

		mapTmp[0] = "one";
		mapTmp[1] = "two";
        // m_pfnsigDanger(szVal, nVal++);
        m_pfnsigDanger.emit(mapTmp, nVal++);
    }
};

class CReceiver: public sigslot::has_slots<> 
{

public:
    void OnDanger(std::map<int, string> mapDang, int nVal) 
    {
        cout << mapDang.at(0) << " ==> " << nVal + 10 << endl;
        cout << mapDang.at(1) << " ==> " << nVal + 10 << endl;
    }
};

int main(int argc, char** argv)
{
    int count = 5;
    CSender sender;
    CReceiver recever;

    sender.m_pfnsigDanger.connect(&recever, &CReceiver::OnDanger);

    cout << "connect succ!" << endl;

    while(count-- > 0) {
        cout << "in while..." << endl;

        sender.Panic();
		
		Sleep(200);

        cout << "end of sleep" << endl;
    }

    sender.m_pfnsigDanger.disconnect(&recever);

	getchar();

    return 0;
}

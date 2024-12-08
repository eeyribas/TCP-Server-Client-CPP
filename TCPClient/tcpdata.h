#ifndef TCPDATA_H
#define TCPDATA_H

#include <iostream>

class TCPData
{
public:
    TCPData();

    void SetValue1(int value);
    int GetValue1();
    void SetValue2(int value);
    int GetValue2();

private:
    int value_1;
    int value_2;
};

#endif // TCPDATA_H

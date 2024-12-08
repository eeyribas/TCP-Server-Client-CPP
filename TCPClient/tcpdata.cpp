#include "tcpdata.h"

TCPData::TCPData()
{
    value_1 = 0;
    value_2 = 0;
}

void TCPData::SetValue1(int value)
{
    value_1 = value;
}

int TCPData::GetValue1()
{
    return value_1;
}

void TCPData::SetValue2(int value)
{
    value_2 = value;
}

int TCPData::GetValue2()
{
    return value_2;
}

#ifndef CTIMECVT_H
#define CTIMECVT_H
/********************************************************
 * public c/c++ standard headers
 *******************************************************/
#include <stdio.h>
#include <iostream>
#include <string.h>

/********************************************************
 * boost usart headers
 *******************************************************/
#include <boost/asio.hpp>
#include <boost/bind.hpp>

/********************************************************
 * lib headers
 *******************************************************/
#include <log.h>
#include <ctime>

class CTimeCvt
{
public:
    CTimeCvt();
public:
    bool string2Datetime(string str,time_t & time);
    bool datetime2String(time_t time,string & str);
};

#endif // CTIMECVT_H

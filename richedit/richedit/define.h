/**
*@file define.h
*@brief 预定义宏部分
*
*各种全局预定义宏放在此文件
*
*@author 林新弟(Sindy)<goki123@126.com>
*@version 1.0.0
*@data 04/23/2012
*/
#define WINVER		0x0500
#define _WIN32_WINNT	0x0501

#define MY_CHAR wchar_t

#define CARE_HEIGHT 12

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a > b ? b : a)

#define NAMESPACE_BEGIN(name) namespace name{
#define NAMESPACE_END(name)};
#define NAMESPACE_USE(name) using namespace name;

//#define NO_TRACE
#define USE_TEST
//#define USE_LOG

#ifdef NO_TRACE
#define ATLTRACE
#define ATLTRACE2 ATLTRACE
#endif
NAMESPACE_BEGIN(misc)
NAMESPACE_END(misc)
NAMESPACE_USE(misc)
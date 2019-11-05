#ifndef DEBUG_H
#define DEBUG_H
#include <fstream>
#include <iostream>
using namespace std;
//**************************************

//全部注释,则关闭debug

// #define DEBUGLEVEL 0

// #define FLUSH_FORCE 0

//是否打开assert检查
// #define NDEBUG

// #define LOG_FILE "log.txt"

//以上部分是选项
//***************************************
//以下部分是实现
#include <cassert>

//是否对log重定向
#ifdef LOG_FILE
ofstream file(LOG_FILE);
streambuf *log = clog.rdbuf(file.rdbuf());
#undef LOG_FILE
#endif

void DEBUG(int level, string log_info, string file = __FILE__, int line = __LINE__);

#endif
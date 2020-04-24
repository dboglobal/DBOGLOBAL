
void Error(const char* error_type, const char* error_string, const char** attributes = 0);

#define test(expr) \
{ \
    if(!(expr))\
        ReportTestFailure(__FILE__,__LINE__,#expr);\
}
void ReportTestFailure(const char* file, long line, const char* expression);

#define assertR(expr) \
{ \
    static bool on = true;\
    if(on && !(expr))\
        on = ReportAssertionFailure(__FILE__,__LINE__,#expr);\
}                                                
#ifdef _DEBUG
#define assertD(expr) assertR(expr)
#define invalid() \
{ \
    static bool on=true;\
    if(on)\
        on = ReportAssertionFailure(__FILE__,__LINE__,"<invalid>");\
}                                                
#define ASSERTIONS_ON
#else
#define assertD(expr) {}
#define invalid() {}
#endif
bool ReportAssertionFailure(const char* file, long line, const char* expression);

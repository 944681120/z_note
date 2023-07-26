#include "CfgFactory.h"
#include "JsonPs.h"
#include "IniPs.h"

using namespace LLZ;

#define CFGCREAT(TYPE, STR, LEN, OBJNAME)   \
    do                                      \
    {                                       \
        if (type == TYPE && LEN == -1)      \
            CFGOBJ(OBJNAME, STR);           \
        if (type == TYPE)                   \
            CFGOBJ(OBJNAME, STR, LEN);      \
    } while (0);

#define CFGOBJADD(TYPE, OBJNAME) CFGCREAT(TYPE, filepathOrStr, strLen, OBJNAME)
#define CFGOBJLIST()                        \
    CFGOBJADD("yyjson", JsonPs)             \
    CFGOBJADD("simpleini", IniPs)           \

AbsCfgParser* CfgFactory::newObj(const std::string &type, const char *filepathOrStr, int strLen)
{
#define CFGOBJ(OBJNAME, ...) return new OBJNAME(__VA_ARGS__)
    CFGOBJLIST();
#undef CFGOBJ

    return nullptr;
}

bool CfgFactory::delObj(AbsCfgParser *obj)
{
    if ( obj == nullptr )
    {
        return true;
    }
    
    delete obj;
    obj = nullptr;
    return true;
}

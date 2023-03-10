#include "teacher.h"

teacher::teacher()
{

}

teacher::~teacher()
{

}

int teacher::addClass(std::string className, void* pClass)
{
    this->class_map[className] = pClass;
    return 0;
}

int teacher::getClassNum()
{
    return this->class_map.size();
}

std::string teacher::stepClassStr()
{
    static std::map<std::string, void*>::iterator __it = class_map.begin();
    std::map<std::string, void*>::iterator ret;

    if ( __it != class_map.end() )
    {
        ret = __it;
        __it++; 
    }
    
    return ret->first;
}

void* teacher::operator[](std::string className)
{
    std::map<std::string, void*>::iterator it;
    it = this->class_map.find(className);
    if (it == class_map.end())
    {
        std::cout << "无法访问类: " << className << std::endl;
        return NULL;
    }
    else
        return class_map[className];
}
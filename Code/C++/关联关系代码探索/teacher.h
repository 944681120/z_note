#ifndef TEACHER_H
#define TEACHER_H

#pragma once
#include <map>
#include <string>
#include <iostream>

class teacher
{
public:
    teacher();
    ~teacher();

    int addClass(std::string className, void* pClass);
    int getClassNum();
    std::string stepClassStr();  // 没调用一次
    void* operator[](std::string className);

private:
    std::map<std::string, void*> class_map;
};

#endif
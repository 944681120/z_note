#ifndef STUDENT_H
#define STUDENT_H

#pragma once
#include <iostream>

class student
{
public:
    student(int i);
    ~student();
    void printtt()
    {
        std::cout << "我是学生" << m_i << std::endl;
    }

private:
    int m_i;
};

#endif
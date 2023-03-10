#include <stdio.h>
#include "teacher.h"
#include "student.h"

#define T2S(aaa) (*(aaa*)((*tea)[#aaa]))

int main(int argc, char const *argv[])
{
    student* stu1 = new student(1);
    student* stu2 = new student(2);
    student* stu3 = new student(3);
    teacher* tea = new teacher();
    tea->addClass("student1", stu1);
    tea->addClass("student2", stu2);
    tea->addClass("student3", stu3);

    // ((student*)((*tea)["student1"]))->printtt();
    // ((student*)((*tea)["student2"]))->printtt();

    for (int i = tea->getClassNum() - 1; i >= 0; i--)
    {
        if ( tea->stepClassStr() == "student1" )
        {
            T2S(student).printtt();
        }
        else if ( tea->stepClassStr() == "student2" )
        {
            T2S(student).printtt();
        }
        else if ( tea->stepClassStr() == "student3" )
        {
            T2S(student).printtt();
        }
    }
    
    T2S(student).printtt();

    return 0;
}

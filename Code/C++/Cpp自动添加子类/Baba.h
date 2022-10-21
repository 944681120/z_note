#ifndef BABA_H
#define BABA_H
#pragma once

#include <cstdio>
#include <vector>
#include <string>
using namespace std;

class Baba
{
public:
    Baba();
    ~Baba();

    virtual int run(void* arg=nullptr);

private:
    static int addChild(Baba* p);



public:
    string name;

private:
    vector<Baba*> childvec;
};



#endif
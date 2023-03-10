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

protected:
    static int addChild(Baba* p);

protected:
    string name;
    Baba* pOwn;
    vector<Baba*> childvec;
};



#endif
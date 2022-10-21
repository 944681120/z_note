#include "Baba.h"

Baba::Baba()
{
    this->name = "你们Baba";
    // printf("Baba的构造函数\n");
    this->addChild(this);
}

Baba::~Baba()
{

}

int Baba::addChild(Baba* p)
{
    static Baba* pBa = nullptr;

    if ( p == nullptr )
    {
        return -1;
    }

    if ( pBa == nullptr )
    {
        pBa = p;
        return 0;
    }
    
    pBa->childvec.push_back(p);
    
}

int Baba::run(void* arg)
{
    printf("我是%s啊, 我的地址:%x\n", this->name.c_str(), this);
    for ( vector<Baba*>::iterator it = this->childvec.begin(); it != this->childvec.end(); it++)
    {
        (*it)->run();
    }

    return 0;
}

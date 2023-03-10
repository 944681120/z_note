#include "Baba.h"
#include "Son.h"
#include "GSon.h"
#include <vector>
#include <cstdio>
#include <list>
using namespace std;

int main(int argc, char const *argv[])
{
    Baba* pBaba = new Baba();
    GSon* pGSon1 = new GSon("孙子1号");
    Son* pSon1 = new Son("儿子1号");
    Son* pSon2 = new Son("儿子2号");
    GSon* pGSon2 = new GSon("孙子2号");

    printf("******************************************************\n");
    pBaba->run();
    printf("******************************************************\n");
    
    // pSon1->run();
    // printf("******************************************************\n");
    // pSon2->run();
    // printf("******************************************************\n");
    
    return 0;
}

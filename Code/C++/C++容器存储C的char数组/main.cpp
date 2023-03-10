#include <iostream>
#include <vector>
#include <list>

using namespace std;

int main(int argc, char const *argv[])
{
    char test1[128] = "i am a test1";
    char test2[128] = "i am a test2";

    vector<char> arr_sub;
    list<vector<char>> arr;
    arr_sub.insert(arr_sub.begin(), test1, (char*)&test1+128);
    arr.push_back(arr_sub);

    arr_sub.insert(arr_sub.begin(), test2, (char*)&test2+128);
    arr.push_back(arr_sub);

    for (int i = 0; i < 10; i++)
    {
        if ( arr.size() > 0 )
        {
            std::cout << (char*)&arr.begin()->at(0) << std::endl;
            // std::cout << (char*)&(*arr.begin())[0] << std::endl;
            std::cout << "size:" << arr.size() << std::endl;
            std::cout << "------------------------------" << i << std::endl;
            arr.pop_front();
        }
        else
        {
            break;
        }
    }

    return 0;
}

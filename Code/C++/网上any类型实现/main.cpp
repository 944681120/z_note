#include <iostream>
#include <vector>

using namespace std;

struct Any
{
    Any() : ptr{0} {}
    template <typename T>
    Any(const T &t) : ptr{new Data<T>(t)} {};
    Any(const Any &rhs)
    {
        ptr = rhs.ptr->clone();
    }
    Any &operator=(const Any &rhs)
    {
        if (ptr)
            delete ptr;
        ptr = rhs.ptr->clone();
        return *this;
    }
    Any(Any &&rhs) noexcept
    {
        ptr = rhs.ptr;
        rhs.ptr = 0;
    }
    Any &operator=(Any &&rhs) noexcept
    {
        if (ptr)
            delete ptr;
        ptr = rhs.ptr;
        rhs.ptr = 0;
        return *this;
    }
    struct Base
    {
        virtual Base *clone() = 0;
        virtual ostream &print(ostream &out) = 0;
    };
    template <typename T>
    struct Data : Base
    {
        T data;
        Data(const T &t) : data(t) {}
        virtual Base *clone() { return new Data<T>(data); }
        virtual ostream &print(ostream &out)
        {
            out << data;
            return out;
        }
    };
    Base *ptr;
    template <typename T>
    T &get_data()
    {
        return ((Data<T> *)ptr)->data;
    }
    ~Any()
    {
        if (ptr)
            delete ptr;
    }
};
ostream &operator<<(ostream &out, const Any &oth)
{
    oth.ptr->print(out);
    return out;
}
template <typename T>
ostream &operator<<(ostream &out, const vector<T> &v)
{
    out << "[";
    for (auto it = v.begin(); it != v.end(); it++)
    {
        if (it != v.begin())
            out << ", ";
        out << *it;
    }
    out << "]";
    return out;
}
// int main()
// {
//     vector<Any> vec = {114, 514, 1919, 8.10, string("asoul"), string("4soul")};
//     cout << vec << endl;
//     int i = vec[0].get_data<int>();
//     cout << i << endl;
//     vector<Any> oth = {string("ygg"), 233};
//     vec.push_back(oth);
//     cout << vec << endl;
//     vec[1] = vector<Any>{oth, oth};
//     cout << vec << endl;
// }

template <class T1, class T2>
auto add(T1 t1, T2 t2)
{
    return t1 + t2;
}

int main()
{
    auto a = add(5, 1.66);
    cout << "a=" << a << " type is:" << typeid(a).name() << endl;
    return 0;
}

/*
## 外部定义了一下类型：
### json类型形式
#### 常见类型 name:type
    "a" : "int",

#### 常见类型带变量名
    "int": { "ia": 12, "ib": 41 },
    "double": { "da": 72.5, "db": 21.8 },
    "struct" : {
        "int": 
        {
            "ia": 12,
            "ib": 41
        },
        "double": 
        {
            "da": 72.5,
            "db": 21.8
        }
    }

 */
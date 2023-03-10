1、继承关系
    a、内存构造关系：
        先调用父类构造函数构造自身内存里的父类内存，因为是自身内存调用父类构造函数，也就是说此时this指针是子类的，
        那么就可以利用这点让父类或者子类*自动*建立关联

2、C++ static的使用
    a、静态成员函数：
        静态成员函数只有当前类或者其子类访问，静态成员函数内存存在当前类和子类之外的静态储存区，即不占类对象的内存空间而是在类初始化时构建
    b、私有静态成员的用法：
        静态成员虽然独一份，但是仍然受作用域限定符影响
    c、私有静态成员与父类构造函数
        

3、私有静态成员与父类构造函数
    a、间接调用父类私有静态成岩
        经测试gcc编译器，虽然子类不能直接访问父类私有静态成员，但是如果父类构造函数调用私有静态成员，那么子类构造时调用父类构造函数就可以调用父类私有静态成员
        这很有趣：）
        eg：
            Baba::Baba()
            {
                this->name = "你们Baba";    //父类内存的name，如果
                this->addChild(this);       //addChild<----父类私有成员 this调用者指针
            }

    
#include "../JsonPs.h"
#include "../IniPs.h"
#include "../yyjson/yyjson.h"
#include "../CfgFactory.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

// 官方示例 https://ibireme.github.io/cfg/doc/doxygen/html/index.html#autotoc_md7

int main(int argc, char const *argv[])
{
    string str;
    int intVal = 0;
    list<std::string> obj;
    /* 测试new对象 */
    LLZ::AbsCfgParser* cfg = LLZ::CfgFactory::newObj("yyjson", "./mqtt_setting.json");

    printf("json123: %s\n", cfg->cfg_print_str().c_str());
    cfg->cfg_set_int("/param/mqtt/0/port", 1475);   // 写入数据
    cfg->cfg_get_int("/param/mqtt/0/port", intVal);   // 读取数据
    printf("val: %d\n", intVal);
    cfg->cfg_set_str("/param/mqtt/0/user", "testUser");
    cfg->cfg_get_str("/param/mqtt/0/user", str);
    printf("str: %s\n", str.c_str());
    printf("\n");
    delete cfg; // 别忘了delete
    
    /* 测试局部文件变量 */
    // JsonPs json("test.json"); // 打开一个空json文件，需要存在
    LLZ::JsonPs json = LLZ::JsonPs("test.json"); // 打开一个空json文件，需要存在
    json.cfg_arr_iter("/", &obj);
    for (auto &&cobj : obj)
    {
        printf("获取/下的全部key项: %s\n", cobj.c_str());
    }
    obj.clear(); // 获取失败部改变obj，所以项目使用时需要判断返回值
    json.cfg_arr_iter("/param", &obj);
    for (auto &&cobj : obj)
    {
        printf("获取/param下的全部key项: %s\n", cobj.c_str());
    }
    json.cfg_arr_iter("/param/iam1/iam2/arr", &obj);
    for (auto &&cobj : obj)
    {
        printf("获取/param/iam1/iam2/arr下的全部value项: %s\n", cobj.c_str());
    }

    str = "dsadase";
    int val = json.cfg_get_str("/param/name", str);
    printf("val : %d, str2: %s\n", val, str.c_str());
    json.cfg_set_double("/param/val", 478.5);
    json.cfg_set_str("/param/str/dda", "param/str/add");
    json.cfg_set_str("/param/hits/00", "40");     //修改已有的数组里的值
    json.cfg_set_str("/param/hits/01", "30");
    json.cfg_set_str("/param/hits/02", "200");
    json.cfg_set_str("/param/hits/03", "10");
    json.cfg_add_arr("/param/iam1/iam2/arr");                  // 没有的数组先添加对应数组
    json.cfg_add_int("/param/iam1/iam2/arr/0/val", 11);        // 再添加相应的key-val
    json.cfg_add_str("/param/iam1/iam2/arr/1/val", "22");      // 再添加相应的key-val
    json.cfg_add_double("/param/iam1/iam2/arr/2/val", 33.3);   // 再添加相应的key-val
    json.cfg_add_bool("/param/iam1/iam2/arr/3/val", true);     // 再添加相应的key-val
    json.cfg_set_int("/param/iam1/iam2/arr/0/val", 44);        // 再修改相应的key-val
    json.cfg_set_str("/param/iam1/iam2/arr/1/val", "33");      // 再修改相应的key-val
    json.cfg_set_double("/param/iam1/iam2/arr/2/val", 22.2);   // 再修改相应的key-val
    json.cfg_set_bool("/param/iam1/iam2/arr/3/val", false);    // 再修改相应的key-val
    /* 获取一个json str对象 */
    string iters;
    int cnt = json.cfg_obj_iter("/param/iam1", iters);
    printf("cfg_obj_iter[%d] :%s\n\n", cnt, iters.c_str());

    /* 测试json字符串 */
    LLZ::JsonPs jsonstr(obj.front().c_str(), obj.front().size());
    jsonstr.cfg_get_int("/star", intVal);
    printf("val3: %d\n", intVal);
    jsonstr.cfg_set_int("/star", 8);
    jsonstr.cfg_get_int("/star", intVal);
    printf("val3: %d\n", intVal);
    jsonstr.cfg_set_str("/name", "llz");
    jsonstr.cfg_get_str("/name", str);
    printf("str3: %s\n", str.c_str());
    printf("\n");

    /* 测试json字符串 */
    const char *jsonn = "";
    LLZ::JsonPs jsonnull(jsonn, strlen(jsonn));
    jsonnull.cfg_set_str("/other/addr/0/AD/province", "广州北");
    jsonnull.cfg_set_str("/other/addr/0/AD/region", "天河朝阳");
    jsonnull.cfg_set_str("/other/addr/1/AD/province", "广州北京");
    jsonnull.cfg_set_str("/other/addr/1/AD/region", "天河朝阳");
    jsonnull.cfg_add_arr("/param/iam1/iam2/arr");                  // 没有的数组先添加对应数组
    jsonnull.cfg_add_int("/param/iam1/iam2/arr/0/val", 11);        // 再添加相应的key-val
    jsonnull.cfg_add_str("/param/iam1/iam2/arr/1/val", "22");      // 再添加相应的key-val
    jsonnull.cfg_add_double("/param/iam1/iam2/arr/2/val", 33.3);   // 再添加相应的key-val
    jsonnull.cfg_add_bool("/param/iam1/iam2/arr/3/val", true);     // 再添加相应的key-val
    jsonnull.cfg_set_int("/param/iam1/iam2/arr/0/val", 44);        // 再修改相应的key-val
    jsonnull.cfg_set_str("/param/iam1/iam2/arr/1/val", "33");      // 再修改相应的key-val
    jsonnull.cfg_set_double("/param/iam1/iam2/arr/2/val", 22.2);   // 再修改相应的key-val
    jsonnull.cfg_set_bool("/param/iam1/iam2/arr/3/val", false);    // 再修改相应的key-val
    printf("json4: %s\n", jsonnull.cfg_print_str().c_str());
    printf("\n");

    /* ******************************** ini测试 ******************************** */
    const char* keyval;
    int iniInt = 100;
    // LLZ::IniPs* ini = LLZ::IniPs("NoThisExample.ini");   // 不存在的ini
    LLZ::AbsCfgParser* ini = LLZ::CfgFactory::newObj("simpleini", "./NoThisExample.ini");
    if ( ini->get_init() == false )
    {
        printf("初始化失败,其他的操作也不会成功\n");     // nullptr
        keyval = (char*)ini->cfg_get_val("section/key");
        printf("NoThisExample.ini > section/key : %s\n", keyval);
    }
    // ini = new LLZ::IniPs("example.ini");
    ini = LLZ::CfgFactory::newObj("simpleini", "example.ini");
    ini->cfg_arr_iter("/", &obj);
    for (auto &&cobj : obj)
    {
        printf("获取一个文件下的全部section项: %s\n", cobj.c_str());
    }
    ini->cfg_arr_iter("/test", &obj);
    for (auto &&cobj : obj)
    {
        printf("获取一个section下的全部key项: %s\n", cobj.c_str());
    }
    ini->cfg_arr_iter("/test/arrval", &obj);
    for (auto &&cobj : obj)
    {
        printf("获取一个key下的全部value项: %s\n", cobj.c_str());
    }
        
    keyval = (char*)ini->cfg_get_val("section/key");
    printf("example.ini > 获取值成功返回非空字符串指针 : %s\n", keyval);
    keyval = (char*)ini->cfg_get_val("test/other");
    printf("example.ini > other : %s\n", keyval);
    if ( ini->cfg_get_int("test/intval", iniInt) == 0 )
    {
        printf("example.ini > 获取整型 : %d\n", iniInt);
    }
    if ( ini->cfg_get_int("test/errInt", iniInt) == -1 )
    {
        printf("example.ini > 非整型值不会修改原来的值 : %d\n", iniInt);
    }
    if ( ini->cfg_get_str("/test/other", str) == 0 )
    {
        printf("成功获取字符串 : %s\n", str.c_str());
    }
    if ( ini->cfg_get_str("test/NoString", str) == -1 )
    {
        printf("失败获取字符串, 不会修改原字符串值 : %s\n", str.c_str());
    }
    if ( ini->cfg_get_str("test/intval", str) == 0 )
    {
        printf("整型当作字符串获取会怎么样呢 : %s\n", str.c_str());
        printf("没错,获取到整型字符串, ini没有类型概念\n");
    }

    bool iniBool = false;
    if ( ini->cfg_get_bool("test/boolval", iniBool) >= -1 )
    {
        printf("value当作bool获取会怎么样呢 : %d\n", iniBool);
        printf("个人定义value设为true返回true, flase返回flase, 其他的不改变\n");
    }

    double inidouble = -123.456;
    if ( ini->cfg_get_double("test/doubleval", inidouble) == 0 )
    {
        printf("成功获取double : %f\n", inidouble);
    }
    if ( ini->cfg_get_double("test/other", inidouble) == -1 )
    {
        printf("失败获取double, 不改变入参值 : %f\n", inidouble);
    }

    ini->cfg_set_int("test/wrintval", 258);              // 修改已有的key值
    ini->cfg_set_double("test/wrdoubleval", 258.54);     // 不存在的key值设置会添加并设置
    ini->cfg_set_bool("test/wrboolval", false);
    ini->cfg_set_str("test/wrstrval", "string");
    /* 获取全部的key值 */
    string mstrr;
    ini->cfg_obj_iter("/test", mstrr);
    printf("Section obj: \n%s\n", mstrr.c_str());
    ini->cfg_obj_iter("/test/wrboolval", mstrr);
    printf("非obj项则获取上一层级的obj: \n%s\n", mstrr.c_str());

    /* 打印全部数据 */
    printf("\ninifile :\n%s\n", ini->cfg_print_str().c_str());

    return 0;
}

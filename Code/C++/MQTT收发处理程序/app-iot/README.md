### 简介
- cmake小项目通用CMakeLists.txt
- 需要手动添加编译目录
- 本项目文件如下

```c
├── Test
│   ├── Test1.cpp
│   ├── Test1.h
│   └── main.cpp    // 单元测试main所在，在CMakeLists.txt中被剔除
├── Test2
│   ├── Test2.cpp
│   ├── Test2.h
│   └── Test3
│       ├── Test3.cpp
│       └── Test3.h
└── main.cpp        // 整个项目main所在，在CMakeLists.txt中被添加
```
    
### vscode的cmake设置
```c
    需要在launch.json的gdb配置添加
    "program": "${command:cmake.launchTargetPath}"
    详情见"launch.json"
```
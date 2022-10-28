
1、个人常用插件 --- C++
	a、环境类
		01)、 One Dark Pro， 				// VSCode的主题
		02)、 GBKtoUTF8，					// 将 GBK 转换为 UTF8
		03)、 Chinese(Simplified)			// 即中文环境。
		04)、 vscode-icons	 				// VSCode图标插件，主要是资源管理器下各个文件夹的图标。
		
		
		
	b、代码阅读帮助
		01)、 Better C++ Syntax				// C++语法高亮
		02)、 highlight						// 关键字高亮
	
	
	c、代码编写
		01)、 C/C++ Project Gene			// C++工程创建，创建Makefile文件、src目录、include目录等工程项目文件
				How to use
					Go to command pallete (usually : ctrl + shift + p)
					Search for "Create C project" or "Create c++ project" depending on your preference.
					
		02)、 C/C++ Snippets				// C/C++重用代码块""
			怎么使用
				命令面板 (ctrl + shift + p)
				搜索"Snippets"
				选择"cpp"
				编辑"cpp.json"
					例如：添加如下
						"fpln": { // 名称
							"prefix": "fpln", 					// 在代码中写入这个前缀 fpln 就可以快速插入下面 body 中的内容片段
							"body": "fmt.Println($0)", 			// $0 是表示光标的位置，这里插入这个片段后，光标移动到双括号中间的位置
							"description": "fmt.Println()" 		// 描述信息，在键入上面的前缀 fpln 后，在快速插入的提示中能看到这个描述 
						}
					效果：
						vscode 输入fpln按tab会显示如下
						fmt.Println()
				
		03)、 Include AutoComplete			// 即自动头文件包含
		04)、 C/C++ Advanced Lint			// C/C++静态检测
		05)、 TabNine						// 一款 AI 自动补全插件，强烈推荐，谁用谁知道！
		06)、 C++ Class Creator				// c++类文件创建，创建.cpp和.h
			怎么使用
				Alt+X
		
		
		
	d、代码编译运行
		01)、 C/C++							// 这个肯定是必须的
		02)、 Code Runner					// 即代码运行
		


	z、other
		01)、 compareit						// 比较插件，可以用于比较两个文件的差异。
		02)、 DeviceTree					// 设备树语法插件。
	

	
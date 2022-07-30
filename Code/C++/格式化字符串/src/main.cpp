#include <iostream>
#include <stdio.h>
#include "FormatHelper.h"

int main()
{
	std::cout << "Hello world!" << std::endl;

	FormatHelper fmtp;
	printf("测试 CheckHexstr ------------ \n");
	printf("测试'123'结果 %d \n", fmtp.CheckHexstr("123"));
	printf("测试'1234'结果 %d \n", fmtp.CheckHexstr("1234"));
	printf("测试''结果 %d \n", fmtp.CheckHexstr(""));

	printf("测试 HexstrToBytes ------------ \n");
	for (size_t i = 0; i < fmtp.HexstrToBytes("123").size(); i++)
	{
		printf("测试'123'结果 %02x \n", fmtp.HexstrToBytes("123")[i]);
	}
	for (size_t i = 0; i < fmtp.HexstrToBytes("1234").size(); i++)
	{
		printf("测试'1234'结果 %02x\n", fmtp.HexstrToBytes("1234")[i]);
	}

	printf("测试 Format ------------ \n");
	printf("测试'123'结果 %s \n", fmtp.Format("%02x", 159).c_str());

}
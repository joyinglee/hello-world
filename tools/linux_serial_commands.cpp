// 阅读shell.txt文件，运行一行，删除一行，之后一直循环，linux多命令逐行运行
// read shell.txt, run the first line, delete first line, and loop
// for run multi commands serial in linux
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include <unistd.h>
int main()
{
	while(true){
	std::fstream file("shell.txt");
	if(!file.is_open()){std::cout << "打开失败";return -1;}
	std::string contents, tem_str;
	if(getline(file, tem_str)){
	system(tem_str.c_str());}else{std::cout<< "无命令，继续";}
	while(std::getline(file,tem_str))
	contents = contents + tem_str + "\n";
	file.close();
	std::ofstream file2("test.txt",std::ios::trunc);
	file2 << contents;
	file2.close();
	std::cout << "中场休息，后面的指令为:" << std::endl
		<< contents << "中场一分钟休息中...... " << std::endl;
	sleep(60);}
	return 0;
}

// 事件影响表达.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"

#include<iostream>
#include<string>
#include<map>
#include<set>
#include<vector>
#include<fstream>
#include <iomanip>
using namespace std;
#include<stdlib.h>
#include<time.h>

#define PATH_LENGTH 10//可调参数 路径长度
#define SAMPLE_LENGTH 200//抽取路径的长度
#define PERCENT 2
#define TRAIN 896
#define TEST 256
#define VAL 128
#define ALPHA 30 //控制指数函数的系数
#define LENGTH 720//控制时间间隔 以1分钟为单位
#define Prediction_length 720//控制预测的时间范围

void selected_path();//选择路径
void divide_event();//分割训练集,测试机,验证集数据
double random_probability();
void divide_test_data();//分割测试数据
void time_walk_train();//训练集时间随机游走序列
void time_walk_test();//测试集时间随机游走序列
void time_walk_val();//验证集时间随机游走序列
void random_walk();
void deepwalk();
void divide_random_walk();
void time_walk_train();
int computeEventNumber(int **p,int a);//划分事件的人数,p为事件的数组,a为事件的人数
int compute_number_time_long(int **p, int a,int length);//划分事件的人数,p为事件的数组,a为事件的人数,length为计算的事件间隔
void compute_prediction_number();//在t时间间隔后的传播范围

int main()
{
	//
	//divide_event();
	//time_walk_train();
	//time_walk_val();
	//time_walk_test();
	//random_walk();
	compute_prediction_number();
	divide_event();
    return 0;
}

void deepwalk()
{
	fstream file;
	ofstream file1;
	file.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/all_data.txt");
	file1.open("");
}

void compute_prediction_number()
{
	fstream file;
	ofstream file1;
	file.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/all_data.txt");
	file1.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/event_data.txt");
	string temp, temp1, temp2, temp3;
	int event_id, event_number;
	int begin_node;
	int number = 0;
	int save_time = 0;
	int temp_time, begin_time, end_time;//begin_time时间的开始时间,end_time事件的观测时间加预测时间
	int i = 0, t = 0;//循环变量
	while (!file.eof() && i < 1280)
	{
		number = 0;
		file >> temp >> temp1;
		event_id = atof(temp.c_str());
		event_number = atof(temp1.c_str());
		for (t = 0;t < event_number;t++)
		{
			file >> temp >> temp1;
			if (t == 0)
			{
				save_time = atof(temp.c_str());
				begin_time = atof(temp.c_str()) / 60;
				end_time = begin_time + LENGTH + Prediction_length;
				begin_node = atof(temp1.c_str());//初始的传播节点
			}
			temp_time = atof(temp.c_str()) / 60;
			if (temp_time < end_time)
			{
				number++;
			}
		}
		file1 << event_id << "\t" << number << "\t" << save_time << "\t" << begin_node << endl;
		i++;
	}
}

void time_walk_train()
{
	fstream file_train, file_test;
	string temp, temp1, temp2;
	file_train.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/train_data.txt");
	ofstream file2, file3;
	file2.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/time_train.txt");
	file3.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/影响表达/train_data.txt");
	int number[896][2], i = 0, j = 0, k = 0, next = 0, l = 0, run = 0;
	int path_length[LENGTH];
	int test;
	int t = 0;
	int sample = 0, sample1 = 0, sample2 = 0;//用来存取下一时间的工作
	double save_probability = 0;
	int flag = 0;
	srand((unsigned)time(NULL));
	for (j = 0;j < 10;j++)
	{
		path_length[j] = 0;
	}
	while (!file_train.eof() && i < 896)
	{
		file_train >> temp >> temp1;
		file2 << temp << "\t";
		number[i][0] = atof(temp.c_str());//number存储事件的传播规模
		number[i][1] = atof(temp1.c_str());
		int **path = new int *[number[i][1]];
		for (j = 0;j < number[i][1];j++)//存储路径信息
		{
			path[j] = new int[2];
			file_train >> temp >> temp1;
			path[j][0] = atof(temp.c_str());//存储时间
			path[j][0] = path[j][0] / 60;
			path[j][1] = atof(temp1.c_str());//存储节点
		}
		//file2 << number[i][0] << "\t";
		file3 << number[i][0] << " " << SAMPLE_LENGTH << " " << LENGTH << endl;
		test = compute_number_time_long(path, number[i][1], LENGTH);
		number[i][1] = test;
		save_probability = random_probability();
		for (t = 0;t < SAMPLE_LENGTH;t++)
		{
			j = rand() % number[i][1];
			if (j == 0)
			{
				k = 1;
				path_length[0] = path[0][1];
			}
			else
			{
				k = 2;
				path_length[0] = path[0][1];
				path_length[1] = path[j][1];
			}
			sample1 = j;
			next = j;
			for (;k < PATH_LENGTH;k++)
			{
				for (j=next+1;j < test;j++)
				{
					if (exp(path[j][0] - path[sample1][0]) > save_probability)
					{
						continue;
					}
					else
					{
						j--;
						break;
					}
				}
				next = rand() % (j - sample1) + sample1;
				if (sample1 == next)
				{
					path_length[k] = 0;
				}
				else
				{
					path_length[k] = path[next][1];
				}
				sample1 = next;
			}
			for (k = 0;k < PATH_LENGTH;k++)
			{
				if (path_length[k] != 0)
				{
					file2 << path_length[k] << " ";
					file3 << path_length[k] << " ";
				}
				else
				{
					break;
				}
			}
			file2 << "\t";
			file3 << endl;	
		}
		file2 << "\n";	
		i++;
	}
}

void time_walk_test()
{
	fstream file_train, file_test;
	string temp, temp1, temp2;
	file_train.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/test_data.txt");
	ofstream file2, file3;
	file2.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/time_test.txt");
	file3.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/影响表达/test_data.txt");
	int number[256][2], i = 0, j = 0, k = 0, next = 0, l = 0, run = 0;
	int path_length[LENGTH];
	int test;
	int t = 0;
	int sample = 0, sample1 = 0, sample2 = 0;//用来存取下一时间的工作
	double save_probability = 0;
	int flag = 0;
	srand((unsigned)time(NULL));
	for (j = 0;j < 10;j++)
	{
		path_length[j] = 0;
	}
	while (!file_train.eof() && i < 256)
	{
		file_train >> temp >> temp1;
		file2 << temp << "\t";
		number[i][0] = atof(temp.c_str());//number存储事件的传播规模
		number[i][1] = atof(temp1.c_str());
		int **path = new int *[number[i][1]];
		for (j = 0;j < number[i][1];j++)//存储路径信息
		{
			path[j] = new int[2];
			file_train >> temp >> temp1;
			path[j][0] = atof(temp.c_str());//存储时间
			path[j][0] = path[j][0] / 60;
			path[j][1] = atof(temp1.c_str());//存储节点
		}
		//file2 << number[i][0] << "\t";
		file3 << number[i][0] << " " << SAMPLE_LENGTH << " " << LENGTH << endl;
		test = compute_number_time_long(path, number[i][1], LENGTH);
		number[i][1] = test;
		save_probability = random_probability();
		for (t = 0;t < SAMPLE_LENGTH;t++)
		{
			j = rand() % number[i][1];
			if (j == 0)
			{
				k = 1;
				path_length[0] = path[0][1];
			}
			else
			{
				k = 2;
				path_length[0] = path[0][1];
				path_length[1] = path[j][1];
			}
			sample1 = j;
			next = j;
			for (;k < PATH_LENGTH;k++)
			{
				for (j = next + 1;j < test;j++)
				{
					if (exp(path[j][0] - path[sample1][0]) > save_probability)
					{
						continue;
					}
					else
					{
						j--;
						break;
					}
				}
				next = rand() % (j - sample1) + sample1;
				if (sample1 == next)
				{
					path_length[k] = 0;
				}
				else
				{
					path_length[k] = path[next][1];
				}
				sample1 = next;
			}
			for (k = 0;k < PATH_LENGTH;k++)
			{
				if (path_length[k] != 0)
				{
					file2 << path_length[k] << " ";
					file3 << path_length[k] << " ";
				}
				else
				{
					break;
				}
			}
			file2 << "\t";
			file3 << endl;
		}
		file2 << "\n";
		i++;
	}
}

void time_walk_val()
{
	fstream file_train, file_test;
	string temp, temp1, temp2;
	file_train.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/val_data.txt");
	ofstream file2, file3;
	file2.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/time_val.txt");
	file3.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/影响表达/val_data.txt");
	int number[128][2], i = 0, j = 0, k = 0, next = 0, l = 0, run = 0;
	int path_length[LENGTH];
	int test;
	int t = 0;
	int sample = 0, sample1 = 0, sample2 = 0;//用来存取下一时间的工作
	double save_probability = 0;
	int flag = 0;
	srand((unsigned)time(NULL));
	for (j = 0;j < 10;j++)
	{
		path_length[j] = 0;
	}
	while (!file_train.eof() && i < 128)
	{
		file_train >> temp >> temp1;
		file2 << temp << "\t";
		number[i][0] = atof(temp.c_str());//number存储事件的传播规模
		number[i][1] = atof(temp1.c_str());
		int **path = new int *[number[i][1]];
		for (j = 0;j < number[i][1];j++)//存储路径信息
		{
			path[j] = new int[2];
			file_train >> temp >> temp1;
			path[j][0] = atof(temp.c_str());//存储时间
			path[j][0] = path[j][0] / 60;
			path[j][1] = atof(temp1.c_str());//存储节点
		}
		//file2 << number[i][0] << "\t";
		file3 << number[i][0] << " " << SAMPLE_LENGTH << " " << LENGTH << endl;
		test = compute_number_time_long(path, number[i][1], LENGTH);
		number[i][1] = test;
		save_probability = random_probability();
		for (t = 0;t < SAMPLE_LENGTH;t++)
		{
			j = rand() % number[i][1];
			if (j == 0)
			{
				k = 1;
				path_length[0] = path[0][1];
			}
			else
			{
				k = 2;
				path_length[0] = path[0][1];
				path_length[1] = path[j][1];
			}
			sample1 = j;
			next = j;
			for (;k < PATH_LENGTH;k++)
			{
				for (j = next + 1;j < test;j++)
				{
					if (exp(path[j][0] - path[sample1][0]) > save_probability)
					{
						continue;
					}
					else
					{
						j--;
						break;
					}
				}
				next = rand() % (j - sample1) + sample1;
				if (sample1 == next)
				{
					path_length[k] = 0;
				}
				else
				{
					path_length[k] = path[next][1];
				}
				sample1 = next;
			}
			for (k = 0;k < PATH_LENGTH;k++)
			{
				if (path_length[k] != 0)
				{
					file2 << path_length[k] << " ";
					file3 << path_length[k] << " ";
				}
				else
				{
					break;
				}
			}
			file2 << "\t";
			file3 << endl;
		}
		file2 << "\n";
		i++;
	}
}
void random_walk()
{
	fstream file;
	file.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/event_data.txt");
	ofstream file1;
	file1.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/random_train.txt");
	int event_info[1280][3];
	string temp, temp1, temp2, temp3;
	int i = 0, t = 0,k=0;
	int event_number = 0, event_id = 0;
	int number=0;
	int random_quit = 0;
	int path[PATH_LENGTH];
	int next = 0, test = 0;
	srand((unsigned)time(NULL));
	while (!file.eof() && i < 1280)
	{
		file >> temp >> temp1 >> temp2 >> temp3;
		event_info[i][0] = atof(temp.c_str());
		event_info[i][1] = atof(temp2.c_str()) / 60;
		event_info[i][2] = atof(temp1.c_str());
		i++;
	}
	file.close();
	file.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/all_data.txt");
	i = 0;
	while (!file.eof() && i < TRAIN)
	{
		file >> temp >> temp1;
		event_id = atof(temp.c_str());
		event_number = atof(temp1.c_str());
		int **p = new int *[event_number];
		for (t = 0;t < event_number;t++)
		{
			p[t] = new int[2];
			file >> temp >> temp1;
			p[t][0] = atof(temp1.c_str());
			p[t][1] = atof(temp.c_str()) / 60;
		}
		number = 0;
		for (t = 0;t < event_number;t++)
		{
			if (p[t][1] < event_info[i][1]+LENGTH)
			{
				number++;
			}
			else
			{
				event_info[i][2] = number - 1;
				break;
			}
		}
		cout << number << endl;
		file1 << event_id << "\t";
		for (t = 0;t < SAMPLE_LENGTH;t++)
		{
			test = rand() % number;
			path[0] = p[test][0];
			next = test;
			for (k = 1;k < PATH_LENGTH;k++)
			{
				if (next != number)
				{
					next = ((rand() % (number - next)) + next + 1);
				}
				

				if (path[k - 1] == p[next - 1][0] || path[k - 1] == 0)
				{
					path[k] = 0;
				}
				else
				{
					path[k] = p[k - 1][0];
				}
				random_quit = rand() % 4;
				if (random_quit < 3)
				{
					path[k] = 0;
				}
			}
			for (k = 0;k < PATH_LENGTH;k++)
			{
				if (path[k] != 0)
				{
					file1 << path[k] << " ";
				}
			}
			file1 << "\t";
		}
		file1 << "\n";
		i++;
		cout << i << endl;
	}
	file1.close();
	file1.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/random_val.txt");
	while (!file.eof() && i < TRAIN+VAL)
	{
		file >> temp >> temp1;
		event_id = atof(temp.c_str());
		event_number = atof(temp1.c_str());
		int **p = new int *[event_number];
		for (t = 0;t < event_number;t++)
		{
			p[t] = new int[2];
			file >> temp >> temp1;
			p[t][0] = atof(temp1.c_str());
			p[t][1] = atof(temp.c_str()) / 60;
		}
		number = 0;
		for (t = 0;t < event_number;t++)
		{
			if (p[t][1] < event_info[i][1] + LENGTH)
			{
				number++;
			}
			else
			{
				event_info[i][2] = number - 1;
				break;
			}
		}
		cout << number << endl;
		file1 << event_id << "\t";
		for (t = 0;t < SAMPLE_LENGTH;t++)
		{
			test = rand() % number;
			path[0] = p[test][0];
			next = test;
			for (k = 1;k < PATH_LENGTH;k++)
			{
				if (next != number)
				{
					next = ((rand() % (number - next)) + next + 1);
				}

				if (path[k - 1] == p[next - 1][0] || path[k - 1] == 0)
				{
					path[k] = 0;
				}
				else
				{
					path[k] = p[next - 1][0];
				}
				random_quit = rand() % 4;
				if (random_quit < 3)
				{
					path[k] = 0;
				}
			}
			for (k = 0;k < PATH_LENGTH;k++)
			{
				if (path[k] != 0)
				{
					file1 << path[k] << " ";
				}

			}
			file1 << "\t";
		}
		file1 << "\n";
		i++;
		cout << i << endl;
	}
	file1.close();
	file1.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/random_test.txt");
	while (!file.eof() && i < TRAIN + VAL+TEST)
	{
		file >> temp >> temp1;
		event_id = atof(temp.c_str());
		event_number = atof(temp1.c_str());
		int **p = new int *[event_number];
		for (t = 0;t < event_number;t++)
		{
			p[t] = new int[2];
			file >> temp >> temp1;
			p[t][0] = atof(temp1.c_str());
			p[t][1] = atof(temp.c_str()) / 60;
		}
		number = 0;
		for (t = 0;t < event_number;t++)
		{
			if (p[t][1] < event_info[i][1] + LENGTH)
			{
				number++;
			}
			else
			{
				event_info[i][2] = number - 1;
				break;
			}
		}
		cout << number << endl;
		file1 << event_id << "\t";
		for (t = 0;t < SAMPLE_LENGTH;t++)
		{
			test = rand() % number;
			path[0] = p[test][0];
			next = test;
			for (k = 1;k < PATH_LENGTH;k++)
			{
				if (next != number)
				{
					next = ((rand() % (number - next)) + next + 1);
				}

				if (path[k - 1] == p[next - 1][0] || path[k - 1] == 0)
				{
					path[k] = 0;
				}
				else
				{
					path[k] = p[next - 1][0];
				}
				random_quit = rand() % 4;
				if (random_quit <3)
				{
					path[k] = 0;
				}
			}
			for (k = 0;k < PATH_LENGTH;k++)
			{
				if (path[k] != 0)
				{
					file1 << path[k] << " ";
				}

			}
			file1 << "\t";
		}
		file1 << "\n";
		i++;
		cout << i << endl;
	}
	file1.close();
}

//划分事件的人数,p为事件的数组,a为事件的人数,length为计算的事件间隔
int compute_number_time_long(int **p, int a, int length)
{
	int number = 0;
	int time, i;
	time = p[0][0] + length;
	for (i = 0;i < a;i++)
	{
		if (p[i][0] < time)
		{
			number++;
		}
		else
		{
			break;
		}
	}
	cout << number << endl;
	return number;
}


int computeEventNumber(int **p, int a)
{
	int time = 0;
	time = (p[0][0] + p[a - 1][0])/2;
	int i = 0;
	//cout << time << endl;
	//cout << a << endl;
	int number = 0;
	for (i = 0;i < a;i++)
	{
		if (p[i][0] < time)
		{
			number++;
			//cout << number << endl;
		}
		else
		{
			break;
		}
	}
	//cout << "=============="<<number << endl;
	return number;
}

void divide_event()
{
	fstream file;
	ofstream file1;
	string temp, temp1, temp2, temp3;
	int event_id, event_number;
	int i = 0, t = 0;//循环变量
	i = 0;

	file.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/event_data.txt");
	file1.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/event_train.txt");
	while (!file.eof() && i < TRAIN)
	{
		file >> temp >> temp1 >> temp2 >> temp3;
		file1 << temp << "\t" << temp1 << "\t" << rand() % 3 + 1 << endl;
		i++;
	}
	file1.close();
	file1.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/event_val.txt");
	while (!file.eof() && i < (TRAIN + VAL))
	{
		file >> temp >> temp1 >> temp2 >> temp3;
		file1 << temp << "\t" << temp1 << "\t" << rand() % 3 + 1 << endl;
		i++;
	}
	file1.close();
	file1.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/event_test.txt");
	while (!file.eof() && i < (TRAIN + VAL + TEST))
	{
		file >> temp >> temp1 >> temp2 >> temp3;
		file1 << temp << "\t" << temp1 << "\t" << rand() % 3 + 1 << endl;
		i++;
	}
}

void divide_test_data()
{
	fstream file;
	file.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/all_data.txt");
	ofstream file1;
	file1.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/train_data.txt");
	string temp, temp1, temp2;
	int event_id, event_number;
	int i = 0, t = 0;//循环变量
	while (!file.eof() && i < TRAIN)
	{
		file >> temp >> temp1;
		event_id = atof(temp.c_str());
		event_number = atof(temp1.c_str());
		file1 << temp << "\t" << temp1 << endl;
		for (t = 0;t < event_number;t++)
		{
			file >> temp >> temp1;
			file1 << temp << " " << temp1 << endl;
		}
		i++;
		cout << i << endl;
	}
	file1.close();
	file1.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/val_data.txt");
	while (!file.eof() && i < (TRAIN + VAL))
	{
		file >> temp >> temp1;
		event_id = atof(temp.c_str());
		event_number = atof(temp1.c_str());
		file1 << temp << "\t" << temp1 << endl;
		for (t = 0;t < event_number;t++)
		{
			file >> temp >> temp1;
			file1 << temp << "\t" << temp1 << endl;
		}
		i++;
		cout << i << endl;
	}
	file1.close();
	file1.open("D:/代码/C/刘子图/基于事件相关的影响范围预测/事件影响表达/data/数据集1280/test_data.txt");
	while (!file.eof() && i < (TRAIN + VAL + TEST))
	{
		file >> temp >> temp1;
		event_id = atof(temp.c_str());
		event_number = atof(temp1.c_str());
		file1 << temp << "\t" << temp1 << endl;
		for (t = 0;t < event_number;t++)
		{
			file >> temp >> temp1;
			file1 << temp << "\t" << temp1 << endl;
		}
		i++;
		cout << i << endl;
	}
}

double random_probability()
{
	//cout << ((double)(rand() % 20)+1) / 2000 << endl;
	return ((double)(rand() % 20) + 1) / 1000000;
}
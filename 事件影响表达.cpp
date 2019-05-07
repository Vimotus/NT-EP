// �¼�Ӱ����.cpp : �������̨Ӧ�ó������ڵ㡣
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

#define PATH_LENGTH 10//�ɵ����� ·������
#define SAMPLE_LENGTH 200//��ȡ·���ĳ���
#define PERCENT 2
#define TRAIN 896
#define TEST 256
#define VAL 128
#define ALPHA 30 //����ָ��������ϵ��
#define LENGTH 720//����ʱ���� ��1����Ϊ��λ
#define Prediction_length 720//����Ԥ���ʱ�䷶Χ

void selected_path();//ѡ��·��
void divide_event();//�ָ�ѵ����,���Ի�,��֤������
double random_probability();
void divide_test_data();//�ָ��������
void time_walk_train();//ѵ����ʱ�������������
void time_walk_test();//���Լ�ʱ�������������
void time_walk_val();//��֤��ʱ�������������
void random_walk();
void deepwalk();
void divide_random_walk();
void time_walk_train();
int computeEventNumber(int **p,int a);//�����¼�������,pΪ�¼�������,aΪ�¼�������
int compute_number_time_long(int **p, int a,int length);//�����¼�������,pΪ�¼�������,aΪ�¼�������,lengthΪ������¼����
void compute_prediction_number();//��tʱ������Ĵ�����Χ

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
	file.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/all_data.txt");
	file1.open("");
}

void compute_prediction_number()
{
	fstream file;
	ofstream file1;
	file.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/all_data.txt");
	file1.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/event_data.txt");
	string temp, temp1, temp2, temp3;
	int event_id, event_number;
	int begin_node;
	int number = 0;
	int save_time = 0;
	int temp_time, begin_time, end_time;//begin_timeʱ��Ŀ�ʼʱ��,end_time�¼��Ĺ۲�ʱ���Ԥ��ʱ��
	int i = 0, t = 0;//ѭ������
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
				begin_node = atof(temp1.c_str());//��ʼ�Ĵ����ڵ�
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
	file_train.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/train_data.txt");
	ofstream file2, file3;
	file2.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/time_train.txt");
	file3.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/Ӱ����/train_data.txt");
	int number[896][2], i = 0, j = 0, k = 0, next = 0, l = 0, run = 0;
	int path_length[LENGTH];
	int test;
	int t = 0;
	int sample = 0, sample1 = 0, sample2 = 0;//������ȡ��һʱ��Ĺ���
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
		number[i][0] = atof(temp.c_str());//number�洢�¼��Ĵ�����ģ
		number[i][1] = atof(temp1.c_str());
		int **path = new int *[number[i][1]];
		for (j = 0;j < number[i][1];j++)//�洢·����Ϣ
		{
			path[j] = new int[2];
			file_train >> temp >> temp1;
			path[j][0] = atof(temp.c_str());//�洢ʱ��
			path[j][0] = path[j][0] / 60;
			path[j][1] = atof(temp1.c_str());//�洢�ڵ�
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
	file_train.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/test_data.txt");
	ofstream file2, file3;
	file2.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/time_test.txt");
	file3.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/Ӱ����/test_data.txt");
	int number[256][2], i = 0, j = 0, k = 0, next = 0, l = 0, run = 0;
	int path_length[LENGTH];
	int test;
	int t = 0;
	int sample = 0, sample1 = 0, sample2 = 0;//������ȡ��һʱ��Ĺ���
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
		number[i][0] = atof(temp.c_str());//number�洢�¼��Ĵ�����ģ
		number[i][1] = atof(temp1.c_str());
		int **path = new int *[number[i][1]];
		for (j = 0;j < number[i][1];j++)//�洢·����Ϣ
		{
			path[j] = new int[2];
			file_train >> temp >> temp1;
			path[j][0] = atof(temp.c_str());//�洢ʱ��
			path[j][0] = path[j][0] / 60;
			path[j][1] = atof(temp1.c_str());//�洢�ڵ�
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
	file_train.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/val_data.txt");
	ofstream file2, file3;
	file2.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/time_val.txt");
	file3.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/Ӱ����/val_data.txt");
	int number[128][2], i = 0, j = 0, k = 0, next = 0, l = 0, run = 0;
	int path_length[LENGTH];
	int test;
	int t = 0;
	int sample = 0, sample1 = 0, sample2 = 0;//������ȡ��һʱ��Ĺ���
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
		number[i][0] = atof(temp.c_str());//number�洢�¼��Ĵ�����ģ
		number[i][1] = atof(temp1.c_str());
		int **path = new int *[number[i][1]];
		for (j = 0;j < number[i][1];j++)//�洢·����Ϣ
		{
			path[j] = new int[2];
			file_train >> temp >> temp1;
			path[j][0] = atof(temp.c_str());//�洢ʱ��
			path[j][0] = path[j][0] / 60;
			path[j][1] = atof(temp1.c_str());//�洢�ڵ�
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
	file.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/event_data.txt");
	ofstream file1;
	file1.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/random_train.txt");
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
	file.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/all_data.txt");
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
	file1.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/random_val.txt");
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
	file1.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/random_test.txt");
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

//�����¼�������,pΪ�¼�������,aΪ�¼�������,lengthΪ������¼����
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
	int i = 0, t = 0;//ѭ������
	i = 0;

	file.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/event_data.txt");
	file1.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/event_train.txt");
	while (!file.eof() && i < TRAIN)
	{
		file >> temp >> temp1 >> temp2 >> temp3;
		file1 << temp << "\t" << temp1 << "\t" << rand() % 3 + 1 << endl;
		i++;
	}
	file1.close();
	file1.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/event_val.txt");
	while (!file.eof() && i < (TRAIN + VAL))
	{
		file >> temp >> temp1 >> temp2 >> temp3;
		file1 << temp << "\t" << temp1 << "\t" << rand() % 3 + 1 << endl;
		i++;
	}
	file1.close();
	file1.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/event_test.txt");
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
	file.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/all_data.txt");
	ofstream file1;
	file1.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/train_data.txt");
	string temp, temp1, temp2;
	int event_id, event_number;
	int i = 0, t = 0;//ѭ������
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
	file1.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/val_data.txt");
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
	file1.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/test_data.txt");
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
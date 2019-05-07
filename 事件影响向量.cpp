// �¼�Ӱ������.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;
#include <time.h>
#include <math.h>
#include <stdlib.h>

#define LENGTH 10//�ɵ����� ·������
#define SAMPLE_LENGTH 200//��ȡ·���ĳ���
#define PERCENT 2
#define TRAIN 896
#define TEST 256
#define VAL 128
#define ALPHA 30 //����ָ��������ϵ��
#define TIME_LENGTH 2160 //��1����Ϊ��λʱ�� ʱ��������TIME_LENGTH��Ϊ��Ӱ��
int influence[1280][1280];
vector<int> vec_arry[1280];//ʱ�� �ڵ�
int event_info[1280][4];
double parameter[SAMPLE_LENGTH][50];
void set_walks();//����Ӱ���·��
double random_probability();
double learning_rate = 0.001;
void write_influence();
void init();//��ʼ������
int walks(int id,int number);//idΪ�¼���,numberΪ�¼�������λ��
void compute_influence_vector();//�����¼���Ӱ��������һ��
void compute_influence_number();//����һ���¼���Ӱ�췶Χ

void compute_prediction_vector();//�����¼��Ĵ�������
void training_vector(int training_iter);
void divide_embedding_data();
//void test();

int main()
{

	//init();
	//training_vector(5000);
	//set_walks();
	//compute_influence_vector();
	//compute_influence_number();
	divide_embedding_data();
    return 0;
}

void compute_prediction_vector()
{
	fstream file, file1;
	ofstream file2;
	file.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/vector.txt");
	file2.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/event_influence.txt");
	string temp, temp1, temp2;
	int i = 0, t = 0, all = 0, length = 0, n = 0, l = 0;
	int path_length = 0;
	int remember = 0;
	double test;
	map<int, vector<double>>node_vector;
	map<int, vector<double>>::iterator it;
	vector<double>::iterator it1;
	file >> temp >> temp1;
	all = atof(temp.c_str());
	length = atof(temp1.c_str());
	while (!file.eof() && t < all)
	{
		file >> temp;
		node_vector.insert(pair<int, vector<double>>(atof(temp.c_str()), 0));
		it = node_vector.find(atof(temp.c_str()));
		for (n = 0;n < length;n++)
		{
			file1 >> temp;
			it->second.push_back(atof(temp.c_str()));
		}
		t++;
	}
	file.close();
	double *all_vector = new double[length];
	for (i = 0;i < 50;i++)
	{
		all_vector[i] = 0;
	}
	file.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/all_vec_walks.txt");
	i = 0;
	while (!file.eof() && i < 1280)
	{
		file >> temp >> temp1 >> temp2;//�¼���,·����,һ��·���Ľڵ���
		file2 << temp;
		remember = 0;
		for (n = 0;n < SAMPLE_LENGTH;n++)
		{
			file >> temp;
			path_length = atof(temp.c_str());
			remember = remember + path_length;
			for (l = 0;l < path_length;l++)
			{
				file >> temp;
				test = atof(temp.c_str());
				it = node_vector.find(test);
				if (it == node_vector.end())
				{
					continue;
				}
				else
				{
					for (t = 0, it1 = it->second.begin();it1 != it->second.end() && t<length;t++, it1++)
					{
						all_vector[t] = *it1 + all_vector[t];
					}
				}
			}
		}
		cout << remember << endl;
		for (t = 0;t < length;t++)
		{
			file2 << " " << all_vector[t] / remember;
			all_vector[t] = 0;
		}
		file2 << endl;
		i++;
	}
}

void divide_embedding_data()
{
	fstream file, file1;
	file.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/final_vector.txt");
	file1.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/time_order.txt");
	ofstream file2;
	int i = 0, t = 0;
	string temp, temp1, temp2;
	file2.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/embedding_train.txt");
	while (!file1.eof() && i < TRAIN)
	{
		file1 >> temp >> temp1 >> temp2;
		file2 << temp ;
		for (t = 0;t < 50;t++)
		{
			file >> temp;
			file2 << " " << temp;
		}
		file2 << endl;
		i++;
	}
	file2.close();
	file2.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/embedding_val.txt");
	while (!file1.eof() && i < TRAIN + VAL)
	{
		file1 >> temp >> temp1 >> temp2;
		file2 << temp;
		for (t = 0;t < 50;t++)
		{
			file >> temp;
			file2 << " " << temp;
		}
		file2 << endl;
		i++;
	}
	file2.close();
	file2.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/embedding_test.txt");
	while (!file1.eof() && i < TRAIN+VAL+TEST)
	{
		file1 >> temp >> temp1 >> temp2;
		file2 << temp;
		for (t = 0;t < 50;t++)
		{
			file >> temp;
			file2 << " " << temp;
		}
		file2 << endl;
		i++;
	}
	file2.close();
}

void training_vector(int training_iter)
{
	double event_vector[1280][50];
	fstream file;
	file.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/event_influence.txt");
	ofstream file1;
	file1.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/final_vector.txt");
	string temp, temp1, temp2;
	int i = 0, t = 0, n = 0, l = 0;
	double event_influence_vector[1280][50];
	double loss = 0;
	double event_number[1280], err[1280];
	double event_para[1280][50];
	srand((int)time(0));
	int test = 0;
	while (!file.eof() && i < 1280)//��ȡ�¼��Ĵ�������
	{
		for (t = 0;t < 50;t++)
		{
			file >> temp;
			event_vector[i][t] = atof(temp.c_str());
		}
		i++;
	}
	file.close();
	file.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/time_influence.txt");
	i = 0;
	while (!file.eof() && i < 1280)
	{
		file >> temp >> temp1;
		event_number[i] = log10(atof(temp1.c_str()));
		cout << event_number[i] << endl;
		if (i == 1278)
		{
			event_number[i + 1] = 1;
			break;
		}
		i++;
	}
	for (i = 0;i < 1280;i++)//��ʼ���¼�Ӱ�����
	{
		for (t = 0;t < 50;t++)
		{
			event_influence_vector[i][t] = (double)(rand() % 1000000000) / 100000 ;
		}
	}
	for (i = 0;i < training_iter;i++)
	{
		for (t = 0;t < 1280;t++)
		{
			for (n = 0;n < 1280;n++)
			{
				err[n] = 0;
			}
			for (n = 0;n < 50;n++)
			{
				err[t]+=event_influence_vector[t][n] * event_vector[t][n];
			}
		}
		loss = 0;
		for (n = 0;n < 1280;n++)
		{
			err[n] = err[n] - event_number[n];
			loss += err[n] * err[n];
		}
		for (t = 0;t < 1280;t++)
		{
			for (n = 0;n < 50;n++)
			{
				event_influence_vector[t][n] += learning_rate*err[n] * event_influence_vector[t][n];
			}
		}
		cout << i << " " << "loss:" << loss/1280 << endl;
	}
	for (i = 0;i < 1280;i++)
	{
		for (t = 0;t < 50;t++)
		{
			file1 << setprecision(10)<< event_influence_vector[i][t]*3 << " ";
		}
		file1 << endl;
	}
	file1.close();
}

void compute_influence_number()
{
	fstream file;
	file.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/time_order.txt");
	ofstream file1;
	file1.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/time_influence.txt");
	string temp, temp1, temp2;
	int i = 0, t = 0, n = 0;
	int event_id[1280][2];
	int id, event_number, time, number;
	while (!file.eof() && i < 1280)
	{
		file >> temp >> temp1 >> temp2;
		event_id[i][0] = atof(temp.c_str());
		event_id[i][1] = atof(temp1.c_str());
		i++;
	}
	file.close();
	file.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/all_data.txt");
	t = 0;
	for (i = 1;i < 1280;i++)
	{
		for (t = i-1;t < i;t++)
		{
			file >> temp >> temp1;
			id = atof(temp.c_str());
			event_number = atof(temp1.c_str());
			number = 0;
			for (n = 0;n < event_number;n++)
			{
				file >> temp >> temp1;
				time = atof(temp.c_str());
				//cout << event_id[i][1] << " " << time << endl;;
				if ( event_id[i][1]>time)
				{
					number++;
				}
			}
		}
		cout << number << endl;
		if (number < 2)
		{
			number = 2;
		}
		file1 << event_id[i - 1][0] << " " << number << endl;
	}
}

void test()
{
	fstream file;
	string name, name1;
	name = "D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/Ӱ����/vector";
	name1 = ".txt";
	int i = 1;
	string temp;
	temp = name + to_string(i) + name1;
	cout << temp << endl;
	//cin >> i;
	file.open(temp);
	while (!file.eof())
	{
		file >> temp;
		cout << temp;
	}
}

void compute_influence_vector()
{
	fstream file, file1;
	ofstream file2;
	file.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/all_vec_walks.txt");
	file2.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/event_influence.txt");
	string temp, temp1, temp2;
	string name, name1, name2;
	name = "D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/Ӱ����/vector";
	name1 = ".txt";
	int loop = 1;
	int i = 0, t = 0, all = 0, vector_length, n = 0, l = 0;
	int path_length = 0;
	int remember = 0;//��¼һ���¼��Ľڵ���
	int id = 0;
	double test;
	map<int, vector<double>> node_vector;
	map<int, vector<double>>::iterator it;
	vector<double>::iterator it1;
	while (!file.eof() && i < 1280)
	{
		name2 = name + to_string(loop) + name1;
		file1.open(name2);
		file1 >> temp >> temp1;
		all = atof(temp.c_str());
		vector_length = atof(temp1.c_str());
		double *all_vector = new double[vector_length];
		remember = 0;
		for (t = 0;t < vector_length;t++)
		{
			all_vector[t] = 0;
		}
		t = 0;
		while (!file1.eof() && t < all)
		{
			file1 >> temp;
			node_vector.insert(pair<int, vector<double>>(atof(temp.c_str()), 0));
			it = node_vector.find(atof(temp.c_str()));
			for (n = 0;n < vector_length;n++)
			{
				file1 >> temp;
				it->second.push_back(atof(temp.c_str()));
			}
			t++;
		}
		/*for (it = node_vector.begin();it != node_vector.end();it++)
		{
			for (it1 = it->second.begin();it1 != it->second.end();it1++)
			{
				cout <<it->first<<" "<< *it1 << endl;
			}
		}*/
		file >> temp >> temp1 >> temp2;//�¼���,·����,һ��·���Ľڵ���
		file2 << temp ;
		remember = 0;
		for (n = 0;n < SAMPLE_LENGTH;n++)
		{
			file >> temp;
			path_length = atof(temp.c_str());
			remember = remember + path_length;
			for (l = 0;l < path_length;l++)
			{
				file >> temp;
				test = atof(temp.c_str());
				it=node_vector.find(test);
				if (it == node_vector.end())
				{
					for (t = 0;t < vector_length;t++)
					{
						all_vector[t] += 0.001;
					}
					continue;
				}
				else
				{
					for (t=0,it1 = it->second.begin();it1 != it->second.end()&&t<vector_length;t++,it1++)
					{
						all_vector[t] =*it1+ all_vector[t];
					}
				}
			}
		}
		cout << remember << endl;
		for (t = 0;t < vector_length;t++)
		{
			file2 << " " << all_vector[t] / remember;
		}
		file2 << endl;
		node_vector.clear();
		i++;
		loop++;
		file1.close();
		cout << i << endl;
	}
}

//idΪ�¼���,numberΪ�¼�������λ��
int walks(int id, int number)
{
	ofstream file2, file3;
	file2.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/all_walks.txt",ios::app);
	file3.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/all_vec_walks.txt", ios::app);
	if (number == 0)
	{
		file2 << event_info[id][0] << endl;
		file3 << event_info[id][0] << " " << SAMPLE_LENGTH << " " << LENGTH << endl;
		return 0;
	}
	vector<int>::iterator it;
	int i = 0, t = 0, next = 0, k = 0, j = 0, l = 0;
	int n = 0;
	int path_length[LENGTH];
	int sample = 0, sample1 = 0, sample2 = 0;//������ȡ��һʱ��Ĺ���
	double save_probability = 0;
	int flag = 0;
	int **path = new int *[number];
	for (i=0,it = vec_arry[id].begin();it != vec_arry[id].end()&&i<number;it++,i++)
	{
		path[i] = new int[2];
		path[i][0] = *it;
		it++;
		path[i][1] = *it;
	}
	file2 << event_info[id][0] << "\t";
	file3 << event_info[id][0] << " " << SAMPLE_LENGTH << " " << LENGTH << endl;
	for (t = 0;t < SAMPLE_LENGTH;t++)
	{
		j = rand() % number;
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
		for (;k < LENGTH;k++)
		{
			for (j = next + 1;j < number;j++)
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
			if (j == sample1)
			{
				next = sample1;
			}
			else 
			{
				next = rand() % (j - sample1) + sample1;
			}
			
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
		n = 0;
		for (k = 0;k < LENGTH;k++)
		{
			if (path_length[k] != 0)
			{
				n++;
				file2 << path_length[k] << " ";
			}
		}
		file2 << "\t";
		file3 << n << endl;
		for (k = 0;k < n;k++)
		{
			file3 << path_length[k] << " ";
		}
		file3 << endl;
	}
	file2 << "\n";
	for (j = 0;j < number;j++)//�ͷ��ڴ�
	{
		delete[]path[j];
	}
}

void set_walks()
{
	fstream file;
	ofstream file1;
	string temp, temp1, temp2, temp3;
	int i = 0, t = 0;
	int find = 0;//�������¼�����Ӱ����¼�
	int length = 0, end_time;//��¼���Ⱥͽ���ʱ��
	file.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/all_data.txt");
	file1.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/all_walks.txt");
	file1.close();
	file1.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/all_vec_walks.txt");
	file1.close();
	//file1.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/influence_number.txt");
	vector<int>::iterator it;
	/*for (i = 0;i < 1280;i++)
	{
		for (it = vec_arry[i].begin();it != vec_arry[i].end();it++)
		{
			cout << *it<<" ";
			it++;
			cout << *it;
			cout << endl;
		}
		cout << i << endl;
		cin >> t;
	}*/
	for (i = 0;i < 1280;i++)
	{//��ȡ���¼���
		cout << i << endl;
		length = 0;
		for (find = 0;find < 1280;find++)//�ҵ�����Ӱ��Ľڵ�
		{
			if (influence[i][find] == 1)
			{
				break;
			}
		}
		//cout << find << endl;
		if (find == 1280)
		{
			length = event_info[i][1];
			cout << event_info[i][1] << "===========" << endl;
		}
		else
		{
			end_time = event_info[find][2];
			for (it = vec_arry[i].begin();it != vec_arry[i].end();it++)
			{
				if (*it < end_time)
				{
					length++;
					it++;
				}
				else { 
					file1 << event_info[i][0] << " " << event_info[i][2] << " " << event_info[find][0] << " " << event_info[find][2] << " " << length << endl;
					break; 
				}
			}	
		}
		cout << length << endl;
		if (length == 0)
		{
			cout << "1111111" << endl;
			walks(i,5);
		}
		else
		{
			walks(i, length);
		}
		
	}
}

void init()
{
	fstream file;
	file.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/event_data.txt");
	string temp, temp1, temp2, temp3;
	vector<int>::iterator it;
	int i = 0, t = 0;
	int find = 0;//�������¼�����Ӱ����¼�
	int length = 0;//��¼
	while (!file.eof() && i < 1280)
	{
		file >> temp >> temp1 >> temp2 >> temp3;
		event_info[i][0] = atof(temp.c_str());//�¼���
		event_info[i][1] = atof(temp1.c_str());//����Ӱ������
		event_info[i][2] = atof(temp2.c_str()) / 60;//����ʱ�� 1����Ϊ��λʱ��
		event_info[i][3] = atof(temp3.c_str());//��ʼ�ڵ�
		i++;
	}
	file.close();
	for (i = 0;i < 1280;i++)
	{
		for (t = 0;t < 1280;t++)
		{
			influence[i][t] = 0;
		}
	}
	for (i = 0;i < 1280;i++)//����¼����໥Ӱ��
	{
		for (t = i;t < 1280;t++)
		{
			if (t == i)
			{
				continue;
			}
			if ((event_info[t][2] - event_info[i][2]) < TIME_LENGTH)
			{
				influence[i][t] = 1;
			}
			else
			{
				break;
			}
		}
	}
	write_influence();
	file.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/all_data.txt");
	i = 0;
	while (!file.eof() && i < 1280)
	{
		file >> temp >> temp1;
		for (t = 0;t < event_info[i][1];t++)
		{
			file >> temp >> temp1;
			vec_arry[i].insert(vec_arry[i].end(), atof(temp.c_str())/60);//��ʱ��ڵ�Ų���vector
			vec_arry[i].insert(vec_arry[i].end(), atof(temp1.c_str()));
		}
		i++;
	}
	/*for (i = 0;i < 1280;i++)
	{
		for (it = vec_arry[i].begin();it != vec_arry[i].end();it++)
		{
			cout << *it << " ";
			it++;
			cout << *it;
			cout << endl;
		}
		cout << i << endl;
		cin >> t;
	}*/
}

void write_influence()
{
	ofstream file1;
	file1.open("D:/����/C/����ͼ/�����¼���ص�Ӱ�췶ΧԤ��/�¼�Ӱ����/data/���ݼ�1280/test.txt");
	int i = 0, t = 0;
	for (i = 0;i < 1280;i++)
	{
		for (t = 0;t < 1280;t++)
		{
			file1 << influence[i][t] << " ";
		}
		file1 << endl;
	}
}

double random_probability()
{
	//cout << ((double)(rand() % 20)+1) / 2000 << endl;
	return ((double)(rand() % 20) + 1) / 200000;
}
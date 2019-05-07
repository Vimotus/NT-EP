# -*- coding: utf-8 -*-
"""
Created on Fri Nov  2 15:45:20 2018

@author: Vimotus
"""

import numpy as np
import random
import six.moves.cPickle as pickle

DATA_PATH = '../data/test-net/'

LABEL_NUM = 1

graphs_train = {}
with open("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\random_train.txt", 'r') as f:
#with open("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\time_train.txt", 'r') as f:
    for line in f:
        walks = line.strip().split('\t')
        graphs_train[walks[0]] = []
       # print(len(walks))
        for i in range(1, len(walks)):
            graphs_train[walks[0]].append([int(x) for x in walks[i].split()])
       # print(graphs_train[walks[0]])
#print(len(graphs_train))
graphs_val = {}
with open("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\random_val.txt", 'r') as f:
#with open("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\time_val.txt", 'r') as f:
    for line in f:
        walks = line.strip().split('\t')
        graphs_val[walks[0]] = []
        #print(len(walks))
        for i in range(1, len(walks)):
            graphs_val[walks[0]].append([int(x) for x in walks[i].split()])

graphs_test = {}
with open("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\random_test.txt", 'r') as f:
#with open("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\time_test.txt", 'r') as f:
    for line in f:
        walks = line.strip().split('\t')
        graphs_test[walks[0]] = []
       # print(len(walks))
        for i in range(1, len(walks)):
            graphs_test[walks[0]].append([int(x) for x in walks[i].split()])

embedding_test={}            
print(type(embedding_test))
with open("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\embedding_test.txt",'r') as f:
    for line in f:
        #print(line)
        line=line[:-1]
        embedding=line.rsplit(' ')
        number=embedding[0]
        embedding=embedding[1:]
        embedding_test[number]=embedding
        #print(embedding_test[number])

embedding_train={}
with open("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\embedding_train.txt",'r') as f:
    for line in f:
        #print(line)
        line=line[:-1]
        embedding=line.rsplit(' ')
        number=embedding[0]
        embedding=embedding[1:]
        embedding_train[number]=embedding
        #print(embedding_test[number])

embedding_val={}
with open("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\embedding_val.txt",'r') as f:
    for line in f:
        #print(line)
        line=line[:-1]
        embedding=line.rsplit(' ')
        number=embedding[0]
        embedding=embedding[1:]
        embedding_val[number]=embedding
        #print(embedding_test[number])

labels_train = {}
sizes_train = {}
i=0

with open("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\event_train.txt", 'r') as f:
    for line in f:
        profile = line.split('\t')
        labels_train[profile[0]] = int(profile[2])
        sizes_train[profile[0]] = int(profile[1])
        i=i+1

labels_val = {}
sizes_val = {}
i=0

with open("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\event_val.txt", 'r') as f:
    for line in f:
        profile = line.split('\t')
        labels_val[profile[0]] = int(profile[2])
        sizes_val[profile[0]] = int(profile[1])
        #print(profile[2])
        i=i+1
        
labels_test = {}
sizes_test = {}
i=0

with open("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\event_test.txt", 'r') as f:
    for line in f:
        profile = line.split('\t')
        labels_test[profile[0]] = int(profile[2])
        sizes_test[profile[0]] = int(profile[1])
        i=i+1
        
        
class IndexDict:
    def __init__(self, original_ids):
        self.original_to_new = {}
        self.new_to_original = []
        cnt = 0
        for i in original_ids:
            new = self.original_to_new.get(i,cnt)
            if new == cnt:
                self.original_to_new[i] = cnt
                cnt += 1
                self.new_to_original.append(i)
    def new(self, original):
        if type(original) is int:
            return self.original_to_new[original]
        else:
            if type(original[0]) is int:
                return [self.original_to_new[i] for i in original]
            else:
                return [[self.original_to_new[i] for i in l] for l in original]
    def original(self, new):
        if type(new) is int:
            return self.new_to_original[new]
        else:
            if type(new[0]) is int:
                return [self.new_to_original[i] for i in new]
            else:
                return [[self.new_to_original[i] for i in l] for l in new]
    def length(self):
        return len(self.new_to_original)
    
    
original_ids = set()
for graph in graphs_train.keys():
    for walk in graphs_train[graph]:
        for i in set(walk):
            original_ids.add(i)
for graph in graphs_val.keys():
    for walk in graphs_val[graph]:
        for i in set(walk):
            original_ids.add(i)
for graph in graphs_test.keys():
    for walk in graphs_test[graph]:
        for i in set(walk):
            original_ids.add(i)

original_ids.add(-1)
index = IndexDict(original_ids)


x_data = []
y_data = []
sz_data = []
embedding_data=[]
for key,graph in graphs_train.items():
    #label = labels_train[key].split()
    #print(labels_train[key])
    y = labels_train[key]
    temp = []
    for walk in graph:
        if len(walk) < 10:
            for i in range(10 - len(walk)):
                walk.append(-1)
        temp.append(index.new(walk))
    x_data.append(temp)
    y_data.append(np.log(y+2.0)/np.log(2.0))
    sz_data.append(sizes_train[key])
    embedding_data.append(embedding_train[key])
#pickle.dump((x_data, y_data, sz_data, index.length()), open('D:\\代码\\python\\毕业论文代码\\NI-GP\\data\\data2\\data2_train.pkl','wb'))

pickle.dump((x_data, y_data, sz_data, embedding_data,index.length()), open('D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\train.pkl','wb'))

x_data = []
y_data = []
sz_data = []
embedding_data = []
for key,graph in graphs_val.items():
    #label = labels_val[key].split()
    y = labels_val[key]
    temp = []
    for walk in graph:
        if len(walk) < 10:
            for i in range(10 - len(walk)):
                walk.append(-1)
        temp.append(index.new(walk))
    x_data.append(temp)
    y_data.append(np.log(y+2.0)/np.log(2.0))
    sz_data.append(sizes_val[key])
    #print(embedding_val[key])
    embedding_data.append(embedding_val[key])
#pickle.dump((x_data, y_data, sz_data, index.length()), open('D:\\代码\\python\\毕业论文代码\\NI-GP\\data\\data2\\data2_val.pkl','wb'))

pickle.dump((x_data, y_data, sz_data, embedding_data,index.length()), open('D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\val.pkl','wb'))

x_data = []
y_data = []
sz_data = []
embedding_data=[]
for key,graph in graphs_test.items():
    #label = labels_test[key].split()
    y = labels_test[key]
    temp = []
    for walk in graph:
        if len(walk) < 10:
            for i in range(10 - len(walk)):
                walk.append(-1)
        temp.append(index.new(walk))
    x_data.append(temp)
    y_data.append(np.log(y+2.0)/np.log(2.0))
    sz_data.append(sizes_test[key])
    embedding_data.append(embedding_test[key])
pickle.dump((x_data, y_data, sz_data, embedding_data,index.length()), open('D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\test.pkl','wb'))
#pickle.dump((x_data, y_data, sz_data, index.length()), open('D:\\代码\\python\\毕业论文代码\\NI-GP\\data\\data2\\data2_test.pkl','wb'))
np.random.seed(13)
#with open('D:\\代码\\python\\毕业论文代码\\NI-GP\\data\\data2\\vector.txt', 'r') as f:
with open('D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\vector.txt', 'r') as f:
    line = f.readline()
    temp = line.strip().split()
    num_nodes = int(temp[0])
    num_dims = int(temp[1])
    node_vec = np.random.normal(size=(index.length(), num_dims))
    for i in range(num_nodes):
        line = f.readline()
        temp = line.strip().split()
        #print(temp)
        node_id = int(temp[0])
        if not node_id in original_ids:
            continue

        node_vec[index.new(node_id), :] = np.array([float(temp[j]) for j in range(1, len(temp))])

        
#pickle.dump(node_vec, open('D:\\代码\\python\\毕业论文代码\\NI-GP\\data\\data2\\vector.pkl','wb'))        
pickle.dump(node_vec, open('D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\vector.pkl','wb'))
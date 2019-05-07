# -*- coding: utf-8 -*-
"""
Created on Tue Jan 22 07:37:55 2019

@author: Vimotus
"""

import numpy as np
import os
from optparse import OptionParser
import sys
import networkx as nx
import time
from gensim.models import Word2Vec

walks=list()
rfile=open("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\all_walks.txt",'r')
temp='D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\影响表达\\vector'
temp1='.txt'
t=0

for line in rfile:
    line=line.rstrip('\n')
    #print(type(line))
    #print(line)
    walk_strings=line.split('\t')
    #print(type(walk_strings))
    #print(walk_strings[0])
    del(walk_strings[0])
    #print(walk_strings)
    #print(type(walk_strings))
    #test=" ".join(walk_strings)
    #walks.append(test.split(" "))
    for i in range(0,walk_strings.__len__()):
        walk_str=walk_strings[i][:-1]
        #print(type(walk_str))
        #print(walk_str.split("  "))
        walks.append(walk_str.split())
    if t==0 :
        model=Word2Vec(walks,size=50,window=10,min_count=0, sg=1,workers=5,iter=8)
        t=t+1
    if t>0:
        model.build_vocab(walks,update=True)
        model.train(walks,total_examples=model.corpus_count,epochs=model.iter)
        path=temp+str(t)+temp1 
        t=t+1
        model.wv.save_word2vec_format(path)
    print(t)
#model=Word2Vec.load("D:\\代码\\C\\刘子图\\data\\3200事件数据集\\data\\vector_test.txt")
'''

model.wv.save_word2vec_format("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\vector.txt")
model.save("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\temp.txt")
print("1111")
rfile.close()
temp='D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\影响表达\\vector'
temp1='.txt'
t=0
rfile=open("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\time_val.txt",'r')
for line in rfile:
    walks.clear()
    line=line.rstrip('\n')
    walk_strings=line.split('\t')
    del(walk_strings[0])
    for i in range(0,walk_strings.__len__()):
        walk_str=walk_strings[i][:-1]
        walks.append(walk_str.split())
    model.build_vocab(walks,update=True)
    model.train(walks,total_examples=model.corpus_count,epochs=model.iter)
    path=temp+str(t)+temp1 
    t=t+1
    model.wv.save_word2vec_format(path)
rfile.close()
rfile=open("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\time_test.txt",'r')
for line in rfile:
    walks.clear()
    line=line.rstrip('\n')
    walk_strings=line.split('\t')
    del(walk_strings[0])
    for i in range(0,walk_strings.__len__()):
        walk_str=walk_strings[i][:-1]
        walks.append(walk_str.split())
    model.build_vocab(walks,update=True)
    model.train(walks,total_examples=model.corpus_count,epochs=model.iter)
    path=temp+str(t)+temp1 
    t=t+1
    model.wv.save_word2vec_format(path)'''
# -*- coding: utf-8 -*-
"""
Created on Fri Nov  2 13:09:11 2018

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
rfile=open("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\random_train.txt",'r')
#rfile=open("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\time_train.txt",'r')
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
#model=Word2Vec.load("D:\\代码\\C\\刘子图\\data\\3200事件数据集\\data\\vector_test.txt")
#alpha默认0.025
model=Word2Vec(walks,size=50,window=10,min_count=0, sg=1,workers=5,iter=8)
model.wv.save_word2vec_format("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\vector.txt")
model.save('D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\vector.model')

# -*- coding: utf-8 -*-
"""
Created on Fri Nov  2 19:32:09 2018

@author: Vimotus
"""

# -*- coding: utf-8 -*-
"""
Created on Sun Apr 22 16:12:11 2018

@author: Vimotus
"""

import sys
import numpy as np
import tensorflow as tf
from model import NTEP

import six.moves.cPickle as pickle
import gzip
tf.set_random_seed(0)
import time

NUM_THREADS = 20

tf.flags.DEFINE_float("learning_rate", 0.01,"learning_rate")
tf.flags.DEFINE_integer("sequence_batch_size", 20, "sequence batch size.")
tf.flags.DEFINE_integer("batch_size", 32, "batch size.")
tf.flags.DEFINE_integer("n_hidden_gru", 32, "hidden gru size.")
tf.flags.DEFINE_float("l1", 5e-5, "l1.")
tf.flags.DEFINE_float("l2", 1e-8, "l2.")
tf.flags.DEFINE_float("l1l2", 1.0, "l1l2.")
tf.flags.DEFINE_string("activation", "relu", "activation function.")
tf.flags.DEFINE_integer("n_sequences", 200, "num of sequences.")
tf.flags.DEFINE_integer("training_iters", 50*3200 + 1, "max training iters.")
tf.flags.DEFINE_integer("display_step", 100, "display step.")
tf.flags.DEFINE_integer("embedding_size", 50, "embedding size.")
tf.flags.DEFINE_integer("n_input", 50, "input size.")
tf.flags.DEFINE_integer("n_steps", 10, "num of step.")
tf.flags.DEFINE_integer("n_hidden_dense1", 32, "dense1 size.")
tf.flags.DEFINE_integer("n_hidden_dense2", 16, "dense2 size.")
tf.flags.DEFINE_string("version", "v4", "data version.")
tf.flags.DEFINE_integer("max_grad_norm", 100, "gradient clip.")
tf.flags.DEFINE_float("stddev", 0.01, "initialization stddev.")
tf.flags.DEFINE_float("emb_learning_rate", 5e-03, "embedding learning_rate.")
tf.flags.DEFINE_float("dropout_prob", 1., "dropout probability.")

config = tf.flags.FLAGS

def get_batch(x, y, sz,embedding, step, batch_size=128):
    batch_x = np.zeros((batch_size, len(x[0]), len(x[0][0])))
    batch_y = np.zeros((batch_size, 1))
    batch_sz = np.zeros((batch_size, 1))
    batch_embedding=np.zeros((batch_size,50))
    start = step * batch_size % len(x)
    '''print(len(embedding))
    print(len(y))
    print(len(batch_embedding))
    print(len(batch_y))
    print(type(embedding))'''
    for i in range(batch_size):
        batch_y[i, 0] = y[(i + start) % len(x)]
        batch_sz[i, 0] = sz[(i + start) % len(x)]
        batch_x[i,:] = np.array(x[(i + start) % len(x)])
        batch_embedding[i]=embedding[(i+start)%len(x)]
        #print(len(batch_embedding[i]))
        #print(len(batch_x[i]))
        #print(len(batch_embedding))
        #print(batch_embedding[i])
    #print(batch_embedding)
    #print(len(batch_embedding))
    #print(len(batch_y))
    #print(len(batch_x))
    return batch_x, batch_y, batch_sz, batch_embedding

version = config.version
'''x_train, y_train, sz_train, vocabulary_size = pickle.load(open('D:\\代码\\python\\毕业论文代码\\NI-GP\\data\\data2\\data2_train.pkl','rb'))
x_test, y_test, sz_test, _ = pickle.load(open('D:\\代码\\python\\毕业论文代码\\NI-GP\\data\\data2\\data2_test.pkl','rb'))
x_val, y_val, sz_val, _ = pickle.load(open('D:\\代码\\python\\毕业论文代码\\NI-GP\\data\\data2\\data2_val.pkl','rb'))
node_vec = pickle.load(open('D:\\代码\\python\\毕业论文代码\\NI-GP\\data\\data2\\vector.pkl', 'rb'))'''
x_train, y_train, sz_train, embedding_train,vocabulary_size = pickle.load(open('D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\train.pkl','rb'))
x_test, y_test,sz_test ,embedding_test, _ = pickle.load(open('D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\test.pkl','rb'))
x_val, y_val,sz_val,embedding_val , _ = pickle.load(open('D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\val.pkl','rb'))
node_vec = pickle.load(open('D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\vector.pkl', 'rb'))
#print(x_train)
#print(y_train)
#print(len(sz_train))
#print(embedding_val)
training_iters = config.training_iters
batch_size = config.batch_size
display_step = min(config.display_step, len(sz_train)/batch_size)

  
np.set_printoptions(precision=2)

sess = tf.Session(config = tf.ConfigProto(allow_soft_placement = True))

start = time.time()

model = NTEP(config, sess, node_vec)

step = 1
best_val_loss = 1000
best_test_loss = 1000
# Keep training until reach max iterations or max_try
test_pred=[]
test_sz=[]
train_loss = []
max_try = 10
patience = max_try
i=0
saver = tf.train.Saver(max_to_keep=4)
while step * batch_size < training_iters:
    batch_x, batch_y, batch_sz,batch_embedding = get_batch(x_train, y_train, sz_train, embedding_train,step, batch_size=batch_size)
    '''print("aaaaa")
    print(type(batch_embedding))
    print(batch_sz.size)
    print(len(batch_embedding))'''
    
    model.train_batch(batch_x, batch_y, batch_sz,batch_embedding)
    train_loss.append(model.get_error(batch_x, batch_y, batch_sz,batch_embedding))

    if step % display_step == 0:
        # Calculate batch loss
        val_loss = []
        for val_step in range(int(len(y_val)/batch_size)):
            val_x, val_y, val_sz ,val_embedding= get_batch(x_val, y_val, sz_val,embedding_val, val_step, batch_size=batch_size)

            val_loss.append(model.get_error(val_x, val_y, val_sz,val_embedding))

            #print("111111")
        test_loss = []
        test_pred.clear()
        for test_step in range(int(len(y_test)/batch_size)):
            test_x, test_y, test_sz,test_embedding = get_batch(x_test, y_test, sz_test,embedding_test, test_step, batch_size=batch_size)

            test_loss.append(model.get_error(test_x, test_y, test_sz,test_embedding))

            test_pred.append(model.get_pred(test_x,test_y,test_sz,test_embedding))  
        if np.mean(val_loss) < best_val_loss:
            best_val_loss = np.mean(val_loss)
            best_test_loss = np.mean(test_loss)
            patience = max_try
            saver.save(sess,"D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\热点预测\\model")
        print("#" + str(step/display_step) + 
              ", Training Loss= " + "{:.6f}".format(np.mean(train_loss)) + 
              ", Validation Loss= " + "{:.6f}".format(np.mean(val_loss)) + 
              ", Test Loss= " + "{:.6f}".format(np.mean(test_loss)) + 
              ", Best Valid Loss= " + "{:.6f}".format(best_val_loss) + 
              ", Best Test Loss= " + "{:.6f}".format(best_test_loss)
             )
        train_loss = []
        patience -= 1
        if not patience:
            break
       
    step += 1
 
f=open("D:\\代码\\C\\刘子图\\基于事件相关的影响范围预测\\事件影响表达\\data\\数据集1280\\热点预测\\random_pred.txt","w+")
for i in test_pred:
    f.write(str(i)+'\n')
f.close()

print ("Finished!\n----------------------------------------------------------------")
print ("Time:", time.time()-start)
print ("Valid Loss:", best_val_loss)
print ("Test Loss:", best_test_loss)



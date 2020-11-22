#!/usr/bin/env python3
import matplotlib.pyplot as plt
import numpy as np


def read_file(filename):
    xs = []
    ins_data = []
    del_data = []
    with open(filename) as f:
        xs = [int(x) for x in f.readline().split()]

        while True:
            line = f.readline()
            if not line:
                break
            if line.isspace():
                continue

            ins_block = {'name': line}
            del_block = {'name': line}
            ins_block['data'] = [float(x) for x in f.readline().split()]
            del_block['data'] = [float(x) for x in f.readline().split()]
            
            ins_data.append(ins_block)
            del_data.append(del_block)
    
    return xs, ins_data, del_data

def get_scale(data):
    max_y = 0
    for block in data:
        max_y = max(max_y, max(block['data']))

    return np.ceil(max_y/100)*100

def plot_comp(title, xs, data):
    fig, axs = plt.subplots(1, len(data))
    max_y = get_scale(data)

    fig.suptitle(title)
    for i, (block, ax) in enumerate(zip(data, axs)):
        ax.set_title(block['name'])
        ax.set_xlabel("N")
        ax.set_ylabel("t, нс")

        ax.set_ylim([0, max_y])
        ax.plot(xs, block['data'],'o')
    
    plt.show()
    

xs, ins_data, del_data  = read_file(input())

plot_comp("insertion speed", xs, ins_data)
plot_comp("deletion speed", xs, del_data)

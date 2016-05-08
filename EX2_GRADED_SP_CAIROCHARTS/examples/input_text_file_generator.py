#!/usr/bin/python3
from random import randint
import os,sys

name = None
n = None
limit = None
type = None

if(len(sys.argv) > 1):
    name = sys.argv[1]
if(len(sys.argv) > 2):
    n = int(sys.argv[2])
if(len(sys.argv) > 3):
    limit = int(sys.argv[3])
if(len(sys.argv) > 4):
    type = sys.argv[4]

def increase_sequence_generator(n,range):
    increasingSequence = []
    increasingSequence.append(randint(0,range//n))

    while(n > 1):
        increasingSequence.append(randint(increasingSequence[-1],range//n))
        n -= 1

    return increasingSequence

def positive_sequence_generator(n,range):
    A = []
    while(n > 0):
        A.append(randint(0,range))
        n -= 1

    return A

def x_y_sequence_generator(n,limit):
    A = increase_sequence_generator(n,limit)
    B = positive_sequence_generator(n,limit)
    for i in range(len(A)):
        A[i] = str(A[i]) + ',' + str(B[i])

    return A

def create_input_file(name=None,n=None,limit=None,type=None):
    A = []
    if not name:
        name = "input.txt"
    if not n:
        n = 10
    if not limit:
        limit = 100
    if not type:
        type = 'y'
    format = ".txt"
    if (name.find(format) == -1):
        name += format

    if not os.path.exists(name):
        open(name, 'w')
    file = open(name, 'w')
    if(type == 'y'):
        A = positive_sequence_generator(n,limit)
    elif(type == 'xy'):
        A = x_y_sequence_generator(n,limit)
    for number in A:
        file.write(str(number) + " ")

create_input_file(name=name,n=n,limit=limit,type=type)

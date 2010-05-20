#!/usr/bin/python
# -*- coding: utf-8 -*-

# I Used this with:
# find /home/moshe/Desktop/TEBooks/ -name *Interleave* -exec ./MixedConfs.py {} \;

import sys
import subprocess
import os

f = open (sys.argv[1], "r")

l = f.read().split('\n')

f.close()

bn = ""

fpl = []
ftl = []

j = 0

for i in range(len(l)):
    if l[i].find("FileName=") != -1:
	bn += l[i][10:l[i].find("\"", 10)]
	j = i
	break
	
process = subprocess.Popen(['find', '.', '-name', bn], shell=False, stdout=subprocess.PIPE)
basefile = process.communicate()[0].replace('.txt\n', '.conf')

mynum = bn[1:3]

for i in range(j + 1,len(l)):
    if l[i].find("FileName=") != -1:
	s = l[i][10:l[i].find("\"", 10)]
	process = subprocess.Popen(['find', '.', '-iname', s], shell=False, stdout=subprocess.PIPE)
	q = process.communicate()[0]
	if (q.strip() != ""):
	    fpl.append(q)
	else:
	    s = mynum + "*" + l[i][10:l[i].find("\"", 10)]
	    process = subprocess.Popen(['find', '.', '-iname', s], shell=False, stdout=subprocess.PIPE)
	    fpl.append(process.communicate()[0])
	fpl[len(fpl) -1] = fpl[len(fpl) -1].replace("./", "").replace("\n","")

    if l[i].find("Title=") != -1:
	ftl.append(l[i][7:l[i].find("\"", 7)])
    #print l[i]
#print fpl[1]


st = ""
nf = open(basefile,"r")
l = nf.read().split('\n')
for line in l:
    if line.find("AddSource") == -1:
	st += line + "\n"

nf.close()

nf = open(basefile,"w")
nf.write(st)
for i in range(len(fpl)):
    if fpl[i].strip() != "":
	nf.write("AddSource ")
	nf.write(fpl[i] + ":" + ftl[i] + "\n")
    else:
	print ftl[i] + " Missing!"
nf.close()
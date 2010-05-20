#!/usr/bin/env python
# -*- coding: utf-8 -*-
#	Author - Moshe Wagner <moshe.wagner@gmail.com>

#Make an externall conf file from the confs embedded in the bookfile.
# Used for migrating books from Windows Torat Emet

import sys
import os.path

if len(sys.argv) != 2:
    print "Usage: extractconfs.py [bookfile]"
else:
    filename = sys.argv[1]
    f = open(filename, "r")
    
    outf = open(filename.replace(".txt",".conf"), "w")
    
    fl = ""
    bookname = ""
    
    for line in f:
        if line.startswith("&") == True:
	    fl = line.split("&")
	if line.startswith("$") == True:
	    bookname = line[1:]
	    
    #print fl

    outf.write("DisplayName=" + bookname + "\n")
    for a in fl:
	outf.write(a + "\n")
	
    outf.close()
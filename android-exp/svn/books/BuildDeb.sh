#! /bin/bash

echo "Builds uploadable files for the books (for PPA)"

echo "* Exporting books..."
#Clean folder
rm -fr ../bookstmp

p=`pwd`
cd ..
cp -fr $p bookstmp

cd bookstmp

echo "* Removing junk ..."
find -name ".svn" -exec rm -rf {} \;

echo "* Building makefile ..."
./GenMakeFile.sh

echo "* Building package ..."
dch -imD natty
debuild -S -k2C383923

echo "Now you can upload your files, using:"
echo "dput ppa:moshe-wagner/orayta <source.changes>"
#cd ..
#dput ppa:moshe-wagner/orayta <source.changes>
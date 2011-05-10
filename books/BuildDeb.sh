#! /bin/bash

echo "Builds uploadable files for the books (for PPA)"

echo "* Exporting books..."
#Clean folder
rm -fr ../bookstmp

p=`pwd`
cd ..
cp -fr $p bookstmp

cd bookstmp

find -name ".svn" -exec rm -rf {} \;

./GenMakeFile.sh

dch -imD maverick 
debuild -S -k2C383923

dch -imD natty
debuild -S -k2C383923

dch -imD lucid
debuild -S -k2C383923

dch -imD oneiric
debuild -S -k2C383923

echo "Now you can upload your files, using:"
echo "dput ppa:moshe-wagner/orayta <source.changes>"
#cd ..
#dput ppa:moshe-wagner/orayta <source.changes>
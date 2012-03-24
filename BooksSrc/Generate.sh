#! /bin/bash

echo "Copying books"

outDir="GeneratedBooks"

curdir=`pwd`
cd ..
rm -fr $outDir
cp -r $curdir $outDir
cd $outDir


for f in `find . -iname "*.txt"`
do
    echo "Creating DB for: "$f
    ./orayta -D $f
    ./genBook.sh $f
done


#Change folder's ".conf" files to ".folder" files
for f in `find . -iname "*.conf"`
do
    b=`echo $f | sed 's/[.][/]//g' | sed -e 's/[.].*//g'`
    mv $f $b".folder"
done



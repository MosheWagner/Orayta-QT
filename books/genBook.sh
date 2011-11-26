#! /bin/bash


# for f in `find . -iname "*.txt"`
# do
#    ./genBook.sh $f
# done
# ------>
# for f in `find . -iname "*.txt"`; do ./genBook.sh $f; done



#for f in `find . -iname "*.conf"`
#do
#    b=`echo $f | sed 's/[.][/]//g' | sed -e 's/[.].*//g'`
#    mv $f $b".folder"
#done
# ------>
# for f in `find . -iname "*.conf"`; do b=`echo $f | sed 's/[.][/]//g' | sed -e 's/[.].*//g'`; mv $f $b".folder"; done


#Remove file suffix
base=`echo $1 | sed 's/[.][/]//g' | sed -e 's/[.].*//g'`

echo "Packing: "$base
confile=$base".conf"

mkdir "tmp"

textfle=$base".txt"
mv $textfle "tmp/BookText"

dbfile=$base".TDB"
mv $dbfile "tmp/SearchDB"

lmfile=$base".LMP"
mv $lmfile "tmp/LevelMap"

zipfile=$base".obk"

mv $confile "tmp/Conf"

cd tmp

sed -i 's/.[tT][xX][tT]/.obk/g' "Conf"
#echo "Book's conf entry:"
#cat "Conf"

zip -9 "../"$zipfile "BookText" "SearchDB" "LevelMap" -z < "Conf"  > /dev/null 2>&1

#Clean up
cd ..
rm -fr "tmp"

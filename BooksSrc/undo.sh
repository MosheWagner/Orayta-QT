#! /bin/bash
# converts obk files to their original txt, lmp, tdb and conf files.
# operates on all files in current dir, recursively.







for f in `find . -iname "*.obk"`
do
    echo "deflating: "$f
    base=`echo $f | sed 's/[.][/]//g' | sed -e 's/[.].*//g'`
    
    tmp="tmp"
    
        
    mkdir $tmp
    unzip $f -d "tmp" &> /dev/null
    #read zipnote to file, removing lines that hold unnecceary info.
    zipnote $f |sed 's/^@.*$//'| sed '/^$/d'> $tmp"/conf"
    
    mv $tmp"/BookText" $base".txt"
    mv $tmp"/SearchDB" $base".TDB"
    mv $tmp"/LevelMap" $base".LMP"
    mv $tmp"/conf" $base".conf" 
    
    #cleanup:
    
#    when you're sure it works:
    rm $f
    
    rm -r $tmp
done
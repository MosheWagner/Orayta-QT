#! /bin/bash

#This script should update all needed files that configure the .deb package, and build a new package for you

#Run this script only from the source dir that you want to build the .deb file from

echo "This script should update all needed files that configure Toratemet's .deb package, and build a new package for you"
echo "Internal use only! This should not be used for anythong else, ever!!!"

#update debian it's a new version:
dch -i

#Hack around qmake's bug...
touch Orayta

qmake-qt4

sed -i 's/[-][$][(]DEL_FILE[)] [$][(]TARGET[)]//g' Makefile 
sed -i 's/[-][$][(]DEL_FILE[)] Makefile//g' Makefile 

sed -i 's|[$][(]INSTALL_ROOT[)]/usr/local/bin|$(DESTDIR)/usr/bin|g' Makefile 
sed -i 's|[$][(]INSTALL_ROOT[)]/usr/share/Orayta/|$(DESTDIR)/usr/share/Orayta|g' Makefile 

sed -i 's|[$][(]INSTALL_ROOT[)]/usr/share/applications/|$(DESTDIR)/usr/share/applications|g' Makefile 

sed -i 's|[@][$][(]QMAKE[)] -unix -o Makefile Orayta.pro||g' Makefile 

sed -i 's|Orayta.pro||g' Makefile 

mv Orayta.pro hide.tmp


#Build the .deb package
dpkg-buildpackage

#Clean up
make clean

mv hide.tmp Orayta.pro
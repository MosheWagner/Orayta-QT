#! /bin/bash

echo "Generating makefile"


echo "* Generating Makefile..."
#Generate makefile

mkfile="#AutoBuilt MakeFile. <moshe.wagner@gmail.com>\n"
mkfile=$mkfile"COPY          = cp -fr\n"
mkfile=$mkfile"COPY_FILE     = \$(COPY)\n"
mkfile=$mkfile"COPY_DIR      = \$(COPY) -r\n"
Cmkfile=$mkfile"HK_DIR_EXISTS= test -d\n"
mkfile=$mkfile"MKDIR         = mkdir -p\n"
mkfile=$mkfile"RM		= rm -fr \n\n"

mkfile=$mkfile"clean:"
mkfile=$mkfile"\n\n"
mkfile=$mkfile"distclean:"
mkfile=$mkfile"\n\n"
mkfile=$mkfile"install: install_books"
mkfile=$mkfile"\n\n"
mkfile=$mkfile"uninstall: uninstall_books \n"

il="install_books:\n"
il=$il"\t@\$(CHK_DIR_EXISTS) \$(DESTDIR)/usr/share/Orayta || \$(MKDIR) \$(DESTDIR)/usr/share/Orayta/\n"
il=$il"\t@\$(CHK_DIR_EXISTS) \$(DESTDIR)/usr/share/Orayta/Books/ || \$(MKDIR) \$(DESTDIR)/usr/share/Orayta/Books/\n"

rml="uninstall_books:\n" 

l=$l`find * -name '*_*' -prune | sed 's/\(.*\)/\x27\1\x27/g'`
l=$l" Pics"
for j in $l; 
    do il=$il"\t\$(COPY) "$j" \$(DESTDIR)/usr/share/Orayta/Books/\n";
    rml=$rml"\t\$(RM) \$(DESTDIR)/usr/share/Orayta/Books/"$j"\n";
done

il=$il"\tchmod 777 \$(DESTDIR)/usr/share/Orayta/Books\n"

mkfile=$mkfile"\n\n"$il"\n"$rml

echo -e $mkfile > Makefile
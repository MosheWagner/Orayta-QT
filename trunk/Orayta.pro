# -------------------------------------------------
# Project created by QtCreator 2009-03-15T13:50:06
# Author: Moshe Wagner. <moshe.wagner@gmail.com>
# -------------------------------------------------
QT += webkit
TARGET = Orayta
TEMPLATE = app

!win32 {
    CONFIG += khtml
}

khtml {
    DEFINES += KHTML
}

SOURCES += main.cpp \
    mainwindow.cpp \
    htmlgen.cpp \
    functions.cpp \
    book.cpp \
    bookiter.cpp \
    booklist.cpp \
    addcomment.cpp \
    bookmarktitle.cpp \
    mywebview.cpp \
    mytreetab.cpp \
    about.cpp \
    errorreport.cpp \
    searchform.cpp \
    bookfind.cpp

khtml {
    SOURCES += khtmlwidget.cpp
}

HEADERS += htmlgen.h \
    functions.h \
    book.h \
    bookiter.h \
    booklist.h \
    mainwindow.h \
    addcomment.h \
    bookmarktitle.h \
    mywebview.h \
    mytreetab.h \
    about.h \
    errorreport.h \
    searchform.h \
    bookfind.h

khtml {
    HEADERS += khtmlwidget.h
}

FORMS += mainwindow.ui \
    addcomment.ui \
    bookmarktitle.ui \
    about.ui \
    errorreport.ui \
    searchform.ui \
    bookfind.ui
RESOURCES += Orayta.qrc

win32 {
    RC_FILE = orayta.rc
}


KDEBASE=`kde-config --prefix`
KDELIBS=`kde-config --expandvars --install lib`
KDEINCLUDES=`kde-config --expandvars --install include`
khtml {
    win32 {
        INCLUDEPATH += C:\KDE\include
        INCLUDEPATH += C:\KDE\include\KDE

        LIBS += -LC:\KDE\lib
        LIBS += -LC:\KDE\lib\kde4
        LIBS += -llibkhtml -lkdeui -lkparts -lkdecore
    }
    unix {
       INCLUDEPATH += $KDEBASE/include/kde4 \
                      $KDEBASE/include
       LIBS += -L$KDELIBS

       LIBS += -lkhtml -lkdeui -lkparts -lkdecore
    }
}



TRANSLATIONS = Hebrew.ts \
               French.ts

# Install binary
target.path = /usr/bin

# Install books
books.path = /usr/share/Orayta/
books.files = Books/.

# Install icon
icon.path = /usr/share/Orayta/
icon.files = Icons/Orayta.png

# Desktop shortcut
desktop.path = /home/*/Desktop/
desktop.files = Orayta.desktop

# Install shortcut
menu.path = /usr/share/applications
menu.files = Orayta.desktop

# Install translation
trans.path = /usr/share/Orayta/
trans.files = Hebrew.qm

INSTALLS += target
INSTALLS += books
INSTALLS += icon
INSTALLS += trans

# INSTALLS += desktop
INSTALLS += menu

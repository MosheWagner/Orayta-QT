# -------------------------------------------------
# Project created by QtCreator 2009-03-15T13:50:06
# Author: Moshe Wagner. <moshe.wagner@gmail.com>
# -------------------------------------------------
QT += webkit
TARGET = Orayta
TEMPLATE = app

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
    bookfind.cpp \
    bookdisplayer.cpp \
    bookmark.cpp \
    search.cpp \
    settings.cpp
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
    bookfind.h \
    bookdisplayer.h \
    settings.h
FORMS += \
    mainwindow.ui \
    addcomment.ui \
    bookmarktitle.ui \
    about.ui \
    errorreport.ui \
    bookfind.ui \
    settings.ui
RESOURCES += Orayta.qrc
win32:RC_FILE = orayta.rc

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
#Install wait image
icon.files += Images/Wait.gif

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

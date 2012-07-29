# -------------------------------------------------
# Project created by QtCreator 2009-03-15T13:50:06
# Author: Moshe Wagner. <moshe.wagner@gmail.com>
# -------------------------------------------------

QT += core \
    gui \
    webkit

TARGET = orayta
TEMPLATE = app

CONFIG += poppler


win32{
    INCLUDEPATH +=  $$quote(D:\Qt\4.8.2\src\3rdparty\zlib) \
                    $$quote(D:\Libs\quazip-0.4.4)
    LIBS += "D:\Libs\quazip-0.4.4\quazip\Release\quazip.lib"
} else {
     INCLUDEPATH += "/home/avi/downloads/svn/yoch_desktop/"
    LIBS += "/home/avi/downloads/svn/yoch_desktop/libquazip.so.1.0.0"
}


SOURCES += main.cpp \
    mainwindow.cpp \
    htmlgen.cpp \
    functions.cpp \
    bookiter.cpp \
    addcomment.cpp \
    about.cpp \
    errorreport.cpp \
    bookfind.cpp \
    bookdisplayer.cpp \
    settings.cpp \
    importbook.cpp \
    guematria.cpp \
    bookviewinterface.cpp \
    bookview_orayta.cpp \
    treeitem_base.cpp \
    addbookmark.cpp \
    booktree.cpp \
    bookview_html.cpp \
    mainwnd_bookmark.cpp \
    mainwnd_search.cpp \
    treeitem_directory.cpp \
    treeitem_orayta.cpp \
    treeitem_html.cpp \
    treeitem_base_book.cpp \
    bookview_basehtml.cpp \
    treeitem_link.cpp

HEADERS += htmlgen.h \
    functions.h \
    bookiter.h \
    mainwindow.h \
    addcomment.h \
    about.h \
    errorreport.h \
    bookfind.h \
    bookdisplayer.h \
    settings.h \
    importbook.h \
    guematria.h \
    bookviewinterface.h \
    addbookmark.h \
    booktree.h \
    treeitem_base.h \
    treeitem_directory.h \
    treeitem_html.h \
    treeitem_orayta.h \
    bookview_orayta.h \
    bookview_html.h \
    bookview_basehtml.h \
    treeitem_base_book.h \
    searchresults.h \
    treeitem_link.h

FORMS += \
    mainwindow.ui \
    addcomment.ui \
    about.ui \
    errorreport.ui \
    bookfind.ui \
    settings.ui \
    importbook.ui \
    addbookmark.ui

poppler {
    DEFINES += POPPLER

    SOURCES +=  pdfwidget.cpp \
        bookview_pdf.cpp \
        treeitem_pdf.cpp \

    HEADERS +=  bookview_pdf.h \
        pdfwidget.h \
        treeitem_pdf.h \
}

unix:poppler {
    # The following are the correct include and library paths for poppler on my system (Ubuntu 10.10).
    # Modify these to refer to the directories on your system
    # that contain the poppler-qt4.h header file and [lib]poppler-qt4 library.
    INCLUDEPATH += /usr/include/poppler/qt4
    LIBS += -lpoppler-qt4

    #Fribidi
    LIBS += -lfribidi
}
win32:poppler {
    # The following are the correct include and library paths for poppler on my system (Windows Vista).
    # Modify these to refer to the directories on your system
    # that contain the poppler-qt4.h header file and [lib]poppler-qt4 library.
    INCLUDEPATH += $$quote(D:\Libs\poppler-0.20.1\qt4\src) \
                  $$quote(D:\Libs\fribidi-10.9\include)
    LIBS +=  "D:\Libs\poppler-0.20.1\qt4\src\Release\poppler-qt4.lib" \
             "D:\Libs\fribidi-10.9\lib\libfribidi.dll.lib"
#    LIBS =  "C:\Users\Laurent Picard\Desktop\poppler 1.6.5 - mingw\lib\libpoppler-qt4.dll.a" \
#            "E:\MSVC-libs\fribidi-10.9\lib\libfribidi.dll.a"
}

RESOURCES += Orayta.qrc

win32:RC_FILE = orayta.rc

#QMAKE_CXXFLAGS_DEBUG = DDEBUG

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


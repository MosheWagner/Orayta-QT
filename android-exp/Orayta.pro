# -------------------------------------------------
# Project created by QtCreator 2009-03-15T13:50:06
# Author: Moshe Wagner. <moshe.wagner@gmail.com>
# -------------------------------------------------
QT += webkit
TARGET = orayta
TEMPLATE = app


linux:CONFIG += poppler


poppler {
    DEFINES += POPPLER

    # The following are the correct include and library paths for poppler on my system (Ubuntu 10.10).
    # Modify these to refer to the directories on your system
    # that contain the poppler-qt4.h header file and [lib]poppler-qt4 library.
    INCLUDEPATH += /usr/include/poppler/qt4
    LIBS += -lpoppler-qt4

    #Fribidi
    LIBS += -lfribidi
}



SOURCES += main.cpp \
    mobileapp.cpp \
    desktopapp.cpp \
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
    settings.cpp \
    importbook.cpp \
    guematria.cpp \
    quazip/quazip.cpp \
    quazip/zip.c \
    quazip/unzip.c \
    quazip/quazipnewinfo.cpp \
    quazip/quazipfile.cpp \
    quazip/JlCompress.cpp \
    quazip/quacrc32.cpp \
    quazip/quaadler32.cpp \
    quazip/qioapi.cpp

poppler {
    SOURCES +=
}

HEADERS += \
    mobileapp.h \
    htmlgen.h \
    functions.h \
    book.h \
    bookiter.h \
    booklist.h \
    desktopapp.h \
    addcomment.h \
    bookmarktitle.h \
    mywebview.h \
    mytreetab.h \
    about.h \
    errorreport.h \
    bookfind.h \
    bookdisplayer.h \
    settings.h \
    importbook.h \
    guematria.h \
    quazip/quazip.h \
    quazip/zip.h \
    quazip/unzip.h \
    quazip/quazipnewinfo.h \
    quazip/quazipfileinfo.h \
    quazip/quazipfile.h \
    quazip/quazip_global.h \
    quazip/JlCompress.h \
    quazip/ioapi.h \
    quazip/crypt.h \
    quazip/quacrc32.h \
    quazip/quachecksum32.h \
    quazip/quaadler32.h


FORMS += \
    mobileapp.ui \
    addcomment.ui \
    bookmarktitle.ui \
    about.ui \
    errorreport.ui \
    bookfind.ui \
    settings.ui \
    importbook.ui \
    desktopapp.ui

RESOURCES += Orayta.qrc
win32:RC_FILE = orayta.rc

TRANSLATIONS = Hebrew.ts \
    French.ts

# Install binary
target.path = /usr/bin

# Install books
books.path = /usr/share/Orayta/
books.files = Books/.



# Install translation
trans.path = /usr/share/Orayta/
trans.files = Hebrew.qm
INSTALLS += target
INSTALLS += books
INSTALLS += icon
INSTALLS += trans

# INSTALLS += desktop
INSTALLS += menu

OTHER_FILES += \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/res/drawable-ldpi/icon.png \
    android/res/values-it/strings.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-id/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/drawable/logo.png \
    android/res/drawable/icon.png \
    android/res/values-de/strings.xml \
    android/res/values-es/strings.xml \
    android/res/drawable-mdpi/icon.png \
    android/res/values-ms/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/drawable-hdpi/icon.png \
    android/res/values-fa/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ro/strings.xml \
    android/AndroidManifest.xml

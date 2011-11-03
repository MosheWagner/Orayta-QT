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
    settings.cpp \
    importbook.cpp \
    guematria.cpp \

poppler {
    SOURCES += pdfwidget.cpp
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
    bookfind.h \
    bookdisplayer.h \
    settings.h \
    importbook.h \
    guematria.h \

poppler {
    HEADERS += pdfwidget.h
}

FORMS += \
    mainwindow.ui \
    addcomment.ui \
    bookmarktitle.ui \
    about.ui \
    errorreport.ui \
    bookfind.ui \
    settings.ui \
    importbook.ui
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

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/src/eu/licentia/necessitas/mobile/QtLocation.java \
    android/src/eu/licentia/necessitas/mobile/QtFeedback.java \
    android/src/eu/licentia/necessitas/mobile/QtSystemInfo.java \
    android/src/eu/licentia/necessitas/mobile/QtMediaPlayer.java \
    android/src/eu/licentia/necessitas/mobile/QtSensors.java \
    android/src/eu/licentia/necessitas/mobile/QtAndroidContacts.java \
    android/src/eu/licentia/necessitas/mobile/QtCamera.java \
    android/src/eu/licentia/necessitas/ministro/IMinistroCallback.aidl \
    android/src/eu/licentia/necessitas/ministro/IMinistro.aidl \
    android/src/eu/licentia/necessitas/industrius/QtSurface.java \
    android/src/eu/licentia/necessitas/industrius/QtLayout.java \
    android/src/eu/licentia/necessitas/industrius/QtApplication.java \
    android/src/eu/licentia/necessitas/industrius/QtActivity.java \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/drawable-hdpi/icon.png \
    android/res/values/libs.xml \
    android/res/values/strings.xml

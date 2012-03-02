# -------------------------------------------------
# Project created by QtCreator 2009-03-15T13:50:06
# Author: Moshe Wagner. <moshe.wagner@gmail.com>
# -------------------------------------------------
QT += webkit network
TARGET = orayta
TEMPLATE = app
INCLUDEPATH += $$PWD

    #for test only
    DEFINES += MOBILE

android {
#    INSTALL_PATH = "/sdcard/Orayta/"
#    INSTALL_BOOKS_PATH = "/sdcard/Orayta/Books/"
    INSTALL_PATH = "/assets/Orayta/"
#    INSTALL_BOOKS_PATH = $${INSTALL_PATH}"Books/"
    INSTALL_BOOKS_PATH = $${INSTALL_PATH}

    #this is a mobile app
    #DEFINES += MOBILE
}
else: win32{
    INSTALL_PATH = quote(!:\\progarm files\\orayta\\) #TODO: set the root dynamicly or ask yoch to fix this.
    INSTALL_BOOKS_PATH = quote(!:\\progarm files\\orayta\\books)
}
else:unix{
    CONFIG += poppler \
        linux
    INSTALL_PATH = /usr/share/Orayta/
    INSTALL_BOOKS_PATH = $${INSTALL_PATH}"Books/"

    #Zlib
    LIBS += -lz

    #set a test mobile version
    #DEFINES += MOBILE_TEST
}


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
    quazip/qioapi.cpp \
    QKinetic/qtscrollevent.cpp \
    QKinetic/qtscrollerproperties.cpp \
    QKinetic/qtscrollerfilter.cpp \
    QKinetic/qtscroller.cpp \
    QKinetic/qtflickgesture.cpp \
    pdfwidget.cpp \
    filedownloader.cpp \
    QKinetic/flickcharm.cpp

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
    quazip/quaadler32.h \
    QKinetic/qtscrollevent_p.h \
    QKinetic/qtscrollevent.h \
    QKinetic/qtscrollerproperties_p.h \
    QKinetic/qtscrollerproperties.h \
    QKinetic/qtscrollerfilter_p.h \
    QKinetic/qtscroller_p.h \
    QKinetic/qtscroller.h \
    QKinetic/qtflickgesture_p.h \
    pdfwidget.h \
    search.h \
    filedownloader.h \
    QKinetic/flickcharm.h

FORMS += \
    mobileapp.ui \
    addcomment.ui \
    bookmarktitle.ui \
    about.ui \
    errorreport.ui \
    bookfind.ui \
    settings.ui \
    importbook.ui \
    desktopapp.ui \

RESOURCES += Orayta.qrc \

win32:RC_FILE = orayta.rc

TRANSLATIONS = Hebrew.ts \
    French.ts

# Install binary
target.path = /usr/bin

# Install books
books.path = $${INSTALL_BOOKS_PATH}
books.files = Books/.
#android: books.files += android/res/raw/.
#android: books.files += android/assets/.

# Install icon
icon.path = $${INSTALL_PATH}

icon.files = Icons/Orayta.png
#Install wait image
icon.files += Images/Wait.gif

andrid {
} else : linux {
# Desktop shortcut
desktop.path = /home/*/Desktop/
desktop.files = Orayta.desktop

# Install shortcut
menu.path = /usr/share/applications
menu.files = Orayta.desktop
}

# Install translation
trans.path = $${INSTALL_PATH}
trans.files = Hebrew.qm

# install fonts
#fonts.path = $${INSTALL_PATH}"fonts/"
fonts.path = $${INSTALL_PATH}
fonts.files = fonts/.

# copy licence info
licence.path = $${INSTALL_PATH}
licence.files = licence/.

INSTALLS += target
INSTALLS += books
message("Books target set to:" $${INSTALL_BOOKS_PATH})
INSTALLS += icon
INSTALLS += trans
#fonts are now in the qrc file, no need for them in assets.
INSTALLS += fonts
INSTALLS += licence

# INSTALLS += desktop
linux: INSTALLS += menu


OTHER_FILES += \
    android/AndroidManifest.xml \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/res/values-ja/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable/logo.png \
    android/res/drawable/icon.png \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-it/strings.xml \
    android/res/drawable-mdpi/icon.png \
    android/res/drawable-hdpi/icon.png \
    android/res/values-ms/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-fr/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/src/org/kde/necessitas/origo/CopyResources.java \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/origo/QtApplication.java \
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
    android/AndroidManifest.xml \
    android/AndroidManifest.xml \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/res/values-ja/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable/logo.png \
    android/res/drawable/icon.png \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-it/strings.xml \
    android/res/drawable-mdpi/icon.png \
    android/res/drawable-hdpi/icon.png \
    android/res/values-ms/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-fr/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/src/org/kde/necessitas/origo/CopyResources.java \
    android/AndroidManifest.xml \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/res/values-ja/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable/logo.png \
    android/res/drawable/icon.png \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-it/strings.xml \
    android/res/drawable-mdpi/icon.png \
    android/res/drawable-hdpi/icon.png \
    android/res/values-ms/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-fr/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-zh-rTW/strings.xml

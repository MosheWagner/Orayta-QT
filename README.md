# Orayta-QT

Orayta is an open-source, cross platform port to the "תורת אמת" project.
( http://www.toratemetfreeware.com/ ) - http://orayta.googlecode.com/

The project intends to allow free and simple access to Jewish sources, from the Bible, Mishna and Talmud, to books written in the last years.

This port was written by Moshe Wagner (moshe.wagner@gmail.com), and is licensed under GPLv2.
The books are the basically the same ones used in the original project - And are licensed (mainly) under the CC license.

See the attached "License.txt" for detailed license specifications.

Bug reports, ideas, and comments are greatly welcome at -
moshe.wagner@gmail.com

# Installing

This repository contains both the project source code and an excutable suitable for debian and its derivatives, giving you the choice of either building the program from the source code or attempting to use the pre-compiled binary.

**Note: Currently, the pre-compiled binary suffers from some [issues](https://github.com/MosheWagner/Orayta-QT/issues/45).**

## Installing from source

### Pre-requisites

You may need to first install several development packages using your distribution's package manager.

1. Basic development packages:

   1.1 `build-essential` or whatever else your distribution uses to supply basic compilers, linkers and make utilities.

   1.2 QT build packages: `qt4-qmake`

2. Qt development libraries: These are usually something like `libqt4-dev` and `libqtwebkit-dev`.

3. Bidi support: `libfribidi-dev`.

4. Poppler: `libpoppler-qt4-dev`.


### Performing the install

**Note: There are currently [issues](https://github.com/MosheWagner/Orayta-QT/issues/45) with running orayta under Qt5, so if you have both Qt4 & Qt5 installed, make sure to use Qt4 below, e.g. by using `qmake-qt4` instead of `qmake`.**

To compile and install under linux, run from the main folder of the project:
```
$ qmake
$ make
```

### Installing system-wide

Perform the following, as root:
```
# make install
```
### Installing the books

If the books were downaloaded separately, you should run (as root) in their folder:
```
# make install
```
Or unpack them to this folder before installing the program.

## Moshe Wagner's note
I was successful compiling the code under windows using QT Creator only. Anyway, the simplest way would be to download the binaries from the site. 

If you downloaded the source code without the books, the books must be downloaded separately and extracted to a folder named "Books" in the main folder of the project.

Please report to me on successes or failures.

## Boruch Baum's note (2018-06)

I was successful in installing the pre-compiled binary in a debian 9 system, using the following procedure, as root:

### 1. Create the following script as `/usr/bin/orayta`:

```
#!/bin/sh
cd /usr/share/orayta
orayta
cd -
```

This is necessary in order to resolve a (temporary) bug that has the executable expect the root book directory to be either in the user's home directory or the executable's directory. This same bug will require us to install the executable in `/usr/share/orayta` instead of somewhere "normal". Don't forget to make the script executable:

```
# chmod 755 /usr/bin/orayta
```

### 2. `cd` to the base directory of the project

### 3. Perform the following
```
mkdir -p /usr/share/orayta/Books
install debian/orayta/usr/bin/orayta /usr/share/orayta/
cp debian/orayta/usr/share/applications/Orayta.desktop /usr/share/applications/orayta.desktop
chmod 644 /usr/share/applications/orayta.desktop
```

### 4. Install books

The package `orayta-books` is quite large, so if you would like to evaluate the program on a set of books that you have unwittingly already downloaded, perform the following (as root, from the project's base directory):

```
cp -r android-orayta/assets/Orayta/Books /usr/share/orayta/
find /usr/share/orayta/ -type d -exec chmod 755 '{}' \;
find /usr/share/orayta/ -type f -exec chmod 644 '{}' \;
```

That will give you a set of Tanach to use.

### 5. Install fonts

If you don't already have fonts with nikud etc., you can find a selection in the project's `fonts` directory. It should be sufficient to copy them to `/usr/share/fonts`, but it would be better to first see if your distribution already packages those fonts (which is the case for debian).

# אורייתא

אורייתא הינו הסבה פתוחה, שרצה על כל מערכת הפעלה - לתוכנה המצויינת "תורת אמת".
"תורת אמת" היא תוכנה לצפייה במקורות יהודיים במחשב, החל מהתנ"ך המשנה והתלמוד, ועד לספרים בני ימינו.

ההסבה כוללת את ממשק המשתמש ומנוע הרינדור של הטקסטים בלבד, כאשר הטקסטים עצמם נשארו כמעט בדיוק כפי שהם בפרוייקט המקורי, אותו ניתן למצוא כאן:
http://www.oraytafreeware.com/

הסבה זו של התוכנה נכתבה ע"י משה וגנר
(moshe.wagner@gmail.com)
ומשחוררת תחת רשיון הקוד הפתוח GPLv2.

הספרים מהפרוייקט המקורי הינם תחת רשיון Creative Commons.

## התקנה מקוד מקור (תחת לינוקס) :
כדי לקמפל התוכנה מקוד המקור, יש לוודא שחבילות הפיתוח של qt4 מתוקנות.
(בדר"כ הם נמצאות בחבילות בשם  'qtbase4-dev' ו- 'libqt4webkit5-devel')
(כמו כן צריך להתקין libfribidi-dev וגם libpoppler-qt5-dev
בנוסף, החבילות make ו g++ נצרכות לכל התקנה של תוכנה מבוססת c++, ורוב הסיכויים שהן כבר מותקנות אצלכם)

יש להריץ בטרמינל מתוך התקייה של התוכנה את הפקודה:
```
$ qmake
$ make
```
בשלב זה ניתן לבדוק שהתכנה רצה טוב על ידי הפקודה הבאה (רק במידה שכבר התקנתם את הספרים):
./orayta
וכמשתמש על (באובונטו sudo):
```
# make install
```
אם הספרים הורדו בנפרד, יש להריץ מתוך התקייה שלהם (כמשתמש על):
```
# make install
```
או לפרוס אותם לתיקיה הנוכחית לפני התקנת התוכנה.


תחת חלונות הצלחתי לקמפל את התוכנה רק בעזרת QT Creator.
בכל מקרה, לחלונות, עדיף להוריד את תוכנת ההתקנה מהאתר ולהשתמש בה.


## Donation

If you'd like to support Orayta, please donate!

<a href="https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=P8RH8U6ABNJ38"><img src="https://www.paypalobjects.com/en_US/i/btn/btn_donate_LG.gif" alt="[paypal]" /></a>




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

## Ubuntu installation using Orayta's PPA:

This is the simplest way to install Orayta. It was tested on Ubuntu 19.04 WSL2(!), but should work for all recent or LTS ubuntu versions.

1. sudo add-apt-repository ppa:moshe-wagner/orayta
2. sudo apt update
3. sudo apt install orayta orayta-books

After that you can simply run orayta from the command line.

## Installing from source

### Pre-requisites

You may need to first install several development packages using your distribution's package manager.

1. Basic development packages:

   1.1 `build-essential` or whatever else your distribution uses to supply basic compilers, linkers and make utilities.

   1.2 QT build packages: `qt5-qmake`

   1.3 Zlib (`libz-dev`)

2. Qt development libraries: These are usually something like `qtbase5-dev` and `libqt5webkit5-dev`.

3. Bidi support: `libfribidi-dev`.

4. Poppler: `libpoppler-qt5-dev`.

### Performing the install 

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
Or unpack them to this folder (or /usr/share/Orayta/Books).

## Moshe Wagner's note

I was successful compiling the code under windows using QT Creator only. Anyway, the simplest way would be to download the binaries from the site. 

If you downloaded the source code without the books, the books must be downloaded separately and extracted to a folder named "Books" in the main folder of the project.

Please report to me on successes or failures.

## Windows binary

See info here: https://github.com/MosheWagner/Orayta-QT/wiki/Windows-Version

# אורייתא

אורייתא הינו הסבה פתוחה, שרצה על כל מערכת הפעלה - לתוכנה המצויינת "תורת אמת".
"תורת אמת" היא תוכנה לצפייה במקורות יהודיים במחשב, החל מהתנ"ך המשנה והתלמוד, ועד לספרים בני ימינו.

ההסבה כוללת את ממשק המשתמש ומנוע הרינדור של הטקסטים בלבד, כאשר הטקסטים עצמם נשארו כמעט בדיוק כפי שהם בפרוייקט המקורי, אותו ניתן למצוא כאן:
http://www.oraytafreeware.com/

הסבה זו של התוכנה נכתבה ע"י משה וגנר
(moshe.wagner@gmail.com)
ומשחוררת תחת רשיון הקוד הפתוח GPLv2.

הספרים מהפרוייקט המקורי הינם תחת רשיון Creative Commons.

## התקנה על אובונטו

הדרך הקלה ביותר להתקין את אורייתא על אובונטו היא פשוט להריץ את הפקודות הבאות:

1. sudo add-apt-repository ppa:moshe-wagner/orayta
2. sudo apt update
3. sudo apt install orayta orayta-books

ומשם ניתן להריץ את אורייתא בקלות משורת הפקודה ע"י הפקודה "orayta"


## התקנה מקוד מקור (תחת לינוקס) :
כדי לקמפל התוכנה מקוד המקור, יש לוודא שחבילות הפיתוח של qt5 מתוקנות.
(בדר"כ הם נמצאות בחבילות בשם  'qtbase5-dev' ו- 'libqt5webkit5-dev')
(כמו כן צריך להתקין libfribidi-dev וגם libpoppler-qt5-dev, libz-dev
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

## התקנה בחלונות
ניתן למצוא מידע בנושא כאן: https://github.com/MosheWagner/Orayta-QT/wiki/Windows-Version


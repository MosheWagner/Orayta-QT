<div dir='rtl'>

<br>
<br>
<BR><br>
<br>
<br>
<br>
<BR><br>
<br>
<br>
<br>
<BR><br>
<br>
<br>
<br>
<BR><br>
<br>
<br>
<br>
<h1>התקנת אורייתא בלינוקס</h1>

ניתן להתקין את אורייתא בכמה דרכים.<br>
<br>
<br>
<BR><br>
<br>
<br>
<br>
<br>
<h3>1. התקנה ממאגר האובונטו של התוכנה:</h3>
<i>עובד רק על אובונטו, החל מגרסא <b>8.10 .</b></i>
<br>
<br>
<BR><br>
<br>
<br>
<br>
נוסיף את המאגר למערכת עם הפקודה הבאה: <br>
<br>
<BR><br>
<br>
<br>
<pre><code>sudo add-apt-repository ppa:moshe-wagner/orayta<br>
</code></pre>
<br>
<br>
<BR><br>
<br>
<br>
לאחר עדכון המאגרים עם הפקודה:<br>
<br>
<BR><br>
<br>
<br>
<pre><code>sudo apt-get update<br>
</code></pre>

נתקין את התוכנה עם הפקודה: <br>
<br>
<BR><br>
<br>
<br>
<pre><code>sudo apt-get install orayta orayta-books --no-install-recommends<br>
</code></pre>

מכאן ואילך, כל עדכון של המערכת גם יעדכן את התוכנה (אם יש לה עדכון, כמובן).<br>
<br>
<br>
<BR><br>
<br>
<br>
<br>
<BR><br>
<br>
<br>
<h4>מאגר לגירסת בטא</h4>
<i>ניתן גם להתקין את אורייתא ממאגר אוטומטי שמתעדכן בכל פעם שנעשה שינוי בקוד של התוכנה.(היתרון הוא שאתם תמיד מעודכנים בגירסא הכי חדשה, החיסרון הוא שיכולות להיות לגרסאות הללו יותר באגים)</i>

<BR>

<br>
כדי לעשות זאת נתקין עם ההוראות שלעיל, אלא שנחליף את השורה:<br>
<br>
<BR><br>
<br>
<br>
<pre><code>sudo add-apt-repository ppa:moshe-wagner/orayta<br>
</code></pre>
<br>
<br>
<BR><br>
<br>
<br>
בשורה הבאה:<br>
<br>
<BR><br>
<br>
<br>
<pre><code>sudo add-apt-repository ppa:moshe-wagner/orayta-daily<br>
</code></pre>
<br>
<br>
<BR><br>
<br>
<br>
<br>
<h3>2. התקנה מחבילות:</h3>

אפשר להוריד את החבילות מהמאגר<br>
<a href='https://launchpad.net/~moshe-wagner/+archive/orayta'>https://launchpad.net/~moshe-wagner/+archive/orayta</a>
ולהתקין אותם בעזרת הפקודה: <br>
<br>
<BR><br>
<br>
<br>
<pre><code>sudo dpkg -i *.deb<br>
</code></pre>
<br>
<br>
<BR><br>
<br>
<br>
כאשר אתם בתוך התיקיה אליה הורדתם את החבילות.<br>
<br>
<BR><br>
<br>
<br>
לחלופין אפשר להתקין את החבילות גם בעזרת תוכנה גרפית. באובונטו זה נעשה פשוט על ידי לחיצה כפולה על החבילה.<br>
<br>
<BR><br>
<br>
<br>
<br>
<h3>3. התקנה מקוד המקור של התוכנה:</h3>

צריך להוריד את כל הקוד של התכנה מכאן:<br>
<a href='http://orayta.googlecode.com/svn/trunk/'>http://orayta.googlecode.com/svn/trunk/</a>
ואת הספרים מכאן:<br>
<a href='http://orayta.googlecode.com/svn/books/'>http://orayta.googlecode.com/svn/books/</a>
ולעבוד בעזרת ההוראות שבקבצי readme.<br>
<br>
<br>
<BR><br>
<br>
<br>
<h1>התקנת אורייתא בחלונות</h1>

פשוט להוריד את קובץ הexe ולהפעיל אותו.<br>
<br>
מי שחסר לו Microsoft Visual C++ 2008 SP1 Redistributable Package (x86)<br>
יכול להוריד אותו מכאן : <a href='http://www.microsoft.com/download/en/details.aspx?id=5582'>http://www.microsoft.com/download/en/details.aspx?id=5582</a>

<h1>התקנת אורייתא במק</h1>

אין עדיין הוראות מדויקות, קיימת גירסא נסיונית בלבד. באופן עקרוני מה שצריך לעשות הוא:<br>
<br>
להוריד את הקובץ orayta.zip מדף ההורדות ולחלץ את הקבצים מתוכו.<br>
<br>
כנראה שצריך להתקין ספריות של QT. - ניתן להוריד מהאתר של QT או בכתובת הבאה: <a href='http://qt.nokia.com/downloads/qt-for-open-source-cpp-development-on-mac-os-x'>http://qt.nokia.com/downloads/qt-for-open-source-cpp-development-on-mac-os-x</a>

להריץ את הקובץ orayta בתיקיה שחילצתם מקובץ הzip.<br>
<br>
נא לדווח על כל הצלחה או כשלון בשימוש בגירסת מק ל izar00@gmail.com
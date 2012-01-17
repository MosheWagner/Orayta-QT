/* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2
* as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
* Author: Moshe Wagner. <moshe.wagner@gmail.com>
*/

#include "functions.h"

#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QObject>


using namespace std;

//Reads the file with the given name, and inserts it's contents into the given vector
bool ReadFileToList(QString filename, QList <QString>& text, const char* encoding_name, bool skipconflines)
{
    bool conflinesended = false;
//    qDebug() << "ReadFileToList(" << filename << ")";
    //Stop if it's not a valid file:
    QFileInfo fi(filename);
    if ( fi.isDir() || !fi.exists() )
        return false;

    //Read the source file:
    QFile infile(filename);
    // Open the file
    if ( !infile.open(QIODevice::ReadOnly) )
    {
        qDebug() << "cant open file:"  << filename;
        return false;
    }

    // Set the stream to read from the file
    QTextStream t( &infile );
    t.setCodec(QTextCodec::codecForName(encoding_name));
    while (!t.atEnd())
    {
        QString line = t.readLine();
        // skip the first lines in a file if they start with & or //.
        if ( skipconflines && !conflinesended )
        {
            if ( !line.startsWith("&") && !line.startsWith("//") && !line.isEmpty() )
            {
                conflinesended = true;
                text << line;
            }
        }
        else if (!line.isEmpty())
        {
            text << line;
        }
    }
    infile.close();

    return true;
}


//Reads the comment file into two vectors, one for the titles and one for the texts.
// If a book's id is given, only comments for that book are put into the vector, with the "id:" part of the line choped.
bool ReadCommentFile(QString path, vector<QString>& titles, vector<QString>& texts, const char* encoding_name, int id)
{
    QList <QString> text;
    if (ReadFileToList(path, text, encoding_name))
    {
        titles.clear();
        texts.clear();
        for (int i=0; i<text.size(); i+=2)
        {
            if (id == -1)
            {
                titles.push_back(text[i]);
                texts.push_back(text[i+1]);
            }
            else
            {
                int p = text[i].indexOf(":");
                QString t_id = text[i].mid(0,p);
                if (t_id == QString::number(id))
                {
                    QString title = text[i].mid(p+1);
                    titles.push_back(title);
                    texts.push_back(text[i+1]);
                }
            }
        }
        return true;
    }
    else return false;
}

QString readfile(QString filename, const char* encoding_name)
{
    //Stop if it's not a valid file:
    QFileInfo fi(filename);
    if ( fi.isDir() || !fi.exists() )
    {
        //qDebug() << "wrong file: " << filename;
        return "";
    }

    //Read the source file:
    QFile infile(filename);
    // Open the file
    if ( !infile.open(QIODevice::ReadOnly) )
    {
        qDebug() << "cant open file:"  << filename;
        return "";
    }

    // Set the stream to read from the file
    QTextStream t( &infile );
    t.setCodec(QTextCodec::codecForName(encoding_name));

    return t.readAll();
}

//Writes the given data to the given file path
void writetofile(QString filename, const QString& data, const char* encoding_name, bool overwrite)
{
    //Open given file
    QFile outfile(filename);
    //Set a TextStream to it
    QTextStream outstream( &outfile );
    //Set incoding to the one sent to the function
    outstream.setCodec(QTextCodec::codecForName(encoding_name));

    QIODevice::OpenMode mode;
    if (overwrite == false) mode = QIODevice::WriteOnly | QIODevice::Append;
    else mode = QIODevice::WriteOnly;

    //Write the data to the file
    if ( outfile.open(mode) )
        outstream << data;

    outfile.close();
}

//Split given string by given seperater into the given vector, but allways to two
// (split by first appearance of the seperator)
QStringList splittotwo(const QString& str, const QString& separator) //will split a string into 2
{
    QStringList split;
    split << str.section(separator, 0, 0) << str.section(separator, 1);
    return split;
}

//Sets the given int to the value represented by the string
//  Returns if it failed or succedeed
bool ToNum(QString str, int *out)
{
    bool ok;
    *out = str.toInt(&ok);
    return ok;
}


//Returns the Gematria of the given string
int GematriaValueOfString (const QString& str)
{
    int gematria = 0;
    for(int i=0; i<str.length(); i++)
        gematria += GematriaOfChar(str[i]);

    return gematria;
}

//Returns the gematria of the single given hebrew char
inline int GematriaOfChar(const QChar ch)
{
    QString hchars = "אבגדהוזחטיךכלםמןנסעףפץצקרשת";

    int vals[] = {0,1,2,3,4,5,6,7,8,9,10,20,20,30,40,40,50,50,60,70,80,80,90,90,100,200,300,400};
    return vals[hchars.indexOf(ch)+1];

    /*
    //Use binary search - hchars must be sorted
    int _vals[] = {1,2,3,4,5,6,7,8,9,10,20,20,30,40,40,50,50,60,70,80,80,90,90,100,200,300,400,0};
    int pos = distance(hchars.begin(), qBinaryFind(hchars.begin(), hchars.end(), ch));
    return _vals[pos];
    */
}


//Returns a hebrew string representing the given gematria value
// You can tell it weather to add quote signs (Geresh or Gershaiim) or not
QString NumberToGematria (int num, bool addquotes)
{
    QString hundredChars[] = {"ק","ר","ש","ת"};
    QString tenChars[] = {"י","כ","ל","מ","נ","ס","ע","פ","צ"};
    QString oneChars[] = {"א","ב","ג","ד","ה","ו","ז","ח","ט"};

    QString str = "";

    while (num > 400)
    {
        str += "ת";
        num = num - 400;
    }

    int hundreds = (int)(num / 100);
    num = num % 100;

    if (hundreds > 0 ) str += hundredChars[hundreds - 1];

    if (num == 16) str += "טז";
    else if (num == 15) str += "טו";
    else
    {
        int tens = (int)(num / 10);
        num = num % 10;
        if (tens > 0) str += tenChars[tens-1];

        if (num > 0) str += oneChars[num -1];
    }

    if (addquotes)
    {
        // '
        if (str.length() == 1)
            str += "'";
        // ""
        else
        {
            QChar c = str[str.length() - 1];
            str[str.length() - 1] = '\"';
            str += c;
        }
    }

    return str;
}

//Returns gmara page name of the given page number (From 4, which is Bet Amud Alef)
QString GmaraPage(int num)
{
    QChar amud[2] = {'.',':'};
    return NumberToGematria(num / 2, false) + amud[num%2];
}

//Fix file path to the correct way they should be,
// By switch the "\"'s to "/", so POSIX os's can deal with it (But will still work on MS systems)
// And replacing the relative path's to the installed ones, if available
QString absPath(QString str)
{
    //Make into full path
    return QFileInfo(str).absoluteFilePath();
}

// Returns the given string without it's "(", ")", "{", and "}".
QString RemoveBrackets(QString str)
{
    return str.replace("{", "").replace("}", "").replace("(", "").replace(")", "");
}

//BEWARE: Ugly hack !
//Because for some stupid reason QT can't handle internal html links with Hebrew chars, this function
// (which is called by the itr's toString), converts any given QString to a string representing each char's
// Base64 value, (seperated by 'U's), thus giving a valid digit-or-english-letter-only string.

//Thanks to Josh Rosenfeld for making me do this correctly :-D

QString escapeToBase64(const QString& str)
{
    return QString::fromAscii(str.toUtf8().toBase64());
}

QString unescapeFromBase64(const QString& str)
{
    return QString::fromUtf8(QByteArray::fromBase64(str.toAscii()).constData());
}

//Gets a line in the form of foo=bar,
// and iserts bar to the pointed integer (if it's a valid integer)
// (Otherwise the value isn"t changed)
void GetIntValue (const QString& valueline, int * pVar)
{
    //Split along the = sign
    QStringList parts = splittotwo(valueline, "=");

    if (parts[1] != "")
        ToNum(parts[1], pVar);
}

//Removes any sign from the given unicode string
// (Nikud or teamim)
QString removeSigns(const QString& str)
{
    return removeNikud(removeTeamim(str));
}

//Removes any nikud sign from the given unicode string
QString removeNikud( const QString& str )
{
    //Make sure nikud is in the normall format:
    // QString::normalized(QString::NormalizationForm_D) - Means nikud is allways treated as two chars
    // See: http://www.unicode.org/reports/tr15/#Norm_Forms
    QString s = str.normalized(QString::NormalizationForm_D);

    //These are all of the nikud signs besides the teamim ones mixed in them
    QString pattern = QString("[") + QChar(0x05B0) + "-" + QChar(0x05BC) +
            QChar(0x05C1) + QChar(0x05C2) + QChar(0x05C4) + QChar(0x05C5) + QChar(0x05C7) +
            "]";
    QRegExp regexp(pattern);

    return s.replace(regexp, "");
}

//Removes any teamim sign from the given unicode string
QString removeTeamim(const QString& str)
{
    QString s = str.normalized(QString::NormalizationForm_D);
    //QChar(0x0591) - HEBREW ACCENT ETNAHTA
    //QChar(0x05AF) - HEBREW MARK MASORA CIRCLE
    //QChar(0x05C0) - HEBREW PUNCTUATION PASEQ
    //QChar(0x05C6) - HEBREW PUNCTUATION NUN HAFUKHA
    //QChar(0x05BD) - HEBREW POINT METEG (?) -> nikud ?
    QString pattern = QString("[") + QChar(0x0591) + '-' +  QChar(0x05AF) +
            //Include the PUNCTUATION signs
            QChar(0x05C0) + QChar(0x05C6) + QChar(0x05BD) +
            QChar(0x05F3) + QChar(0x05F4) +
            "]";

    QRegExp regexp(pattern);

    //QChar(0x05C3) - HEBREW PUNCTUATION SOF PASUQ
    //QChar(0x05BE) - HEBREW PUNCTUATION MAQAF
    //Replace sof pasuk with ":" and makaf with " "
    return s.replace(QChar(0x05C3), ":").replace(QChar(0x05BE), " ").replace(regexp, "");
}

QString allowNikudAndTeamim( const QString& str )
{
    //TODO: remove makaf?

    //These are all of the nikud and teeamim signs
    QString nikudchars = QString(QChar(0x0591)) + "-"  + QChar(0x05C7);

    //Build a RegExp from the search term, allowing nikud and teamin between every letter
    QString pat = "";
    for (int i=0; i < str.length(); i++)
    {
        pat += str[i];
        pat += "[" + nikudchars+  "]*";
    }

    return pat;
}

QRegExp withNikudAndTeamim( const QString& str )
{
    return QRegExp("(" + allowNikudAndTeamim(str) + ")");
}

/*
//Returns a regexp pattern all strings that are the same as the given string,
//  ignoring differences in the ו-s and י-s they have.
QString AllowKtivMaleh(QString str)
{
    QString exp;
    //Remove ו-s and י-s, because they become optional now
    str = str.replace("ו","");
    str = str.replace("י","");
    for (int i=0; i<str.size()-1; i++)
    {
        //Allow any number (including zero) of ו and י after every char in the string
        exp += str.at(i) + QString("[וי]*");
    }
    exp += str[str.size()-1];

    return exp;
}
*/

QString AllowKtivHasser(const QString& str)
{
    QString s = str;
    return s.replace("ו","ו?").replace("י","י?");
}

/*
QString pluginPage(QString title)
{
    QString html =  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"\n"
                    "\"http://www.w3.org/TR/html4/loose.dtd\">\n"
                    "<html >\n<head>\n\t"
                    "<meta http-equiv=Content-Type content=\"text/html; charset=UTF-8\">\n"
                    "<title>" + title + "</title>\n"
                    "</head>\n";

    html +=         "<script type=\"text/javascript\">\n";

    html +=         "function availablePlugins() {"
                    "  document.writeln('<table border=\"1\">');"
                    "  for(var i=0; i<navigator.mimeTypes.length; i++) {"
                    "    document.writeln('<tr>');"
                    "    document.writeln('<td>' + navigator.mimeTypes[i].type + '</td>');"
                    "    document.writeln('<td>' + navigator.mimeTypes[i].suffixes + '</td>');"
                    "    document.writeln('<td>' + navigator.mimeTypes[i].description + '</td>');"
                    "    if (navigator.mimeTypes[i].enabledPlugin)"
                    "      document.writeln('<td>' + navigator.mimeTypes[i].enabledPlugin + '</td>');"
                    "    else document.writeln('<td>Not available</td>');"
                    "  document.writeln('</tr>');"
                    "  }"
                    "  document.writeln('</table>');"
                    "}\n";

    html +=         "function testPdfPlugin() {"
                    "  var mimetype = navigator.mimeTypes['application/pdf'];"
                    "  if (mimetype) {"
                    "    var plugin = mimetype.enabledPlugin;"
                    "    if (plugin) {"
                    "      return 'yes';"
                    "    }"
                    "  }"
                    "  document.writeln(\"Sorry, can't show you this file.\");"
                    "  document.writeln('You must install a PDF plugin to see this file.');"
                    "  availablePlugins();"
                    "  return 'no';"
                    "}\n";

    html +=         "</script>\n"
                    "<body></body>"
                    "</html>";

    return html;
}
*/

QString simpleHtmlPage(QString title, QString contents)
{
    QString html =  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"\n"
                    "\"http://www.w3.org/TR/html4/loose.dtd\">\n"
                    "<html>\n<head>\n\t"
                    "<meta http-equiv=Content-Type content=\"text/html; charset=UTF-8\">\n"
                    "<title>" + title + "</title>\n"
                    "</head>\n"
                    "<body><br><br><br><div style=\"text-align:center\"; dir=\""+ QObject::tr("LTR")+ "\"><h1>" + contents + "</h1></div></body>"
                    "</html>";

    return html;
}

//Copy the given folder (recursively) to the given path
void copyFolder(QString sourceFolder, QString destFolder, QStringList fileNameFilters)
{
    QDir sourceDir(sourceFolder);
    if( !sourceDir.exists() )
        return;

    QDir destDir(destFolder);
    if(!destDir.exists())
    {
        destDir.mkdir(destFolder);
    }

    sourceDir.setNameFilters(fileNameFilters);

    QStringList files = sourceDir.entryList(QDir::Files);
    for(int i = 0; i< files.count(); i++)
    {
        QString srcName = sourceFolder + "/" + files[i];
        QString destName = destFolder + "/" + files[i];
        if (!QFile::copy(srcName, destName))
            qDebug() << "Couldn't copy file: " << srcName;
    }

    files.clear();
    files = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for(int i = 0; i< files.count(); i++)
    {
        QString srcName = sourceFolder + "/" + files[i];
        QString destName = destFolder + "/" + files[i];
        copyFolder(srcName, destName, fileNameFilters);
    }
}

//delete the given folder (recursively) to the given path
void deleteBooksFolder(QString sourceFolder)
{
    QDir sourceDir(sourceFolder);
    if( !sourceDir.exists() )
        return;

    QStringList files = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for(int i = 0; i < files.count(); i++)
    {
        deleteBooksFolder(sourceFolder + "/" + files[i]);
    }

    QStringList filters;
    filters << "*.html" << "*.htm" << "*.pdf" << "*.txt" << "*.conf" << "*.dbf" << "*.gdb";
    sourceDir.setNameFilters(filters);

    files = sourceDir.entryList(QDir::Files);
    for(int i = 0; i < files.count(); i++)
    {
        QString srcName = sourceFolder + "/" + files[i];
        if ( !QFile::remove(srcName) )
            qDebug() << "Couldn't remove file: " << srcName;
    }

    if ( !sourceDir.rmdir( sourceFolder ) )
        qDebug() << "Couldn't remove directory: " << sourceFolder;
}

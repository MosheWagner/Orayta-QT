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


//Global path holders. Set in mainwindow::initPaths, and used all over.
// Should allways by absolute.
QString BOOKPATH;
QString TMPPATH;
QString HEBREWBOOKSPATH;
QString USERPATH;

//Global translator object
QTranslator *translator;

//Defualt lang. After all, this is a Hebrew program...
QString LANG="Hebrew";

//Reads the file with the given name, and inserts it's contents into the given vector
bool ReadFileToList(QString filename, QList <QString>& text, const char* encoding_name, bool skipconflines)
{
    bool conflinesended = false;
    //Read the source file:
    QFile infile(filename);

    QString line;

    //Stop if it's not a valid file:
    QFileInfo fi(filename);
    if ( (fi.isDir() == true) || (fi.exists() == false)) return false;

    // Open the file
    if ((!infile.open(QIODevice::ReadOnly))) return false;

    // Set the stream to read from the file
    QTextStream t( &infile );
    t.setCodec(QTextCodec::codecForName(encoding_name));
    while (!t.atEnd())
    {
        line = t.readLine();
        if ( skipconflines == false || conflinesended == true )
        {
            text << line;
        }
        else if ((line[0] == '$') || (line[0] == '!') || (line[0] == '~') || (line[0] == '@') || (line[0] == '#') || (line[0] == '^'))
        {
            conflinesended = true;
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
                if (t_id == stringify(id))
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
    //Read the source file:
    QFile infile(filename);

    //Stop if it's not a valid file:
    QFileInfo fi(filename);
    if ( fi.isDir() == true || fi.exists() == false ) return false;

    // Open the file
    if ((!infile.open(QIODevice::ReadOnly))) return false;

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

    QFlag *mode;
    if (overwrite == false) mode = new QFlag(QIODevice::WriteOnly | QIODevice::Append);
    else mode = new QFlag(QIODevice::WriteOnly);

    //Write the data to the file
    if (outfile.open(*mode))
    {
        outstream << data;
    }
    outfile.close();

    delete mode;
}

//Split given string by given seperater into the given vector, but allways to two
// (split by first appearance of the seperator)
void splittotwo(QString str, vector <QString>& split, QString separator) //will split a string into 2
{
    split.push_back(str.section(separator, 0, 0));
    split.push_back(str.section(separator, 1));
}

//Sets the given int to the value represented by the string
//  Returns if it failed or succedeed
bool ToNum(QString str, int *out)
{
    bool ok;
    *out = str.toInt(&ok);
    return ok;
}

//Prints (couts) the given Qstring
void print(QString qstr)
{
    cout << string(qstr.toUtf8()) << endl;
}

//Prints (couts) the given int
void print(int num)
{
    QString qstr = stringify(num);
    string str(qstr.toUtf8());
    cout << str << endl;
}


//Returns the Gematria of the given string
int GematriaValueOfString (QString& str)
{
    int gematria = 0;
    for(int i=0; i<str.length(); i++) gematria += GematriaOfChar(str.mid(i,1));

    return gematria;
}

//Returns the gematria of the single given hebrew char
int GematriaOfChar(QString ch)
{
    QString hchars = "אבגדהוזחטיכלמנסעפצקרשת";
    int vals[] = {0,1,2,3,4,5,6,7,8,9,10,20,30,40,50,60,70,80,90,100,200,300,400};

    return vals[hchars.indexOf(ch)+1];
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

//Converts numbers into strs:
QString stringify(double x)
{
    return QString::number(x);
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
QString RemoveBrackets(QString& str)
{
    return str.replace("{", "").replace("}", "").replace("(", "").replace(")", "");
}

//BEWARE: Ugly hack !
//Because for some stupid reason QT can't handle internal html links with Hebrew chars, this function
// (which is called by the itr's toString), converts any given QString to a string representing each char's
// Base32 value, (seperated by 'U's), thus giving a valid digit-or-english-letter-only string.

//Thanks to Josh Rosenfeld for making me do this correctly :-D

QString escapeToBase32(QString str)
{
    QString base32 = "";

    for (int i=0; i<str.length(); i++)
    {
        base32 += QString::number(str[i].unicode(), 32) + 'U';
    }

    return base32;
}

QString unescapeFromBase32(QString str)
{
    QString newstr = "";

    //'U' is the splitter between chars, not part of the value
    QStringList list = str.split("U");

    //The last one is for some reason extra
    for (int i=0; i<list.size()-1; i++)
    {
        bool ok;
        int x = list[i].toInt(&ok, 32);

        newstr += QChar(x);
    }
    return newstr;
}


/*
//TODO: BUG: אורות התשובה for instance won't work here, as it cut's the word תשובה off even when it's needed

// Returns a string the search should display for this book and position
QString BookSearchDisplay (QString BookDisplayName , QString PositionName)
{
    QString str="";

    //Exclude from the display name, words that already in the positon name
    vector <QString> excluded;
    WordExclude(BookDisplayName ,PositionName, excluded);

    for (unsigned int i=0; i < excluded.size(); i++)
    {
        if ((excluded[i] != "") && (excluded[i] != " ") && (excluded[i] != "-"))
            str += excluded[i] + " ";
    }
    str += PositionName;

    return str;
}
*/


// Puts into the vector, all words of the first string,
//  excluding any words that are also in the second.
void WordExclude(QString stra , QString strb, vector<QString>& out)
{
    bool match = false;

    QStringList splita = stra.split(" ");

    QStringList splitb = strb.split(" ");

    //Loop over all words of first string
    for (int i=0; i<splita.size(); i++)
    {
        //For each word loop over all words of second string
        for (int j=0; j<splitb.size(); j++)
        {
            //Both have the same word
            if (splita[i] == splitb[j])
            {
                match = true;
            }
        }
        if (match == true) match = false;
        else out.push_back(splita[i]);
    }
}

//Gets a line in the form of foo=bar,
// and iserts bar to the pointed integer (if it's a valud integer)
// (Otherwise the value isn"t changed)
void GetIntValue (QString& valueline, int * pVar)
{
    vector<QString> parts;
    //Split along the = sign
    splittotwo(valueline, parts, "=");

    parts[1].replace('\t', ' ');
    int p = parts[1].indexOf(' ');

    if (parts[1] != "")
    {
        int num;
        if (ToNum(parts[1].mid(0,p), &num)) *pVar = num;
    }
}

//Gets a line in the form of foo="bar",
// and iserts bar to the pointed qstring, unless it's empty
void GetStringValue (QString& valueline, QString * pStr)
{
    vector<QString> parts;
    //Split along the = sign
    splittotwo(valueline, parts, "=");

    if (parts[1] != "")
    {
        if (parts[1][0] != '\"')
        {
            *pStr = parts[1];
        }
        else
        {
            int p = parts[1].lastIndexOf("\"");
            *pStr = parts[1].mid(1, p-1);
        }
    }
}

//Fix the syntax of the 'span' attributes given in source files
//Fixes the double single quotes to a single double quote
//Fixes the '=' sign given with "color=", to "color:"
QString fixSpan(QString& str)
{
    QString newstr = str.replace("''", "\"");
    newstr = newstr.replace("color=", "color:");

    return newstr;
}

//Returns the given string the given number of times
QString stringTimes(QString& str, int t)
{
    QString s="";
    for (int i=0; i<t; i++) s+= str;
    return s;
}

//Limits the given string to the given length, removing the beggining of the string.
// Cuts only at spaces.
QString startChop(QString& str, int limit)
{
    QString newstr = str.mid(str.length() - limit);
    int p = newstr.indexOf(" ");
    return newstr.mid(p);
}

//Limits the given string to the given length, removing the end of the string.
// Cuts only at spaces.
QString endChop(QString& str, int limit)
{
    QString newstr = str.mid(0, limit);
    int p = newstr.lastIndexOf(" ");
    return newstr.mid(0,p+1);
}

//Removes any sign from the given unicode string
// (Nikud or teamim)
QString removeSigns(QString str)
{
    return removeNikud(removeTeamim(str));
}

//Removes any nikud sign from the given unicode string
QString removeNikud(QString str)
{
    //Make sure nikud is in the normall format:
        // QString::normalized(QString::NormalizationForm_D) - Means nikud is allways treated as two chars
        // See: http://www.unicode.org/reports/tr15/#Norm_Forms
        QString s = str.normalized(QString::NormalizationForm_D);

    //These are all of the nikud signs besides the teamim ones mixed in them
    QRegExp regexp(QString("[") + QChar(0x05B0) + "-" + QChar(0x05BC) +
                   QChar(0x05C1) + QChar(0x05C2) + QChar(0x05C4) + QChar(0x05C5) + QChar(0x05C7) +
                   "]");

    return s.replace(regexp, "");
}

//Removes any teamim sign from the given unicode string
QString removeTeamim(QString str)
{
    //QChar(0x0591) - HEBREW ACCENT ETNAHTA
    //QChar(0x05AF) - HEBREW MARK MASORA CIRCLE
    //QChar(0x05C0) - HEBREW PUNCTUATION PASEQ
    //QChar(0x05C6) - HEBREW PUNCTUATION NUN HAFUKHA
    //QChar(0x05BD) - HEBREW POINT METEG (?) -> nikud ?
    QRegExp regexp(QString("[") + QChar(0x0591) + '-' +  QChar(0x05AF) +
                   //Include the PUNCTUATION signs
                   QChar(0x05C0) + QChar(0x05C6) + QChar(0x05BD) +
                   QChar(0x05F3) + QChar(0x05F4) +
                   "]");

    //QChar(0x05C3) - HEBREW PUNCTUATION SOF PASUQ
    //QChar(0x05BE) - HEBREW PUNCTUATION MAQAF
    //Replace sof pasuk with ":" and makaf with " "
    return str.replace(QChar(0x05C3), ":").replace(QChar(0x05BE), " ").replace(regexp, "");
}

QString allowNikudAndTeamim( QString str )
{
    //TODO: remove makaf?

    //These are all of the nikud and teeamim signs
    QString nikudchars = QString(QChar(0x0591)) + "-"  + QChar(0x05C7);

    //Build a RegExp from the search term, allowing nikud and teamin between every letter

    QString pat = "";
    while (str.length() > 0)
    {
        pat += "[" + str.mid(0,1) + "]" + "[" + nikudchars+  "]" + "*";
        str = str.mid(1);
    }

    return pat;
}

QRegExp withNikudAndTeamim( QString str )
{
    return QRegExp("(" + allowNikudAndTeamim(str) + ")");
}

////Returns a regexp pattern all strings that are the same as the given string,
////  ignoring differences in the ו-s and י-s they have.
//QString AllowKtivMaleh(QString str)
//{
//    QString exp;
//    //Remove ו-s and י-s, because they become optional now
//    str = str.replace("ו","");
//    str = str.replace("י","");
//    for (int i=0; i<str.size()-1; i++)
//    {
//        //Allow any number (including zero) of ו and י after every char in the string
//        exp += str.at(i) + QString("[וי]*");
//    }
//    exp += str[str.size()-1];

//    return exp;
//}

QString AllowKtivHasser(QString& str)
{
    return str.replace("ו","ו?").replace("י","י?");
}

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
                    "    document.writeln('<td>' + navigator.mimeTypes[i].type + '<\/td>');"
                    "    document.writeln('<td>' + navigator.mimeTypes[i].suffixes + '<\/td>');"
                    "    document.writeln('<td>' + navigator.mimeTypes[i].description + '<\/td>');"
                    "    if (navigator.mimeTypes[i].enabledPlugin)"
                    "      document.writeln('<td>' + navigator.mimeTypes[i].enabledPlugin + '<\/td>');"
                    "    else document.writeln('<td>Not available<\/td>');"
                    "  document.writeln('<\/tr>');"
                    "  }"
                    "  document.writeln('<\/table>');"
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

QString simpleHtmlPage(QString title, QString contents)
{
    QString html =  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"\n"
                    "\"http://www.w3.org/TR/html4/loose.dtd\">\n"
                    "<html >\n<head>\n\t"
                    "<meta http-equiv=Content-Type content=\"text/html; charset=UTF-8\">\n"
                    "<title>" + title + "</title>\n"
                    "</head>\n"
                    "<body><br><br><br><div style=\"text-align:center\"; dir=\"RTL\"><h1>" + contents + "</h1></div></body>"
                    "</html>";

    return html;
}
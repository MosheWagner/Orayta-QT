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
* Author: Yoch Melka. <yoch.melka@gmail.com>
*/


//TODO: Keep same font for mixed sources
//TODO: Merge as much of Mixed and Normal diaplay as possible
//TODO: make spaces after level signs be done after the comments (thus in the next entry of the loop)
//TODO: Fix they way books such as חפץ חיים look
//TODO: fix remove until (RUS) ...
//TODO: implement "ip" and "is"
//TODO: Implement more features
//TODO: Take over the world :-)

#include "htmlgen.h"
#include "settings.h"
#include "functions.h"
#include "htmlrender.h"


#include <QTime>

using namespace std;


#define QT_USE_FAST_CONCATENATION
#define QT_USE_FAST_OPERATOR_PLUS



//Font sizes each level's label (in the text itself) should add
const int LevelFontSizeAdd[] = {2,10,12,14,16,24};


QString basicCSS(const QFont& font)
{
    QString fontFamily = font.family();
    int basesize = font.pointSize();

    QString ret = "  body { dir='RTL'; text-align: justify; font-family: '" + gFont.family() + "'; font-size: " +
            QString::number(gFont.pointSize()) + " px; }\n"
            "   A { text-decoration: none; margin: 1.3em 0 }\n"
            "   A:hover { color: red; }\n"
            "   div { line-height: 1.5; }\n"
            "   h1 { text-align: center; font-family: '" + gFont.family() + "'; font-size:" + QString::number(basesize + LevelFontSizeAdd[5]) + "px; margin: 0.3em 0 }\n"
            "   h2 { font-family: '" + gFont.family() + "'; font-size:" + QString::number(basesize + LevelFontSizeAdd[4]) + "px; margin: 0.4em 0 }\n"
            "   h3 { font-family: '" + gFont.family() + "'; font-size:" + QString::number(basesize + LevelFontSizeAdd[3]) + "px; margin: 0.5em 0 }\n"
            "   h4 { font-family: '" + gFont.family() + "'; font-size:" + QString::number(basesize + LevelFontSizeAdd[2]) + "px; margin: 0.5em 0 }\n"
            "   h5 { font-family: '" + gFont.family() + "'; font-size:" + QString::number(basesize + LevelFontSizeAdd[1]) + "px; margin: 0.75em 0 }\n"
            "   h6 { font-family: '" + gFont.family() + "'; font-size:" + QString::number(basesize + LevelFontSizeAdd[0]) + "px; margin: 1.em 0 }\n"
            "   div.Content { font-family: '" + fontFamily + "'; font-size: " + QString::number(basesize) + "px; }\n"
            "   div.Content A { font-family: '" + gFont.family() + "'; color:indigo; }\n"
            "   div.Content A:hover { color:red; }\n"
            "   .Aliyah { text-align: center; font-family:'" + gFont.family() + "'; font-size:" +
            QString::number(basesize - 4) + "px; font-weight:bold; color:indigo; }\n"
            "   .S0 { font-size:" + QString::number(basesize - 5) + "px;  font-weight:bold;}\n";

    return ret;
}

static QString weavedCSS(const QFont font, QString color, int num)
{
    QString fontFamily = font.family();
    int fontsize = font.pointSize();

    QString ret;
    ret += "   div.mixed_" + QString::number(num) + " { color: " + color + "; font-family: '" +
            fontFamily + "'; font-size: " + QString::number(fontsize) +
            "px; margin-right: 10px; margin-top: 3px; margin-bottom: 3px }\n";
    ret += "   span.title_mixed_"  + QString::number(num) + "{ font: bold italic " +
            QString::number(gFont.pointSize()) + "px '" + gFont.family() + "'; }\n";

    return ret;
}

QString oraytaScripts()
{
    QString str;

    //Script showing active part and putting it's link in the status bar
    str += "currentlyPainted=null;\n";
    str += "var currentlyPaintedColor;\n";

    // ######### windows.status set the status bar (currently obsolete)
    str +="function paintMe(obj) {\n"
          "   if(currentlyPainted){\n"
          "        currentlyPainted.style.color = currentlyPaintedColor;\n"
          "   }\n"
          "   currentlyPainted=obj;\n"
          "   currentlyPaintedColor = currentlyPainted.style.color;\n"
          "   obj.style.color='red';\n"
          "   obj.scrollIntoView();"
          "   window.status = obj.getAttributeNode(\"name\").value\n"
          "}\n";

    str += "function ReturnValue(varname){\n"
           "    return varname;\n"
           "}\n";

    str +="function ClosestElementToView(){\n"
          "    var closest = null;\n"
          "    var closestoffset = -1 * window.innerHeight;\n"
          "    var i = 0;\n"
          "    var obj = document.getElementById(\"id_\" + i);\n"
          "    while(obj) {\n"
          "       var offset = obj.offsetTop - window.pageYOffset;\n"
          "       if (offset > closestoffset && offset < 10) {\n"
          "           closestoffset = offset;\n"
          "           closest = obj;\n"
          "       }\n"
          "         i++;\n"
          "         var id = \"id_\" + i;\n"
          "         var obj = document.getElementById(id);\n"
          "    }\n"
          "    return closest;\n"
          "}\n";

    str +="function paintById(id) {\n"
          "   var obj = document.getElementById(id);\n"
          "   if (obj) {paintMe(obj); } \n"
          "}\n";

    str += "function paintByName(name) {\n"
           "  var obj = document.getElementsByName(name)[0];\n"
           "  paintMe(obj);\n"
           "}\n";

    str += "function paintWhoILinkTo(obj) {\n"
           "  var p = obj.href.indexOf(\"#\");\n"
           "  paintByName(obj.href.substr(p+1));\n"
           "}\n";

/*  // obsolete functions
    str += "function findByLink(link) {\n"
           "   var a = document.getElementsByTagName(\"A\");\n"
           "   for(var i=0;i<a.length;i++) {\n"
           "      if (a[i].href.indexOf(link) != -1) {\n"
           "          return a[i];\n"
           "      }\n"
           "  }\n"
           "  return null;\n"
           "}\n";

    str += "function paintByHref(href) {\n"
            "  var obj = findByLink(href);\n"
            "  paintMe(obj);\n"
            "}\n";
*/

    str += "function paintNext() {\n"
           "  var i = 0;\n"
           "  if(currentlyPainted){\n"
           "    i = parseInt(currentlyPainted.id.substring(3)) + 1;\n"
           "  }\n"
           "  paintById(\"id_\" + i);\n"
           "}\n";

    str += "function paintPrevious() {\n"
           "   var i = 0;\n"
           "   if(currentlyPainted){\n"
           "      var num = parseInt(currentlyPainted.id.substring(3)) - 1;\n"
           "      if ( num >= 0 ) { i = num; }\n"
           "   }\n"
           "   paintById(\"id_\" + i);\n"
           "}\n";

    str += "function removeNikud() {\n"
            "  var walker = document.createTreeWalker( document.body, NodeFilter.SHOW_TEXT, null, false);\n"
            "  var node;\n"
            "  while(node = walker.nextNode()) {\n"
            "    var text = node.nodeValue;\n"
            "    node.nodeValue = node.nodeValue.replace(/[ְ-ׇּׁׂׅׄ]+/g, '');\n"
            "  }\n"
            "}\n";

    str += "function removeTeamim() {\n"
            "  var walker = document.createTreeWalker( document.body, NodeFilter.SHOW_TEXT, null, false);\n"
            "  var node;\n"
            "  while(node = walker.nextNode()) {\n"
            "    var text = node.nodeValue;\n"
            "    node.nodeValue = node.nodeValue.replace(/[֑-֯׀׆ֽ׳״]+/g, '').replace(/־/g, ' ');\n"
            "  }\n"
            "}\n";

    // ########## le commentaire n'est pas inséré exactement au bon endroit, il faut pouvoir l'insérer à la fin
    // du texte, à revoir avec la génération html

    str += "function comment_line(lines){\n"
            "  var lst = lines.split('|');\n"
            "  var ret = []; ret.push(document.createTextNode(' [*] ' + lst[0]));"
            "  for (var i=1; i < lst.length; i++){\n"
            "    ret.push(document.createTextNode(lst[i]));\n"
            "    ret.push(document.createElement('br'));\n"
            "  }\n"
            "  ret.push(document.createElement('br'));\n"
            "  return ret;\n"
            "}\n";

    str += "function addComment (name, comment) {\n"
           "  if ( comment != '' ){"
           "    var newLink = document.createElement('a');\n"
           "    newLink.href = newLink.name = '*' + name;\n"
           "    newLink.setAttribute('style', 'text-decoration:none; color:blue; font-size:14px');\n"
           "    lst = comment_line(comment); for (var i=0; i < lst.length; i++) { newLink.appendChild(lst[i]); }\n"
           "  }\n"
           "  var nameElem = document.getElementsByName(name)[0];\n"
           "  if (nameElem != null){\n"
           "    var commentExists = document.getElementsByName('*' + name)[0];\n"
           "    if ( commentExists != null ) commentExists.parentNode.removeChild(commentExists);\n"
           "    if ( comment != '' ) nameElem.parentNode.appendChild(newLink);\n"
           "  }\n"
           "}\n";
/*
    str += "<code>"
            "<object type=\"application/x-qt-plugin\" classid=\"QProgressBar\" "
            "name=\"progressbar\" height=30></object>"
            "<script>"
            "function display(){"
            "    if (++document.progressbar.value != 100)"
            "        setTimeout(\"display()\", 50)"
            "}"
            "display();"
            "</script>"
            "</code>";
*/
    return str;
}


//Returns a QString that will be the header of the output HTML file
QString html_head(const QString& title, const QFont& font)
{
    QString t;
    t = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"\n ";
    t += "\"http://www.w3.org/TR/html4/loose.dtd\">\n";
    t += "<html dir=\"RTL\">\n<head>\n\t";

    t += "<meta http-equiv=Content-Type content=\"text/html; charset=UTF-8\">";

    t += "\n<title>";
    t += title;
    t += "</title>\n";

    //Add CSS settings to the html file:
    t += "<style type=\"text/css\">\n" + basicCSS(font) + "</style>\n";

    //Add scripts to the HTML
    t += "<script type=\"text/javascript\">\n" + oraytaScripts() + "</script>\n";

    t +="\n</head>";

    return t;
}

/*
//Returns true if the given line has either a empty line after it,
// or only lines with "//" and an empty line at the end of them.
// (Recusive function). Used to recognize the 'special name' links.
bool space_or_slash_after_this(const vector <QString>& text, int line)
{
    if ( text[line+1] == "" )
    {
        return true;
    }
    else if ( text[line+1][0] == '/' && text[line+1][1] == '/' )
    {
        return space_or_slash_after_this(text, line+1);
    }
    else
        return false;
}
*/

//Generate the title part of the Html file
QString html_book_title(QString name, QString copyright, QString low_comments)
{
    QString t = "";
    if (copyright!="")
    {
        t += "<div style=\"text-align:center; font-weight:bold; font-size:18px; color:#BE32BE;\">";
        t +=  QT_TR_NOOP("All right reserved ");
        t +=  "&#169 " + copyright + "</div>\n";
    }

    t += "<h1>" + name + "</h1>";

    if (low_comments!="")
    {
        t += "<p style=\"font-weight:bold; font-size:14px;\">";
        t += low_comments + "</p>\n";
    }
    t += "<P><HR>";
    return t;
}

//Return html code of dots:
QString bluedot()
{  return "<span style=\"color:blue\"><B>&bull;</B></span>"; }

QString reddot()
{   return "<span style=\"color:red\"><B>&bull;</B></span>";  }


QString index_to_index(const vector<IndexItem>& indexitemlist, int level)
{
    QString str="";

    int indexcount = 0;

    str += "<center><span style=\"font-size:16px;\">";
    for(unsigned int i=0; i<indexitemlist.size(); i++)
    {
        if(indexitemlist[i].level == level)
        {
            str += reddot() + "&nbsp;" + link("#Index" + QString::number(indexcount), indexitemlist[i].displayText) + "&nbsp;";

            indexcount++;
        }
    }
    str += "</span></center>";

    if (indexcount != 0) str += "<HR><BR>";

    return str;
}

QString html_link_table(const vector<IndexItem>& indexitemlist, int short_index_level, bool dot, bool hasRUS)
{
    //TODO: Make those in tables look better (strech to both sides?)

    //NOTE: This is a bit tricksy (see LOTR if you don't know that word),
    //      this should be tested with more books, and documented a lot more

    QString link_table="";

    //Levels possible here are 2,3,4,5
    bool haslevel[4]= { false };

    for (unsigned int i=0; i<indexitemlist.size(); i++)
    {
        haslevel[indexitemlist[i].level - 2] = true;
    }


    //Get i to one above the lowest existing level:
    int levelMIN=0, higherLevel=0;
    //Find lowest level
    for (levelMIN=0; levelMIN < 4 && !haslevel[levelMIN]; levelMIN++) {};

    //Find the next one above it:
    for (higherLevel = levelMIN + 1 ; higherLevel < 4 && !haslevel[higherLevel]; higherLevel++) {};

    // (The found numbers are 2 under the real levels, because they started from 0 and the levels start from 2)
    levelMIN += 2;
    higherLevel += 2;


    //If only one link level is present (and thus higherLevel became 6)
    if ( higherLevel == 6)
    {
        link_table += "<span style=\"font-size:20px;\">";
        for (unsigned int j=0; j<indexitemlist.size(); j++)
        {
            link_table += "&nbsp;&nbsp;&nbsp;" + bluedot() + "&nbsp;";

            link_table += link(indexitemlist[j].linkPoint, indexitemlist[j].displayText);

            if (!hasRUS) link_table +="<BR>\n";
        }
        link_table += "</span>";
    }
    //If more than one level,
    // the lowest is in a table under the one closest above it,
    // and the higher one(s) get a <P> before them.
    else
    {
        int iln = 0;
        bool opentable = false;

        for (unsigned int j=0; j<indexitemlist.size(); j++)
        {
            if (short_index_level == indexitemlist[j].level)
            {
                link_table += "<a name=\"Index" + QString::number(iln) + "\"></a>\n";
                iln ++;
            }

            if (opentable && indexitemlist[j].level >= higherLevel)
            {
                link_table += "</td></tr></tbody></table>\n";
                opentable = false;
            }

            //Higher than one above the lowest
            if (indexitemlist[j].level > higherLevel)
            {
                link_table += "<p style=\"text-align:center; font-size:40px;\">";
                link_table += link(indexitemlist[j].linkPoint, indexitemlist[j].displayText);
                link_table += "</p>\n";
            }
            else if (indexitemlist[j].level == higherLevel)
            {
                link_table += "<span style=\"font-size:28px;\">&nbsp;";
                link_table += link(indexitemlist[j].linkPoint, indexitemlist[j].displayText);
                link_table +=  "</span>\n";

                opentable = true;
                link_table += "<table border=\"0\" cellpadding=\"8\" cellspacing=\"2\" width=\"100%\"><tbody><tr><td width=\"24\"><td align=\"right\">";
            }
            else
            {
                if(dot)
                    link_table += bluedot() + "&nbsp;" + link(indexitemlist[j].linkPoint, indexitemlist[j].displayText);
                else
                    link_table += link(indexitemlist[j].linkPoint, indexitemlist[j].displayText);
                link_table +="&nbsp;\n";
            }
        }
        if (opentable)
            link_table += "</td></tr></tbody></table>\n";
    }


    if (link_table != "") link_table += "<HR>";

    return link_table;
}


//External links:

//Returns the Html code for the given external link
QString ExternalLink (QString linkcode)
{
    //bool ToraOr;
    //bool ShouldBePrintedOnNewLine;

    //UniqueId of target link
    int  BookUniqueId = 0;
    //Label within the target book
    QString BookInternalLabel = "";
    //Link's display name
    QString DisplayedText = "";
    //Link's display style
    int  DisplayStyle = 0;  // BITS:  0-bold, 1-underline, 2-italic, 3-small, 4-big, 5-red, 6-green, 7-blue

    //Validate line
    if ( !linkcode.startsWith("<!--ex") )
        return "";
/*
    //Check type
    if (linkcode[6] == 'a')
    {
        ToraOr = false;
        ShouldBePrintedOnNewLine = false;
    }
    else if (linkcode[6] == 'b')
    {
        //ToraOr = false;
        ShouldBePrintedOnNewLine = true;
    }
    else if (linkcode[6] == 'c')
    {
        //ToraOr = true;
        ShouldBePrintedOnNewLine = false;
    }
    else
        return "";
*/
    //Find location of "-->"
    size_t ptr = linkcode.indexOf("-->");

    //Get displayed text
    DisplayedText = linkcode.mid(ptr+3, linkcode.length()-1);

    //Get data : skip the first 7 chars - cut of the lenth of the displayed text and the "-->"
    QString Data = linkcode.mid(7, ptr-4);  //linkcode.mid(0, ptr+3).mid(7);

    //Decrypt link, and convert the data to unicode
    QByteArray qba = Decrypt(string(Data.toUtf8()), true).c_str();
    QTextCodec * codec = QTextCodec::codecForName("ISO-88598");
    Data = codec->toUnicode(qba);

    if (Data == "")
        return "";

    //Split to parts
    QStringList split_up = Data.split("|");

    //Get display style
    if (!ToNum( split_up[0], &DisplayStyle))
        return "";

    if(split_up[1]!="")
    {
        //If part starts with "bm:"
        if(split_up[1].startsWith("bm:"))
        {
            //Find #
            ptr = split_up[1].indexOf("#");
            //Get BookUniqueId
            if(!ToNum(split_up[1].mid(3, ptr-4), &BookUniqueId))
                return "";
            //Get BookInternalLabel
            BookInternalLabel = split_up[1].mid(ptr+1);
        }
    }

    QString Html="";

    // DisplayStyle BITS:  0-bold, 1-underline, 2-italic, 3-small, 4-big, 5-red, 6-green, 7-blue
    int DS = DisplayStyle;

    //I'm ignoring the coloers at the moment
    if ( DS>>0 & 0x1 ) Html += "<B>";
    //if ( DS>>1 & 0x1 ) Html += "<U>";
    if ( DS>>2 & 0x1 ) Html += "<I>";
    if ( DS>>3 & 0x1 ) Html += "<small>";
    if ( DS>>4 & 0x1 ) Html += "<big>";

    //Escape the hebrew chars
    QString linkto = escapeToBase64(BookInternalLabel);

    if (BookUniqueId != 0)
    {
        Html += "<a href=\"!" + QString::number(BookUniqueId);
        Html += ":" +  linkto + "\">";
        Html += DisplayedText + "</a>\n";
    }
    else
    {
        Html += DisplayedText;
    }

    //I'm ignoring the coloers at the moment
    //revert order to respect html tags nested
    if ( DS>>4 & 0x1 ) Html += "</big>";
    if ( DS>>3 & 0x1 ) Html += "</small>";
    if ( DS>>2 & 0x1 ) Html += "</I>";
    //if ( DS>>1 & 0x1 ) Html += "</U>";
    if ( DS>>0 & 0x1 ) Html += "</B>";

    return Html;
}

//I have no idea how this works, it just does.
// Many thanks to the developer of the original program
string Decrypt (string text, bool decrypt)
{
    string ret = text;
    ret += text;  // allocate buffer

    unsigned char *s = (unsigned char *)(text.c_str());
    unsigned char *d = (unsigned char *)(ret.c_str());

    //Encrypt
    if (!decrypt)
    {
        while (*s)
        {
            *(d++) = 'A' + ((*s) & 15);
            *(d++) = 'A' + (((*(s++)) >> 4) & 15);
        }
        return ret;
    }
    //Decrypt
    else
    {
        while (*s && s[1])
        {
            (*d)  =  (*(s++) - 'A');
            (*(d++)) |=  (*(s++) -  'A') << 4;
        }
        return ret;
    }
    //Failure
    return "";
}

///////////////////////////////////////////////////////////////////


//Renders a html file from the given Orayta file.
//Generates a full file into the given outfile name, and a header file into outfile + ".header.html"
// TODO : return false if have a problem
bool OraytaBookItem::htmlrender() const
{
    //qDebug() << "entry in experimental render";

    QTime tm;
    tm.start();

    HtmlRender render( getPath() );
    if (!render.isValid()) { return false; }

    render.addCss( basicCSS( getFont() ) );
    render.addScript( oraytaScripts() );
    render.addComments( getCommentsForBookId(USERPATH + "CommentList.txt", getRealUniqueId()) );

    render.setOrySpecificInfos(mCopyrightInfo, (QString*)mRemoveSuffix, mShortIndexLevel);
    render.setReplacements(replaceFrom, replaceTo);
    render.setWDispMinLevel(weaveLevel);

    if ( hasNikud && !shownikud ) render.remNikud();
    if ( hasTeamim && !showteamim ) render.remTeamim();
    if ( PutNewLinesAsIs ) render.setNewLine();
    if ( !IsUserBook() && (mPath.contains("tlmod") && mPath.contains("_Bav_")) ) render.setDafAmudFormat();  // #### not work

    if ( !ShowAlone() )
    {
        QStringList colors;
        colors << "#009000" << "#0000FF" << "#A52A2A" << "#4B0082";

        for (int i=0, n=0; i < mWeavedSources.size(); i++)
        {
            if (mWeavedSources[i].show)
            {
                n++;

                OraytaBookItem* obook = dynamic_cast<OraytaBookItem*>(getBookPtrFromId(mWeavedSources[i].id));

                render.addWsource( obook->getPath(), n );
                render.addCss( weavedCSS(obook->getFont(), colors[n%colors.size()], n) );
                render.setReplacements( obook->replaceFrom, obook->replaceTo, n);
            }
        }
    }

    qDebug() << " Render : elapsed " << tm.elapsed() << "ms";

    writetofile(HTMLFileName(), render.toHtml(), "UTF-8");

    return true;
}

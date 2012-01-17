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

using namespace std;


#define QT_USE_FAST_CONCATENATION
#define QT_USE_FAST_OPERATOR_PLUS


//Symbols representing level marks in the text. '!' is the lowest (1), and '^' is the highest (5)
const QString LevelSigns = "!~@^#";

//Font sizes each level's label (in the text itself) should add
const int LevelFontSizeAdd[] = {2,10,12,14,16,24};
const QString LevelTagStart[] = {"<h6>", "<h5>", "<h4>", "<h3>", "<h2>", "<h1>"};
const QString LevelTagEnd[]   = {"</h6>", "</h5>", "</h4>", "</h3>", "</h2>", "</h1>"};

struct weavedSourceData : weavedSource
{
    QList <QString> text;
    QString str;
    QString Prefix;
    QString Suffix;
    BookIter itr;
    int line;
};

//Renders a html file from the given Orayta file.
//Generates a full file into the given outfile name, and a header file into outfile + ".header.html"
//bool Book::htmlrender(QString filename, QString outfilename)
bool OraytaBookItem::htmlrender() const
{
    if (ShowAlone())
    {
        return normalHtmlRender();
    }
    else
    {
        return mixedHtmlRender();
    }
}

weavedSourceData initWsdFrom (const weavedSource& src)
{
    weavedSourceData ret;
    ret.Title = src.Title;
    ret.FileName = src.FileName;
    ret.show = src.show;
    return ret;
}

QString OraytaBookItem::insertCommentInHtml( const QString& strforlink, const vector<QString>& comment_titles, const vector<QString>& comment_texts) const
{
    QString ret = "";
    //See if the is a comment for the past position, and if so, insert it now
    vector<QString>::const_iterator vitr = find(comment_titles.begin(), comment_titles.end(), strforlink);
    if (vitr != comment_titles.end())
    {
        int index = distance (comment_titles.begin (), vitr);
        //Add the text as a special link so menu's can be opened here (and know where this is)
        ret += "\n\t<a href=\"*" + strforlink + "\"* name=\"*" + strforlink + "\" style=\"text-decoration:none; color:blue; font-size:14px\">";
        ret += " [*] " + QString(comment_texts[index]).replace("\\|", "|").replace("|", "<BR>") + "</a>\n";
    }
    return ret;
}

bool OraytaBookItem::mixedHtmlRender() const
{
    QString outfile = HTMLFileName();

    int linkid = 0;

    //TODO: implement zoom attribute?

    QList <weavedSourceData> Sources;
    for (int i=0; i<mWeavedSources.size(); i++)
    {
        if (mWeavedSources[i].show == true)
        {
            weavedSourceData src = initWsdFrom ( mWeavedSources[i] );
            Sources.append(src);
        }
    }

    if (Sources.size() == 0) return false;


    vector<QString> comment_titles, comment_texts;
    //Read coment file into it's vectors
    ReadCommentFile(USERPATH + "CommentList.txt", comment_titles, comment_texts, "UTF-8", mUniqueId);


    QStringList colors;
    colors << "#009000" << "#0000FF" << "#A52A2A" << "#4B0082";
    for (int i=1; i<Sources.size(); i++)
    {
        int j = ((i - 1) % colors.size());

        //Title is now before the text, not after
        if (Sources[i].Prefix == "")
        {
            Sources[i].Prefix = "<span style= color:" + colors[j] + ">";
            Sources[i].Prefix += "<i><b> " +  Sources[i].Title + ": </b></i><br>";
        }
        if (Sources[i].Suffix == "") Sources[i].Suffix = "</span>";
        //if (Sources[i].Suffix == "") Sources[i].Suffix = "<small> [" +  Sources[i].Title + ") </small></span>";
    }

    QString html;
    QString htmlbody;
    QString last_label="", lastlink="", last_level_line="";
//    QString mNameForTitle = "";

    vector <IndexItem> indexitemlist;

    //Load all sources to the memory:
    for (int i=0; i<Sources.size(); i++)
    {
        Sources[i].text.clear();
        if(!ReadFileToList(Sources[i].FileName, Sources[i].text, "UTF-8", true))
        {
            qDebug() << "ERROR: Unable to open file: " << Sources[i].FileName << " !";
        }
        Sources[i].str = "";
        Sources[i].line = 0;
    }

    //BEWARE: Super ugly hack; But it's still better than what we had before.
    //Add a fake level sign to the end of the main source, so we know we're done
    Sources[0].text.push_back("! {EOF}");


    //Go over every line in the main source:
    for (int i=0; i<Sources[0].text.size(); i++)
    {
        //Be nice to other people too
        QApplication::processEvents();

        //Use the first line of whats left of the file, and chop it off
        QString line = Sources[0].text[i];

        //Display nikud and teamim depending on the NikudMode
        if ( hasNikud && !shownikud ) line = removeNikud(line);
        if ( hasTeamim && !showteamim ) line = removeTeamim(line);

        //A new level is reached, so add all that came before it to the Html, and get all of the sources' text for this level
        //NOTE: this is a bit funny (think about it). Maybe this should be fixed. But it still works...
        int level;
        if ( (level = LevelSigns.indexOf(line[0])) != -1 )
        {
            // Go over all other sources
            for (int j=1; j<Sources.size(); j++)
            {
                if (Sources[j].line < Sources[j].text.size())
                {
                    //Update the sources' itr to this line
                    // (It should be a level line that matters, because the loop doesn't stop before that)
                    Sources[j].itr.SetLevelFromLine(Sources[j].text[Sources[j].line]);

                    //If it's the same as the one level just passed in the main source, add this level's text to the Html too.
                    if (Sources[0].itr.toHumanString() == Sources[j].itr.toHumanString())
                    {
                        Sources[j].line ++;

                        QString source_line = Sources[j].text[Sources[j].line];

                        //Clone the sources' itr, so we can see if it changed
                        BookIter tmpitr(Sources[j].itr);
                        tmpitr.SetLevelFromLine(source_line);

                        //As long as the file didn't end and no level that matters was changed, keep on adding text
                        while ( (tmpitr.toHumanString() == Sources[0].itr.toHumanString()) && (Sources[j].line < Sources[j].text.size()))
                        {
                            if (LevelSigns.indexOf(source_line[0]) == -1) Sources[j].str += source_line + "\n";

                            Sources[j].line ++;

                            if (Sources[j].line < Sources[j].text.size())
                            {
                                //Update the cloned itr
                                source_line = Sources[j].text[Sources[j].line];
                                tmpitr.SetLevelFromLine(source_line);
                            }
                        }
                    }
                }
            }

            if (last_label != "")
            {
                for (int j=0; j<Sources.size(); j++)
                {
                    if ( Sources[j].str.replace("\n", "") != "" )
                    {
                        htmlbody += Sources[j].Prefix;
                        htmlbody += Sources[j].str;
                        htmlbody += Sources[j].Suffix;
                        htmlbody += "<BR><BR>\n";
                    }
                }
            }

            //Emtpy strs
            for (int j=0; j<Sources.size(); j++) Sources[j].str.clear();

            //See if the is a comment for the past position, and if so, insert it now
            //htmlbody += insertCommentInHtml(lastlink, comment_titles, comment_texts);

            //Deal with the level sign for this book itself:

            //Advance itr:
            Sources[0].itr.SetLevelFromLine(line);

            QString source0_strforlink = Sources[0].itr.toStringForLinks(level + 1);

            last_label = line;

            lastlink = source0_strforlink;

            if (level == 0)
            {
                //This level dosn't get indexed

                //Add the text as a special link so menu's can be opened here (and know where this is), unless it's a fake sign.
                if (line.mid(2).indexOf("EOF") == -1)
                {
                    htmlbody += "<span>" + link("$" + source0_strforlink, line.mid(2), linkid);
                    htmlbody += insertCommentInHtml(lastlink, comment_titles, comment_texts);
                    htmlbody += "</span>\n";
                    linkid ++;
                }
            }
            else
            {
                IndexItem indexitem;

                indexitem.level = level + 1;

                //Add a name point ("<a name=...") to html index (for the small index to point to it)
                indexitem.linkPoint =  "#" + source0_strforlink;

                //Display of link levels in the Html itself, and in the index
                QString dispname = "";

                //If the book is a gmara, give the pages (that are level 1) thier special names:
                if ( !IsUserBook() && (mPath.contains("תלמוד") || mPath.contains("שס")) && level == 1 )
                {
                    dispname = Sources[0].itr.toGmaraString();
                }
                else
                {
                    //If only part of the link name should be in the index - cut it.
                    if (mRemoveSuffix[level] != "")
                    {
                        QStringList tmp = splittotwo ( line.mid(2), mRemoveSuffix[level]);
                        if(tmp[1] != "")
                            dispname = tmp[1];
                        else
                            dispname = tmp[0];
                    }
                    else
                        dispname = line.mid(2);
                }

                indexitem.displayText = dispname;

                //Add the new indexitem to the indexitems list
                indexitemlist.push_back(indexitem);

                htmlbody += "<span>";
                htmlbody += LevelTagStart[level];
                //Add the text as a special link so menu's can be opened here (and know where this is)
                htmlbody += link("$" + source0_strforlink, dispname, linkid);
                htmlbody += LevelTagEnd[level];

                htmlbody += insertCommentInHtml(lastlink, comment_titles, comment_texts);
                htmlbody += "</span>\n";

                linkid ++;
            }

            last_level_line = line;
        }

        //External link ("<!--ex" type)
        else if(line.startsWith("<!--ex"))
        {
            //htmlbody += ExternalLink(line);
            Sources[0].str += ExternalLink(line);
        }
        else
        {
            //Hope this is OK... Not realy tested yet
            //Run all pre-set replaces
            for (int i=0; i<replaceFrom.size(); i++)
            {
                line.replace(replaceFrom[i], replaceTo[i]);
            }

            Sources[0].str += line + " ";
        }
    }

    for (int i=0; i<Sources.size(); i++) Sources[i].text.clear();


    //Stick together all parts of HTML:
    html += html_head(mNormallDisplayName, mFont.family(), mFont.pointSize());

    html += "<body>";

    html += namepoint("Top");

    html += html_book_title(mNormallDisplayName, mCopyrightInfo, "");

    html += index_to_index(indexitemlist,mShortIndexLevel);

    html += "<div class=\"Index\">";
    html += html_link_table(indexitemlist, mShortIndexLevel , true, mRemoveSuffix[1]!="");
    html += "</div>\n";

    html += "<div class=\"Content\">";
    html += htmlbody;
    html += "</div>\n";

    html += "</body>\n</html>";

    //Save the Html file
    writetofile(outfile, html, "UTF-8");

    return true;
}

bool OraytaBookItem::normalHtmlRender() const
{
    QString outfilename = HTMLFileName();

    int linkid = 0;

    vector <IndexItem> indexitemlist;

    BookIter itr;

    //These are just like consts, but I might change these some day...
    bool dot = true;

    QList <QString> text;

    QString low_comments="";
    QString html="";
    QString htmlbody="";
    //QString lastlabel = "";
    QString mNameForTitle = "";

    //Read the source file associated to this book:
    QString filename = absPath(mPath);
    if(!ReadFileToList(filename, text, "UTF-8", true))
    {
        qDebug() << "ERROR: Unable to open file: " << filename << " !";
        return false;
    }

    //TODO: %$%#%$
    //Simple check to make sure we don't get stuck with a emtpy (or almost empty) file
    if ( text.size() < 2 )
    {
        qDebug() << "ERROR: Invalid file: " << filename << " !";
        return false;
    }

    vector<QString> comment_titles, comment_texts;
    //Read coment file into it's vectors
    ReadCommentFile(USERPATH + "CommentList.txt", comment_titles, comment_texts, "UTF-8", mUniqueId);

    for(int i=0; i<text.size(); i++)
    {
        //Be nice to other people too
        QApplication::processEvents();

        //Display nikud and teamim depending on the NikudMode
        if ( hasNikud && !shownikud ) text[i] = removeNikud(text[i]);
        if ( hasTeamim && !showteamim ) text[i] = removeTeamim(text[i]);

        int level;

        //Book name (usually second line)
        if (text[i][0] == '$')
        {
            //There's a space after the $ sign, so the name is from char 2 untill the end
            mNameForTitle = text[i].mid(2);

            //TODO: dangerous...
            //Usually, after the line with the $, a comment comes until an empty line.
            //This is a bit dangerous, I hope it makes no problems now
            int tmp = i;

            while( i+1 < text.size() && text[i+1] != "" && text[i+1] != " " )
            {
                //A level sign was reached, meaning something went wrong. Undo all that was done here
                if ( LevelSigns.indexOf(text[i][0]) != -1 )
                {
                    i = tmp;
                    low_comments = "";

                    break;
                }

                low_comments += text[i+1] + "<BR>";
                i++;
            }
        }
        //If it's one of the level signs
        else if ( (level = LevelSigns.indexOf(text[i][0])) != -1 )
        {
            //Advance the book itr to the new position
            itr.SetLevelFromLine(text[i]);

            QString strforlink = itr.toStringForLinks(level + 1);

            if (level == 0)  //This level dosn't get indexed
            {
                htmlbody += "<span>";
                //Add the text as a special link so menu's can be opened here (and know where this is)
                htmlbody += link("$" + strforlink, text[i].mid(2), linkid);

                //See if the is a comment for the past position, and if so, insert it now
                htmlbody += insertCommentInHtml(strforlink, comment_titles, comment_texts);

                htmlbody += "</span>\n";
                linkid ++;
            }
            else
            {
                IndexItem indexitem;

                indexitem.level = level + 1;

                /////????
                //Add a name point ("<a name=...") to html index (for the small index to point to it)
                indexitem.linkPoint =  "#" + strforlink;

                //Display of link levels in the Html itself, and in the index
                QString dispname;

                //If the book is a gmara, give the pages (that are level 1) thier special names:
                if ( !IsUserBook() && (mPath.contains("תלמוד") || mPath.contains("שס")) && level == 1 )
                {
                    dispname = itr.toGmaraString();
                }
                else
                {
                    //If only part of the link name should be in the index - cut it.
                    if (mRemoveSuffix[level] != "")
                    {
                        QStringList tmp = splittotwo ( text[i].mid(2), mRemoveSuffix[level]);
                        dispname = (tmp[1] != "" ? tmp[1] : tmp[0]);
                    }
                    else
                    {
                        dispname = text[i].mid(2);
                    }
                }

                indexitem.displayText = dispname;

                //Add the new indexitem to the indexitems list
                indexitemlist.push_back(indexitem);

                htmlbody += "<span>";
                htmlbody += LevelTagStart[level];
                //Add the text as a special link so menu's can be opened here (and know where this is)
                htmlbody += link("$" + strforlink, dispname, linkid);
                htmlbody += LevelTagEnd[level];

                //See if the is a comment for the past position, and if so, insert it now
                htmlbody += insertCommentInHtml(strforlink, comment_titles, comment_texts);

                htmlbody += "</span>\n";
                linkid ++;
            }
        }
        //External link ("<!--ex" type)
        else if(text[i].startsWith("<!--ex"))
        {
            htmlbody += ExternalLink(text[i]);
        }
        //Plain text
        else
        {
            QString txt = text[i];

            //Run all pre-set replaces
            for (int i=0; i<replaceFrom.size(); i++)
                txt.replace(replaceFrom[i], replaceTo[i]);

            htmlbody += txt + " \n";

            if (PutNewLinesAsIs) htmlbody += "<BR>";
        }
    }

    //Stick together all parts of HTML:
    html += html_head(mNameForTitle, mFont.family(), mFont.pointSize());

    html += "<body>";

    html += namepoint("Top");
    
    html += html_book_title(mNameForTitle, mCopyrightInfo, low_comments);

    html += index_to_index(indexitemlist,mShortIndexLevel);

    html += "<div class=\"Index\">";
    html += html_link_table(indexitemlist, mShortIndexLevel , dot, mRemoveSuffix[1]!="");
    html += "</div>\n";

    html += "<div class=\"Content\">";
    html += htmlbody;
    html += "</div>\n";

    html += "</body>\n</html>";

    //Save the Html file
    writetofile(outfilename, html, "UTF-8");

    return true;
}

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

//Returns a QString that will be the header of the output HTML file
QString html_head(QString title, QString fontFamily, int basesize)
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
    t += CSS(fontFamily, basesize);

    //Add scripts to the HTML
    t += Script();
    
    t +="\n</head>";

    return t;
}


QString CSS(QString fontFamily, int basesize)
{
    QString ret = "<style type=\"text/css\">\n"
            "  body { dir='RTL'; text-align: justify; font-family: '" + gFontFamily + "'; font-size: " + QString::number(basesize) + " px; }\n"
            "   A { text-decoration: none; }\n"
            "   A:hover { color: red; }\n"
            "   div { line-height: 1.5; }\n"
            "   h1 { text-align: center; font-family: '" + gFontFamily + "'; font-size:" + QString::number(basesize + LevelFontSizeAdd[5]) + "px; }\n"
            "   h2 { font-family: '" + gFontFamily + "'; font-size:" + QString::number(basesize + LevelFontSizeAdd[4]) + "px; }\n"
            "   h3 { font-family: '" + gFontFamily + "'; font-size:" + QString::number(basesize + LevelFontSizeAdd[3]) + "px; }\n"
            "   h4 { font-family: '" + gFontFamily + "'; font-size:" + QString::number(basesize + LevelFontSizeAdd[2]) + "px; }\n"
            "   h5 { font-family: '" + gFontFamily + "'; font-size:" + QString::number(basesize + LevelFontSizeAdd[1]) + "px; }\n"
            "   h6 { font-family: '" + gFontFamily + "'; font-size:" + QString::number(basesize + LevelFontSizeAdd[0]) + "px; }\n"
            "   div.Content { font-family: '" + fontFamily + "'; font-size: " + QString::number(basesize) + "}"
            "   div.Content A { font-family: '" + gFontFamily + "'; color:indigo; }\n"
            "   div.Content A:hover { color:red; }\n"
            "</style>\n";

    return ret;
}

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

//Returns html code of a "<a name" tag, for the given name
inline QString namepoint (QString name)
{     return "<a name=\"" + name + "\">&nbsp;</a>\n";          }


//Returns an html link by the given link_to and display text
inline QString link (QString linkto, QString text, int id)
{
    if (linkto.startsWith("$"))
    {
        return "<a id=id_" + QString::number(id) + " name=\"" + linkto.mid(1) +
                "\" onclick='paintMe(this)'>" + text + "</a>";
    }
    else if (linkto.startsWith("#"))
    {
        return  "<a href=\"" + linkto + "\" onclick='paintWhoILinkTo(this)'>" +
                text + "</a> &nbsp;";
    }
    else
    {
        return  "<a href=\"" + linkto + "\">" + text + "</a> &nbsp;";
    }
}

//Return html code of dots:
QString bluedot()
{	return "<span style=\"color:blue\"><B>&bull;</B></span>"; }

QString reddot()
{	return "<span style=\"color:red\"><B>&bull;</B></span>";  }


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


QString Script()
{

    QString str = "<script type=\"text/javascript\">\n";

    //Script showing active part and putting it's link in the status bar
    str += "currentlyPainted=null;\n";
    str += "var currentlyPaintedColor;\n";

    str +="function paintMe(obj) {\n"
          "   if(currentlyPainted){\n"
          "        currentlyPainted.style.color = currentlyPaintedColor;\n"
          "   }\n"
          "   currentlyPainted=obj;\n"
          "   currentlyPaintedColor = currentlyPainted.style.color;\n"
          "   obj.focus();\n"
          "   obj.blur();\n"
          "   obj.style.color='red';\n"
          "   window.status = obj.getAttributeNode(\"href\").value\n"
          "}\n";

/*  // obsolete function
    str += "function ScrollToElement(theElement) {\n"
           "    var selectedPosX = 0;\n"
           "    var selectedPosY = 0;\n"
           "\n"
           "    while(theElement != null) {\n"
           "        selectedPosX += theElement.offsetLeft;\n"
           "        selectedPosY += theElement.offsetTop;\n"
           "        theElement = theElement.offsetParent;\n"
           "    }\n"
           "    if (selectedPosY < window.pageYOffset || selectedPosY > ((window.innerHeight + window.pageYOffset) * 0.992 ))\n"
           "    window.scrollTo(selectedPosX , selectedPosY);\n"
           "}\n";
*/

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
            "    node.nodeValue = node.nodeValue.replace(/[֑-֯׀׆ֽ׳״]+/g, '');\n"
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
            "  return ret;\n"
            "}\n";

    str += "function addComment (name, comment) {\n"
           "  if ( comment != '' ){"
           "    var newLink = document.createElement('a');\n"
           "    newLink.href = '*' + name;\n"
           "    newLink.name = '*' + name;\n"
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

    str += "</script>\n";
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


    // Eliminate spaces
    QString  linkto = BookInternalLabel.replace(' ','_');
    //Escape the hebrew chars
    linkto = escapeToBase64(linkto);

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

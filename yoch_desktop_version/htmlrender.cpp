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

#include "htmlrender.h"
#include "functions.h"
#include "htmlgen.h"



#if defined(_MSC_VER) && _MSC_VER >= 1500
# include <functional>
#else
# include <tr1/functional>
#endif

#include <QDebug>
#include <QApplication>



//Returns html code of a "<a name" tag, for the given name
inline QString namepoint (QString name)
{     return "<a name=\"" + name + "\">&nbsp;</a>\n";    }



////////////////////////////////////////////////////////////////////

QString LevelNode::_levels = "!~@^#$";



LevelNode::LevelNode(int lvl, const QString& line) :
    level(lvl),
    firstline(line)
{}

LevelNode::~LevelNode()
{
    qDeleteAll(children.begin(), children.end());
}

// build the html tree recursively
void LevelNode::parse (QList<QString>& liste)
{
    while (!liste.empty())
    {
        // if level occured
        int p = _levels.indexOf(liste.front()[0]);
        if ( p != -1 )
        {
            if (p < level)
            {
                LevelNode* node = new LevelNode(p, liste.takeFirst());

                // recursive call
                node->parse(liste);

                children.push_back(node);
            }
            else
            {
                return;
            }
        }
        else
        {
            data << liste.takeFirst();
        }
    }
}

bool line_equal(const LevelNode* n1, const LevelNode* n2)
{
    return n1->firstline == n2->firstline;
}

//#include <iostream>

// improve this
void LevelNode::addTree(const LevelNode* node, int num)
{
    QList<LevelNode*>::iterator it = children.begin();
    foreach (const LevelNode* nnode, node->children)
    {
        it = std::find_if(it, children.end(), std::tr1::bind(line_equal, nnode, std::tr1::placeholders::_1));

        if (it == children.end())
        {
            /* // for debug
            QTextStream stream(stdout);
            stream.setCodec("UTF-8");
            stream << "no node found for " << nnode->firstline << ";" << nnode->data.join("") << " in " << firstline << "\n"; */

            qDebug() << "no node found for " << nnode->firstline << ";" << nnode->data.join("") << " in " << firstline << "\n";
            continue;
        }

        (*it)->addTree(nnode, num);
    }

    if (!node->data.empty())
    {
        weaved << weavedSrcData(num, node->data.join(""));
    }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

#define QT_USE_FAST_CONCATENATION
#define QT_USE_FAST_OPERATOR_PLUS


const QString LevelTagStart[] = {"<h6>", "<h5>", "<h4>", "<h3>", "<h2>", "<h1>"};
const QString LevelTagEnd[]   = {"</h6>", "</h5>", "</h4>", "</h3>", "</h2>", "</h1>"};


// static function member
LevelNode* HtmlRender::loadTree( const QString& path )
{
    QList<QString> lines;
    if( !ReadFileFromZip(path, "BookText", lines, "UTF-8", true) )
    {
        qDebug() << "ERROR: Unable to open file: " << path << " !";
        return NULL;
    }

    QString title;
    while (!lines.empty())
    {
        if (lines.front()[0] == '$')
        {
            title = lines.takeFirst();
            break;
        }
        lines.pop_front();
    }
/*
// impossible si on vire les lignes vides...
    QString lowcomment = "";
    while ( !lines.empty() && lines.first().simplified() != "" && LevelNode::_levels.indexOf( lines.first()[0] ) == -1 )
        lowcomment += lines.takeFirst();
*/

    if (lines.empty())
    {
        qDebug() << "ERROR: Invalid orayta file: " << path << " !";
//        qDebug() << lowcomment;
        return NULL;
    }

    LevelNode* tree = new LevelNode(LevelNode::_levels.indexOf("$"), title);
    tree->parse(lines);

    if (!lines.empty())
    {
        qDebug() << "WARNING : trailing lines in " << title;
    }

    return tree;
}

HtmlRender::HtmlRender( const QString& path ) :
    wdisplayMinLevel(0),
    remove_Nikud(false),
    remove_Teamim(false),
    newline_AsIs(false),
    dafAmudFormat(false)
{
    root = HtmlRender::loadTree(path);
    if (root)
    {
        title = root->firstline.mid(2);
    }
}

HtmlRender::~HtmlRender()
{
    delete root;
}

bool HtmlRender::isValid() const
{  return root != NULL;  }

void HtmlRender::addWsource( const QString& path, int num )
{
    LevelNode* tree = HtmlRender::loadTree(path);

    if (tree)
    {
        QString title = tree->firstline.mid(2);

        root->addTree(tree, num);

        weavedTitles[num] = title;
    }

    delete tree;
}

void HtmlRender::addScript( const QString& script)
{
    scripts << script;
}

void HtmlRender::addCss( const QString& style)
{
    css << style;
}

void HtmlRender::addComments( const QHash<QString,QString>& hashtbl )
{
    comments = hashtbl;
}

void HtmlRender::setWDispMinLevel(const QChar c)
{
    wdisplayMinLevel = LevelNode::_levels.indexOf(c);
}

void HtmlRender::remNikud()
{
    remove_Nikud = true;
}

void HtmlRender::remTeamim()
{
    remove_Teamim = true;
}

void HtmlRender::setNewLine()
{
    newline_AsIs = true;
}

void HtmlRender::setDafAmudFormat()
{
    dafAmudFormat = true;
}

void HtmlRender::setOrySpecificInfos(QString copyright, QString* RUS, int shortIndexLvl)
{
    copyrightInfo = copyright;
    removeSuffix = RUS;
    shortIndexLevel = shortIndexLvl;
}

void HtmlRender::setReplacements(const QList<QString> &from, const QList<QString> &to, int num)
{
    for (int i=0; i < from.size(); ++i)
    {
        if (num == -1)
        {
            modifiers << std::tr1::bind(
                             static_cast<rep>(&QString::replace),
                             std::tr1::placeholders::_1,
                             from[i],
                             to[i],
                             Qt::CaseSensitive );
        }
        else
        {
            weavedModifiers[num] << std::tr1::bind(
                                        static_cast<rep>(&QString::replace),
                                        std::tr1::placeholders::_1,
                                        from[i],
                                        to[i],
                                        Qt::CaseSensitive );
        }
    }
}

QString HtmlRender::toHtml()
{
    indexitemlist.clear();

    // BEWARE : must call this BEFORE create index !!!
    int id = 0;
    QString content = recToHtml(root, id);

    QString body = "<body>\n";

    body += namepoint("Top");
    body += html_book_title(title, copyrightInfo, "" /*low_comments*/);
    body += index_to_index(indexitemlist, shortIndexLevel);

    body += "<div class=\"Index\">\n" + html_link_table(indexitemlist, shortIndexLevel, true/*dot*/, removeSuffix[1]!="")
            + "</div>\n";

    body += "<div class=\"Content\">\n" + content + "</div>\n";

    body += "</body>\n";

    QString html;

    html += "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"\n"
            "\"http://www.w3.org/TR/html4/loose.dtd\">\n"
            "<html dir=\"RTL\">\n"
            "<head>\n"
            "\t<meta http-equiv=Content-Type content=\"text/html; charset=UTF-8\">\n";

    html += "<title>" + title + "</title>\n";

    if (!css.empty())
        html += "<style type=\"text/css\">\n" + css.join("\n") + "</style>\n";

    if (!scripts.empty())
        html += "<script type=\"text/javascript\">\n" + scripts.join("\n") + "</script>\n";

    html += body + "</html>\n";

    return html;
}

//Returns an html link by the given link_to and display text
QString link (QString linkto, QString text, int id)
{
    if (linkto.startsWith("$"))
    {
        return "<a id=id_" + QString::number(id) + " name=\"" + linkto.mid(1) +
                "\" href=\"" + linkto + "\" onclick='paintMe(this)'>" + text + "</a> ";
    }
    else if (linkto.startsWith("#"))
    {
        return  "<a href=\"" + linkto + "\" onclick='paintWhoILinkTo(this)'>" + text + "</a> &nbsp;";
    }
    else
    {
        return  "<a href=\"" + linkto + "\">" + text + "</a> &nbsp;";
    }
}


QString HtmlRender::recToHtml(LevelNode* node, int &id)
{
    //Be nice to other people too
    QApplication::processEvents();

    QString ret;

    ++id;
    bookitr.SetLevelFromLine(node->firstline);

    QString first_line = node->firstline.mid(2);
    if (remove_Nikud) first_line = removeNikud(first_line);
    if (remove_Teamim) first_line = removeTeamim(first_line);

    ret += "<span>";

    if (node->level > 0)
    {
        ret +=  LevelTagStart[node->level]
              + link("$" + bookitr.toStringForLinks(node->level + 1), first_line, id)
              + LevelTagEnd[node->level];

        if (node->level < 5) setIndex(node);
    }
    // last level occured
    else
    {
        ret += link("$" + bookitr.toStringForLinks(node->level + 1), first_line, id);
    }

    foreach (const QString line, node->data)
    {
        // apply some functions here (and on firstline)
        QString _line;
        if (line.startsWith("<!--ex"))
            _line = ExternalLink(line);
        else
            _line = line + (newline_AsIs ? "<br />" : "\n");

        if (remove_Nikud) _line = removeNikud(_line);
        if (remove_Teamim) _line = removeTeamim(_line);

        foreach (modifier modify, modifiers)
        {
            modify(_line);
        }

        _line = lnkModifier(_line, TitleToId);

        ret += _line;
    }

    // insert comments here
    ret += insertComment( bookitr.toStringForLinks(node->level + 1) );

    ret += "</span>\n";

    if (!node->weaved.empty() && node->level > wdisplayMinLevel)
        ret += insertWSources(node);

    // recursive calls
    for (QList<LevelNode*>::iterator it = node->children.begin(); it != node->children.end(); ++it)
    {
        ret += recToHtml(*it, id);
    }

    if (!node->weaved.empty() && node->level <= wdisplayMinLevel)
        ret += insertWSources(node);

    return ret;
}

inline QString HtmlRender::insertWSources(LevelNode* node)
{
    QString ret;

    foreach (const weavedSrcData wsData, node->weaved)
    {
        int num = wsData.num;
        QString text = wsData.data;

        if (remove_Nikud) text = removeNikud(text);
        if (remove_Teamim) text = removeTeamim(text);

        foreach (modifier modify, weavedModifiers[num])
        {
            modify(text);
        }

        ret += "<div class=\"mixed_" + QString::number(num) + "\">"
                + "<span class=\"title_mixed_" + QString::number(num) + "\">" + weavedTitles[num]
                + ": </b></i></span><br>" + text + "</div>";
    }

    return ret;
}

inline QString HtmlRender::insertComment(const QString& strforlink)
{
    QString ret;

    QHash<QString,QString>::const_iterator it = comments.find(strforlink);
    if (it != comments.end())
    {
        //Add the text as a special link so menu's can be opened here (and know where this is)
        ret += "\n\t<a href=\"*" + strforlink + "\"* name=\"*" + strforlink + "\" style=\"text-decoration:none; color:blue; font-size:14px\">";
        ret += " [*] " + QString(it.value()).replace("\\|", "|").replace("|", "<BR>") + "<br /></a>\n";
    }

    return ret;
}

void HtmlRender::setIndex(LevelNode* node)
{
    IndexItem indexitem;

    indexitem.level = node->level + 1;  // ??

    /////????
    //Add a name point ("<a name=...") to html index (for the small index to point to it)
    indexitem.linkPoint =  "#" + bookitr.toStringForLinks(node->level + 1);

    //Display of link levels in the Html itself, and in the index
    QString dispname;

    //If the book is a gmara, give the pages (that are level 1) thier special names:
    if ( dafAmudFormat && node->level == 1 )
    {
        dispname = bookitr.toGmaraString();
    }
    else
    {
        //If only part of the link name should be in the index - cut it.
        if (removeSuffix[node->level] != "")
        {
            QStringList tmp = splittotwo ( node->firstline.mid(2), removeSuffix[node->level]);
            dispname = (tmp[1] != "" ? tmp[1] : tmp[0]);
        }
        else
        {
            dispname = node->firstline.mid(2);
        }
    }

    indexitem.displayText = dispname;

    //Add the new indexitem to the indexitems list
    indexitemlist.push_back(indexitem);
}

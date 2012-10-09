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

#ifndef LEVELNODE_H
#define LEVELNODE_H

#include <QString>
#include <QList>
#include <QStringList>
#include <QHash>
#include <QTextStream>

#if defined(_MSC_VER) && _MSC_VER >= 1500
# include <functional>
#else
# include <tr1/functional>
#endif

#include "treeitem_orayta.h"
#include "htmlgen.h"


class HtmlRender;


struct weavedSrcData
{
    weavedSrcData(int n, const QString& s):
        num(n),
        data(s)
    {}
    int num;
    QString data;
};


class LevelNode
{
friend class HtmlRender;
friend bool line_equal(const LevelNode* n1, const LevelNode* n2);

public:
    LevelNode(int lvl, const QString& line = QString());
    ~LevelNode();

    void parse (QList<QString>& liste);
    void addTree(const LevelNode* node, int num);

    static QString _levels;

private:
    int level;
    QString firstline;

    QStringList data; // QString ?
    QList<weavedSrcData> weaved;

    QList<LevelNode*> children;

};


class HtmlRender
{
typedef QString& (QString::*rep)(const QString&, const QString&, Qt::CaseSensitivity);
typedef std::function< QString&(QString&) > modifier;

public:
    HtmlRender( const QString& path );
    ~HtmlRender();

    bool isValid() const;

    void addWsource( const QString& path, int num );
    void addScript( const QString& script);
    void addCss( const QString& style);
    void addComments( const QHash<QString,QString>& );

    void setReplacements(const QList<QString>& from, const QList<QString>& to, int num = -1);
    void setOrySpecificInfos(QString copyright, QString* RUS, int shortIndexLvl);
    void setWDispMinLevel(const QChar c);
    void remNikud();
    void remTeamim();
    void setNewLine();
    void setDafAmudFormat();

    QString toHtml();

    // static members
    static LevelNode* loadTree( const QString& path );

private:

    QString recToHtml(LevelNode* node, int& id);
    void setIndex(LevelNode* node);
    inline QString insertWSources(LevelNode* node);
    inline QString insertComment(const QString &strforlink);


    LevelNode* root;
    QString title;
    QHash<int, QString> weavedTitles;
    int wdisplayMinLevel;

    QStringList css;
    QStringList scripts;

    QHash<QString,QString> comments;

    BookIter bookitr;
    std::vector<IndexItem> indexitemlist;

    QList<modifier> modifiers;
    QHash<int, QList<modifier> >  weavedModifiers;

    /*
    QList<QString> replaceFrom;
    QList<QString> replaceTo;
    */

    QString copyrightInfo;
    QString* removeSuffix;
    int shortIndexLevel;

    bool remove_Nikud;
    bool remove_Teamim;
    bool newline_AsIs;
    bool dafAmudFormat;
};


#endif // LEVELNODE_H

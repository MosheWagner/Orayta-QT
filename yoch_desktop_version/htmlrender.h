#ifndef LEVELNODE_H
#define LEVELNODE_H

#include <QString>
#include <QList>
#include <QStringList>
#include <QHash>
#include <QTextStream>

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
public:
    HtmlRender( const QString& path );
    ~HtmlRender();

    void addWsource( const QString& path );
    void addScript( const QString& script);
    void addCss( const QString& style);
    void addComments( const QHash<QString,QString>& );

    void setReplacements(const QList<QString>& from, const QList<QString>& to);
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
    inline QString insertWSources(LevelNode* node);
    inline QString insertComment(const QString &strforlink);
    void setIndex(LevelNode* node);


    LevelNode* root;
    QString title;
    QHash<int, QString> weavedTitles;
    int wdisplayMinLevel;

    QStringList css;
    QStringList scripts;

    QHash<QString,QString> comments;

    BookIter bookitr;
    std::vector<IndexItem> indexitemlist;

    QList<QString> replaceFrom;
    QList<QString> replaceTo;

    QString copyrightInfo;
    QString* removeSuffix;
    int shortIndexLevel;

    bool remove_Nikud;
    bool remove_Teamim;
    bool newline_AsIs;
    bool dafAmudFormat;
};


#endif // LEVELNODE_H

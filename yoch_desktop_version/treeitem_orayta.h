#ifndef TREEITEM_ORAYTA_H
#define TREEITEM_ORAYTA_H


#include "treeitem_base_book.h"
#include "bookiter.h"
#include "guematria.h"
#include "searchresults.h"

#include <QVector>
#include <QMap>


//A simple struct holding all information needed for every source in the weaved display mode
struct weavedSource
{
    QString Title;
    QString FileName;
    bool show;
};

struct OraytaConfs
{
    int bookid;
    QFont font;
    bool hasDefaultFont;
    QList<bool> weavedSrcShown;
    bool checked;
    bool showAlone;
    bool isNikudShown;
    bool isTeamimShown;
};

class OraytaBookItem : public NodeBook
{
//    Q_GADGET
public:

    OraytaBookItem ( BaseNodeItem* parent, QString path, QString name, bool isUserBook );

    //Getters
    Booktype booktype() const;
    bool IsMixed() const;
    bool ShowAlone() const;
    bool ShowMixed() const;
    bool HasNikud() const;
    bool HasTeamim() const;
    bool areNikudShown() const;
    bool areTeamimShown() const;
    QFont getFont() const;
    OraytaConfs publicConfs() const;
    virtual bool isSearchable() const;
    virtual QString getTreeDisplayName() const;
    virtual QString getLoadableFile() const;
    //Returns the filename that should be used for the book, depending on the shown commentaries
    QString HTMLFileName() const;

    //Setters
    void setCosmetics(QString);
    void setShowAlone( bool );
    void showNikud( bool );
    void showTeamim( bool );
    void setFont( const QFont& );
    virtual void changeFont( const QFont& );

    inline QString DBFilePath() const;
    inline QString GDBFilePath()const;

    //Fills "pureText" and "levelMap" with their values
    void BuildSearchTextDB() const;
    void BuildGuematriaDb() const;

    //Return a list of results to all occurrences of the requested phrase in the book
    QList <SearchResult> findInBook(const QRegExp& regexp) const;
    //Overloaded function of the above
    QList <SearchResult> findInBook(const QString& phrase) const;


    QList <SearchResult> findGuematria(const int value) const;

    bool htmlrender() const;

    const QList <weavedSource>& weavedSources() const;
    void setWeavedSourceState ( int, bool );

private:

    virtual void setIcon();

    void AddBookConfs();

    //Returns true if a DB file for this book already exists.
    bool hasDBFile() const;
    bool hasGDBFile() const;   // for guematria search

    //Get the book's puretext
    void getSearchDB(QString& pureText, QMap<int, BookIter>& levelMap) const;
    //serialize Search database
    void writeSearchDB(const QString& pureText, const QMap<int, BookIter>& levelMap) const;

    void getGuematriaDB(QVector<GuematriaDb>& guematriaDbase) const;
    void writeGuematriaDB(const QVector<GuematriaDb>& guematriaDbase) const;

    //Renders a html file from the given Orayta file.
    // Generates a full file into the given outfile name, and a header file into outfile + ".header.html"
    // (Returns false on failure, true on success)
    // Implemented in htmlgen.cpp
    bool normalHtmlRender() const;
    bool mixedHtmlRender() const;

    QString insertCommentInHtml( const QString& strforlink, const std::vector<QString>& comment_titles, const std::vector<QString>& comment_texts) const;

    //Display name for most purposes (search results, etc.)
    QString mNormallDisplayName;
/*
    // pour charger le fichier de confs
    struct OraytaBookInfos
    {

    };
*/
    bool showAlone;
    bool hasNikud;
    bool hasTeamim;
    bool shownikud;
    bool showteamim;

    bool PutNewLinesAsIs;

    //Information about copyrights of this book
    QString mCopyrightInfo;
    QString Cosmetics;

    //Variables that don't seem to do much yet; maybe I'll use them later
    /*
    int LastLevelIndex;
    QString GroupId;
    QString Comments;
    */

    //font to display (if it's directory, applied on all children)
    QFont mFont;
    bool defaultFont;

    QList <weavedSource> mWeavedSources;

    //Level of the short index (index to index)
    int mShortIndexLevel;

    QList <QString> replaceFrom;
    QList <QString> replaceTo;

    // Suffix to remove from the name of the links in it's level
    // Take Notice: this is allways one lower than the link level -
    //      I.E., rus2 is entered to mRemoveSuffix[1], and it's for level 2.
    QString mRemoveSuffix[5];

    //TODO: use these :-)
    QString mIndexSuffix[5];
    QString mIndexPrefix[5];


    int mIndexTextSize[5];
    int mTitleEmptyLines[5];
};


QDataStream & operator<< (QDataStream& out, const OraytaConfs& Value);
QDataStream & operator>> (QDataStream& in, OraytaConfs& Value);

#endif // TREEITEM_ORAYTA_H

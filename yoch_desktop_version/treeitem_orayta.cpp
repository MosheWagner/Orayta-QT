#include "treeitem_orayta.h"
#include "bookdisplayer.h"
#include "functions.h"
#include "settings.h"

#include "bookview_orayta.h"


#include <QFile>
#include <QtCore>

#include <quazip/quazip.h>
#include <quazip/quazipfile.h>


/////////////////////////////////////////////////////
//           Orayta Book
/////////////////////////////////////////////////////

OraytaBookItem::OraytaBookItem ( BaseNodeItem* parent, QString path, QString name, bool isUserBook ) :
    NodeBook(parent, path, name, isUserBook),
    mNormallDisplayName(""),
    mShortIndexLevel(0),
    mCopyrightInfo(""),
    Cosmetics(""),
    hasNikud(false),
    hasTeamim(false),
    showAlone(true),
    shownikud(true),
    showteamim(true),
    PutNewLinesAsIs(true),
    mFont(QFont()),
    defaultFont(true)
{
    if (isUserBook)  // set default QIcon for user book
        QTreeWidgetItem::setIcon(0, QIcon(":/Icons/book-user.png"));

//qDebug() << "entry OraytaBook constructor: " << mPath;

    for(int i=0; i<5; i++)
    {
        mRemoveSuffix[i]="";
        mTitleEmptyLines[i] = 0;
        mIndexSuffix[i] = "";
        mIndexPrefix[i] = "";
    }

    // load confs file
    AddBookConfs();
}

NodeBook::Booktype OraytaBookItem::booktype() const
{  return Orayta;  }

bool OraytaBookItem::isSearchable() const
{   return true && !mIsHidden;  }

QString OraytaBookItem::getTreeDisplayName() const
{  return mNormallDisplayName != "" ? mNormallDisplayName : mTreeDisplayName;  }

QString OraytaBookItem::getLoadableFile() const
{
    bool renderedOK = true;

    QString htmlfilename = HTMLFileName();

    //Check if file already exists. If not, make sure it renders ok.
    QFile f(htmlfilename);
    if ( !f.exists() )
    {
        renderedOK = htmlrender();
    }

    return renderedOK ? absPath(htmlfilename) : QString();
}

bool OraytaBookItem::IsMixed() const
{  return !mWeavedSources.empty(); }

bool OraytaBookItem::ShowMixed() const
{
    if (!IsMixed() || showAlone)
        return false;

    for (int i=1; i<mWeavedSources.size(); i++)
        if (mWeavedSources[i].show)
            return true;

    return false;
}

bool OraytaBookItem::ShowAlone() const
{  return !ShowMixed(); }

bool OraytaBookItem::HasNikud() const
{  return hasNikud;  }

bool OraytaBookItem::HasTeamim() const
{  return hasTeamim;  }

bool OraytaBookItem::areNikudShown() const
{  return shownikud;  }

bool OraytaBookItem::areTeamimShown() const
{  return showteamim;  }

void OraytaBookItem::showNikud(bool show)
{  shownikud = show;  }

void OraytaBookItem::showTeamim(bool show)
{  showteamim = show;  }

QFont OraytaBookItem::getFont() const
{  return defaultFont ? gFont : mFont;  }

void OraytaBookItem::setWeavedSourceState (int index, bool state)
{
    if (index >= mWeavedSources.size())  // sanity check
    {
        qDebug() << "  setWeavedSourceState() : index out of range";
        return;
    }
    mWeavedSources[index].show = state;
}

void OraytaBookItem::setShowAlone( bool show )
{  showAlone = show;  }

void OraytaBookItem::setFont( const QFont& font )
{
    mFont = font;
    defaultFont = false;
}

void OraytaBookItem::changeFont( const QFont& font )
{
    setFont(font);

    BookDisplayer* w = mTabWidget;
    if (w)
    {
        OraytaBookView* oview = dynamic_cast<OraytaBookView*>(w->bookView());
        if (oview) oview->reload();
    }
/*  // ####### no, regenerate only if already generated...
    else
    {
        QtConcurrent::run( *this, &OraytaBookItem::htmlrender );
    }
*/
}

//Add the book's confs, from it's conf file
void OraytaBookItem::AddBookConfs()
{
    QString filename = QString(mPath).replace(".obk", ".conf", Qt::CaseInsensitive);

    QStringList text;

    //Read the book's conf file file:
    if(!ReadFileToList(filename, text, "UTF-8"))
        return;

    for(int i=0; i<text.size() ; i++)
    {
/*
        if (text[i].indexOf("Comments") != -1)
        {
            Comments = text[i].mid(9);
        }
        else if (text[i].indexOf("GroupId") != -1)
        {
            GroupId = text[i].mid(8);
        }
        else if (text[i].indexOf("LastLevelIndex") != -1)
        {
            int n;
            if(ToNum(text[i].mid(15),&n))
                LastLevelIndex = n;
        }
*/
        if (text[i].indexOf("DisplayName") != -1)
        {
            mNormallDisplayName = text[i].mid(12).trimmed();
        }
        // ########## obsolete ?
        else if (text[i].indexOf("ForcedBookName") != -1)
        {
            QStringList linesplit = splittotwo(text[i], "=");

            if (linesplit[1] != "")
            {
                mTreeDisplayName = linesplit[1];
            }
        }
        else if (text[i].indexOf("PutNewLinesAsIs") != -1)
        {
            PutNewLinesAsIs = text[i].mid(16) == "1";
        }
        else if (text[i].indexOf("TextSource") != -1)
        {
            mCopyrightInfo = text[i].mid(11);
        }
        else if (text[i].indexOf("HiddenFromIndex") != -1)
        {
            int t;
            GetIntValue(text[i], &t);
            if (t==1)
            {
                mIsHidden = true;
            }
        }
/*
            else if (text[i].indexOf("MixedDisplay") != -1)
            {
                GetIntValue(text[i], &(book->mWeavedSources));

                //All mixed books can't be searched or selected from the tree
                if (book->IsMixed()) book->setIsUnSelectable(true);
            }
*/
        else if (text[i].indexOf("AddSource") != -1)
        {
            if ( mWeavedSources.size() == 0)
            {
                //Add the book it self as the first source
                weavedSource base;
                base.FileName = getPath();
                base.show = true;
                mWeavedSources.push_back(base);
            }

            QStringList p = text[i].mid(10).split(":");

            weavedSource src;
            src.FileName = BOOKPATH + p[0];
            src.Title = p[1];
            src.show = false;
            mWeavedSources.push_back(src);
        }
        else if (text[i].indexOf("CosmeticsType") != -1)
        {
            setCosmetics(text[i]);
        }
        else if (text[i].indexOf("UniqueId") != -1)
        {
            int id;
            if(ToNum(text[i].mid(9), &id))
                mUniqueId = id;
        }
        else if (text[i].indexOf("Nikud") != -1)
        {
            hasNikud=true;
        }
        else if (text[i].indexOf("Teamim") != -1)
        {
            hasTeamim=true;
        }
    }
}

//Sets the cosmetic types of the book, by the given conf line
void OraytaBookItem::setCosmetics(QString confline)
{
    Cosmetics = confline.mid(14);

    QStringList costype;
    int pos = confline.indexOf("=");
    confline = confline.mid(pos + 1);
    costype = confline.split("#");

    //Handle 'CosmeticsType' confs:
    for(int j=0; j<costype.size(); j++)
    {
        QStringList costypevals = splittotwo(costype[j], "=");

        int num, num2;

        //The rus[x] option, says names of the given level links should be trimmed until the given suffix
        if(costypevals[0].startsWith("rus"))
        {
            if ( ToNum((QString(costypevals[0][3])), &num) && num <= 5 && num > 0)
            {
                //In some sources there's a '_' instead of a ' '. this should deal with that.
                for(int k=0;k<costypevals[1].length();k++)
                    if(costypevals[1][k] == '_')
                        costypevals[1][k] = ' ';
                //(num - 1) because the array is from 0 - 4, but the levels it refers to are 1-5
                mRemoveSuffix[num - 1] = costypevals[1];
            }
        }
        //Short index level
        else if(costypevals[0] == "sil")
        {
            if(ToNum(costypevals[1], &num))
                mShortIndexLevel = num;
        }
        //Index prefix code (for the given level)
        else if(costypevals[0].startsWith("ip"))
        {
            if ( ToNum((QString(costypevals[0][2])), &num) && num <= 5 && num > 0)
            {
                //In some sources there's a '_' instead of a '&'. this should deal with that.
                for(int k=0; k<costypevals[1].length(); k++)
                    if(costypevals[1][k] == '_')
                        costypevals[1][k] = '&';
                mIndexPrefix[num - 1] = costypevals[1];
            }
        }
        //Index suffix code (for the given level)
        else if(costypevals[0].startsWith("ix"))
        {
            if ( ToNum((QString(costypevals[0][2])), &num) && num <= 5 && num > 0)
            {
                //In some sources there's a '_' instead of a '&'. this should deal with that.
                for(int k=0; k<costypevals[1].length(); k++)
                    if(costypevals[1][k] == '_')
                        costypevals[1][k] = '&';
                mIndexSuffix[num - 1] = costypevals[1];
            }
        }
        //Index suffix code (for the given level)
        else if(costypevals[0].startsWith("is"))
        {
            if ( ToNum((QString(costypevals[0][2])), &num) && num < 5 && num > 0)
            {
                //In some sources there's a '_' instead of a '&'. this should deal with that.
                if ( ToNum(QString(costypevals[1]), &num2))
                    mIndexTextSize[num - 1] = num2;
            }
        }
        //Index suffix code (for the given level)
        else if(costypevals[0].startsWith("sp"))
        {
            if ( ToNum((QString(costypevals[0][2])), &num) && num <= 5 && num > 0)
            {
                //In some sources there's a '_' instead of a '&'. this should deal with that.
                if ( ToNum(QString(costypevals[1]), &num2))
                    mTitleEmptyLines[num - 1] = num2;
            }
        }

        else if(costypevals[0].startsWith("rep"))
        {
            QString reper = costype[j].mid(costype[j].indexOf("=") + 1);
            QStringList p = reper.split(QString("^^"));
            for (int i=0; i<p.size(); i++)
            {
                QStringList pp = splittotwo(p[i], "=");

                replaceFrom << pp[0];
                replaceTo << pp[1];
            }
        }

        costypevals.clear();
    }
    costype.clear();
}


const QList <weavedSource>& OraytaBookItem::weavedSources() const
{  return mWeavedSources;  }


//Returns the filename that should be used for the book, depending on the shown commentaries
QString OraytaBookItem::HTMLFileName() const
{
    QString name = ( getUniqueId() == -1 ? mNormallDisplayName : QString::number(getUniqueId()) );

    QString CommenteriesMagicString = "";

    if ( !ShowAlone() )
    {
        CommenteriesMagicString += "-"; //QString representing the commenteries state
        for (int i=1; i<mWeavedSources.size(); i++)
            CommenteriesMagicString += QString::number(mWeavedSources[i].show);
    }

    return TMPPATH + name + CommenteriesMagicString + ".html";
}

inline QString OraytaBookItem::DBFilePath() const
{
    return "SearchDB";
}

//Get the book's puretext
void OraytaBookItem::getSearchDB(QString& pureText, QMap<int, BookIter>& levelMap) const
{
    if (!hasDBFile())
        BuildSearchTextDB();

    QuaZip zip(mPath);
    if (!zip.open(QuaZip::mdUnzip)) qDebug() << "Error!";
    if (!zip.setCurrentFile(DBFilePath())) qDebug() <<  "Error!";

    QuaZipFile zfile(&zip);
    zfile.open(QIODevice::ReadOnly);

    // Set the stream to read from the file
    QDataStream in(&zfile);

    in >> pureText >> levelMap;
}

//serialize Search database
void OraytaBookItem::writeSearchDB(const QString& pureText, const QMap<int, BookIter>& levelMap) const
{
    QuaZip zip(mPath);
    if (!zip.open(QuaZip::mdAdd)) qDebug() << "Error!";

    QuaZipFile zfile(&zip);
    zfile.open(QIODevice::WriteOnly, QuaZipNewInfo(DBFilePath()));
    QDataStream out(&zfile);

    out << pureText << levelMap;
}

bool OraytaBookItem::hasDBFile() const
{
    QuaZip zip(mPath);
    if (!zip.open(QuaZip::mdUnzip)) return false;
    if (!zip.setCurrentFile(DBFilePath())) return false;

    return true;
}

// ############ A voir si l'on peut adapter ca à d'autres livres
void OraytaBookItem::BuildSearchTextDB() const
{
    //Read book's contents
    QStringList text;
    if (!ReadFileFromZip(mPath, "BookText", text, "UTF-8", true))
    {
        qDebug() << "Error reading the book's text" << mPath;
        return ;
    }

    //TODO: remove references to other books

    QString pureText = "";

    QMap<int, BookIter> levelMap;

    BookIter itr;

    //If a line starts with one of these, it's a level sign
    QString levelSigns = "!@#$^~";
    //Any char not matchong this pattern, is no *pure* text.
    QRegExp notText("[^×-×ªa-zA-Z0-9 ()'\"]");
    //These are turned into spaces, and not just ignored.
    QString spaceSigns = "[-_:.,?]";

    //Pad with spaces, so "full word searches" work on begining and end of text too
    pureText = " ";

    for (int i=0; i < text.size(); i++)
    {
        //Level line
        if (levelSigns.contains(text[i][0]))
        {
            //Update iter
            //but if levelSign is '$' ???
            itr.SetLevelFromLine(text[i]);

            //Map with it's position in the pureText
            //levelMap.insertMulti(pureText.length(), itr);
            levelMap[pureText.length()] = itr;
        }
        //Link
        else if (text.startsWith("<!--ex"))
        {
        }
        //Text line
        else
        {
            //Test if book is from the bible. Hope this is ok...
            if ( mPath.contains("×ž×§×¨×") )
            {
                //Turn ×§×¨×™ ×•×›×ª×™×‘ into only ×§×¨×™. Maybe this should be an option?
                text[i].replace( QRegExp ("[(][^)]*[)] [[]([^]]*)[]]"), "\\1");
            }

            //Remove html tags
            text[i].replace( QRegExp("<[^>]*>"), "" );

            //Remove all non "pure-text" chars
            text[i].replace(QChar(0x05BE), " "); //Makaf
            text[i].replace(QRegExp(spaceSigns), " ");
            text[i].replace("{", "(").replace("}",")");
            text[i].replace("nbsp", " ");
            text[i].replace(notText, "");

            //Remove double spaces and line breaks
            pureText += text[i].simplified() + " ";
        }
    }

    //Pad with spaces, so "full word searches" work on begining and end of text too
    pureText += " ";

    writeSearchDB(pureText, levelMap);

/*  // old code for HTML files. seem ugly
        else if (bookType == Html)
        {
            QFile file(mPath);
            if (!file.open(QIODevice::ReadOnly))
            {
                qDebug() << "couldn't open " << mPath;
                return;
            }

            QByteArray data = file.read( file.size() );
            file.close();

            QTextCodec* codec = QTextCodec::codecForHtml(data, QTextCodec::codecForName("UTF-8"));
            text = codec->toUnicode(data).replace(QRegExp("<[^>]*>"), "").split('\n', QString::SkipEmptyParts);
        }
*/
}


QList<SearchResult> OraytaBookItem::findInBook(const QString& phrase) const
{
    //Naive convert to regexp
    QRegExp regexp = QRegExp(phrase);
    return findInBook(regexp);
}

#define CHAR_LIMIT 300
inline QString resultPreview(const QString& pureText, const QRegExp& exp, int startOffset, int endOffset, int offset, int len)
{
    QString s = pureText.mid(offset, len);

    //Force full words
    if (offset > startOffset)
    {
        s = "..." + s.mid(s.indexOf(" "));
    }
    if (offset + len < endOffset)
    {
        s = s.mid(0, s.lastIndexOf(" ")) + "...";
    }

    return s.replace(exp, "<span style='background-color:Yellow'>\\1</span>");
}

QList<SearchResult> OraytaBookItem::findInBook(const QRegExp& exp) const
{
    //qDebug() << "searching" << exp.pattern() << " in: " << getTreeDisplayName();

    QString pureText = "";
    QMap<int, BookIter> levelMap;

    getSearchDB(pureText, levelMap);

    QList <SearchResult> results;
    QRegExp regexp = QRegExp("(" + exp.pattern()+ ")");

    if (pureText == "")
        qWarning() << getTreeDisplayName() << " has no Search DB, so it couldn't be searched!";

    int curr, next = 0, startOffset = -1, stopOffset = -1;
    while ( (curr = exp.indexIn(pureText, next)) != -1 && results.size() < MAX_RESULTS_PER_BOOK )
    {
        QMap<int, BookIter>::const_iterator mapitr = levelMap.upperBound(curr);
        if (mapitr != levelMap.begin())
            --mapitr;

        BookIter itr = ( !levelMap.empty() ? mapitr.value() : BookIter() );

        if (curr > stopOffset)
        {
            const QMap<int, BookIter>::const_iterator nextitr = levelMap.lowerBound(curr + exp.matchedLength());

            int itrOffset = mapitr.key();
            int nextitrOffset = (nextitr != levelMap.end() ? nextitr.key() : pureText.length());

            startOffset = std::max(itrOffset, curr - (CHAR_LIMIT/2));
            stopOffset = std::min(nextitrOffset, curr + (CHAR_LIMIT/2));

            SearchResult s;
            s.link = "!" + QString::number(getUniqueId()) + ":" + itr.toStringForLinks() + ":" + escapeToBase64(pureText.mid(curr, exp.matchedLength()));
            s.linkdisplay = getTreeDisplayName() + " " + ( mPath.contains("×ª×œ×ž×•×“") ? itr.toGmaraString() : itr.toHumanString() );
            s.preview = resultPreview(pureText, regexp, itrOffset, nextitrOffset, startOffset, stopOffset-startOffset);
            s.nbResults = 1;

            results.push_back(s);
        }
        else
        {
            results.back().nbResults++;
        }

        next = curr + 1;
    }

    return results;
}

QList<SearchResult> OraytaBookItem::findGuematria(const int value) const
{
    QList<SearchResult> ret;

    QVector<GuematriaDb> bookGuematriaDb;
    getGuematriaDB(bookGuematriaDb);

    for (int i=0; i < bookGuematriaDb.size(); ++i)
    {
        // search the guematria
        int calc = bookGuematriaDb[i].values[0];
        int first = 0, last = 0;
        while ( last < bookGuematriaDb[i].values.size() )
        {
            if (calc == value)
            {
                SearchResult result;

                QString occ = "";
                for (int cmpt = first; cmpt <= last; ++cmpt)
                {
                    occ += bookGuematriaDb[i].words[cmpt];
                    if (cmpt < last) occ += " ";
                }

                //Get the text best to show for this reult's description
                result.linkdisplay = getTreeDisplayName() + " " + bookGuematriaDb[i].itr.toHumanString();
                //Add the full result to the page
                result.link = "!" + QString::number(getUniqueId()) + ":" + bookGuematriaDb[i].itr.toStringForLinks() +
                                ":" + escapeToBase64(occ);
                result.preview = bookGuematriaDb[i].words.join(" ").
                                    replace( QRegExp("(" + occ + ")"), "<span style='background-color:Yellow'>\\1</span>" );

                ret.push_back( result );

                last = ++first;
                if (last < bookGuematriaDb[i].values.size())
                    calc = bookGuematriaDb[i].values[first];
            }

            while (calc < value)
            {
                last++;
                if (last < bookGuematriaDb[i].values.size())
                    calc += bookGuematriaDb[i].values[last];
                else
                    break;
            }
            while (calc > value)
            {
                calc -= bookGuematriaDb[i].values[first];
                first++;
            }
        }
    }

    return ret;
}

void OraytaBookItem::setIcon()
{
    // ####### ajouter la gestion de unchecked / partially checked ?
    if ( mUserBook )
        return;

    if ( !IsMixed() )
    {
        if (mIconState == BLUE)
            QTreeWidgetItem::setIcon(0, QIcon(":/Icons/book-blue.png"));
        else if (mIconState == BaseNodeItem::GREY)
            QTreeWidgetItem::setIcon(0, QIcon(":/Icons/book-grey.png"));
    }
    else
    {
        if (mIconState == BaseNodeItem::BLUE)
            QTreeWidgetItem::setIcon(0, QIcon(":/Icons/book-blue-double.png"));
        else if (mIconState == BaseNodeItem::GREY)
            QTreeWidgetItem::setIcon(0, QIcon(":/Icons/book-grey-double.png"));
    }
}

OraytaConfs OraytaBookItem::publicConfs() const
{
    OraytaConfs ret;
    ret.bookid = mUniqueId;
    ret.font = mFont;
    ret.hasDefaultFont = defaultFont;
    ret.checked = mInSearch;
    ret.isNikudShown = shownikud;
    ret.isTeamimShown = showteamim;
    ret.showAlone = showAlone;
    for (int i=0; i<mWeavedSources.size(); ++i)
        ret.weavedSrcShown.push_back(mWeavedSources[i].show);
    return ret;
}


//////////////////////////////////////////////////////////////////////
//            serialize releveant confs for OraytaBookItem
//////////////////////////////////////////////////////////////////////
QDataStream & operator<< (QDataStream& out, const OraytaConfs& Value)
{
    out << Value.bookid << Value.checked << Value.font << Value.hasDefaultFont << Value.isNikudShown << Value.isTeamimShown << Value.showAlone << Value.weavedSrcShown;
    return out;
}

QDataStream & operator>> (QDataStream& in, OraytaConfs& Value)
{
    in >> Value.bookid >> Value.checked >> Value.font >> Value.hasDefaultFont >> Value.isNikudShown >> Value.isTeamimShown >> Value.showAlone >> Value.weavedSrcShown;
    return in;
}

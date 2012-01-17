#include "treeitem_orayta.h"
#include "guematria.h"
#include "functions.h"
#include "qtiocompressor.h"

#include <QFile>



inline QString OraytaBookItem::GDBFilePath() const
{
    return QString(mPath).replace(".txt", ".gdb", Qt::CaseInsensitive);
}

//Get the book's puretext
void OraytaBookItem::getGuematriaDB(QVector<GuematriaDb>& guematriaDbase) const
{
    if (!hasGDBFile())
        BuildGuematriaDb();

    QFile file(GDBFilePath());
    QtIOCompressor compressor(&file);
    compressor.open(QIODevice::ReadOnly);
    QDataStream in(&compressor);

    in >> guematriaDbase;
}

//serialize Search database
void OraytaBookItem::writeGuematriaDB(const QVector<GuematriaDb>& guematriaDbase) const
{
    QFile file(GDBFilePath());
    QtIOCompressor compressor(&file);
    compressor.open(QIODevice::WriteOnly);
    QDataStream out(&compressor);

    out << guematriaDbase;
}

bool OraytaBookItem::hasGDBFile() const
{
    QFile f(GDBFilePath());

    if (!f.exists() || !f.open(QIODevice::ReadOnly))
        return false;

    return true;
}

void OraytaBookItem::BuildGuematriaDb() const
{
    if (hasGDBFile())
        return;

    QVector <GuematriaDb> guematriaDb;

    GuematriaDb current;

    //Read book's contents
    QStringList text;
    if (!ReadFileToList( mPath, text, "UTF-8", true))
    {
        qDebug() << "Error reading the book's text" << mPath;
        return;
    }

    //If a line starts with one of these, it's a level sign
    QString levelSigns = "!@#$^~";
    //Any char not matchong this pattern, is no *pure* text.
    QRegExp notText("[^ א-ת]");
    //These are turned into spaces, and not just ignored.
    QString spaceSigns = "[-_:.,?]";

    for (int i=0; i < text.size(); i++)
    {
        //Level line
        if (levelSigns.contains(text[i][0]))
        {
            //Update iter
            //but if levelSign is '$' ???
            current.itr.SetLevelFromLine(text[i]);
        }
        //Text line
        else
        {
            //Turn קרי וכתיב into only קרי. Maybe this should be an option?
            text[i].replace( QRegExp ("[(][^)]*[)] [[]([^]]*)[]]"), "\\1");

            //Remove all non "pure-text" chars
            text[i].replace(QChar(0x05BE), " "); //Makaf
            text[i].replace(notText, "");

            //Remove double spaces and line breaks
            //and split in words
            current.words = text[i].simplified().split(' ');

            current.values.resize( current.words.size() );
            for (int n=0; n < current.words.size(); n++)
                current.values[n] = GematriaValueOfString(current.words[n]);

            guematriaDb.push_back( current );
        }
    }

    writeGuematriaDB(guematriaDb);
}

QDataStream & operator<< (QDataStream & out, const GuematriaDb & Value)
{
    out << Value.itr << Value.words << Value.values;
    return out;
}

QDataStream & operator>> (QDataStream & in, GuematriaDb & Value)
{
    in >> Value.itr >> Value.words >> Value.values;
    return in;
}

#include "book.h"
#include "guematria.h"


void Book::BuildGuematriaDb()
{
    if (guematriaDbExists)
        return;

    GuematriaDb current;
    QString line;

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
            current.itr.SetLevelFromLine(text[i]);
        }
        //Text line
        else
        {
            //Turn קרי וכתיב into only קרי. Maybe this should be an option?
            line = text[i].replace( QRegExp ("[(][^)]*[)] [[]([^]]*)[]]"), "\\1");

            //Remove all non "pure-text" chars
            line = line.replace(QChar(0x05BE), " "); //Makaf
            line = line.replace(notText, "");

            //Remove double spaces and line breaks
            line = line.simplified();

            current.words = line.split(' ');

            current.values.resize( current.words.size() );
            for (int n=0; n < current.words.size(); n++)
                current.values[n] = GematriaValueOfString(current.words[n]);

            guematriaDb.push_back( current );
        }
    }


    guematriaDbExists = true;
}

const vector <GuematriaDb>& Book::getGuematriaDb()
{
    BuildGuematriaDb();
    return guematriaDb;
}

#ifndef SEARCHRESULTS_H
#define SEARCHRESULTS_H

#include <QString>

struct SearchResult
{
    QString link;
    QString linkdisplay;
    QString preview;
    int nbResults;
};

#endif // SEARCHRESULTS_H

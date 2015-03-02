#ifndef GUEMATRIA_H
#define GUEMATRIA_H

#include <QString>
#include <QStringList>
#include <QVector>


#include "bookiter.h"


struct GuematriaDb {
    BookIter itr;
    QStringList words;
    QVector<int> values;
};


#endif // GUEMATRIA_H

#ifndef GUEMATRIA_H
#define GUEMATRIA_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QDebug>


#include "bookiter.h"


struct GuematriaDb {
    BookIter itr;
    QStringList words;
    QVector<int> values;
};

QDataStream & operator<< (QDataStream & out, const GuematriaDb & Value);
QDataStream & operator>> (QDataStream & in, GuematriaDb & Value);

#endif // GUEMATRIA_H

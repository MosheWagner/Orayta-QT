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

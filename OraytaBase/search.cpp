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
* Author: Moshe Wagner. <moshe.wagner@gmail.com>
*/

#include "htmlgen.h"
#include "search.h"
#include <QUrl>
#include <QDebug>
#include <QTime>
//#include <QtGui>
#include <QApplication>

// This is just a very simple define. every place in the code,
//  "CURRENT_TAB" simply represents "ui->viewTab->currentIndex()".
#define CURRENT_TAB ui->viewTab->currentIndex()


QString createSearchPattern (QString userInput, bool allWords, bool fullWords, int spacing)
{
    QStringList words = userInput.split(" ", QString::SkipEmptyParts);
    QString pattern;

    if (!fullWords)
    {
        for (int i=0; i < words.size(); i++)
            words[i] = "[^ ]*" + words[i] + "[^ ]*";
    }
    else if (!allWords) // && fullWords
    {
        for (int i=0; i < words.size(); i++)
            words[i] = " " + words[i] + " ";
    }

    int i;
    if (allWords)
    {
        QString sep = (spacing == 0 ? " " : " ([א-ת]+ ){0," + QString::number(spacing) + "}");
        for ( i=0; i < words.size()-1; i++ )
        {
            pattern += words[i] + sep;
        }
        pattern += words[i];

        if (fullWords)
            pattern = " " + pattern + " ";
    }
    else
    {
        for ( i=0; i < words.size()-1; i++ )
        {
            pattern += "(" + words[i] + ")|";
        }
        pattern += "(" + words[i] + ")";
    }

    return pattern;
}

bool stopSearchFlag;

#define RESULTS_MAX 500
QUrl SearchInBooks (const QRegExp& regexp, QString disp, vector<Book*> searchList, QProgressBar *pbar)
{
    //TODO: make preview look nice

    stopSearchFlag = false;

    QString title, Html="", Htmlhead="", HtmlTopLinks="";

    QList <QString> text;

    //vector<Book*> searchList = bookList.BooksInSearch();

    if ( regexp.pattern() != "" )
    {
        //ui->pbarbox->show();
        if (pbar) pbar->setValue(5);
        float percentPerBook = (95.0 / searchList.size());

        //Head and title of the Html
        title = QObject::tr("Search results: ") + "\"" + disp + "\"";

        Htmlhead = html_head(title, QFont(gFontFamily, gFontSize));
        Htmlhead += "<body><div class=\"Section1\" dir=\"RTL\" >";
        Htmlhead += "<div style=\"font-size:30px; text-align:center\"><b><i>";
        Htmlhead += title + ":" + "</i></b></div><BR>";
        Htmlhead += "\n<span style=\"font-size:17px\">";

        int results = 0, allresults = 0;
        for (unsigned int i=0; i < searchList.size() && results < RESULTS_MAX && stopSearchFlag == false; i++)
        {
            if (pbar) pbar->setValue( 5 + (i + 1) * percentPerBook ) ;

            QList <SearchResult> searchResults = searchList[i]->findInBook(regexp);

            //Let the animation move...
            QApplication::processEvents();

            for (int j=0; j<searchResults.size() && results < RESULTS_MAX ; j++)
            {
                bool doubleResult = false;

                if (j > 0) if (searchResults[j].itr == searchResults[j - 1].itr) doubleResult = true;

                if (doubleResult)
                {
                    allresults ++;
                }
                else
                {
                    allresults ++;
                    results ++;
                    //Get the text best to show for this reult's description
                    QString linkdisplay = "";

                    //Regular books:
                    if (!searchList[i]->getPath().contains("תלמוד") && !searchList[i]->getPath().contains("שס"))
                    {
                        //This is an overkill, but I couldn't resist:
                        //linkdisplay += BookSearchDisplay (gBookList[i]->getNormallDisplayName() ,itr.toHumanString());

                        linkdisplay += searchList[i]->getNormallDisplayName() + " " + searchResults[j].itr.humanDisplay();
                    }
                    //Gmarot:
                    else
                    {
                        linkdisplay += searchList[i]->getNormallDisplayName() + " ";
                        linkdisplay += searchResults[j].itr.gmaraDisplay();
                    }

                    //Add a small link (at the index) to the full result
                    HtmlTopLinks += reddot() + "&nbsp;&nbsp;&nbsp;<a href=\"#A" + stringify(results) + "\">" + stringify(results) + ")&nbsp;&nbsp;" + linkdisplay  + "</a><BR>\n";

                    //Add the full result to the page
                    Html += "<a name=\"A" + stringify(results) + "\" href=\"!" + stringify(searchList[i]->getUniqueId()) + ":";
                    Html += searchResults[j].itr.toEncodedString();
                    Html += ":" + escapeToBase32(regexp.pattern()) + "\">";

                    Html += linkdisplay;
                    Html += "</a><BR></span>\n";

                    //Show result
                    Html += searchResults[j].preview;

                    Html += "<br><br><br>\n";
                }
            }
        }

        if (stopSearchFlag == true)
        {
            Htmlhead += "<BR><BR>" + QObject::tr("(Search stopped by user)") + "<BR><BR>";
            stopSearchFlag = false;
        }

        if(results == 0)
        {
            //TODO: write better explenation
            Htmlhead +="<BR><BR>"; Htmlhead += QObject::tr("No search results found:"); Htmlhead += "\"" + disp + "\"";
            Htmlhead += "</B><BR>";
        }
        else
        {
            Htmlhead += "<B>"; Htmlhead += QObject::tr("Short result list: "); Htmlhead += "</B><BR>";
            if (results >= RESULTS_MAX)
            {
                Htmlhead += "(" +  stringify(results) + " " +QObject::tr("first results only") + "<BR><BR>";
            }
            else
            {
                Htmlhead += "(נמצאו " +  stringify(allresults) + " תוצאות ב - " + stringify(results) + " מקומות) " + "<BR><BR>";
            }
            Htmlhead += HtmlTopLinks;

            Htmlhead += "<BR><BR><B>" +  QObject::tr("Full result list:") + "</B><BR><BR>";
        }

        Html = Htmlhead + Html;

        Html += "</span></div>\n";
        Html += "\n</body>\n</html>";

        if (pbar) pbar->setValue(100);
        //ui->pbarbox->hide();
    }

    static int ind = 0;

    QString path = TMPPATH + "SEARCH" + QString::number(ind++) + ".html";
    writetofile(path, Html, "UTF-8", true);

    return QUrl::fromLocalFile(path);
    //return Html;
}

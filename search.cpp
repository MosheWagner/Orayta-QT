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


#include "mainwindow.h"


// This is just a very simple define. every place in the code,
//  "CURRENT_TAB" simply represents "ui->viewTab->currentIndex()".
#define CURRENT_TAB ui->viewTab->currentIndex()

//Another define becuase I'm lasy. "CurrentBookdisplayer" simply represents
// the currently visible "bookdisplayer"
#define CurrentBookdisplayer bookDisplayerList[CURRENT_TAB]


QString createSearchPattern (QString userInput, bool allWords = true, bool fullWords = false, int spacing = 0)
{
    QStringList words = userInput.split(" ", QString::SkipEmptyParts);
    QString pattern;

    if (!fullWords)
    {
        for (unsigned i=0; i < words.size(); i++)
            words[i] = "[^ ]*" + words[i] + "[^ ]*";
    }
    else if (!allWords) // && fullWords
    {
        for (unsigned i=0; i < words.size(); i++)
            words[i] = " " + words[i] + " ";
    }

    unsigned i;
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

void MainWindow::on_SearchInBooksBTN_clicked()
{

    QString otxt = ui->searchInBooksLine->text();
    QString stxt = otxt;
    QRegExp regexp;

    //Search as regexp entered
    if (ui->regexpCheckBox->isChecked())
    {
        //Prevent greedy searches by mistake
        regexp = QRegExp(stxt);
        regexp.setMinimal(true);
        SearchInBooks(regexp, tr("RegExp: ") + otxt);
    }
    else if (ui->guematriaCheckBox->isChecked())
    {
        SearchGuematria (otxt);
    }
    else
    {
        if (ui->fuzzyCheckBox->isChecked())
            stxt = AllowKtivHasser(stxt);

        bool allwords = ui->radioButton_2->isChecked();
        bool fullwords = ui->fullCheckBox->isChecked();
        int spacing = ui->spinBox->value();

        if (ui->radioButton_3->isChecked())
            regexp = QRegExp(stxt);
        else
            regexp = QRegExp( createSearchPattern (stxt, allwords, fullwords, spacing) );
        regexp.setMinimal(true);

        SearchInBooks(regexp, otxt);
    }
}

void MainWindow::on_groupBox_toggled(bool checked)
{
    if (checked)
    {
        ui->guematriaCheckBox->setChecked(false);
        ui->regexpCheckBox->setChecked (false);
    }
}

void MainWindow::on_regexpCheckBox_toggled(bool checked)
{
    if (checked)
    {
        ui->guematriaCheckBox->setChecked(false);
        ui->groupBox->setChecked (false);
    }
}

void MainWindow::on_guematriaCheckBox_toggled(bool checked)
{
    if (checked)
    {
        ui->regexpCheckBox->setChecked(false);
        ui->groupBox->setChecked (false);
    }
}

void MainWindow::on_radioButton_2_toggled(bool checked)
{
    if (checked)
        ui->spinBox->setEnabled (true);
    else
        ui->spinBox->setEnabled (false);
}

void MainWindow::on_radioButton_3_toggled(bool checked)
{
    if (checked)
        ui->fullCheckBox->setEnabled (false);
    else
        ui->fullCheckBox->setEnabled (true);
}

void MainWindow::on_searchInBooksLine_returnPressed()
{
    on_SearchInBooksBTN_clicked();
}



#define RESULTS_MAX 500
void MainWindow::SearchInBooks (QRegExp& regexp, QString disp)
{
    //TODO: make preview look nice

    QString title, Html="", Htmlhead="", HtmlTopLinks="";

    QList <QString> text;

    vector<Book*> searchList = bookList.BooksInSearch();

    if ( regexp.pattern() != "" )
    {
        ui->pbarbox->show();
        ui->progressBar->setValue(5);
        float percentPerBook = (95.0 / searchList.size());

        //Make a new tab if the current tab has something in it
        if (ui->viewTab->tabText(CURRENT_TAB).indexOf(tr("Orayta")) == -1)
        {
            addViewTab(false);
            CurrentBookdisplayer->normalZoom();
            CurrentBookdisplayer->setHtml(simpleHtmlPage(tr("Orayta"), ""));
            ui->viewTab->setTabText(CURRENT_TAB, (tr("Orayta")));
        }

        bookDisplayer *pCurrentBookdisplayer = CurrentBookdisplayer;
        pCurrentBookdisplayer->ShowWaitPage();

        //Set the title of the tab to show what it's searching for
        title = tr("Searching: "); title += "\"" + disp + "\"" + " ...";
        ui->viewTab->setTabText(CURRENT_TAB, title);

        //Head and title of the Html
        title = tr("Search results: ") + "\"" + disp + "\"";

        Htmlhead = html_head(title);
        Htmlhead += "<body><div class=\"Section1\" dir=\"RTL\">";
        Htmlhead += "<div style=\"font-size:30px\"><b><i><center>";
        Htmlhead += title + ":" + "</center></i></b></div><BR>";
        Htmlhead += "\n<span style=\"font-size:17px\">";

        int results = 0;
        for (unsigned int i=0; i < searchList.size() && results < RESULTS_MAX && stopSearchFlag == false; i++)
        {
            ui->progressBar->setValue( 5 + (i + 1) * percentPerBook ) ;

            QList <SearchResult> searchResults = searchList[i]->findInBook(regexp);

            //Let the animation move...
            QApplication::processEvents();

            for (int j=0; j<searchResults.size() && results < RESULTS_MAX ; j++)
            {
                results ++;
                //Get the text best to show for this reult's description
                QString linkdisplay = "";

                //Regular books:
                if (!searchList[i]->getPath().contains("תלמוד") && !searchList[i]->getPath().contains("שס"))
                {

                    //This is an overkill, but I couldn't resist:
                    //linkdisplay += BookSearchDisplay (gBookList[i]->getNormallDisplayName() ,itr.toHumanString());

                    linkdisplay += searchList[i]->getNormallDisplayName() + " " + searchResults[j].itr.toHumanString();
                }
                //Gmarot:
                else
                {
                    linkdisplay += searchList[i]->getNormallDisplayName() + " ";
                    linkdisplay += searchResults[j].itr.toGmaraString();
                }


                //Add a small link (at the index) to the full result
                HtmlTopLinks += reddot() + "&nbsp;&nbsp;&nbsp;<a href=\"#" + stringify(results) + "\">" + stringify(results) + ")&nbsp;&nbsp;" + linkdisplay  + "</a><BR>";

                //Add the full result to the page
                Html += "<span style=\"font-size:23px\">";
                Html += "<a name=\"" + stringify(results) + "\"></a>";

                Html += "<a href=!" + stringify(searchList[i]->getUniqueId()) + ":";
                Html += searchResults[j].itr.toStringForLinks();
                Html += ":" + escapeToBase32(regexp.pattern()) + ">";

                Html += linkdisplay;
                Html += "</a><BR></span>\n";

                //Show result
                Html += searchResults[j].preview;

                Html += "<br><br><br>\n";
            }

        }

        if (stopSearchFlag == true)
        {
            Htmlhead += "<BR><BR>" + tr("(Search stopped by user)") + "<BR><BR>";
            stopSearchFlag = false;
        }

        if(results == 0)
        {
            //TODO: write better explenation
            Htmlhead +="<BR><BR>"; Htmlhead += tr("No search results found:"); Htmlhead += "\"" + disp + "\"";
            Htmlhead += "</B><BR>";
        }
        else
        {
            Htmlhead += "<B>"; Htmlhead += tr("Short result list: "); Htmlhead += "</B><BR>";
            if (results >= RESULTS_MAX)
            {
                Htmlhead += "(" +  stringify(results) + " " + "תוצאות ראשונות בלבד)" + "<BR><BR>";
            }
            else
            {
                Htmlhead += "(" +  stringify(results) + " " + "תוצאות נמצאו)" + "<BR><BR>";
            }
            Htmlhead += HtmlTopLinks;

            Htmlhead += "<BR><BR><B>" +  tr("Full result list:") + "</B><BR><BR>";
        }

        Html = Htmlhead + Html;

        Html += "</span></div>\n";
        Html += "\n</body>\n</html>";

        //TODO: Search results are special books
        writetofile(TMPPATH + "Search" + ".html", Html, "UTF-8");

        pCurrentBookdisplayer->load(QUrl(TMPPATH + "Search" + ".html"));

        ui->pbarbox->hide();
    }
}

QString lastSearch = "";

void MainWindow::on_searchForward_clicked()
{
    if (ui->lineEdit->text().replace(" ","") == "") return;

    QRegExp regexp = withNikudAndTeamim(ui->lineEdit->text());

    QString text = CurrentBookdisplayer->pageText();

    int ppp = CurrentBookdisplayer->searchPos();

    //Find the closest occurence of the RegExp
    QString next = "";

    int pos = regexp.indexIn(text, ppp + 1);
    if (pos > -1)
    {
        next = regexp.cap(0);
        CurrentBookdisplayer->setSearchPos(pos);
        CurrentBookdisplayer->searchText(next, false);
    }
}

void MainWindow::on_searchBackwords_clicked()
{
    if (ui->lineEdit->text().replace(" ","") == "") return;

    QString text = CurrentBookdisplayer->pageText();

    int ppp = CurrentBookdisplayer->searchPos();

    //Search only in the text before the cursor's position
    if (ppp != -1) text = text.mid(0, ppp - 1);

    QRegExp regexp = withNikudAndTeamim(ui->lineEdit->text());

    //Find the closest occurence of the RegExp, backwards
    QString last = "";

    int pos = regexp.lastIndexIn(text);
    if (pos > -1)
    {
        last = regexp.cap(0);
        CurrentBookdisplayer->setSearchPos(pos);
        CurrentBookdisplayer->searchText(last, true);
    }
}



static vector < pair <BookIter,QString> > TanachBookContent (Book* book)
{
    vector < pair <BookIter,QString> > ret;

    //Read book's contents
    QStringList text;
    if (!ReadFileToList( book->getName(), text, "UTF-8", true))
    {
        qDebug() << "Error reading the book's text" << book->getName();
        return ret;
    }

    BookIter itr;
    QString line;

    //If a line starts with one of these, it's a level sign
    QString levelSigns = "!@#$^~";
    //Any char not matchong this pattern, is no *pure* text.
    QRegExp notText("[^ א-ת]");
    //These are turned into spaces, and not just ignored.
    QString spaceSigns = "[-_:.,?]";

    for (int i=0; i<text.size(); i++)
    {
        //Level line
        if (levelSigns.contains(text[i][0]))
        {
            //Update iter
            itr.SetLevelFromLine(text[i]);
        }
        //Text line
        else
        {
            //Turn קרי וכתיב into only קרי. Maybe this should be an option?
            line = text[i].replace( QRegExp ("[(][^)]*[)] [[]([^]]*)[]]"), "\\1");

            //Remove all non "pure-text" chars
            line = line.replace(QChar(0x05BE), " "); //Makaf
            //line = line.replace(QRegExp(spaceSigns), " ");
            line = line.replace(notText, "");

            //Remove double spaces and line breaks
            line = line.simplified();

            ret.push_back( make_pair(itr,line) );
        }
    }

    return ret;
}


void MainWindow::SearchGuematria (QString txt)
{
/*
    QTime t1;
    t1.start();
*/

    QString title = "", Htmlhead = "", Html = "";

    int guematria = GematriaValueOfString ( txt );

    if ( guematria != 0 )
    {

        Book* tanachRoot = bookList.FindBookByName(" מקרא");
        if (tanachRoot == NULL)
        {
            qDebug("tanach root not found !");
            return;
        }

        vector<Book*> tanach = bookList.Children( tanachRoot );

        //Make a new tab if the current tab has something in it
        if (ui->viewTab->tabText(CURRENT_TAB).indexOf(tr("Orayta")) == -1)
        {
            addViewTab(false);
            CurrentBookdisplayer->normalZoom();
            CurrentBookdisplayer->setHtml(simpleHtmlPage(tr("Orayta"), ""));
        }

        ui->pbarbox->show();
        ui->progressBar->setValue(0);
        float percentPerBook = (100.0 / tanach.size());

        bookDisplayer *pCurrentBookdisplayer = CurrentBookdisplayer;
        pCurrentBookdisplayer->ShowWaitPage();

        //Set the title of the tab to show what it's searching for
        title = tr("Searching guematria for: ");
        title += "\"" + txt + "\"" + " ...";

        ui->viewTab->setTabText(CURRENT_TAB, title);


        int results = 0;
        for (unsigned i=0; i < tanach.size() && stopSearchFlag == false; i++)
        {
            vector < pair <BookIter,QString> > list = TanachBookContent ( tanach[i] );

            ui->progressBar->setValue( (i + 1) * percentPerBook ) ;

            for (unsigned j=0; j < list.size(); j++)
            {
                // split the pasuk in words
                QStringList words = list[j].second.split(" ");

                // retrieve guematria for all words
                vector<int> wordsValues = vector<int>(words.size());
                for (int n=0; n < words.size(); n++)
                    wordsValues[n] = GematriaValueOfString ( words[n] );

                //Let the animation move...
                QApplication::processEvents();

                // search the guematria
                int calc = wordsValues[0];
                unsigned first = 0, last = 0;
                while ( last < wordsValues.size() )
                {
                    if (calc == guematria)
                    {
                        results ++;

                        QString occ = "";
                        for (unsigned cmpt = first; cmpt <= last; cmpt++)
                        {
                            occ += words[cmpt];
                            if (cmpt < last)
                                occ += " ";
                        }

                        //Get the text best to show for this reult's description
                        QString linkdisplay = tanach[i]->getNormallDisplayName() + " " + list[j].first.toHumanString();

                        //Add the full result to the page
                        Html += stringify(results) + ") ";
                        Html += "<span style=\"font-size:20px\">";
                        Html += "<a href=!" + stringify(tanach[i]->getUniqueId()) + ":";
                        Html += list[j].first.toStringForLinks();
                        Html += ":" + escapeToBase32(occ) + ">";
                        Html += linkdisplay;
                        Html += "</a><br></span>\n";
                        Html += list[j].second.replace( QRegExp("(" + occ + ")"), "<span style='background-color:Yellow'>\\1</span>" );
                        Html += "<br><br>\n";

                        //qDebug(occ.toStdString().c_str());

                        first++;
                        last = first;
                        if (last < wordsValues.size())
                            calc = wordsValues[first];
                    }

                    while (calc < guematria)
                    {
                        last++;
                        if (last < wordsValues.size())
                            calc += wordsValues[last];
                        else
                            break;
                    }
                    while (calc > guematria)
                    {
                        calc -= wordsValues[first];
                        first++;
                    }
                }
/*

            for (unsigned j=0; j < list.size(); j++)
            {
                QStringList words = list[j].second.split(" ");

                vector<int> wordsValues = vector<int>(words.size());
                for (unsigned n=0; n < words.size(); n++)
                    wordsValues[n] = GematriaValueOfString ( words[n] );

                //Let the animation move...
                QApplication::processEvents();

                for (unsigned n=0; n < wordsValues.size(); n++)
                {
                    int calc = 0;
                    for (unsigned k = n; k < wordsValues.size() && calc < guematria; k++)
                    {
                        calc += wordsValues[k];
                        if (calc == guematria)
                        {
                            results ++;

                            QString occ = "";
                            for (unsigned cmpt = n; cmpt <= k; cmpt++)
                            {
                                occ += words[cmpt];
                                if (cmpt < k)
                                    occ += " ";
                            }

                            //Get the text best to show for this reult's description
                            QString linkdisplay = tanach[i]->getNormallDisplayName() + " " + list[j].first.toHumanString();

                            //Add the full result to the page
                            Html += "<span style=\"font-size:23px\">";
                            Html += "<a name=\"" + stringify(results) + "\"></a>";

                            Html += "<a href=!" + stringify(tanach[i]->getUniqueId()) + ":";
                            Html += list[j].first.toStringForLinks();
                            Html += ":" + escapeToBase32(occ) + ">";

                            Html += linkdisplay;
                            Html += "</a><br></span>\n";

                            Html += markText (list[j].second, occ) + "<br><br>\n";

                            //qDebug(occ.toStdString().c_str());

                            break;
                        }
                    }
                }
*/
            }
        }

        //Head and title of the Html
        title = tr("Search results for guematria : ") + "\"" + txt + "\"";
        title += " (" + stringify(guematria) + ")";

        Htmlhead = html_head(title);
        Htmlhead += "<body><div class=\"Section1\" dir=\"RTL\">";
        Htmlhead += "<div style=\"font-size:30px\"><b><i><center>";
        Htmlhead += title + ":" + "</center></i></b></div><BR>";
        Htmlhead += "\n<span style=\"font-size:17px\">";

        if (stopSearchFlag == true)
        {
            Htmlhead += "<BR><BR>" + tr("(Search stopped by user)") + "<BR><BR>";
            stopSearchFlag = false;
        }

        if(results == 0)
        {
            Htmlhead +="<br><br>"; Htmlhead += tr("No guematria results found:"); Htmlhead += "\"" + txt + "\"";
            Htmlhead += "</b><br>";
        }
        else
        {
            Htmlhead += "<b>"; Htmlhead += tr("Result list: ");
            Htmlhead += "</b> "+ QString::number(results) + tr(" results founds.") + "<br><br>";
        }

        Html = Htmlhead + Html;

        Html += "</span></div>\n";
        Html += "\n</body>\n</html>";

        /*
                qDebug("elapsed : %d ms", t1.elapsed());
        */

        writetofile(TMPPATH + "Guematria" + ".html", Html, "UTF-8");

        pCurrentBookdisplayer->load(QUrl(TMPPATH + "Guematria" + ".html"));

        ui->pbarbox->hide();
    }
}

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




QString createSearchPattern (QString userInput, bool allWords = true, bool fullWords = false, int spacing = 0)
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

void MainWindow::on_SearchInBooksBTN_clicked()
{

    QString otxt = ui->searchInBooksLine->text();
    QString stxt = otxt;
    QRegExp regexp;


    if (ui->guematriaCheckBox->isChecked())
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
    }
}

void MainWindow::on_guematriaCheckBox_toggled(bool checked)
{
    if (checked)
    {
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
void MainWindow::SearchInBooks (const QRegExp& regexp, QString disp)
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
            CurrentBookdisplayer()->normalZoom();
            CurrentBookdisplayer()->setHtml(simpleHtmlPage(tr("Orayta"), ""));
            ui->viewTab->setTabText(CURRENT_TAB, (tr("Orayta")));
        }

        bookDisplayer *pCurrentBookdisplayer = CurrentBookdisplayer();
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
    if (ui->lineEdit->text().replace(" ","") == "")
        return;

    if ( CurrentBookdisplayer()->book()->fileType() == Book::Normal )
    {
        QRegExp regexp = withNikudAndTeamim(ui->lineEdit->text());

        QString text = CurrentBookdisplayer()->pageText();

        int ppp = CurrentBookdisplayer()->searchPos();

        //Find the closest occurence of the RegExp
        QString next = "";

        int pos = regexp.indexIn(text, ppp + 1);
        if (pos > -1)
        {
            next = regexp.cap(0);
            CurrentBookdisplayer()->setSearchPos(pos);
            CurrentBookdisplayer()->searchText(next, false);
        }
    }
    else if ( CurrentBookdisplayer()->book()->fileType() == Book::Html )
    {
        CurrentBookdisplayer()->webview()->findText ( ui->lineEdit->text() );
    }
    else if ( CurrentBookdisplayer()->book()->fileType() == Book::Pdf )
    {
        CurrentBookdisplayer()->pdfview()->searchForwards( ui->lineEdit->text() );
    }
}

void MainWindow::on_searchBackward_clicked()
{
    if (ui->lineEdit->text().replace(" ","") == "")
        return;

    if ( CurrentBookdisplayer()->book()->fileType() == Book::Normal )
    {
        QString text = CurrentBookdisplayer()->pageText();

        int ppp = CurrentBookdisplayer()->searchPos();

        //Search only in the text before the cursor's position
        if (ppp != -1) text = text.mid(0, ppp - 1);

        QRegExp regexp = withNikudAndTeamim(ui->lineEdit->text());

        //Find the closest occurence of the RegExp, backwards
        QString last = "";

        int pos = regexp.lastIndexIn(text);
        if (pos > -1)
        {
            last = regexp.cap(0);
            CurrentBookdisplayer()->setSearchPos(pos);
            CurrentBookdisplayer()->searchText(last, true);
        }
    }
    else if ( CurrentBookdisplayer()->book()->fileType() == Book::Html )
    {
        CurrentBookdisplayer()->webview()->findText ( ui->lineEdit->text(), QWebPage::FindBackward );
    }
    else if ( CurrentBookdisplayer()->book()->fileType() == Book::Pdf )
    {
        CurrentBookdisplayer()->pdfview()->searchBackwards( ui->lineEdit->text() );
    }
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
            CurrentBookdisplayer()->normalZoom();
            CurrentBookdisplayer()->setHtml(simpleHtmlPage(tr("Orayta"), ""));
        }

        ui->pbarbox->show();
        ui->progressBar->setValue(0);
        float percentPerBook = (100.0 / tanach.size());

        bookDisplayer *pCurrentBookdisplayer = CurrentBookdisplayer();
        pCurrentBookdisplayer->ShowWaitPage();

        //Set the title of the tab to show what it's searching for
        title = tr("Searching guematria for: ");
        title += "\"" + txt + "\"" + " ...";

        ui->viewTab->setTabText(CURRENT_TAB, title);

        int results = 0, nbBooksInSearch = 0;
        for (unsigned i=0; i < tanach.size() && stopSearchFlag == false; i++)
        {
            if (tanach[i]->IsInSearch() == false)
                continue;

            nbBooksInSearch++;
            vector <GuematriaDb> bookGuematriaDb = tanach[i]->getGuematriaDb();

            ui->progressBar->setValue( (i + 1) * percentPerBook ) ;

            for (unsigned j=0; j < bookGuematriaDb.size(); j++)
            {
                //Let the animation move...
                QApplication::processEvents();

                // search the guematria
                int calc = bookGuematriaDb[j].values[0];
                unsigned first = 0, last = 0;
                while ( last < bookGuematriaDb[j].values.size() )
                {
                    if (calc == guematria)
                    {
                        results ++;

                        QString occ = "";
                        for (unsigned cmpt = first; cmpt <= last; cmpt++)
                        {
                            occ += bookGuematriaDb[j].words[cmpt];
                            if (cmpt < last) occ += " ";
                        }

                        //Get the text best to show for this reult's description
                        QString linkdisplay /* = tanach[i]->getNormallDisplayName() + " "*/;  // title not needed in tanach...
                        linkdisplay += bookGuematriaDb[j].itr.toHumanString();
                        //Add the full result to the page
                        Html += "<span style=\"font-size:20px\">";
                        Html += "<a href=!" + stringify(tanach[i]->getUniqueId()) + ":";
                        Html += bookGuematriaDb[j].itr.toStringForLinks();
                        Html += ":" + escapeToBase32(occ) + ">";
                        Html += stringify(results) + ") " + linkdisplay;
                        Html += "</a><br></span>\n";
                        Html += bookGuematriaDb[j].words.join(" ").replace( QRegExp("(" + occ + ")"), "<span style='background-color:Yellow'>\\1</span>" );
                        Html += "<br><br>\n";

                        last = ++first;
                        if (last < bookGuematriaDb[j].values.size())
                            calc = bookGuematriaDb[j].values[first];
                    }

                    while (calc < guematria)
                    {
                        last++;
                        if (last < bookGuematriaDb[j].values.size())
                            calc += bookGuematriaDb[j].values[last];
                        else
                            break;
                    }
                    while (calc > guematria)
                    {
                        calc -= bookGuematriaDb[j].values[first];
                        first++;
                    }
                }
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

        if (nbBooksInSearch == 0)
        {
            Htmlhead +="<br><br><center><b>";
            Htmlhead += tr("No tanach books selected : please select books in tanach and search again.");
            Htmlhead += "</b></center><br>";
        }
        else if(results == 0)
        {
            Htmlhead +="<br><b>";
            Htmlhead += tr("No guematria results found:");
            Htmlhead += "\"" + txt + "\"";
            Htmlhead += "</b><br>";
        }
        else
        {
            Htmlhead += "<b>";
            Htmlhead += tr("Result list: ");
            Htmlhead += "</b> "+ QString::number(results) + tr(" results founds.") + "<br><br>";
        }

        Html = Htmlhead + Html;

        Html += "</span></div>\n";
        Html += "\n</body>\n</html>";

/*
qDebug("elapsed : %d ms", t1.elapsed());
*/
        writetofile(TMPPATH + "Guematria.html", Html, "UTF-8");

        pCurrentBookdisplayer->load(QUrl(TMPPATH + "Guematria.html"));

        ui->pbarbox->hide();
    }
}

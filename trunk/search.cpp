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

//Another define becuase I'm lasy. "CurrentBook" simply represents
// the currently visible "bookdisplayer"
#define CurrentBook bookDisplayerList[CURRENT_TAB]


void MainWindow::on_SearchInBooksBTN_clicked()
{
    //TODO: כתיב מלא / חסר

    QString otxt = ui->searchInBooksLine->text();
    QString stxt = otxt;

    //Search as regexp entered
    if (ui->regexpCheckBox->isChecked())
    {
        //Prevent greedy searches by mistake
        QRegExp regexp(stxt);
        regexp.setMinimal(true);
        SearchInBooks(regexp, tr("RegExp: ") + otxt);
    }
    else
    {
        //Find any of the words
        if ( ui->radioButton->isChecked() )
        {
            QString pat = "";

            if (ui->fuzzyCheckBox->isChecked()) stxt = AllowKtivHasser(stxt);

            QStringList words = stxt.split(" ");
            for (int i=0; i<words.size(); i++)
            {
                if (words[i] != "")
                {
                    //See if to force full words
                    // (This is OK because the search DB has forced spaces at it's beginning and end too)
                    if ( ui->fullCheckBox->isChecked())
                        pat += " " + words[i] + " ";
                    else
                        pat += words[i]; //Part of word will be found too
                }
                if ( i != words.size()-1 ) pat+= "|";
            }
            SearchInBooks(QRegExp(pat), otxt);
        }
        //Search for all of the words
        else
        {
            if (ui->fuzzyCheckBox->isChecked()) stxt = AllowKtivHasser(stxt);

            if (ui->spinBox->value() > 0)
                stxt = stxt.replace(" ", " ([א-ת]+ ){0," + QString::number(ui->spinBox->value()) + "}");

            if ( ui->fullCheckBox->isChecked()) stxt = " " + otxt + " ";

            SearchInBooks(QRegExp(stxt), otxt);
        }
    }
}

void MainWindow::on_regexpCheckBox_stateChanged(int state)
{
    if (state == Qt::Checked)
        ui->groupBox->setEnabled (false);
    else
        ui->groupBox->setEnabled (true);
}

void MainWindow::on_radioButton_2_toggled(bool checked)
{
    if (checked)
        ui->spinBox->setEnabled (true);
    else
        ui->spinBox->setEnabled (false);
}

void MainWindow::on_searchInBooksLine_returnPressed()
{
    on_SearchInBooksBTN_clicked();
}



#define RESULTS_MAX 500
void MainWindow::SearchInBooks (QRegExp regexp, QString disp)
{
    //TODO: make preview look nice

    QString title, Html="",Htmlhead="", HtmlTopLinks="";

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
            CurrentBook->normalZoom();
            CurrentBook->setHtml(simpleHtmlPage(tr("Orayta"), ""));
            ui->viewTab->setTabText(CURRENT_TAB, (tr("Orayta")));
        }

        bookDisplayer *pCurrentBook = CurrentBook;
        pCurrentBook->ShowWaitPage();

        //Set the title of the tab to show what it's searching for
        title = tr("Searching: "); title += "\"" + disp + "\"" + " ...";
        ui->viewTab->setTabText(CURRENT_TAB,title);

        //Head and title of the Html
        title = tr("Search results: "); title +="\"" + disp+ "\"";
        Htmlhead = html_head(title);

        Htmlhead += "<body><div class=\"Section1\" dir=\"RTL\">";
        Htmlhead += "<div style=\"font-size:30px\"><b><i><center>";
        Htmlhead += title + ":" + "</center></i></b></div><BR>";

        Htmlhead += "\n<span style=\"font-size:17px\">";

        int results = 0;
        for (unsigned int i=0; i < searchList.size() && results <= RESULTS_MAX && stopSearchFlag == false; i++)
        {
            ui->progressBar->setValue( 5 + (i + 1) * percentPerBook ) ;

            QList <SearchResult> searchResults = searchList[i]->findInBook(regexp);

            //Let the animation move...
            QApplication::processEvents();

            for (int j=0; j<searchResults.size(); j++)
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
                HtmlTopLinks += reddot() + "&nbsp&nbsp&nbsp<a href=\"#" + stringify(results) + "\">" + linkdisplay  + "</a><BR>";

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
                Htmlhead += "(" +  stringify(RESULTS_MAX) + " ";
                Htmlhead += "תוצאות ראשונות בלבד )"; Htmlhead += "<BR><BR>";
            }
            Htmlhead += HtmlTopLinks;

            Htmlhead += "<BR><BR><B>" +  tr("Full result list:") + "</B><BR><BR>";
        }

        Html = Htmlhead + Html;

        Html += "</span></div>\n";
        Html += "\n</body>\n</html>";

        //TODO: Serch results are special books
        writetofile(TMPPATH + "Search" + ".html", Html, "UTF-8");

        pCurrentBook->load(QUrl(TMPPATH + "Search" + ".html"));

        ui->pbarbox->hide();

        stopSearchFlag = false;
    }
}

QString lastSearch = "";

void MainWindow::on_searchForward_clicked()
{
    if (ui->lineEdit->text().replace(" ","") == "") return;

    QRegExp regexp = withNikudAndTeamim(ui->lineEdit->text());

    //BEWARE: Amaizingly ugly hack! Watch out!

    //Mark current cursor position using JS, grab the Html, and hide the marking
    CurrentBook->execScript("markCursorPos();");

    QString text = CurrentBook->htmlSource();

    int ppp = text.indexOf("**|*|**");

    //Hide the marking in the html
    CurrentBook->execScript("unMarkCursorPos();");

    //Search only in the text after the cursor's position
    if (ppp != -1) text = text.mid(ppp + 7 + 1);

    //Find the closest occurence of the RegExp
    QString next = "";

    int pos = regexp.indexIn(text);
    if (pos > -1)
    {
        next = regexp.cap(0);
    }

    CurrentBook->searchText(next, false);
}

void MainWindow::on_searchBackwords_clicked()
{
    if (ui->lineEdit->text().replace(" ","") == "") return;

    //BEWARE: Amaizingly ugly hack! Watch out!

    //Mark current cursor position using JS, grab the Html, and hide the marking
    CurrentBook->execScript("markCursorPos();");

    QString text = CurrentBook->htmlSource();


    int ppp = text.indexOf("**|*|**");
    int ppb = text.indexOf("</SCRIPT");

    //Hide the marking in the html
    CurrentBook->execScript("unMarkCursorPos();");

    //Search only in the text before the cursor's position
    if (ppp != -1) text = text.mid(ppb, ppp - 1 - ppb);

    QRegExp regexp = withNikudAndTeamim(ui->lineEdit->text());

    //Find the closest occurence of the RegExp, backwards
    QString last = "";

    int pos = regexp.lastIndexIn(text);
    if (pos > -1) {
        last = regexp.cap(0);
    }
    else return;

    CurrentBook->searchText(last, true);
}


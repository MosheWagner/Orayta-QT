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
#include "bookdisplayer.h"


#define QT_USE_FAST_CONCATENATION
#define QT_USE_FAST_OPERATOR_PLUS

// This is just a very simple define. every place in the code,
//  "CURRENT_TAB" simply represents "ui->viewTab->currentIndex()".
#define CURRENT_TAB ui->viewTab->currentIndex()



static QString createSearchPattern (QString userInput, bool allWords = true, bool fullWords = false, int spacing = 0)
{
    QStringList words = userInput.split(" ", QString::SkipEmptyParts);
    QString pattern;

    if (!fullWords)
    {
        for (int i=0; i < words.size(); i++)
            words[i] = "[^ ]*" + words[i] + "[^ ]*";
    }
    else if (fullWords && !allWords)
    {
        for (int i=0; i < words.size(); i++)
            words[i] = " " + words[i] + " ";
    }

    if (allWords)
    {
        QString sep = (spacing == 0 ? " " : " ([א-ת]+ ){0," + QString::number(spacing) + "}");
        pattern = words.join(sep);

        if (fullWords)
            pattern = " " + pattern + " ";
    }
    else
    {
        pattern = "(" + words.join(")|(") + ")";
    }

    return pattern;
}

void MainWindow::on_searchInBooksLine_returnPressed()
{
    on_SearchInBooksBTN_clicked();
}

void MainWindow::on_SearchInBooksBTN_clicked()
{
    QString otxt = ui->searchInBooksLine->text();
    QString stxt = otxt;
    QRegExp regexp;

    if (otxt == "" || otxt == " ")
        return;

    if (ui->guematriaCheckBox->isChecked())
    {
        SearchGuematriaInTanach (otxt);
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

        SearchInSelectedBooks(regexp, otxt);
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


void MainWindow::SearchInSelectedBooks (const QRegExp& regexp, QString disp)
{
    //TODO: make preview look nice
    QTime tm;
    tm.start();

    //qDebug() << "search for " << regexp.pattern();

    QString title, Html="", Htmlhead="", HtmlTopLinks="";

    // ######### A ré-étudier : le type de books rencoyés par cette fonction devrait absolument avoir une methode search
    BookList searchList = ui->treeWidget->BooksInSearch();

    if ( regexp.pattern() != "" )
    {
        ui->pbarbox->show();
        ui->progressBar->setValue(5);
        float percentPerBook = (95.0 / searchList.size());

        BookDisplayer* pCurrentBookdisplayer = addViewTab();

        //Set the title of the tab to show what it's searching for
        title = tr("Searching: ") + "\"" + disp + "\"" + " ...";
        ui->viewTab->setTabText(CURRENT_TAB, title);

        //Head and title of the Html
        title = tr("Search results: ") + "\"" + disp + "\"";

        Htmlhead = html_head(title, gFont);
        Htmlhead += "<body><div class=\"Section1\" dir=\"RTL\">";
        Htmlhead += "<div style=\"font-size:30px\"><b><i><center>";
        Htmlhead += title + ":" + "</center></i></b></div><BR>";
        Htmlhead += "\n<span style=\"font-size:17px\">\n";

        int results = 0, totalResults = 0, nBooksInResults = 0;
        for (int i=0; i < searchList.size() && results < MAX_RESULTS && stopSearchFlag == false; i++)
        {
            ui->progressBar->setValue( 5 + (i + 1) * percentPerBook ) ;

            QList<SearchResult> searchResults = dynamic_cast<OraytaBookItem*>(searchList[i])->findInBook(regexp);

            //Let the animation move...
            QApplication::processEvents();

            if (searchResults.empty()) continue;

            nBooksInResults++;

            //Add a small link (at the index) to the full result
            HtmlTopLinks += reddot() + "&nbsp;&nbsp;<a onclick=\"paintWhoILinkTo(this)\" href=\"#" + QString::number(results + 1) + "\">";

            int resultsInThisBook = 0;
            for (int j=0; j < searchResults.size() && results < MAX_RESULTS; j++)
            {
                results++;
                resultsInThisBook += searchResults[j].nbResults;

                //Add the full result to the page
                Html += "<span style=\"font-size:23px\"><a id=\"id_" + QString::number(results)+ "\" name=\"" + QString::number(results) + "\"";
                Html += " href=\"" + searchResults[j].link + "\">";
                Html += QString::number(results) + ")&nbsp;" + searchResults[j].linkdisplay;
                if (searchResults[j].nbResults > 1)
                    Html += "&nbsp;<small><i>[" + QString::number(searchResults[j].nbResults) + tr(" occurences") +"]</i></small>";
                Html += "</a></span><BR>\n";

                //Show result
                Html += searchResults[j].preview;
                Html += "<br><br><br>\n";
            }
            totalResults += resultsInThisBook;

            HtmlTopLinks += searchList[i]->getTreeDisplayName() + "&nbsp;&nbsp;<small>(" + QString::number(resultsInThisBook) + tr(" results") + ")</small></a><BR>\n";
        }

        if (stopSearchFlag == true)
        {
            Htmlhead += "<BR><BR>" + tr("(Search stopped by user)") + "<BR><BR>";
            stopSearchFlag = false;
        }

        if (results == 0)
        {
            //TODO: write better explenation
            Htmlhead +="<BR><BR>" + tr("No search results found:");
            Htmlhead += "\"" + disp + "\"</B><BR>";
        }
        else
        {
            Htmlhead += "<B>" + tr("Short result list: ") + "</B><BR>";
            if (results >= MAX_RESULTS)
            {
                Htmlhead += "(" + QString::number(totalResults) + tr(" firsts results only") + ")<BR><BR>";
            }
            else
            {
                Htmlhead += "(" + QString::number(totalResults) + tr(" entries founds") + ") <BR><BR>";
            }
            Htmlhead += HtmlTopLinks;

            Htmlhead += "<BR><BR><B>" +  tr("Full result list:") + "</B><BR><BR>";
        }

        Html = Htmlhead + Html;

        Html += "</span></div>\n";
        Html += "\n</body>\n</html>";

        //TODO: Search results are special books
        // ########### On pourrait ajouter les pages de recherches à l'arbre, par exemple...
        writetofile(TMPPATH + "Search.html", Html, "UTF-8");

        pCurrentBookdisplayer->loadSearchPage(QUrl::fromLocalFile(TMPPATH + "Search.html"));

        ui->pbarbox->hide();
    }

    qDebug() << " elapsed " << tm.elapsed() << "ms";
}

static QList<OraytaBookItem*> getBooksChildren( BaseNodeItem* parent )
{
    QList<OraytaBookItem*> ret;

    for ( QList<BaseNodeItem*>::const_iterator it = parent->getChildren().begin(); it != parent->getChildren().end(); ++it )
    {
        if ( (*it)->nodetype() == BaseNodeItem::Leaf )
        {
            OraytaBookItem* obook = dynamic_cast<OraytaBookItem*>(*it);
            if (obook && obook->IsInSearch()) ret << obook;
        }
        else
            ret << getBooksChildren( *it );
    }

    return ret;
}

void MainWindow::SearchGuematriaInTanach( const QString& txt )
{
    QTime t1;
    t1.start();

    int guematria = GematriaValueOfString ( txt );
    if ( guematria == 0 )
        return;

    BaseNodeItem* tanachRoot = ui->treeWidget->tanachRoot();
    if (tanachRoot == NULL)
    {
        qDebug("tanach root not found !");
        return;
    }

    QString title = "", Htmlhead = "", Html = "";
    QList<OraytaBookItem*> tanach = getBooksChildren( tanachRoot );

    ui->pbarbox->show();
    ui->progressBar->setValue(0);
    float percentPerBook = (100.0 / tanach.size());

    BookDisplayer* pCurrentBookdisplayer = addViewTab();

    //Set the title of the tab to show what it's searching for
    title = tr("Searching guematria for: ") + "\"" + txt + "\"" + " ...";
    ui->viewTab->setTabText(CURRENT_TAB, title);

    int nb_results = 0, nbBooksInSearch = 0;
    for (int i=0; i < tanach.size() && stopSearchFlag == false; i++)
    {
        if ( !tanach[i]->IsInSearch() )
            continue;

        nbBooksInSearch++;

        QList<SearchResult> results = tanach[i]->findGuematria(guematria);

        ui->progressBar->setValue( (i + 1) * percentPerBook ) ;

        for (int j=0; j < results.size(); j++)
        {
            //Let the animation move...
            QApplication::processEvents();

            nb_results++;

            //Add the full result to the page
            Html += "<span style=\"font-size:20px\">";
            Html += "<a href=\"" + results[j].link + "\">";
            Html += QString::number(nb_results) + ") " + results[j].linkdisplay;
            Html += "</a><br></span>\n";
            Html += results[j].preview;
            Html += "<br><br>\n";
        }
    }

    //Head and title of the Html
    title = tr("Search results for guematria : ") + "\"" + txt + "\"";
    title += " (" + QString::number(guematria) + ")";

    Htmlhead = html_head(title, gFont);
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
    else if (nb_results == 0)
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
        Htmlhead += "</b> "+ QString::number(nb_results) + tr(" results founds.") + "<br><br>";
    }

    Html = Htmlhead + Html;

    Html += "</span></div>\n";
    Html += "\n</body>\n</html>";


    qDebug() << "  Guematria Search : elapsed : " << t1.elapsed() << " ms";

    writetofile(TMPPATH + "Guematria.html", Html, "UTF-8");

    pCurrentBookdisplayer->loadSearchPage(QUrl::fromLocalFile(TMPPATH + "Guematria.html"));

    ui->pbarbox->hide();
}

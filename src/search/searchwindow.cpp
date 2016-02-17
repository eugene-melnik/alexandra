/*************************************************************************************************
 *                                                                                                *
 *  file: searchwindow.cpp                                                                        *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2016 Eugene Melnik <jeka7js@gmail.com>                                     *
 *                                                                                                *
 *  Alexandra is free software; you can redistribute it and/or modify it under the terms of the   *
 *  GNU General Public License as published by the Free Software Foundation; either version 2 of  *
 *  the License, or (at your option) any later version.                                           *
 *                                                                                                *
 *  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;     *
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.     *
 *  See the GNU General Public License for more details.                                          *
 *                                                                                                *
 *  You should have received a copy of the GNU General Public License along with this program.    *
 *  If not, see <http://www.gnu.org/licenses/>.                                                   *
 *                                                                                                *
  *************************************************************************************************/

#include "searchwindow.h"
#include "filmslist/filmslistmodel.h"
#include "filmslist/filmslistproxymodel.h"
#include "tools/debug.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QStringList>


SearchWindow::SearchWindow( QWidget* parent ) : QDialog( parent )
{
    DebugPrintFunc( "SearchWindow::SearchWindow" );

    setupUi( this );
    setAttribute( Qt::WA_DeleteOnClose );

    connect( tvResult, &QTableView::doubleClicked, this, &SearchWindow::ShowInMainWindow );
    connect( bShow,    &QPushButton::clicked,      this, &SearchWindow::ShowInMainWindow );

    DebugPrintFuncDone( "SearchWindow::SearchWindow" );
}


SearchWindow::~SearchWindow()
{
    DebugPrint( "SearchWindow::~SearchWindow()" );
    delete proxyModel;
}


void SearchWindow::SetModel( QAbstractItemModel* model )
{
    DebugPrintFunc( "SearchWindow::SetModel" );

    this->model = model;
    ConfigureSimpleSearchTab();
    ConfigureAdvancedSearchTab();

      // Show all films
    FilmsListProxyModel* proxy = new FilmsListProxyModel( this );
    proxy->setSourceModel( model );
    SetupModel( proxy );
    SetupResultHeader();

    DebugPrintFuncDone( "SearchWindow::SetModel" );
}


/*************************************************************************************************
 *  "Simple search" tab                                                                           *
  *************************************************************************************************/

void SearchWindow::ConfigureSimpleSearchTab()
{
    cTitle->setProperty( "column", FilmItem::TitleColumn );
    cOriginalTitle->setProperty( "column", FilmItem::OriginalTitleColumn );
    cTagline->setProperty( "column", FilmItem::TaglineColumn );
    cYear->setProperty( "column", FilmItem::YearColumn );
    cGenre->setProperty( "column", FilmItem::GenreColumn );
    cCountry->setProperty( "column", FilmItem::CountryColumn );
    cDirector->setProperty( "column", FilmItem::DirectorColumn );
    cProducer->setProperty( "column", FilmItem::ProducerColumn );
    cScreenwriter->setProperty( "column", FilmItem::ScreenwriterColumn );
    cComposer->setProperty( "column", FilmItem::ComposerColumn );
    cBudget->setProperty( "column", FilmItem::BudgetColumn );
    cTags->setProperty( "column", FilmItem::TagsColumn );
    cStarring->setProperty( "column", FilmItem::StarringColumn );
    cDescription->setProperty( "column", FilmItem::DescriptionColumn );

    connect( bSelectAll0,   &QPushButton::clicked, this, &SearchWindow::SelectAll );
    connect( bUnselectAll0, &QPushButton::clicked, this, &SearchWindow::UnselectAll );
    connect( bSearchSimple, &QPushButton::clicked, this, &SearchWindow::SimpleSearchStart );
}


void SearchWindow::SetOptionsIsChecked( bool isChecked )
{
    for( QCheckBox* checkBox : gbSearchIn->findChildren<QCheckBox*>() )
    {
        checkBox->setChecked( isChecked );
    }
}


void SearchWindow::SimpleSearchStart()
{
    DebugPrintFunc( "SearchWindow::SimpleSearchStart" );
    QString keyword = eKeyword->text();
    QList<int> columns;

    for( QCheckBox* checkBox : gbSearchIn->findChildren<QCheckBox*>() )
    {
        if( checkBox->isChecked() )
        {
            columns.append( checkBox->property( "column" ).toInt() );
        }
    }

    if( keyword.isEmpty() || columns.isEmpty() )
    {
        QMessageBox::information( this, tr( "Simple search" ),
                                  tr( "Nothing to search. Input keyword and select at least one field." ) );
        return;
    }

    FilmsListProxyModel* proxy = new FilmsListProxyModel( this );
    proxy->setSourceModel( model );
    proxy->SetFilter( keyword, columns );

    DebugPrintFuncDone( "SearchWindow::SimpleSearchStart" );
    SetupModel( proxy );
}


/*************************************************************************************************
 *  "Advanced search" tab                                                                         *
  *************************************************************************************************/

void SearchWindow::ConfigureAdvancedSearchTab()
{
    QStringList genres, directors, countries, starring, screenwriters, tags;
    double ratingMin = 0, ratingMax = 0;
    int yearMin = 0, yearMax = 0;

    for( int row = 0; row < model->rowCount(); ++row )
    {
        double filmRating = model->index( row, FilmItem::RatingColumn ).data().toDouble();

          // Minimal rating
        if( ratingMin == 0 )
        {
            ratingMin = filmRating;
        }
        else if( filmRating < ratingMin )
        {
            ratingMin = filmRating;
        }

          // Minimal rating
        if( ratingMax == 0 )
        {
            ratingMax = filmRating;
        }
        else if( filmRating > ratingMax )
        {
            ratingMax = filmRating;
        }

        int filmYear = model->index( row, FilmItem::YearColumn ).data().toInt();

          // Minimal year
        if( yearMin == 0 )
        {
            yearMin = filmYear;
        }
        else if( filmYear < yearMin )
        {
            yearMin = filmYear;
        }

          // Maximal year
        if( yearMax == 0 )
        {
            yearMax = filmYear;
        }
        else if( filmYear > yearMax )
        {
            yearMax = filmYear;
        }

          // Genres
        QStringList filmGenres = model->index( row, FilmItem::GenreColumn ).data(FilmsListModel::StringListRole).toStringList();
        genres.append( filmGenres );
          // Directors
        QStringList filmDerectors = model->index( row, FilmItem::DirectorColumn ).data(FilmsListModel::StringListRole).toStringList();
        directors.append( filmDerectors );
          // Countries
        QStringList filmCountries = model->index( row, FilmItem::CountryColumn ).data(FilmsListModel::StringListRole).toStringList();
        countries.append( filmCountries );
          // Starring
        QStringList filmStarring = model->index( row, FilmItem::StarringColumn ).data(FilmsListModel::StringListRole).toStringList();
        starring.append( filmStarring );
          // Screenwriters
        QStringList filmScreenwriters = model->index( row, FilmItem::ScreenwriterColumn ).data(FilmsListModel::StringListRole).toStringList();
        screenwriters.append( filmScreenwriters );
          // Tags
        QStringList filmTags = model->index( row, FilmItem::TagsColumn ).data(FilmsListModel::StringListRole).toStringList();
        tags.append( filmTags );
    }

      // Ratings
    sbRatingFrom->setMinimum( ratingMin );
    sbRatingFrom->setMaximum( ratingMax );
    sbRatingTo->setMinimum( ratingMin );
    sbRatingTo->setMaximum( ratingMax );
      // Years
    sbYearFrom->setMinimum( yearMin );
    sbYearFrom->setMaximum( yearMax );
    sbYearTo->setMinimum( yearMin );
    sbYearTo->setMaximum( yearMax );

    QString titlePattern = "%1 (%2)";

      // Genres
    genres.sort();
    genres.removeDuplicates();
    gbGenre->setTitle( titlePattern.arg(gbGenre->title()).arg(genres.count()) );
    lwGenre->AddItems( genres );
      // Directors
    directors.sort();
    directors.removeDuplicates();
    gbDirector->setTitle( titlePattern.arg(gbDirector->title()).arg(directors.count()) );
    lwDirector->AddItems( directors );
      // Countries
    countries.sort();
    countries.removeDuplicates();
    gbCountry->setTitle( titlePattern.arg(gbCountry->title()).arg(countries.count()) );
    lwCountry->AddItems( countries );
      // Starring
    starring.sort();
    starring.removeDuplicates();
    gbStarring->setTitle( titlePattern.arg(gbStarring->title()).arg(starring.count()) );
    lwStarring->AddItems( starring );
      // Screenwriters
    screenwriters.sort();
    screenwriters.removeDuplicates();
    gbScreenwriter->setTitle( titlePattern.arg(gbScreenwriter->title()).arg(screenwriters.count()) );
    lwScreenwriter->AddItems( screenwriters );
      // Tags
    tags.sort();
    tags.removeDuplicates();
    gbTags->setTitle( titlePattern.arg(gbTags->title()).arg(tags.count()) );
    lwTags->AddItems( tags );

    connect( bSearchAdvanced, &QPushButton::clicked, this, &SearchWindow::AdvancedSearchStart );
}


void SearchWindow::AdvancedSearchStart()
{
    DebugPrintFunc( "SearchWindow::AdvancedSearchStart" );

    AdvancedSearchProxyModel* proxy = new AdvancedSearchProxyModel( this );
    proxy->setSourceModel( model );
    SetupAdvancedProxy( proxy );

    DebugPrintFuncDone( "SearchWindow::AdvancedSearchStart" );
    SetupModel( proxy );
}


void SearchWindow::SetupAdvancedProxy( AdvancedSearchProxyModel* proxy )
{
      // Title, description and tagline
    if( gbTitleContains->isChecked() && !eTitleContains->text().isEmpty() )
    {
        QStringList words = { eTitleContains->text() };
        proxy->AddColumnFilter( FilmItem::TitleColumn, words );

        if( cSearchInDescription->isChecked() )
        {
            proxy->AddColumnFilter( FilmItem::DescriptionColumn, words );
        }

        if( cSearchInTagline->isChecked() )
        {
            proxy->AddColumnFilter( FilmItem::TaglineColumn, words );
        }
    }

      // Year and rating
    if( gbYear->isChecked() )
    {
        QStringList years;

        for( int i = sbYearFrom->value(); i <= sbYearTo->value(); ++i )
        {
            years.append( QString::number(i) );
        }

        proxy->AddColumnFilter( FilmItem::YearColumn, years );
    }

    if( gbRating->isChecked() )
    {
        QStringList ratings;

        for( double i = sbRatingFrom->value(); i <= sbRatingTo->value(); i += 0.1 )
        {
            ratings.append( QString::number(i) );
        }

        proxy->AddColumnFilter( FilmItem::RatingColumn, ratings );
    }

      // Hide viewed and favourite
    if( cHideViewed->isChecked() )
    {
        proxy->SetHideViewed();
    }

    if( cHideFavourite->isChecked() )
    {
        proxy->SetHideFavourite();
    }

      // All the rest
    if( gbGenre->isChecked() )
    {
        proxy->AddColumnFilter( FilmItem::GenreColumn, lwGenre->GetSelectedItems() );
    }

    if( gbDirector->isChecked() )
    {
        proxy->AddColumnFilter( FilmItem::DirectorColumn, lwDirector->GetSelectedItems() );
    }

    if( gbCountry->isChecked() )
    {
        proxy->AddColumnFilter( FilmItem::CountryColumn, lwCountry->GetSelectedItems() );
    }

    if( gbStarring->isChecked() )
    {
        proxy->AddColumnFilter( FilmItem::StarringColumn, lwStarring->GetSelectedItems() );
    }

    if( gbScreenwriter->isChecked() )
    {
        proxy->AddColumnFilter( FilmItem::ScreenwriterColumn, lwScreenwriter->GetSelectedItems() );
    }

    if( gbTags->isChecked() )
    {
        proxy->AddColumnFilter( FilmItem::TagsColumn, lwTags->GetSelectedItems() );
    }

    proxy->ApplyFilter();
}


/*************************************************************************************************
 *  Global                                                                                        *
  *************************************************************************************************/

void SearchWindow::SetupModel( QAbstractProxyModel* proxy )
{
    delete proxyModel;
    proxyModel = proxy;
    tvResult->setModel( proxyModel );
    tvResult->horizontalHeader()->setSortIndicator( FilmItem::RatingColumn, Qt::DescendingOrder );
    lTotalFounded->setText( QString::number(proxy->rowCount()) );

    if( proxy->rowCount() == 0 )
    {
        QMessageBox::information( this, tr( "Simple search" ), tr( "Nothing was found." ) );
    }
}


void SearchWindow::SetupResultHeader()
{
    QHeaderView* header = tvResult->horizontalHeader();

      // Move to left
    header->moveSection( FilmItem::IsFavouriteColumn, 0 );
    header->moveSection( FilmItem::IsViewedColumn + 1, 0 );

      // Hide
    QList<int> hiddenColumns =
    {
        FilmItem::OriginalTitleColumn, FilmItem::ProducerColumn, FilmItem::ScreenwriterColumn,
        FilmItem::ComposerColumn,      FilmItem::BudgetColumn,   FilmItem::ViewsCountColumn,
        FilmItem::TaglineColumn,       FilmItem::StarringColumn, FilmItem::DescriptionColumn,
        FilmItem::TagsColumn,          FilmItem::FileNameColumn, FilmItem::PosterColumn
    };

    for( int column : hiddenColumns )
    {
        header->hideSection( column );
    }

      // Show
    QList< QPair<int,int> > shownColumns =
    {
        { FilmItem::TitleColumn,    200 }, { FilmItem::YearColumn,         50 },
        { FilmItem::GenreColumn,    100 }, { FilmItem::CountryColumn,     100 },
        { FilmItem::DirectorColumn, 100 }, { FilmItem::RatingColumn,       50 },
        { FilmItem::IsViewedColumn,  20 }, { FilmItem::IsFavouriteColumn,  20 }
    };

    for( QPair<int,int>& column : shownColumns )
    {
        header->showSection( column.first );
        header->resizeSection( column.first, column.second );
    }
}


void SearchWindow::ShowInMainWindow()
{
    QModelIndex index = tvResult->currentIndex();

    if( index.isValid() )
    {
        emit ShowSelectedItem( proxyModel->mapToSource(index) );
    }
}



#ifndef FILMTECHINFOVIEW_H
#define FILMTECHINFOVIEW_H

#include "abstractfilminfoview.h"
#include "alexandrasettings.h"

#include <QLabel>
#include <QMutex>

class FilmTechInfoView : public QLabel, public AbstractFilmInfoView
{
    Q_OBJECT

    public:
        explicit FilmTechInfoView( QWidget* parent = nullptr );
        ~FilmTechInfoView();

    public slots:
        void ShowInformation( const QModelIndex& index ) override;
        void Clear() override { clear(); repaint(); }

    signals:
        void ShortInfoLoaded( const QString& );

    protected slots:
        void showEvent( QShowEvent* event );

    private slots:
        void LoadTechnicalInfo( const QString& fileName );
        void ShowShortInfo( const QString& text ) { setText( text ); }

    private:
        AlexandraSettings* settings;
        QMutex mutexInfoLoad;
};

#endif // FILMTECHINFOVIEW_H


#ifndef FILMPOSTERVIEW_H
#define FILMPOSTERVIEW_H

#include "abstractfilminfoview.h"

#include <QLabel>

class FilmPosterView : public QLabel, public AbstractFilmInfoView
{
    Q_OBJECT

    public:
        explicit FilmPosterView( QWidget* parent = nullptr ) : QLabel( parent ) {}

    public slots:
        void ShowInformation( const QModelIndex& index ) override;
        void Clear() override;

    protected slots:
        void resizeEvent( QResizeEvent* event );
        void showEvent( QShowEvent* event );

    private:
        void SetPixmap( const QPixmap& pixmap );
};

#endif // FILMPOSTERVIEW_H

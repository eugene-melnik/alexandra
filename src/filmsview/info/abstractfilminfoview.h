
#ifndef ABSTRACTFILMINFOVIEW_H
#define ABSTRACTFILMINFOVIEW_H

#include <QModelIndex>

// TODO: comment

class AbstractFilmInfoView
{
    public:
        virtual ~AbstractFilmInfoView() {}

        virtual void ShowInformation( const QModelIndex& index ) = 0;
        virtual void Clear() = 0;
};

#endif // ABSTRACTFILMINFOVIEW_H


#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QListWidget>
#include <QString>
#include <QStringList>

class PlayListWidget : public QListWidget
{
    Q_OBJECT

    public:
        explicit PlayListWidget( QWidget* parent = nullptr );

    public slots:
        void AddItem( const QString& title, const QString& filePath );
        void Clear();

        bool IsEmpty() const { return( !count() ); }

        QStringList GetPathes() const;

    signals:
        void Cleared();

    private slots:
        void ShowContextMenu( const QPoint& pos );
        void RemoveFromList();
};

#endif // PLAYLISTWIDGET_H

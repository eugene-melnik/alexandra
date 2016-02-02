
#ifndef CUTLABEL_H
#define CUTLABEL_H


#include <QApplication>
#include <QFontMetrics>
#include <QLabel>


class CutLabel : public QLabel
{
    Q_OBJECT

    public:
        explicit CutLabel( QWidget* parent = nullptr ) : QLabel( parent ) {}

        QString text() const
        {
            if( fullString != EmptyString )
            {
                return( fullString );
            }
            else
            {
                return( QString() );
            }
        }

    public slots:
        void setText( const QString& text )
        {
            if( text.isEmpty() )
            {
                fullString = EmptyString;
            }
            else
            {
                fullString = text;
            }

            DisplayText();
        }

    protected:
        void resizeEvent( QResizeEvent* event ) override
        {
            QLabel::resizeEvent( event );
            DisplayText();
        }

        void DisplayText()
        {
            QFontMetrics fm( qApp->font() );
            QString displayString = fm.elidedText( fullString, Qt::ElideRight, this->width() );
            QLabel::setText( displayString );
        }

    private:
        QString fullString;

        const QString EmptyString = "-";
};


#endif // CUTLABEL_H


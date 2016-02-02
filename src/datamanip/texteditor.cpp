
#include "texteditor.h"
#include "tools/debug.h"

#include <QCloseEvent>
#include <QMessageBox>


TextEditor::TextEditor( QWidget* parent ) : QDialog( parent )
{
    setupUi( this );
    setAttribute( Qt::WA_DeleteOnClose );
    connect( bOk, &QPushButton::clicked, this, &TextEditor::ReturnData );
    connect( eTextEdit, &QPlainTextEdit::textChanged, this, &TextEditor::SetIsTextChanged );
}


void TextEditor::show()
{
    if( dataSource == nullptr )
    {
        DebugPrint( QString( "TextEditor::show() -- dataSource is nullptr!" ) );
    }
    else
    {
        eTextEdit->setPlainText( dataSource->text() );
        isTextChanged = false;
    }

    QDialog::show();
}


void TextEditor::closeEvent( QCloseEvent* event )
{
    if( isTextChanged )
    {
        int res = QMessageBox::question( this, windowTitle(), tr( "Are you sure you want to discard changes?" ) );

        if( res == QMessageBox::No )
        {
            event->ignore();
            return;
        }
    }

    QDialog::closeEvent( event );
}


void TextEditor::ReturnData()
{
    if( dataReceiver == nullptr )
    {
        DebugPrint( QString( "TextEditor::ReturnData() -- dataReceiver is nullptr!" ) );
    }
    else
    {
        dataReceiver->setText( eTextEdit->toPlainText() );
    }

    isTextChanged = false;
    close();
}


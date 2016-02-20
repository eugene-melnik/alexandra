/*************************************************************************************************
 *                                                                                                *
 *  file: texteditor.cpp                                                                          *
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

#include "texteditor.h"
#include "tools/debug.h"

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


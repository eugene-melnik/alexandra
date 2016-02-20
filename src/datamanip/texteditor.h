/*************************************************************************************************
 *                                                                                                *
 *  file: texteditor.h                                                                            *
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

#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H


#include <QDialog>


#include "cutlabel.h"
#include "ui_texteditor.h"


class TextEditor : public QDialog, protected Ui::TextEditor
{
    Q_OBJECT

    public:
        explicit TextEditor( QWidget* parent = nullptr );

        void SetDataSource( CutLabel* source ) { dataSource = source; }
        void SetDataReceiver( CutLabel* receiver ) { dataReceiver = receiver; }
        void SetTitle( const QString& title ) { setWindowTitle( tr( "Editing \"%1\"").arg(title) ); }

        void show();

    protected:
        void closeEvent( QCloseEvent* event ) override;
        void reject() override { close(); }

    protected:
        void SetIsTextChanged() { isTextChanged = true; }
        void ReturnData();

    private:
        CutLabel* dataSource = nullptr;
        CutLabel* dataReceiver = nullptr;

        bool isTextChanged = false;
};


#endif // TEXTEDITOR_H


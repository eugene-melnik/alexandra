
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

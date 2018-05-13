/*----------------------------------------------------------------------------

Name		mainwindow.h

Purpose		Program main window. Primary View/Controller.

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "parser.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    // Update the text within the browser
    void updateText( QString text );

private slots:
    // Loads a file into the program
    void loadFile();

    // The following group of slots update the parser
    void updatePort();
    void updateAddr();
    void updateObjIdx();
    void updateSubIdx();
    void updateType( PacketType pktType, bool checked );

    // Determines which buttons were selected, the updates the parser
    void parseChkBtnGrp( int );

private:
    // Connects all program signals and slots
    void connectSigSlot();
private:
    Ui::MainWindow *ui;

    Parser mParser;
};

#endif // MAINWINDOW_H

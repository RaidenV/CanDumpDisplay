/*----------------------------------------------------------------------------

Name		mainwindow.cpp

Purpose		Program main window. Primary View/Controller.

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QTextStream>

/*----------------------------------------------------------------------------

Name		MainWindow

Purpose		Constructor

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFont font;
    font.setWeight( QFont::DemiBold );
    font.setFamily("Courier");
    font.setPointSize(11);
    ui->mDumpBrowser->setFont( font );
    connectSigSlot();
}

/*----------------------------------------------------------------------------

Name		MainWindow

Purpose		Destructor

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
MainWindow::~MainWindow()
{
    delete ui;
}

/*----------------------------------------------------------------------------

Name		updateText

Purpose		Slot for updating text returned from the parser

Input       text - text from the parser

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
void MainWindow::updateText(QString text)
{
    ui->mDumpBrowser->setText( text );
}

/*----------------------------------------------------------------------------

Name		loadFile

Purpose		Loads a file into the program.

Input       text - text from the parser

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
void MainWindow::loadFile()
{
    QFileDialog diag(this);
    diag.setFileMode( QFileDialog::ExistingFile );

    QString fname = diag.getOpenFileName( this );

    QFile file( fname );
    if ( !file.open(QIODevice::ReadOnly) )
    {
        return;
    }
    QTextStream in( &file );
    QString txt = in.readAll();

    ui->mDumpBrowser->setText(txt);
    mParser.setText( txt );
}

/*----------------------------------------------------------------------------

Name		connectSigSlot

Purpose		Connects all signals and slots

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
void MainWindow::connectSigSlot()
{
    connect( ui->mActionOpen,       SIGNAL(triggered() ),
             this,                  SLOT( loadFile()) );

    connect( ui->mActionExit,       SIGNAL( triggered()),
             this,                  SLOT(close() ) );

    connect( ui->mPortEdit,         SIGNAL( editingFinished()),
             this,                  SLOT( updatePort() ) );

    connect( ui->mAddrEdit,         SIGNAL( editingFinished()),
             this,                  SLOT( updateAddr() ) );

    connect( ui->mObjIdxEdit,       SIGNAL( editingFinished()),
             this,                  SLOT( updateObjIdx() ) );

    connect( ui->mSubIdxEdit,       SIGNAL( editingFinished()),
             this,                  SLOT( updateSubIdx() ) );

    connect( ui->mTypeBtnGrp,       SIGNAL(buttonClicked( int ) ),
             this,                  SLOT(parseChkBtnGrp( int )) );

    connect( &mParser,              SIGNAL( textChanged(QString) ),
             this,                  SLOT( updateText( QString ) ) );

}

/*----------------------------------------------------------------------------

Name		updatePort

Purpose		Grabs the port information from the appropriate line edit and adds
            it to the parser.

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
void MainWindow::updatePort()
{
    mParser.setPort( ui->mPortEdit->text() );
}

/*----------------------------------------------------------------------------

Name		updateAddr

Purpose		Grabs the port information from the appropriate line edit and adds
            it to the parser.

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
void MainWindow::updateAddr()
{
    mParser.setAddr( ui->mAddrEdit->text() );
}

/*----------------------------------------------------------------------------

Name		updateObjIdx

Purpose		Grabs the port information from the appropriate line edit and adds
            it to the parser.

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
void MainWindow::updateObjIdx()
{
    mParser.setObjIdx( ui->mObjIdxEdit->text() );
}

/*----------------------------------------------------------------------------

Name		updateSubIdx

Purpose		Grabs the port information from the appropriate line edit and adds
            it to the parser.

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
void MainWindow::updateSubIdx()
{
    mParser.setSubIdx( ui->mSubIdxEdit->text() );
}

/*----------------------------------------------------------------------------

Name		updateType

Purpose		Updates the parser based on whether a button was selected or not.

Inputs      pktType - type of packet to add or remove
            checked - true if the checkbox is checked, false otherwise

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
void MainWindow::updateType(PacketType pktType, bool checked )
{
    ( checked ) ? mParser.addType( pktType ) : mParser.removeType( pktType );
}

/*----------------------------------------------------------------------------

Name		parseChkBtnGrp

Purpose		Parses the available buttons, adding the type to the parser

History		12 May 18  AFB	Created
----------------------------------------------------------------------------*/
void MainWindow::parseChkBtnGrp( int )
{
    updateType( NMT,        ui->mNmtChkBox->isChecked() );
    updateType( EMER,       ui->mEmerChkBox->isChecked() );
    updateType( TIME,       ui->mTimeChkBox->isChecked() );
    updateType( R_SDO,      ui->mrSdoChkBox->isChecked() );
    updateType( R_PDO_1,    ui->mrPdo1ChkBox->isChecked() );
    updateType( R_PDO_2,    ui->mrPdo2ChkBox->isChecked() );
    updateType( R_PDO_3,    ui->mrPdo3ChkBox->isChecked() );
    updateType( R_PDO_4,    ui->mrPdo4ChkBox->isChecked() );
    updateType( T_SDO,      ui->mtSdoChkBox->isChecked() );
    updateType( T_PDO_1,    ui->mtPdo1ChkBox->isChecked() );
    updateType( T_PDO_2,    ui->mtPdo2ChkBox->isChecked() );
    updateType( T_PDO_3,    ui->mtPdo3ChkBox->isChecked() );
    updateType( T_PDO_4,    ui->mtPdo4ChkBox->isChecked() );
    updateType( NODE_GUARD, ui->mNodeGuardChkBox->isChecked() );

}

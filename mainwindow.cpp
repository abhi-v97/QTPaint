#include <QtWidgets>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scribblearea.h"


MainWindow::MainWindow(QWidget *parent)

{
    // create and set the central widget
    scribbleArea = new ScribbleArea;
    setCentralWidget(scribbleArea);
    createActions();
    createMenus();

    // app title
    setWindowTitle(tr("Paint"));

    // app size
    resize(500, 500);
}

// close app event
void MainWindow::closeEvent(QCloseEvent *event){
    if(maybeSave()){
        event->accept();
    } else {
        event->ignore();
    }
}

// Check if image has changed, then try to open a file
void MainWindow::open(){

    // maybeSave returns true if no changes were made
    if(maybeSave()){
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
        
        // If a file exists, load it in scribbleArea
        if(!fileName.isEmpty()){
            scribbleArea->openImage(fileName);
        }

    }
}


// Called when Save As is clicked
void MainWindow::save(){
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}


// Open the colour wheel to choose pen colour
void MainWindow::penColour(){
    QColor newCol = QColorDialog::getColor(scribbleArea->penColour());
    if(newCol.isValid()){
        scribbleArea->setPenColour(newCol);
    }
}

// modify pen width
void MainWindow::penWidth(){
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Paint"),
                                        tr("Select Pen Width: "),
                                        scribbleArea->penWidth(), 1, 50, 1, &ok);

    if(ok){
        scribbleArea->setPenWidth(newWidth);
    }
}


// About QT dialog
void MainWindow::about(){
    QMessageBox::about(this, tr("About Paint"), tr("Sample Paint App"));
}


// Menu actions which call functions
void MainWindow::createActions(){
    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));


    // gets list of supported image formats
    // creates an action for each file format
    // each option is added to the Save As function
    foreach(QByteArray format, QImageWriter::supportedImageFormats()){
        QString text = tr("%1...").arg(QString(format).toUpper());
        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        saveAsActs.append(action);
    }
    printAct = new QAction(tr("&Print..."));
    connect(printAct, SIGNAL(triggered()), scribbleArea, SLOT(print()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    penColourAct = new QAction(tr("&Pen Colour..."), this);
    connect(penColourAct, SIGNAL(triggered()), this, SLOT(penColour()));

    penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    // Action which clears the screen
    clearAct = new QAction(tr("&Clear Screen"), this);
    clearAct->setShortcut(tr("Ctrl+L"));
    connect(clearAct, SIGNAL(triggered()),
            scribbleArea, SLOT(clearImage()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    // Create about Qt action and tie to MainWindow::aboutQt()
    aboutQTAct = new QAction(tr("About &Qt..."), this);
    connect(aboutQTAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

}

// Handles the menu bar
void MainWindow::createMenus(){
    saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach(QAction *action, saveAsActs)
        saveAsMenu->addAction(action);
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColourAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQTAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}

bool MainWindow::maybeSave(){

    // check for changes 
    if(scribbleArea->isModified()){
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Scribble"),
                tr("The image has been modified.\n" "Do you want to save your changes?"),
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if(ret == QMessageBox::Save){
            return saveFile("png");
        } else if (ret == QMessageBox::Cancel){
            return false;
        }
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat){
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));

    if(fileName.isEmpty()){
            return false;
    } else {
        return scribbleArea->saveImage(fileName, fileFormat.constData());
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}



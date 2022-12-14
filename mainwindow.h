#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
// drawing board where the magic happens
class ScribbleArea;

class MainWindow : public QMainWindow
{
    // Declares class to be a QTobject
    // handles events
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

// events that can be triggered
private slots:
    void open();
    void save();
    void penColour();
    void penWidth();
    void about();

private:
// actions and functions used by the program
    Ui::MainWindow *ui;
    void createActions();
    void createMenus();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);
    ScribbleArea *scribbleArea;
    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *helpMenu;
    QAction *openAct;
    QList<QAction *> saveAsActs;
    QAction *penColourAct;
    QAction *penWidthAct;
    QAction *printAct;
    QAction *clearAct;
    QAction *aboutAct;
    QAction *aboutQTAct;
    QAction *exitAct;

};
#endif // MAINWINDOW_H

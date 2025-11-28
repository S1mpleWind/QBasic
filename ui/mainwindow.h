#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qtextbrowserstream.h"

#include "../core/program.h"
#include "../interpreter/interpreter.h"
#include "../runtime/parser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void run();
    void runParser(QString &);
    //void runInterpreter();


private slots:
    void on_cmdLineEdit_editingFinished();

    //handle with the btns
    void on_btnLoadCode_clicked();
    void on_btnRunCode_clicked();
    void on_btnClearCode_clicked();

private:
    Ui::MainWindow *ui;

    QTextBrowserStream *coutRedirect;

    // console part
    Program program{};
    Interpreter interpreter{};
    Parser parser{};

    // code text -> program.source && parser -> program.setstatement ->interpreter run && syntax tree;


    // NOTE : call this function every time before running
    void resetAll(){
        program.clear();
        interpreter.reset();
        parser = Parser();
    };

};
#endif // MAINWINDOW_H

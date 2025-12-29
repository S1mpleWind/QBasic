#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <iostream>
#include <sstream>
#include <QInputDialog>

// load file
#include <QMessageBox>
#include <QFileDialog>
#include <QRegularExpression>
#include <QTextStream>


// quit
#include <QApplication>

// update display
#include <QTimer>

//load
#include <QTextStream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //coutRedirect = new QTextBrowserStream(ui->textBrowser);
    //coutRedirect ->install();
    resetAll();
    ui->CodeDisplay->setText("");
    ui->textBrowser->setText(
        "welcome to Qbasic.\n"
        "Key in 'help' to see more help"
        );

    ui->treeDisplay->setText("");
}

MainWindow::~MainWindow()
{
    delete ui;
    //coutRedirect->uninstall();
    //delete coutRedirect;
}

void MainWindow::on_cmdLineEdit_editingFinished()
{
    QString cmd = ui->cmdLineEdit->text();


    /* handle command from cmdline */

    if(cmd.toUpper()==QString("HELP")) {
        showHelp();
        return;
    }

    if(cmd.toUpper()==QString("QUIT")) {
        //showHelp();
        QApplication::quit();
        return;
    }

    if(cmd.toUpper()==QString("CLEAR")) {
        on_btnClearCode_clicked();
        return;
    }

    if(cmd.toUpper()==QString("LOAD")) {
        on_btnLoadCode_clicked();
        return;
    }

    //qDebug()<<cmd;
    ui->cmdLineEdit->setText("");

    ui->CodeDisplay->append(cmd);

    try{
        runParser(cmd);
    }
    catch (const std::runtime_error &e) {
        QMessageBox::critical(this, "Syntax Error", QString::fromStdString(e.what()));
        return;
    } // 停止运行，用户修正代码


    // deliver "this" for lambda to recognize functiosn
    QTimer::singleShot(1000, this, [this](){
        updateCodeDisplay();
    });


}


void MainWindow::on_btnLoadCode_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "选择 BASIC 程序文件",
        "",
        "Text Files (*.txt *.bas);;All Files (*)"
        );

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件");
        return;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    QString text = in.readAll();

    file.close();

    parseTextIntoProgram(text);

    updateCodeDisplay();   // update to ui
}




void MainWindow::on_btnClearCode_clicked()
{
    qDebug()<<"CLEAR";

    // reset data structure
    resetAll();

    //reset ui
    ui->CodeDisplay->setText("");
    ui->textBrowser->setText("");
    ui->treeDisplay->setText(""); // TODO: should clear the datastructure and update automatically

    //TODO:clear the program structure / sytax tree

}

void MainWindow::on_btnRunCode_clicked(){
    ui->textBrowser->clear();
    program.resetStateCount();
    run();
}


/*
 * run the code
 */

void MainWindow::run(){


    Interpreter itp;
    //interpreter.reset();

    QTextBrowserStream qout(ui->textBrowser);
    std::streambuf *oldBuf = std::cout.rdbuf(&qout);

    //qDebug()<<"before setting provider";

    // use lambda
    itp.setInputProvider([this]() -> QString {
        bool ok;
        QString s = QInputDialog::getText(
            this,
            "INPUT",
            "请输入一个整数：",
            QLineEdit::Normal,
            "",
            &ok
            );
        return ok ? s : "";
    });


    //robust
    try{
        itp.run(program);
    }
    catch (const std::runtime_error &e) {
        QMessageBox::critical(this, "Runtime Error", QString::fromStdString(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Unknown Error", "An unknown error occurred during execution.");
    }


    // 4. restore cout
    std::cout.rdbuf(oldBuf);

    std::string tree = itp.toSyntaxTree(program);

    ui->treeDisplay->setPlainText(QString::fromStdString(tree));


}


// from cmdLineEdit->text() to program
void MainWindow::runParser(QString & qline)
{

    //qDebug()<<qline;

    if (qline.trimmed().isEmpty()) return;

    std::string line = qline.toStdString();

    // seperate line and source code from "10 LET X = 1"
    std::stringstream ss(line);
    int lineNumber;
    ss >> lineNumber;

    //qDebug()<<lineNumber;

    if (ss.fail()) {
        ui->textBrowser->append("行号解析失败");
        return;
    }

    std::string code;



    std::getline(ss, code);

    if (!code.empty() && code[0] == ' ') code = code.substr(1);
    qDebug()<<code;


    program.addSourceLine(lineNumber, code);


    Statement* stmt = parser.parseLine(lineNumber, code);

    if (stmt == nullptr) {
        ui->textBrowser->append("语句解析失败\n");
        return;
    }
    qDebug()<<stmt->toString();
    program.setParsedStatement(lineNumber, stmt);



}


void MainWindow::showHelp()
{
    QMessageBox::information(
        this,
        "帮助",
        "这是你的 BASIC 编译器帮助窗口。\n\n"
        "支持的指令：\n"
        "  LET 变量名 = 表达式\n"
        "  PRINT 表达式\n"
        "  INPUT 变量名\n"
        "  IF 条件 THEN 行号\n"
        "  GOTO 行号\n"
        "  END\n\n"
        "你也可以使用 LOAD 从文件加载程序。\n"
        "当你准备好之后，使用 RUN 来运行程序，\n"
        "你也可以使用 CLEAR 来清空当前的程序"
        );
}


void MainWindow::updateCodeDisplay(){
    int firstline = program.getFirstLineNumber();
    if (firstline < 0 ) return;

    std::string sortedSourceCode;

    program.setNextLine(firstline);

    int currentline = program.getNextLine();

    while(currentline!= -1 && ! program.isEnded()){
        sortedSourceCode += std::to_string(currentline)+" "
                            + program.getSourceLine(currentline)
                            + "\n";

        currentline = program.getNextLineNumber(currentline);
    }

    ui->CodeDisplay->setText(QString::fromStdString(sortedSourceCode));
}


void MainWindow::parseTextIntoProgram(const QString &text)
{
    QStringList lines = text.split('\n');

    int count = 0;
    for (QString line : lines) {

        line = line.trimmed();
        qDebug() << "[LINE]" << line;

        if (line.isEmpty()) continue;
        count++;

        // match: 行号 + 代码
        QRegularExpression re(R"(^\s*(\d+)\s+(.*)$)");
        auto match = re.match(line);

        if (!match.hasMatch()) {
            QMessageBox::warning(this, "格式错误", "无法解析行: " + line);
            continue;
        }

        int lineNumber = match.captured(1).toInt();
        QString code = match.captured(2);

        program.addSourceLine(lineNumber, code.toStdString());

        try {
            Statement *stmt = parser.parseLine(
                lineNumber, code.toStdString());
            program.setParsedStatement(lineNumber, stmt);
        }
        catch (const std::exception &e) {
            QMessageBox::warning(this, "解析错误",
                                 "行 " + QString::number(lineNumber) +
                                     " 解析失败: " + e.what());
        }
    }

    qDebug() << "[PARSED LINES]" << count;
}




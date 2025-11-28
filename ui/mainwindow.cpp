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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //coutRedirect = new QTextBrowserStream(ui->textBrowser);
    //coutRedirect ->install();
    resetAll();
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

    //qDebug()<<cmd;
    ui->cmdLineEdit->setText("");

    ui->CodeDisplay->append(cmd);

    runParser(cmd);
}



void MainWindow::on_btnLoadCode_clicked()
{
    // 1. 选择文件
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "选择 BASIC 程序文件",
        "",
        "BASIC Files (*.txt *.bas);;All Files (*)"
        );

    if (fileName.isEmpty()) return;

    // 2. 清空当前 program
    program.clear();
    ui->textBrowser->clear();
    ui->treeDisplay->clear();

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件");
        return;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        // 3. 拆分行号 + 内容
        QRegularExpression re(R"(^\s*(\d+)\s+(.*)$)");
        auto match = re.match(line);

        if (!match.hasMatch()) {
            QMessageBox::warning(this, "格式错误",
                                 "无法解析行: " + line);
            continue;
        }

        int lineNumber = match.captured(1).toInt();
        QString code = match.captured(2);

        // 4. 加入源代码
        program.addSourceLine(lineNumber, code.toStdString());

        // 5. 解析并加入 AST
        try {
            Statement* stmt = parser.parseLine(
                lineNumber,
                code.toStdString()
                );
            program.setParsedStatement(lineNumber, stmt);
        }
        catch (const std::exception &e) {
            QMessageBox::warning(this, "解析错误",
                                 "行 " + QString::number(lineNumber) +
                                     " 解析失败: " + e.what());
        }
    }

    file.close();

    // 6. 用户提示
    QMessageBox::information(this, "完成", "程序加载成功！");
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

    qDebug()<<"before setting provider";

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

    itp.run(program);

    // 4. 恢复 cout
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

    // 加入 Program 的 source map
    program.addSourceLine(lineNumber, code);

    // 使用 Parser 把这行转为 Statement
    Statement* stmt = parser.parseLine(lineNumber, code);

    if (stmt == nullptr) {
        ui->textBrowser->append("语句解析失败");
        return;
    }
    qDebug()<<stmt->toString();
    program.setParsedStatement(lineNumber, stmt);



}


// void MainWindow::runInterpreter() {
//     ui->textBrowser->clear();       // 清空输出
//     ui->treeDisplay->clear();       // 清空语法树

//     // 1. 重定向 cout 到 resultTextBrowser
//    ///QTextBrowserStream qout(ui->textBrowser);
//     //std::streambuf *oldBuf = std::cout.rdbuf(&qout);

//     // 2. 运行解释器
//     //qDebug()<<program.getDisplayText();


//     interpreter.run(program);

//     qDebug()<<"here";
//     // 3. 获取 syntax tree 字符串并显示
//     std::string tree = interpreter.toSyntaxTree(program);

//     //qDebug()<<tree;
//     ui->treeDisplay->setPlainText(QString::fromStdString(tree));

//     // 4. 恢复 cout
//     //std::cout.rdbuf(oldBuf);
// }




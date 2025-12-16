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

// Main window for BASIC interpreter GUI
// Handles UI interactions, code parsing, and execution
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor and destructor
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Execute loaded program
    void run();
    
    // Parse single line of code
    void runParser(QString &);

    // Display help information
    void showHelp();

    // Update code display in UI
    void updateCodeDisplay();

    // Convert user input text into Program object
    void parseTextIntoProgram(const QString &text);
    
    // Decode text file content
    QString decodeTextFile(const QByteArray &raw);

private slots:
    // Handle command line input completion
    void on_cmdLineEdit_editingFinished();

    // Button event handlers
    void on_btnLoadCode_clicked();    // Load code file
    void on_btnRunCode_clicked();      // Run program
    void on_btnClearCode_clicked();    // Clear program

private:
    Ui::MainWindow *ui;

    // Redirect cout to UI text browser
    QTextBrowserStream *coutRedirect;

    // Core interpreter components
    Program program{};                 // Parsed program structure
    Parser parser{};                   // Parser for converting code to AST

    // Reset all interpreter state before running
    // Call this before each run to clear variables and execution state
    void resetAll(){
        program.clear();
        parser = Parser();
    };

};
#endif // MAINWINDOW_H

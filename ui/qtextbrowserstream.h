// QTextBrowserStream.h
#pragma once
#include <streambuf>
#include <string>
#include <QTextBrowser>

class QTextBrowserStream : public std::streambuf {
public:
    QTextBrowserStream(QTextBrowser *browser)
        : browser(browser) {}

protected:
    virtual int_type overflow(int_type ch) override {
        if (ch != EOF) {
            buffer.push_back(char(ch));
            if (ch == '\n') {
                flushBuffer();
            }
        }
        return ch;
    }

    virtual int sync() override {
        flushBuffer();
        return 0;
    }

private:
    void flushBuffer() {
        if (!buffer.empty()) {
            browser->insertPlainText(QString::fromStdString(buffer));
            buffer.clear();
        }
    }

private:
    QTextBrowser *browser;
    std::string buffer;
};

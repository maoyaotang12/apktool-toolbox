#include "widget.h"
#include <QApplication>
#include <QStyleHints>
#include <QPalette>
#include <QStyle>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // ========== 设置窗口图标 + 任务栏图标 ==========
    a.setWindowIcon(QIcon("apktool-toolbox.png"));

    // ========== 自动同步系统主题 ==========
    auto syncTheme = [&]() {
        bool dark = (a.styleHints()->colorScheme() == Qt::ColorScheme::Dark);
        QPalette pal;

        if (dark) {
            pal.setColor(QPalette::Window, QColor(30,30,30));
            pal.setColor(QPalette::WindowText, Qt::white);
            pal.setColor(QPalette::Base, QColor(45,45,45));
            pal.setColor(QPalette::Text, Qt::white);
            pal.setColor(QPalette::Button, QColor(60,60,60));
            pal.setColor(QPalette::ButtonText, Qt::white);
            pal.setColor(QPalette::Highlight, QColor(0,122,204));
            pal.setColor(QPalette::HighlightedText, Qt::white);
        } else {
            pal = a.style()->standardPalette();
        }

        a.setPalette(pal);
        for (QWidget *w : a.allWidgets()) {
            w->setPalette(pal);
            w->update();
        }
    };

    syncTheme();
    QObject::connect(a.styleHints(), &QStyleHints::colorSchemeChanged, &a, syncTheme);

    Widget w;
    w.show();
    return a.exec();
}

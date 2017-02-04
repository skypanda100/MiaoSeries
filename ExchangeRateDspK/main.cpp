#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QFile qss(":/style/dracula.css");
//    qss.open(QFile::ReadOnly);
//    qApp->setStyleSheet(qss.readAll());
//    qss.close();

    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QPalette palette;
    palette.setColor(QPalette::Active, QPalette::Window, QColor(53, 53, 53));
    palette.setColor(QPalette::Active, QPalette::Base, QColor(35, 35, 35));
    palette.setColor(QPalette::Active, QPalette::AlternateBase, QColor(53, 53, 53));
    palette.setColor(QPalette::Active, QPalette::Button, QColor(53, 53, 53));
    palette.setColor(QPalette::Active, QPalette::Light, QColor(63, 63, 63));
    palette.setColor(QPalette::Active, QPalette::Dark, QColor(43, 43, 43));
    palette.setColor(QPalette::Active, QPalette::Mid, QColor(48, 48, 48));
    palette.setColor(QPalette::Active, QPalette::Highlight, QColor(75, 110, 175));
    palette.setColor(QPalette::Active, QPalette::ToolTipBase, QColor(92, 92, 66));
    palette.setColor(QPalette::Active, QPalette::Shadow, QColor(100, 100, 100));
    palette.setColor(QPalette::Active, QPalette::WindowText, QColor(187, 187, 187));
    palette.setColor(QPalette::Active, QPalette::ToolTipText, QColor(187, 187, 187));
    palette.setColor(QPalette::Active, QPalette::Text, QColor(187, 187, 187));
    palette.setColor(QPalette::Active, QPalette::ButtonText, QColor(187, 187, 187));
    palette.setColor(QPalette::Active, QPalette::BrightText, QColor(255, 255, 255));
    palette.setColor(QPalette::Active, QPalette::HighlightedText, QColor(255, 255, 255));

    palette.setColor(QPalette::Inactive, QPalette::Window, QColor(53, 53, 53));
    palette.setColor(QPalette::Inactive, QPalette::Base, QColor(35, 35, 35));
    palette.setColor(QPalette::Inactive, QPalette::AlternateBase, QColor(53, 53, 53));
    palette.setColor(QPalette::Inactive, QPalette::Button, QColor(53, 53, 53));
    palette.setColor(QPalette::Inactive, QPalette::Light, QColor(63, 63, 63));
    palette.setColor(QPalette::Inactive, QPalette::Dark, QColor(43, 43, 43));
    palette.setColor(QPalette::Inactive, QPalette::Mid, QColor(48, 48, 48));
    palette.setColor(QPalette::Inactive, QPalette::Highlight, QColor(75, 110, 175));
    palette.setColor(QPalette::Inactive, QPalette::ToolTipBase, QColor(92, 92, 66));
    palette.setColor(QPalette::Inactive, QPalette::Shadow, QColor(100, 100, 100));
    palette.setColor(QPalette::Inactive, QPalette::WindowText, QColor(187, 187, 187));
    palette.setColor(QPalette::Inactive, QPalette::ToolTipText, QColor(187, 187, 187));
    palette.setColor(QPalette::Inactive, QPalette::Text, QColor(187, 187, 187));
    palette.setColor(QPalette::Inactive, QPalette::ButtonText, QColor(187, 187, 187));
    palette.setColor(QPalette::Inactive, QPalette::BrightText, QColor(255, 255, 255));
    palette.setColor(QPalette::Inactive, QPalette::HighlightedText, QColor(255, 255, 255));

    palette.setColor(QPalette::Disabled, QPalette::Window, QColor(53, 53, 53));
    palette.setColor(QPalette::Disabled, QPalette::Base, QColor(73, 73, 73));
    palette.setColor(QPalette::Disabled, QPalette::AlternateBase, QColor(53, 53, 53));
    palette.setColor(QPalette::Disabled, QPalette::Button, QColor(37, 37, 37));
    palette.setColor(QPalette::Disabled, QPalette::Light, QColor(47, 47, 47));
    palette.setColor(QPalette::Disabled, QPalette::Dark, QColor(27, 27, 27));
    palette.setColor(QPalette::Disabled, QPalette::Mid, QColor(32, 32, 32));
    palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(75, 110, 175));
    palette.setColor(QPalette::Disabled, QPalette::ToolTipBase, QColor(92, 92, 66));
    palette.setColor(QPalette::Disabled, QPalette::Shadow, QColor(100, 100, 100));
    palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(100, 100, 100));
    palette.setColor(QPalette::Disabled, QPalette::ToolTipText, QColor(187, 187, 187));
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor(187, 187, 187));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(100, 100, 100));
    palette.setColor(QPalette::Disabled, QPalette::BrightText, QColor(255, 255, 255));
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(255, 255, 255));

    QApplication::setPalette(palette);

    MainWindow w;
    w.show();

    return a.exec();
}

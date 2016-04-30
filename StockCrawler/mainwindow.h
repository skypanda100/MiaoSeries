#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onSearchClicked();
    void query(QNetworkReply *);

private:
    void initUI();
    void initConnect();

private:
    QLineEdit *addrEdit;
    QPushButton *searchButton;
    QPlainTextEdit *resultEdit;
    QProgressBar *circleProgress;

    QString titles[10];
    QNetworkAccessManager *mgr;

};

#endif // MAINWINDOW_H

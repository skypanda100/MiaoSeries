#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "ea_net.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void startService();
    void stopService();

public slots:
    void onStartClicked();
    void onStopClicked();
    void onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason);
    void onQuitActionTriggered();

protected:
    void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;

private:
    void initUI();
    void initConnect();

private:
    QPushButton *startButton;
    QPushButton *stopButton;
    ExchangeRateNet *eaNet;

    QAction *quitAction;
    QSystemTrayIcon *trayicon;
    QMenu *trayiconMenu;
};

#endif // MAINWINDOW_H

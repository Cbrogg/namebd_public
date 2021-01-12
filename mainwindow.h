#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QtGui>
#include <QLabel>
#include <QStatusBar>
#include <QTextStream>
#include <QFile>
#include <QDialog>
#include <QMessageBox>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_new_clicked();

    void on_pushButton_cansel_clicked();

    void on_checkBox_isAlive_clicked();

    void on_pushButton_del_clicked();

    void on_listWidget_names_currentItemChanged(QListWidgetItem *current);

    void on_pushButton_save_clicked();

    void on_pushButton_edit_clicked();

    void on_listWidget_names_itemDoubleClicked();

    void on_dateEdit_death_dateChanged();

private:
    bool nac;
    int state = 0;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QFile>
#include<QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void setwindow();
private slots:
    void openfile();
    void savetofile();
    void onsavefile();
    void onchangeFont();
    void onAddtime();
    void updateStatu(const QString filepath);
    void updateStaTime();


private:
    Ui::MainWindow *ui;
    QFile *file;
    QString filepath;
    QMenuBar *menubar;
    QStatusBar *statubar;
    QLabel *StatufileName;
    QLabel *time;
};
#endif // MAINWINDOW_H

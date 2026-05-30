#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QFile>
#include<QLabel>
#include<QList>
#include<QAction>
#include<QTextEdit>
#include<QWidget>
#include<QCloseEvent>

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
    void onCreateFile();
    void onOpenFile();
    void onSavetoFile();
    void onSaveFile();
    void onChangeFont();
    void onAddtime();
    void updateStatuPath(const QString filepath);
    void updateStaTime();

protected:
    void closeEvent(QCloseEvent *event)override;


private:
    // Ui::MainWindow *ui;
    QFile *file;
    QString filepath;
    QTextEdit *cenEditor;
    QMenuBar *menubar;
    QStatusBar *statubar;
    QLabel *StatufileName;
    QLabel *time;
    QFont font;
    QList<QAction *> menuAction;
};
#endif // MAINWINDOW_H

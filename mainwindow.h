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

signals:
    void updateFilePath(QString path);      //自定义信号


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
    void updateCursor();

protected:
    void closeEvent(QCloseEvent *event)override;


private:
    bool textIsChanged;
    QString filepath;
    QTextEdit *cenEditor;
    QMenuBar *menubar;
    QStatusBar *statubar;
    QLabel *StatufileName;
    QLabel *time;
    QLabel *cursor;
    QFont font;
    QList<QAction *> menuAction;
};
#endif // MAINWINDOW_H

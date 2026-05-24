#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMenuBar>
#include<QIcon>
#include<QTextEdit>
#include<QDateTime>
#include<QFileDialog>
#include<QByteArray>
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , statubar(this->statusBar())
    , filepath("")
    , StatufileName(new QLabel("当前文件 : "))
{
    ui->setupUi(this);

    //设置图标与标题
    this->setWindowTitle("TextEdit");
    const QIcon icon(":/rec/windowIcon.jpeg");
    this->setWindowIcon(icon);

    //设置窗口布局
    this->setMenuBar(menubar);
    this->setwindow();

    connect(statubar,&QStatusBar::messageChanged,this,&MainWindow::updateStatu);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setwindow()
{
    //设置菜单栏
    menubar = this->menuBar();
    QMenu* filemenu = new QMenu("文件");
    QMenu* editmenu = new QMenu("编辑");

    menubar->addMenu(filemenu);
    menubar->addMenu(editmenu);


    QAction *openfile = filemenu->addAction("打开",QKeySequence("Ctrl+O"));
    QAction *saveto = filemenu->addAction("另存为",QKeySequence("Ctrl+R"));
    QAction *save = filemenu->addAction("保存",QKeySequence("Ctrl+S"));

    QAction *changeFont = editmenu->addAction("字体");
    QAction *addTime = editmenu->addAction("添加时间/日期");

    connect(openfile,&QAction::triggered,this,&MainWindow::openfile);
    connect(openfile,&QAction::triggered,this,&MainWindow::savetofile);
    // connect(openfile,&QAction::triggered,this,&MainWindow::openfile);
    // connect(openfile,&QAction::triggered,this,&MainWindow::openfile);
    // connect(openfile,&QAction::triggered,this,&MainWindow::openfile);

    //设置状态栏
    //时间显示
    QLabel* time = new QLabel(this);
    QDateTime nowtime = QDateTime::currentDateTime();
    time->setText("时间 : "+nowtime.toString("yyyy-MM-dd hh:mm"));
    statubar->addWidget(time,1);

    //当前文件名显示
    statubar->addWidget(StatufileName,1);

    //设置主部件
    QTextEdit* textedit = new QTextEdit(this);
    this->setCentralWidget(textedit);
}

void MainWindow::openfile()
{
    QFileDialog *currentFile = new QFileDialog(this);

    this->filepath = currentFile->getOpenFileName(this,"打开文件","/","(*.txt)");
    this->file = new QFile(this->filepath);
    bool isopen = this->file->open(QIODeviceBase::ReadWrite);
    //将文本文件的内容加载至文本框
    if(isopen){
        QByteArray text = file->readAll();
        QTextEdit *central = qobject_cast<QTextEdit*>(this->centralWidget());

        central->setText(qvariant_cast<QString>(text));
    }
    else{
        QMessageBox *critical = new QMessageBox(QMessageBox::Critical,"错误","文件打开失败",QMessageBox::Ok,this);
    }

    //修改状态栏中的文件名信息
    emit statubar->messageChanged(this->filepath);
}

void MainWindow::savetofile()
{

}

void MainWindow::updateStatu(const QString filepath)
{

}

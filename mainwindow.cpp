#include "mainwindow.h"
// #include "ui_mainwindow.h"
#include<QMenuBar>
#include<QStatusBar>
#include<QIcon>
#include<QTextEdit>
#include<QDateTime>
#include<QFileDialog>
#include<QColorDialog>
#include<QFontDialog>
#include<QByteArray>
#include<QMessageBox>
#include<QFileInfo>
#include<QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    // , ui(new Ui::MainWindow)
    , filepath("")
    , cenEditor(new QTextEdit(this))
    , StatufileName(new QLabel(""))
    , time(new QLabel(this))
    , font()
    , statubar(this->statusBar())
    , menubar(this->menuBar())
{
    // ui->setupUi(this);

    //设置图标与标题
    this->setWindowTitle("TextEdit");
    const QIcon icon(":/rec/windowIcon.jpeg");
    this->setWindowIcon(icon);

    //设置窗口布局
    this->setwindow();
    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&MainWindow::updateStaTime);
    timer->start(6000);

    connect(this,&MainWindow::updateFilePath,this,&MainWindow::updateStatuPath);

}

MainWindow::~MainWindow()
{
    // delete ui;
}

void MainWindow::setwindow()
{
    this->resize(680,400);

    //设置菜单栏
    QMenu* filemenu = new QMenu("文件");
    QMenu* editmenu = new QMenu("编辑");

    this->menubar->addMenu(filemenu);
    this->menubar->addMenu(editmenu);

    QAction *createfile = filemenu->addAction("新建文件",QKeySequence("Ctrl+c"));
    QAction *openfile = filemenu->addAction("打开",QKeySequence("Ctrl+o"));
    QAction *saveto = filemenu->addAction("另存为",QKeySequence("Ctrl+r"));
    saveto->setDisabled(true);      //未打开任何文件时无法点击
    QAction *save = filemenu->addAction("保存",QKeySequence("Ctrl+s"));
    save->setDisabled(true);

    QAction *changeFont = editmenu->addAction("字体");
    changeFont->setDisabled(true);
    QAction *addTime = editmenu->addAction("添加时间/日期");
    addTime->setDisabled(true);

    menuAction << createfile << openfile << saveto << save << changeFont << addTime;

    //设置状态栏
    this->statubar = this->statusBar();
    //时间显示
    QDateTime nowtime = QDateTime::currentDateTime();
    time->setText("时间 : "+nowtime.toString("yyyy-MM-dd hh:mm"));
    statubar->addWidget(time,1);

    //当前文件名显示
    this->statubar->addWidget(StatufileName,1);

    this->setStatusBar(this->statubar);

    //设置主部件
    this->setCentralWidget(this->cenEditor);


    connect(createfile,&QAction::triggered,this,&MainWindow::onCreateFile);
    connect(openfile,&QAction::triggered,this,&MainWindow::onOpenFile);
    connect(saveto,&QAction::triggered,this,&MainWindow::onSavetoFile);
    connect(save,&QAction::triggered,this,&MainWindow::onSaveFile);
    connect(changeFont,&QAction::triggered,this,&MainWindow::onChangeFont);
    connect(addTime,&QAction::triggered,this,&MainWindow::onAddtime);

}

void MainWindow::onCreateFile()
{
    QFileDialog *createFile = new QFileDialog(this);
    // createFile->setAcceptMode(QFileDialog::AcceptSave);

    this->filepath = createFile->getSaveFileName(this,"创建文件","/",tr("(*.txt)"));

    if(this->filepath.isEmpty()){
        return;
    }
    else{
        //设置菜单项使能
        for(auto *action: menuAction){
            if(action->isEnabled() == false){
                action->setDisabled(false);
            }
        }
        emit this->updateFilePath(this->filepath);
        emit onSaveFile();
    }


}

void MainWindow::onOpenFile()
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
        //设置菜单项使能
        for(auto *action: menuAction){
            if(action->isEnabled() == false){
                action->setDisabled(false);
            }
        }
        //修改状态栏中的文件名信息
        emit this->updateFilePath(this->filepath);
    }
    else{
        return;
    }

}


void MainWindow::onSavetoFile()
{
    //创建保存文件对话框并设置默认后缀名
    QFileDialog *currentFile = new QFileDialog(this);
    currentFile->setAcceptMode(QFileDialog::AcceptMode::AcceptSave);

    //获取用户设置的文件路径
    this->filepath = currentFile->getSaveFileName(this,tr("另存为"),"",tr("(*.txt)"));
    //当用户取消保存时
    if(this->filepath.isEmpty()){
        return;
    }

    QFile newFile(this->filepath,this);
    bool IsOpen = newFile.open(QIODevice::WriteOnly);
    if(IsOpen){
        QString text = this->cenEditor->toPlainText();
        newFile.write(qvariant_cast<QByteArray>(text));
    }
    else{
        QMessageBox::StandardButton critical = QMessageBox::critical(this,"错误","文件保存失败",QMessageBox::Ok);
    }
}

void MainWindow::onSaveFile()
{
    QFile newFile(filepath,this);
    bool IsOpen = newFile.open(QIODevice::WriteOnly);
    if(IsOpen){
        QString text = this->cenEditor->toPlainText();
        newFile.write(qvariant_cast<QByteArray>(text));
    }
    else{
        QMessageBox::StandardButton critical = QMessageBox::critical(this,"错误","文件保存失败",QMessageBox::Ok);
    }
}

void MainWindow::onChangeFont()
{
    QFontDialog *FontSetting = new QFontDialog(this);
    bool ok;
    this->font = FontSetting->getFont(&ok,this);

    if(ok){
        this->centralWidget()->setFont(this->font);
    }
}

void MainWindow::onAddtime()
{
    QDateTime nowtime = QDateTime::currentDateTime();
    QString time = nowtime.toString(tr("yyyy-MM-dd hh:mm"));

    this->cenEditor->insertPlainText(time);
}

void MainWindow::updateStatuPath(const QString filepath)
{
    this->StatufileName->clear();
    this->StatufileName->setText(filepath);    
}

void MainWindow::updateStaTime()
{
    QDateTime nowtime = QDateTime::currentDateTime();
    time->setText("时间 : "+nowtime.toString("yyyy-MM-dd hh:mm"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!this->filepath.isEmpty()){
        int select = QMessageBox::question(this,"提示","是否保存文件",QMessageBox::StandardButton::Save|QMessageBox::StandardButton::Cancel,QMessageBox::StandardButton::Save);
        if(select == QMessageBox::StandardButton::Save){
            emit this->onSaveFile();
        }
    }

}

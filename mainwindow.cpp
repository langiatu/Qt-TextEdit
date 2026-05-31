#include "mainwindow.h"
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
#include<QTextCursor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , filepath("")
    , cenEditor(new QTextEdit(this))
    , StatufileName(new QLabel(""))
    , time(new QLabel(this))
    , font()
    , textIsChanged(false)
    , statubar(this->statusBar())
    , menubar(this->menuBar())
    , cursor(new QLabel(this->statubar))
{

    //设置图标与标题
    this->setWindowTitle("TextEdit");
    const QIcon icon(":/rec/windowIcon.jpeg");
    this->setWindowIcon(icon);

    //设置窗口布局
    this->setwindow();

    //设置定时器用于显示当前时间
    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&MainWindow::updateStaTime);
    timer->start(6000);
}

MainWindow::~MainWindow()
{

}

//设置MainWindow的整体布局
void MainWindow::setwindow()
{
    this->resize(680,400);

    //设置菜单栏
    QMenu* filemenu = new QMenu("文件");
    QMenu* editmenu = new QMenu("编辑");

    this->menubar->addMenu(filemenu);
    this->menubar->addMenu(editmenu);

        //设置菜单栏filemenu中的菜单项
    QAction *createfile = filemenu->addAction("新建文件",QKeySequence("Ctrl+n"));
    QAction *openfile = filemenu->addAction("打开",QKeySequence("Ctrl+o"));
    QAction *saveto = filemenu->addAction("另存为",QKeySequence("Ctrl+S"));
    saveto->setDisabled(true);      //未打开任何文件时无法点击
    QAction *save = filemenu->addAction("保存",QKeySequence("Ctrl+s"));
    save->setDisabled(true);

        //设置菜单栏editmenu中的菜单项
    QAction *changeFont = editmenu->addAction("字体");
    changeFont->setDisabled(true);
    QAction *addTime = editmenu->addAction("添加时间/日期");
    addTime->setDisabled(true);

    //将所有菜单项添加到管理容器中
    menuAction << createfile << openfile << saveto << save << changeFont << addTime;

    //设置状态栏
    this->statubar = this->statusBar();
        //时间显示
    QDateTime nowtime = QDateTime::currentDateTime();
    time->setText("时间 : "+nowtime.toString("yyyy-MM-dd hh:mm"));
    statubar->addWidget(time,1);

        //行列号显示 自动调用槽函数
    emit this->cenEditor->cursorPositionChanged();

        //当前文件名显示
    this->statubar->addWidget(StatufileName,1);

    this->setStatusBar(this->statubar);

    //设置主部件
    this->setCentralWidget(this->cenEditor);

    //连接菜单栏相关信号与槽
    connect(createfile,&QAction::triggered,this,&MainWindow::onCreateFile);     //连接创建文件信号槽
    connect(openfile,&QAction::triggered,this,&MainWindow::onOpenFile);         //连接打开文件信号槽
    connect(saveto,&QAction::triggered,this,&MainWindow::onSavetoFile);         //连接另存文件信号槽
    connect(save,&QAction::triggered,this,&MainWindow::onSaveFile);             //连接保存文件信号槽

    connect(changeFont,&QAction::triggered,this,&MainWindow::onChangeFont);     //连接编辑字体信号槽
    connect(addTime,&QAction::triggered,this,&MainWindow::onAddtime);           //连接添加事件信息信号槽

    //连接状态栏相关信号与槽
    connect(this->cenEditor,&QTextEdit::cursorPositionChanged,this,&MainWindow::updateCursor);  //连接更新光标位置信号槽
    connect(this,&MainWindow::updateFilePath,this,&MainWindow::updateStatuPath);                //连接更新文件路径信号槽

}

//创建文件菜单项槽函数  ---- 在文件系统中创建一个新文件
void MainWindow::onCreateFile()
{
    //获取用户创建的新文件的文件路径
    this->filepath = QFileDialog::getSaveFileName(this,"创建文件","/",tr("(*.txt)"));

    if(this->filepath.isEmpty()){       //路径为空 ---- 用户取消创建文件
        return;
    }
    else{                               //用户成功创建新文件
        //清空编辑器文本
        this->cenEditor->clear();
        //设置状态栏文件名
        emit this->updateFilePath(this->filepath);      //发送信号 更新当前文件路径
        //设置菜单项使能
        for(auto *action: menuAction){
            if(action->isEnabled() == false){
                action->setDisabled(false);
            }
        }

        emit this->menuAction[3]->trigger();            //创建文件后同时保存文件
    }


}

//打开文件菜单项槽函数  ----- 打开文件系统中的一个文本文件
void MainWindow::onOpenFile()
{
    //获取用户打开文件的路径
    this->filepath = QFileDialog::getOpenFileName(this,"打开文件","/","(*.txt)");

    //创建QFile对象用于判断文件是否成功打开
    QFile file(this->filepath,this);
    bool isopen = file.open(QIODeviceBase::ReadWrite);


    if(isopen){     //文件成功打开后，将文本文件的内容加载至文本框
        QByteArray text = file.readAll();
        QTextEdit *central = qobject_cast<QTextEdit*>(this->centralWidget());

        central->setText(qvariant_cast<QString>(text));
        //设置菜单项使能
        for(auto *action: menuAction){
            if(action->isEnabled() == false){
                action->setDisabled(false);
            }
        }
        //设置当前文档是否修改标记
        this->textIsChanged = false;

        //修改状态栏中的文件名信息
        emit this->updateFilePath(this->filepath);
    }
    else{       //用户取消打开文件，直接返回主页面
        return;
    }

}

//另存为菜单项槽函数  ----- 为当前编辑器文本文件创建一个副本文件
void MainWindow::onSavetoFile()
{
    //创建保存文件对话框并设置默认后缀名
    QFileDialog *currentFile = new QFileDialog(this);
    currentFile->setAcceptMode(QFileDialog::AcceptMode::AcceptSave);

    //获取用户设置的文件路径
    this->filepath = currentFile->getSaveFileName(this,tr("另存为"),"",tr("(*.txt)"));

    //获取到新文件名后手动释放QFileDialog对话框
    delete currentFile;


    if(this->filepath.isEmpty()){   //当用户取消保存时，直接返回主页面
        return;
    }

    //使用用户选择的文件路径创建新文件
    QFile newFile(this->filepath,this);
    bool IsOpen = newFile.open(QIODevice::WriteOnly);

    if(IsOpen){         //打开文件并将内容保存到文件中
        QString text = this->cenEditor->toPlainText();
        newFile.write(qvariant_cast<QByteArray>(text));

        //设置文件内容已修改标记
        this->textIsChanged = true;
    }
    else{
        QMessageBox::StandardButton critical = QMessageBox::critical(this,"错误","文件保存失败",QMessageBox::Ok);
    }
}

//保存菜单项槽函数   ---- 保存当前编辑器修改至文件中
void MainWindow::onSaveFile()
{
    //创建QFile对象用于处理保存操作
    QFile newFile(filepath,this);
    bool IsOpen = newFile.open(QIODevice::WriteOnly);

    if(IsOpen){         //文件成功打开时将编辑器文本保存到文件
        QString text = this->cenEditor->toPlainText();
        newFile.write(qvariant_cast<QByteArray>(text));

        //设置文件内容已修改标记
        this->textIsChanged = true;
    }
    else{               //文件保存失败提醒用户
        QMessageBox::StandardButton critical = QMessageBox::critical(this,"错误","文件保存失败",QMessageBox::Ok);
    }
}

//编辑字体菜单项槽函数 ---- 修改编辑器文本字体
void MainWindow::onChangeFont()
{
    bool ok;        //用于用户是否确定字体设置

    this->font = QFontDialog::getFont(&ok,this);

    if(ok){         //确定更改编辑器文本字体
        this->centralWidget()->setFont(this->font);
    }

}

//添加时间/日期菜单项槽函数 ---- 在光标后添加时间/日期
void MainWindow::onAddtime()
{
    //获取当前时间并转换为指定格式
    QDateTime nowtime = QDateTime::currentDateTime();
    QString time = nowtime.toString(tr("yyyy-MM-dd hh:mm"));

    //将日期时间信息插入编辑器光标后
    this->cenEditor->insertPlainText(time);
}

//更新当前状态栏文件名信息
void MainWindow::updateStatuPath(const QString filepath)
{
    this->StatufileName->clear();       //清空当前文件名信息
    this->StatufileName->setText(filepath);    //设置当前文件名信息
}

//更新状态栏时间信息
void MainWindow::updateStaTime()
{
    QDateTime nowtime = QDateTime::currentDateTime();
    time->setText("时间 : "+nowtime.toString("yyyy-MM-dd hh:mm"));
}

//更新状态栏光标位置信息
void MainWindow::updateCursor()
{
    //创建QTextCursor对象用于获取当前光标位置信息
    QTextCursor cursor = this->cenEditor->textCursor();
    int row = cursor.blockNumber() + 1;             //行
    int cul = cursor.positionInBlock() + 1;         //列

    //将光标位置信息设置为cursor标签文本
    this->cursor->setText(tr("行：%1 列：%2").arg(row).arg(cul));

    //将cursor标签添加到状态栏
    this->statubar->addWidget(this->cursor);
}

//重写关闭窗口时间处理函数 ---- 再关闭窗口前提示用户
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!this->filepath.isEmpty() && this->textIsChanged){       //当打开文件且编辑器内容修改后，在关闭窗口时提醒用户是否保存文本的修改
        int select = QMessageBox::question(this,"提示","是否保存文件内容更改",QMessageBox::StandardButton::Save|QMessageBox::StandardButton::Cancel,QMessageBox::StandardButton::Save);
        if(select == QMessageBox::StandardButton::Save){
            emit this->onSaveFile();
        }
    }

}

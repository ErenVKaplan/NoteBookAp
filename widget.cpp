#include "widget.h"
#include <QTextEdit>
#include <QFileDialog>
#include <QFontDialog>
#include <QApplication>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    layout = new QVBoxLayout(this);
    menuBar = new QMenuBar(this);
    fileMenu = new QMenu("&Dosya", this);
    appearanceMenu=new QMenu("&Görünüm",this);
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(appearanceMenu);

    QAction *formatFont = new QAction("&Yazı Tipi", this);
    QAction *formatColor= new QAction("&Yazı Rengi",this);
    newAction = new QAction("&New", this); //ALT+N
    openAction = new QAction("&Open", this);//ALT+O
    saveAction = new QAction("&Save", this);//ALT+S
    exitAction = new QAction("&Exit", this);//ALT+E
    undoAction = new QAction("&Undo",this);
    rendoAction = new QAction("&Rendo",this);
    DarkTheme=new QAction("&Dark",this);
    LightTheme=new QAction("&Light",this);
    recentFilesMenu = new QMenu(tr("&Son Açılan Dosyalar"), this);
    appearanceMenu->addAction(DarkTheme);
    appearanceMenu->addAction(LightTheme);
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    fileMenu->addAction(rendoAction);
    fileMenu->addAction(undoAction);
    fileMenu->addAction(formatFont);
    fileMenu->addAction(formatColor);

    fileMenu->addMenu(recentFilesMenu);

    layout->setMenuBar(menuBar);

    textEdit = new QTextEdit(this);
    layout->addWidget(textEdit);

    undoAction->setShortcut(QKeySequence::Undo);
    rendoAction->setShortcut(QKeySequence::Redo);

    connect(newAction, &QAction::triggered, this, &Widget::newFile);
    connect(openAction, &QAction::triggered, this, &Widget::openFile);
    connect(saveAction, &QAction::triggered, this, &Widget::saveFile);
    connect(exitAction, &QAction::triggered, this, &Widget::close);
    connect(undoAction,&QAction::triggered,textEdit,&QTextEdit::undo);
    connect(rendoAction,&QAction::triggered,textEdit,&QTextEdit::redo);
    connect(formatFont,&QAction::triggered,this,&Widget::chooseFont);
    connect(formatColor,&QAction::triggered,this,&Widget::chooseColor);
    connect(DarkTheme,&QAction::triggered,this,&Widget::applyDarkTheme);
    connect(LightTheme,&QAction::triggered,this,&Widget::applyLightTheme);
    setLayout(layout);
}

Widget::~Widget()
{
}

void Widget::newFile()
{
    // Yeni dosya işlevi
}
void Widget::openFile() {
    QString path = QFileDialog::getOpenFileName(this, tr("Dosya Aç"));
    if (!path.isEmpty()) {
        filePath = path;
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString fileContent = in.readAll();
            textEdit->setPlainText(fileContent);
        }
        if (!recentFiles.contains(path)) {
            recentFiles.prepend(path);  // En üste ekle
            if (recentFiles.size() > 5) {  // En fazla 5 dosya tut
                recentFiles.removeLast();
            }
        }
        updateRecentFilesMenu();  // Recent Files menüsünü güncell
    }
}


void Widget::saveFile() {
    if (filePath.isEmpty()) {
        // Dosya yeni ise, kaydetmeden önce kullanıcıdan dosya adı alın
        filePath = QFileDialog::getSaveFileName(this, tr("Dosya Kaydet"));
        if (filePath.isEmpty()) {
            return;
        }
    }

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << textEdit->toPlainText();
        file.close();
    }
}

void Widget::chooseFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this);
    if (ok) {
        textEdit->setCurrentFont(font);
    }
}
void Widget::chooseColor() {
    QColor color = QColorDialog::getColor(Qt::white, this);
    if (color.isValid()) {
        QTextCharFormat fmt;
        fmt.setForeground(color);
        QTextCursor cursor = textEdit->textCursor();
        cursor.mergeCharFormat(fmt);
    }
}
void Widget::applyDarkTheme()
{
    QString darkstyle="QWidget{background-color:#2b2b2b; color: #ffffff;}";
    qApp->setStyleSheet(darkstyle);
}
void Widget::applyLightTheme()
{
    QString lightStyle = "QWidget { background-color: #ffffff; color: #000000; }";
    qApp->setStyleSheet(lightStyle);
}
void Widget::updateRecentFilesMenu()
{
    recentFilesMenu->clear();

    for (const QString &filePath : recentFiles) {
        QAction *action = new QAction(filePath, this);
        connect(action, &QAction::triggered, this, [this, filePath]() {
            openRecentFile(filePath);
        });
        recentFilesMenu->addAction(action);
    }
}
void Widget::openRecentFile(const QString &filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString fileContent = in.readAll();
        textEdit->setPlainText(fileContent);
    }
}

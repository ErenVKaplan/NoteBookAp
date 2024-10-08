#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QColorDialog>
#include <QFileDialog>
#include <QStringList>
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void chooseFont();
    void chooseColor();
    void applyDarkTheme();
    void applyLightTheme();
    void updateRecentFilesMenu();
    void openRecentFile(const QString &filePath);

private:
    QMenuBar *menuBar;
    QMenu *recentFilesMenu;

    QMenu *appearanceMenu;
    QMenu *fileMenu;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *exitAction;
    QVBoxLayout *layout;
    QAction *undoAction;
    QAction *rendoAction;
    QAction *formatFont;
    QTextEdit *textEdit;
    QColorDialog *colorEdit;
    QFileDialog *fileDialog;
    QString filePath;
    QAction *DarkTheme;
    QAction *LightTheme;
    QStringList recentFiles;
    QAction *recentFilesAction;
};

#endif // WIDGET_H

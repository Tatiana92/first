#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_choiceBox_currentIndexChanged(int index);

    void on_convertButton_clicked();

    char* utf16to8( wchar_t* utf16str);
    wchar_t* utf8to16(char* str );
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

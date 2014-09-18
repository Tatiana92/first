#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vector"
#include "utf8\source\utf8.h"
#include <iostream>
#include <QString>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_choiceBox_currentIndexChanged(int index)
{
ui->textResult->clear();
}

void MainWindow::on_convertButton_clicked()
{
    string line=ui->textMy->toPlainText().toStdString();
    switch(ui->choiceBox->currentIndex())
    {
    case 1:
    {
                   vector<unsigned short> utf16line;
                    string utf16line2;
                    utf8::utf8to16(line.begin(), line.end(), back_inserter(utf16line));
                    utf8::utf8to16(line.begin(), line.end(), back_inserter(utf16line2));
                    ui->textResult->setText(QString::fromStdString(utf16line2));
                 /*   for (int i = 0; i<utf16line.size(); ++i){
                        cout << utf16line[i] << " ";
                    }
                    for (int i = 0; i<utf16line2.size(); ++i)
                        cout << utf16line2[i] ;
                    //wcout << utf16line2 << "\n";*/
                    break;
                }
              case 2:
    {
                    vector<unsigned short> utf8line;
                    string utf8line2;
                    utf8::utf16to8(line.begin(), line.end(), back_inserter(utf8line));
                    utf8::utf16to8(line.begin(), line.end(), back_inserter(utf8line2));
                    ui->textResult->setText(QString::fromStdString(utf8line2));
                   /* for (int i = 0; i<utf8line.size(); ++i){
                        cout << utf8line[i] << " ";
                    }
                    cout << "\n";
                    for (int i = 0; i<utf8line2.size(); ++i)
                        wcout << utf8line2[i] ;
                    //wcout << utf8line2 << "\n";*/
    break;
    }
            case 3:
                {
                    QVector<unsigned short> utf32line;
                    string utf32line2;
                    if(utf8::is_valid(line.begin(), line.end()))
                    {
                    utf8::utf8to32(line.begin(), line.end(), back_inserter(utf32line));
                    utf8::utf8to32(line.begin(), line.end(), back_inserter(utf32line2));
                    ui->textResult->setText(QString::fromStdString(utf32line2));
                    }
                    break;
                   /* for (int i = 0; i<utf32line.size(); ++i){
                        cout << utf32line[i] << " ";
                    }
                    cout << "\n";
                    for (int i = 0; i<utf32line2.size(); ++i)
                        wcout << utf32line2[i];

                    //wcout << utf32line2 << "\n";*/
                }
                case 4:
    {
                    //cout << "\0x448\0x65E5\0x10346\0" << "\n";
                    //int utf32string[] = { 0x448, 0x65E5, 0x10346, 0 };
                    vector<unsigned char> utf8result;
                    string utf8result2;
                    if(utf8::is_valid(line.begin(), line.end()))
                    {
                    utf8::utf32to8(line.begin(), line.end(), back_inserter(utf8result2));
                    //utf8::utf32to8(line.begin(), line.end(), back_inserter(utf8result));
                    //utf8::utf32to8(utf32string, utf32string + 3, back_inserter(utf8result));
                    //utf8::utf32to8(utf32string, utf32string + 3, back_inserter(utf8result));
                    ui->textResult->setText(QString::fromStdString(utf8result2));

                    }
                    /*  for (int i = 0; i<utf8result.size(); ++i){
                        cout << utf8result[i];

                    }
                    cout << "\n";
                    /*wcout << utf8result2 << "\n";*/
    break;
    }

}
}

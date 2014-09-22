#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vector"
#include "utf8\source\utf8.h"
#include <iostream>
#include <QString>
#include <stdlib.h>

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
char* MainWindow::utf16to8( wchar_t* utf16str)
{
    //указатель на строку в utf-16 (вход)
    int size=wcslen(utf16str),counter=0;
    char* str=(char*)calloc(wcslen(utf16str)+1,sizeof(char));               //указатель на строку в utf-8  (выход)
    while(*utf16str)
    {
        wchar_t curSymb = *utf16str;
        if (curSymb <= 0x7F)
        {
            *str = (char)curSymb;
        }
        else if (curSymb <= 0x7FF)
        {
            *str++ = (char)(0xC0 | (curSymb >> 6));
            counter++;
            *str = (char)(0x80 | (curSymb & 0x3F));
        }
        else if (curSymb <= 0xFFFF)
        {
            counter+=2;
            *str++ = (char)(0xE0 | (curSymb >> 12));
            *str++ = (char)(0x80 | ((curSymb >> 6) & 0x3F));
            *str = (char)(0x80 | (curSymb & 0x3F));
        }

        utf16str++;
        counter++;
        str++;
    }
    str=str-counter;
    *(str+counter)='\0';
    return str;
}

wchar_t* MainWindow::utf8to16(char* str )
{
    wchar_t* UTF8_16 ;
    UTF8_16=(wchar_t*)calloc(strlen(str),sizeof(wchar_t));
    int i, j, j2, ch, k1, k2, k3, m,count=0;
    i = 0;
    int size=strlen(str);
    while(i<=size)
    {
        wchar_t* val;

        wchar_t val2='\0';
        val=(wchar_t*)calloc(strlen(str),1);
        ch = *(str+i);
        j=ch;
        // j = clng( Asc( ch ) )
        if (j >= 128)
            if (j < 224)
            {
                //2 байта
                k1 = j % 32;
                i = i + 1;
                ch = *(str+i);
                j2 = ch ;
                k2 = j2 % 64;
                //ChrW - символ по UTF-16 значению
                m=_wtoi(UTF8_16);
                m= m |(k2 + k1 * 64);
                val2=(wchar_t)m;
            }
            else
            {
                //3 байта
                k1 = j % 16;
                i = i + 1;
                ch = *(str+i);
                j2 = ch;
                k2 = j2 % 64;
                i = i + 1;
                ch = *(str+i);
                j2 = ch;
                k3 = j2 % 64;
                m=_wtoi(UTF8_16);
                m= m |(k3 + ( k2 + k1 * 64 ) * 64);

                val2=(wchar_t)m;
            }

        else
        {
            m=_wtoi(UTF8_16);
            m= m | ch;
            val2=(wchar_t)m;

        }
        i = i + 1;
        *( UTF8_16)=val2;
        count++;
        UTF8_16++;
    }
    UTF8_16-=count;
    return UTF8_16;
}


void MainWindow::on_choiceBox_currentIndexChanged(int index)
{
    ui->textResult->clear();
}

void MainWindow::on_convertButton_clicked()
{


    switch(ui->choiceBox->currentIndex())
    {
    case 1:
    {
        QString slovo=ui->textMy->toPlainText();
        QByteArray arr=slovo.toUtf8();
        char* str=arr.data();
        wchar_t *WC2= utf8to16(str);
        QString strin=QString::fromWCharArray(WC2,strlen(str));
        ui->textResult->setText(strin );
        break;
    }
    case 2:
    {
        wchar_t *WC ;

        QString str1=ui->textMy->toPlainText();
        WC=(wchar_t*)calloc(str1.size(),sizeof(wchar_t));
        str1.toWCharArray(WC);
        *(WC+str1.size())='\0';
        char* str=utf16to8(WC);
        QString strin=QString::fromLocal8Bit(str,strlen(str));
        ui->textResult->setText(strin );
        break;
    }
   /* case 3:
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
    }
    case 4:
    {
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

        break;
    }
*/
    }
}

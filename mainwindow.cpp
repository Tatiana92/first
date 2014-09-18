#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFile"
#include <math.h>
#include <QPainter>
#include <QPaintEvent>
#include "error.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //readFile();
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readFile()
{
    QFile file(SourceFile);
    QByteArray data; // Создаем объект класса QByteArray, куда мы будем считывать данные
    if (file.open(QIODevice::ReadOnly)) // Проверяем, возможно ли открыть наш файл для чтения
    {
        data = file.readAll(); //считываем все данные с файла в объект data
        content=(int*)calloc(Alph.size(),4);for(int i=0;i<data.size();i++)
        {
            for(int j=0;j<Alph.size();j++)
            {
                if(data.operator [](i)==Alph.operator [](j))
                {
                    content[j]++;
                    break;
                }
            }
        }
        size=Alph.size();
    }
    else
    {

        error error;
        error.setData("Файл не открывается");
        error.exec();

        exit(0);
    }

}
void MainWindow::setData()
{
    readFile();
    ui->widget->installEventFilter(this);
    ui->widget->repaint();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Paint)
    {
        QPaintEvent *event = static_cast<QPaintEvent*>(event);
        QPainter p(ui->widget);
        p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
        int x0=10,y0=ui->widget->height()-20,xEnd=ui->widget->width(),yEnd=5,i,j,step;
        p.drawLine(x0,y0,xEnd,y0);//horizontal
        p.drawLine(x0,y0,x0,yEnd);//vertical
        p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
        step=(int)floor((xEnd-x0)/size);
        char* a=Alph.data();
        for(i=1;i<=size;i++)
        {
            p.drawPoint(x0+step*i,y0);
            p.drawText(x0+step*i,y0+10,QString(*(a+i)));
        }
        p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
        int x,y;
        double max=0.0;

        for(i=0;i<size;i++)
        {
            if(content[i]>=max)
                max=content[i];
        }
        for(i=0;i<size;i++)
        {
            p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
            y=(int)floor(50*yEnd*content[i]/max);
            //p.drawPoint(x0+step*(i+1),y0-y);
            p.setBrush(QBrush(Qt::red));
            p.drawRect(x0+step*(i),y0-y,step,y);
            p.drawText(x0+step*(i),y0-y,QString::number ( content[i], 'g',6 ));


        }


        return true;
    }
    else {
        // standard event processing
        return QWidget::eventFilter(obj, event);
    }
}

QByteArray MainWindow::vBin(QByteArray str)//iz stroki proizvolnyh simvolov v stroku 0 i 1
{
    QByteArray res;
    unsigned char mask=0x80;
    int  size1=str.size();
    int i,j;
    for(i=0;i<size1;i++)
    {
        char a='\0';
        for(j=0;j<8;j++)
        {
            if((str[i]&mask)!=0)
                res.append('1');
            else
                res.append('0');
            mask=mask>>1;
        }
        mask=0x80;
    }
    return res;
}

QByteArray MainWindow::fromBin(QByteArray str)//iz stroki 0 i 1 v proizv. simvoli
{
    int size1=(int)ceil(str.size()/8.0);
    QByteArray res;
    unsigned char mask=0x80;
    int i,j,k;
    for(i=0;i<size1*8;i=i+8)//size*8
    {

        char a='\0';
        for(j=0;j<8;j++)
        {
            if(str.operator [](i+j)=='1')
                a=a|mask;

            mask=mask>>1;
        }
        mask=0x80;
        res.append(a);
        int g=*(res+i/8);
    }
    return res;
}

void MainWindow::on_lab1Button_clicked()
{
    QFile file(SourceFile); // создаем объект класса QFile
    QByteArray data; // Создаем объект класса QByteArray, куда мы будем считывать данные
    if (file.open(QIODevice::ReadOnly)) // Проверяем, возможно ли открыть наш файл для чтения

    {
        data = file.readAll(); //считываем все данные с файла в объект data

        QByteArray res;
        for(int i=0;i<data.size();i++)
        {
            for(int j=0;j<Alph.size();j++)
            {
                if(data.operator [](i)==Alph.operator [](j))
                {

                    QString tmp;
                    tmp=QString("%1").arg(j,0,2);
                    for(int k=0;k<(6 - tmp.size());k++)
                        res.append('0');
                    res.append(tmp);
                }
            }
        }
        file.close();
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);

        file.write(fromBin(res));
        file.close();
        error error;
        error.setData("Файл закодирован успешно!");
        error.exec();
    }
    else
    {

        error error;
        error.setData("Файл не открывается");
        error.exec();

        exit(0);
    }
}

void MainWindow::on_decodeLab1Button_clicked()
{
    QFile file(SourceFile); // создаем объект класса QFile
    QByteArray data; // Создаем объект класса QByteArray, куда мы будем считывать данные
    if (file.open(QIODevice::ReadOnly)) // Проверяем, возможно ли открыть наш файл для чтения
    {

        data = vBin(file.readAll()); //считываем все данные с файла в объект data

        QByteArray res;
        for(int i=0;i<data.size()/6;i++)
        {
            QByteArray tmp;
            for(int j=0;j<6;j++)
                tmp.append(data.operator [](j+i*6));
            bool ok;
            int num=tmp.toInt(&ok,2);
            res.append(Alph.operator [](num));

        }
        file.close();
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);

        file.write(res);
        file.close();
        error error;
        error.setData("Файл декодирован успешно!");
        error.exec();
    }
    else
    {

        error error;
        error.setData("Файл не открывается");
        error.exec();

        exit(0);

    }
}

int MainWindow::makeCodeTable()
{
    int i,j,k=0,h=0;
    Alph2.clear();
    table.clear();
    for(i=0;i<size;i++)
        if(content[i]>0)
        {
            Alph2.append(Alph.operator [](i));
            k++;
        }
    double f=log2(k*1.0);
    if(log2(k*1.0)-int(log2(k*1.0))>0)
        k=(int)ceil(log2(k*1.0));
    else

        k=int(log2(k*1.0));
    k=pow(2,k);

    for(j=0;j<size;j++)
    {
        QByteArray res;
        if(content[j]>0)
        {
            QString tmp;
            tmp=QString("%1").arg(h,0,2);
            for( i=0;i<((int)(log2(k*1.0)) - tmp.size());i++)
                res.append('0');
            res.append(tmp);
            table.append(res);
            h++;
        }
    }
    return (int)(log2(k*1.0));


}

void MainWindow::on_codeLab2Button_clicked()
{
    QFile file(SourceFile); // создаем объект класса QFile
    QByteArray data; // Создаем объект класса QByteArray, куда мы будем считывать данные
    if (file.open(QIODevice::ReadOnly)) // Проверяем, возможно ли открыть наш файл для чтения
    {
        data = file.readAll(); //считываем все данные с файла в объект data

        QByteArray res;
        QString tmp;
        tmp=QString("%1").arg(makeCodeTable(),0,2);//сколько занимает кодовое слово
        for(int k=0;k<(8 - tmp.size());k++)
            res.append('0');
        res.append(tmp);
        tmp=QString("%1").arg(table.size(),0,2);//сколько кодовых слов
        for(int k=0;k<(8 - tmp.size());k++)
            res.append('0');
        res.append(tmp);
        for(int i=0;i<Alph2.size();i++)
        {
            res.append(table.operator [](i));
        }//кодовые слова запишем
        const char *val=Alph2.constData();
        for(int i=0;i<Alph2.size();i++)
        {

            int j=Alph2[i];
            tmp=QString("%1").arg(j,0,2);
            for(int k=0;k<(8 - tmp.size());k++)
                res.append('0');
            res.append(tmp);
        }//алфавит запишем
        for(int i=0;i<data.size();i++)
        {
            for(int j=0;j<Alph2.size();j++)
            {
                if(data.operator [](i)==Alph2.operator [](j))
                {
                    res.append(table.operator [](j));
                }
            }
        }
        file.close();
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);

        file.write(fromBin(res));
        file.close();
        error error;
        error.setData("Файл закодирован успешно!");
        error.exec();
    }
    else
    {

        error error;
        error.setData("Файл не открывается");
        error.exec();

        exit(0);
    }
}


void MainWindow::on_lab2DecodeButton_clicked()
{
    QFile file(SourceFile); // создаем объект класса QFile
    QByteArray data; // Создаем объект класса QByteArray, куда мы будем считывать данные
    if (file.open(QIODevice::ReadOnly)) // Проверяем, возможно ли открыть наш файл для чтения
    {
        data = vBin(file.readAll()); //считываем все данные с файла в объект data
        int wordLen,wordCount;
        QString tmp;
        bool *ok;
        // tmp=QString("%1").arg(makeCodeTable(),0,2);
        int k;
        for( k=0;k<8;k++)//сколько занимает кодовое слово
            tmp.operator [](k)=data[k];
        wordLen=tmp.toInt(ok,2);
        for(;k<16;k++)//сколько кодовых слов
            tmp.operator [](k-8)=data[k];
        wordCount=tmp.toInt(ok,2);

        table.clear();
        for(int i=0;i<wordCount;i++)
        {
            QByteArray word;
            for(int j=0;j<wordLen;j++)
            {
                word.append(data[k+i*wordLen+j]);
            }
            table.append(word);
        }//кодовые слова
        k+=wordCount*wordLen;
        Alph2.clear();
        for(int i=0;i<wordCount;i++)
        {
            for(int j=0;j<8;j++)
            {
                tmp.operator [](j)=data[k+i*8+j];
            }
            bool ok;
            int num=tmp.toInt(&ok,2);
            char a=num;
            Alph2.append(a);

        }//алфавит
        k+=wordCount*8;
        QByteArray res;
        for(int i=0;i<(data.size()-k)/wordLen;i++)
        {
            QByteArray word;
            for(int j=0;j<wordLen;j++)
            {
                word.append(data[k+i*wordLen+j]);
            }
            for(int j=0;j<wordCount;j++)
                if(table[j]==word)
                {
                    res.append(Alph2.operator [](j));
                    break;
                }

        }
        file.close();
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);

        file.write(res);
        file.close();
        error error;
        error.setData("Файл декодирован успешно!");
        error.exec();

    }
    else
    {

        error error;
        error.setData("Файл не открывается");
        error.exec();

        exit(0);
    }
}

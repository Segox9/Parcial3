#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdio.h>
#include <QDebug>
struct dato{
    QVector<double> time;
    QVector<double> voltage;
    QVector<double> current;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

//--------------------------------------
private:
    dato dat;
    void readFile(const char* arch, dato& dat);
    void plot();
    void fase();
    void Potencia();
    double Vrms(dato& x);
    double Irms(dato& x);
    void PotCorrb();

};
#endif // MAINWINDOW_H

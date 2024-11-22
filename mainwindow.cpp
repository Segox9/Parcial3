#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //-----------------------------------
    plot();
    fase();
    Potencia();
    PotCorrb();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readFile(const char *arch, dato &dat)
{
    FILE *f = fopen(arch, "rb");
    if(f == NULL){
        qDebug() << "erro al abrir el archivo";
        return;
    }
    double a;
    while(fread(&a, sizeof(double), 1, f)){
        dat.time.append(a);
        fread(&a, sizeof(double), 1 , f);
        dat.voltage.append(a);
        fread(&a, sizeof(double), 1, f);
        dat.current.append(a);
    }
}
void MainWindow::plot(){
    readFile("C:\\Users\\lauta\\Documents\\Qt\\parcial3\\vi_3.dat", dat);

    //grafico la corriente
    ui->widget->addGraph();
    ui->widget->graph(0)->setData(dat.time , dat.current);
    ui->widget->graph(0)->setPen(QPen(Qt::red, 2));
    //grafico la tension
    ui->widget->addGraph();
    ui->widget->graph(1)->setData(dat.time, dat.voltage);
    ui->widget->graph(1)->setPen(QPen(Qt::blue, 2));
    ui->widget->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
    ui->widget->rescaleAxes();
    ui->widget->replot();
}
void MainWindow::fase() {
    double tiempoI;
    for(int i=0; i<(dat.current.size() -1); i++){
        if(dat.current[i] < 0 && dat.current[i+1] > 0){
            double t1 = dat.time[i];
            double t2 = dat.time[i + 1];
            double c1 = dat.current[i];
            double c2 = dat.current[i + 1];
            tiempoI = t1 - (c1 * (t2 - t1) / (c2 - c1));
        }
    }
    qDebug() << "Tiempo de corriente " << tiempoI;
    double tiempo = tiempoI;
    tiempo = tiempo/0.01;
    double angulo = 180.0*tiempo;
    qDebug() << "Angulo desfase " << angulo;
    ui->Fase->setText(QString("Angulo de fase %1º").arg(angulo));
}
double MainWindow::Vrms(dato& x){
    double tmax = dat.time.back();
    double tmin = dat.time.front();
    qDebug()<< "Tmax " << tmax;
    qDebug()<< "Tmin " << tmin;

    double h1, h2, delta_T;
    double rms=0;
    for(int i=0; i<(x.voltage.size()-1); i++){
        h1= (x.voltage[i+1]*x.voltage[i+1]) - (x.voltage[i]*x.voltage[i]);
        h2 = x.voltage[i]*x.voltage[i];
        delta_T = x.time[i+1] - x.time[i];
        rms += ((h1/2.0) + h2)*delta_T;
    }
    double dift = 1/(tmax-tmin);
    rms = dift*rms;
    double vrms = sqrt(rms);
    qDebug() << "VRMS " << vrms;
    return vrms;
}
double MainWindow::Irms(dato& x){
    double tmax = dat.time.back();
    double tmin = dat.time.front();
    qDebug()<< "Tmax " << tmax;
    qDebug()<< "Tmin " << tmin;

    double h1, h2, delta_T;
    double rms=0;

    for(int i=0; i<(x.current.size()-1); i++){
        h1= (x.current[i+1]*x.current[i+1]) - (x.current[i]*x.current[i]);
        h2 = x.current[i]*x.current[i];
        delta_T = x.time[i+1] - x.time[i];
        rms += ((h1/2.0) + h2)*delta_T;
    }
    double dift = 1/(tmax-tmin);
    rms = dift * rms;
    double irms = sqrt(rms);
    qDebug() << "IRMS " << irms;
    return irms;
}

void MainWindow::Potencia(){
    double pot;
    double tiempoI;
    for(int i=0; i<(dat.current.size() - 1); i++){
        if(dat.current[i] < 0 && dat.current[i+1] > 0){
            double t1 = dat.time[i];
            double t2 = dat.time[i + 1];
            double c1 = dat.current[i];
            double c2 = dat.current[i + 1];
            tiempoI = t1 - (c1 * (t2 - t1) / (c2 - c1));
        }
    }
    double tiempo = tiempoI;
    tiempo = tiempo/0.01;
    double angulo = 180.0*tiempo;
    double angrad = (angulo*M_PI)/180.0;
    qDebug() << "Angulo en rad " << angrad;
    double vrms = Vrms(dat);
    double irms = Irms(dat);
    pot = vrms * irms * cos(angrad);
    ui->Vrms->setText(QString("Valor de vrms %1").arg(vrms));
    ui->Irms->setText(QString("Valor de irms %1").arg(irms));
    ui->Pot->setText(QString("Valor de potencia %1").arg(pot));
}
void MainWindow::PotCorrb()
{
    double tmin = dat.time.front();
    double tmax = dat.time.back();
    double dift = 1/(tmax - tmin);
    double h1, h2, delta_T;
    double intg = 0;
    for(int i=0; i<(dat.current.size() - 1); i++){
        h1 = (dat.current[i+1]*dat.voltage[i+1]) - (dat.voltage[i]*dat.current[i]);
        h2 = dat.current[i]*dat.voltage[i];
        delta_T = dat.time[i+1] - dat.time[i];
        intg += ((h1/2) + h2)*delta_T;
    }
    double pot = dift*intg;
    ui->pcorrb->setText(QString("Valor de potencia verificado %1").arg(pot));
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "simulator.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Simulator& simulator;
    MainWindow(Simulator& simulator, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_InsertProcess_released();
    void on_comboBox_currentTextChanged(const QString &arg1);
    void on_pushButton_StartSimulation_released();
    void updateBurstTime(size_t pid, double newTime);
    void on_simulationFinished(std::tuple<double,double,double> result);
    void on_updateSimulationTime(double newTime);
    void on_checkBox_instantSimulation_checkStateChanged(const Qt::CheckState &arg1);

    void on_pushButton_reset_released();

private:
    Ui::MainWindow *ui;
    void updateGanttChartWindow();
};
#endif // MAINWINDOW_H

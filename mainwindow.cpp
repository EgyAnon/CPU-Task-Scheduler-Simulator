#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QGraphicsScene"
#include "QGraphicsRectItem"

MainWindow::MainWindow(Simulator& _simulator, QWidget *parent)
    : QMainWindow(parent)
    , simulator(_simulator)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_Priority->setVisible(false);
    ui->lineEdit_Priority->setVisible(false);
    ui->label_Quantum->setVisible(false);
    ui->spinBox_Quantum->setVisible(false);
    ui->spinBox_Quantum->setMinimum(1);
    ui->processTable->setColumnHidden(1,true);
    connect(&simulator, &Simulator::signal_processBurstTimeUpdated, this, &MainWindow::updateBurstTime);
    connect(&simulator, &Simulator::signal_simulationFinished, this, &MainWindow::on_simulationFinished);
    connect(&simulator, &Simulator::signal_updateSimulationTime, this, &MainWindow::on_updateSimulationTime);

    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->GanttChartWindow->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_InsertProcess_released()
{
    // Get values from input boxes
    QString burstTimeStr = ui->lineEdit_BurstTime->text();

    /*TODO: Add more robust checking logic*/
    if(burstTimeStr.isEmpty()){
        ui->label_ProgramMessages->setText("Empty Field!");
        ui->lineEdit_ArrivalTime->clear();
        ui->lineEdit_BurstTime->clear();
        return;
    }

    bool validBurst = false;
    size_t burstTime = burstTimeStr.toUInt(&validBurst);

    if(!validBurst){
        ui->label_ProgramMessages->setText("Invalid Arguments!");
        ui->lineEdit_ArrivalTime->clear();
        ui->lineEdit_BurstTime->clear();
        return;
    }

    size_t pid;

    if(simulator.IsRunning()){
        pid = simulator.addProcessDynamically(burstTime);
    }

    else{
        QString arrivalTimeStr = ui->lineEdit_ArrivalTime->text();
        if(arrivalTimeStr.isEmpty()){
            ui->label_ProgramMessages->setText("Empty Field!");
            ui->lineEdit_ArrivalTime->clear();
            ui->lineEdit_BurstTime->clear();
            return;
        }
        bool validArrival = false;
        size_t arrivalTime = arrivalTimeStr.toUInt(&validArrival);
        if(!validArrival){
            ui->label_ProgramMessages->setText("Invalid Arguments!");
            ui->lineEdit_ArrivalTime->clear();
            ui->lineEdit_BurstTime->clear();
            return;
        }
        pid = simulator.addProcess(arrivalTime,burstTime);
    }

    QString currentScheduler = ui->comboBox->currentText();
    bool validPriority = false;

    if(currentScheduler == "Priority (Preemptive)"|| currentScheduler == "Priority (Non-Preemptive)"){
        QString priorityString = ui->lineEdit_Priority->text();

        if(priorityString.isEmpty()){
            ui->label_ProgramMessages->setText("Invalid Arguments!");
            ui->lineEdit_ArrivalTime->clear();
            ui->lineEdit_BurstTime->clear();
            ui->lineEdit_Priority->clear();
            return;
        }

        size_t processPriority = priorityString.toUInt(&validPriority);

        if(!validPriority){
            ui->label_ProgramMessages->setText("Invalid Arguments!");
            ui->lineEdit_ArrivalTime->clear();
            ui->lineEdit_BurstTime->clear();
            ui->lineEdit_Priority->clear();
            return;
        }

        simulator.processVector[pid].priority = processPriority;
    }

    // Add a new row to the table
    int rowCount = ui->processTable->rowCount();
    ui->processTable->insertRow(rowCount);

    QTableWidgetItem* tableItem_pid = new QTableWidgetItem(QString::number(this->simulator.processVector[pid].pid));
    tableItem_pid->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem* tableItem_priority = new QTableWidgetItem(QString::number(this->simulator.processVector[pid].priority));
    tableItem_priority->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem* tableItem_arrivalTime = new QTableWidgetItem(QString::number(this->simulator.processVector[pid].arrival_time));
    tableItem_arrivalTime->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem* tableItem_burstTime = new QTableWidgetItem(QString::number(this->simulator.processVector[pid].burst_time));
    tableItem_burstTime->setTextAlignment(Qt::AlignCenter);

    // Add values to the table
    ui->processTable->setItem(rowCount, 0, tableItem_pid);
    ui->processTable->setItem(rowCount, 1, tableItem_priority);
    ui->processTable->setItem(rowCount, 2, tableItem_arrivalTime);
    ui->processTable->setItem(rowCount, 3, tableItem_burstTime);

    // Clear input boxes
    ui->lineEdit_ArrivalTime->clear();
    ui->lineEdit_BurstTime->clear();
    ui->lineEdit_Priority->clear();
    ui->label_ProgramMessages->clear();
}


void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    QString currentScheduler = ui->comboBox->currentText();

    ui->label_Priority->setVisible(false);
    ui->lineEdit_Priority->setVisible(false);
    ui->spinBox_Quantum->setVisible(false);
    ui->label_Quantum->setVisible(false);
    ui->processTable->setColumnHidden(1,true);

    if(currentScheduler == "Round-Robin"){
        ui->label_Quantum->setVisible(true);
        ui->spinBox_Quantum->setVisible(true);
    }

    else if(currentScheduler == "First-Come, First Serve"){
        simulator.setScheduler("FCFS");
    }

    else if(currentScheduler == "Shortest Job First (Preemptive)"){
        simulator.setScheduler("SJF Preemptive");
    }

    else if(currentScheduler == "Shortest Job First (Non-Preemptive)"){
        simulator.setScheduler("SJF Non-Preemptive");
    }

    else if(currentScheduler == "Priority (Preemptive)"){
        ui->lineEdit_Priority->setVisible(true);
        ui->label_Priority->setVisible(true);
        ui->processTable->setColumnHidden(1,false);

        simulator.setScheduler("Priority Preemptive");
    }

    else if(currentScheduler == "Priority (Non-Preemptive)"){
        ui->lineEdit_Priority->setVisible(true);
        ui->label_Priority->setVisible(true);
        ui->processTable->setColumnHidden(1,false);

        simulator.setScheduler("SJF Non-Preemptive");
    }

    else{
        ui->label_ProgramMessages->setText("ERROR");
    }
}


void MainWindow::on_pushButton_StartSimulation_released()
{
    ui->pushButton_StartSimulation->setDisabled(true);
    QString currentScheduler = ui->comboBox->currentText();

    if(currentScheduler == "Round-Robin"){
        size_t timeQuantum = ui->spinBox_Quantum->value();
        simulator.setScheduler("RR", timeQuantum);
    }

    else if(currentScheduler == "First-Come, First Serve"){
        simulator.setScheduler("FCFS");
    }

    else if(currentScheduler == "Shortest Job First (Preemptive)"){
        simulator.setScheduler("SJF Preemptive");
    }

    else if(currentScheduler == "Shortest Job First (Non-Preemptive)"){
        simulator.setScheduler("SJF Non-Preemptive");
    }

    else if(currentScheduler == "Priority (Preemptive)"){
        simulator.setScheduler("Priority Preemptive");
    }

    else if(currentScheduler == "Priority (Non-Preemptive)"){
        simulator.setScheduler("SJF Non-Preemptive");
    }

    else{
        ui->label_ProgramMessages->setText("ERROR");
        return;
    }

    ui->lineEdit_ArrivalTime->setHidden(true);
    ui->label_ArrivalTime->setHidden(true);

    ui->spinBox_Quantum->setDisabled(true);
    ui->comboBox->setDisabled(true);
    ui->checkBox_instantSimulation->setDisabled(true);

    simulator.start();
}

void MainWindow::updateBurstTime(size_t pid, size_t newTime){
    // Access the Remaining Burst Time in the third column (assuming it's column 2)
    QTableWidgetItem *burstTimeItem = ui->processTable->item(pid, 3);
    if (burstTimeItem) {
        // Update the item with the new remaining burst time
        burstTimeItem->setText(QString::number(newTime));
    }
}

void MainWindow::on_simulationFinished(std::tuple<double,double,double> result){
    //TODO:
    //1. Enable Buttons & GUI Elements
    //2. Display Statistics
    //3. Display Gantt Chart

    ui->lineEdit_ArrivalTime->setHidden(false);
    ui->label_ArrivalTime->setHidden(false);

    ui->spinBox_Quantum->setDisabled(false);
    ui->comboBox->setDisabled(false);
    ui->checkBox_instantSimulation->setDisabled(false);
    ui->pushButton_StartSimulation->setEnabled(true);

    this->updateGanttChartWindow();
    ui->textEdit_Statistics->setText(
        "Average Turnaround: " + QString::number(std::get<0>(result)) + "\n"
        +"Average Waiting Time: " + QString::number(std::get<1>(result)) + "\n"
        +"Average Time Until First Response: " + QString::number(std::get<2>(result)) + "\n"
    );
}

void MainWindow::updateGanttChartWindow(){
    // Clear existing chart
    ui->GanttChartWindow->scene()->clear();

    // Add new data points
    for (const auto& log : simulator.executionLog) {
        int pid = std::get<0>(log);
        int startTime = std::get<1>(log) / 1000; // Convert milliseconds to seconds
        int endTime = std::get<2>(log) / 1000;   // Convert milliseconds to seconds

        // Draw a rectangle for the process
        QGraphicsRectItem *rect = new QGraphicsRectItem(startTime * 50, 0, (endTime - startTime) * 50, 50);
        rect->setBrush(QBrush(Qt::gray));
        ui->GanttChartWindow->scene()->addItem(rect);

        // Calculate the center position of the rectangle
        double centerX = (startTime + (endTime - startTime) / 2.0) * 50; // Center X-coordinate
        QString processName = QString("P%1").arg(pid);                  // Process name (e.g., "P1")
        QGraphicsSimpleTextItem *processLabel = new QGraphicsSimpleTextItem(processName);

        // Set font size and make it bold
        QFont font = processLabel->font();
        font.setPointSize(12); // Increase font size
        font.setBold(true);   // Make the text bold
        processLabel->setFont(font);

        // Adjust the text position to center it
        double textWidth = processLabel->boundingRect().width(); // Width of the text
        double textHeight = processLabel->boundingRect().height(); // Height of the text
        processLabel->setPos(centerX - textWidth / 2, 25 - textHeight / 2); // Center the text
        ui->GanttChartWindow->scene()->addItem(processLabel);

        // Add start time label
        QGraphicsSimpleTextItem *startTimeLabel = new QGraphicsSimpleTextItem(QString::number(startTime));
        startTimeLabel->setFont(font); // Use the same font as the process name
        startTimeLabel->setPos(startTime * 50, 60); // Position below the rectangle
        ui->GanttChartWindow->scene()->addItem(startTimeLabel);

        // Add end time label
        QGraphicsSimpleTextItem *endTimeLabel = new QGraphicsSimpleTextItem(QString::number(endTime));
        endTimeLabel->setFont(font); // Use the same font as the process name
        endTimeLabel->setPos(endTime * 50, 60); // Position below the rectangle
        ui->GanttChartWindow->scene()->addItem(endTimeLabel);
    }
}

void MainWindow::on_checkBox_instantSimulation_checkStateChanged(const Qt::CheckState &arg1)
{
    if(arg1 == 0){
        simulator.setSpeedup(1);
    }
    else{
        simulator.setSpeedup(MAX_SPEEDUP);
    }
}

void MainWindow::on_updateSimulationTime(double newTime){
    ui->label_simulationTime->setText("Simulation Time: " + QString::number(newTime));
}


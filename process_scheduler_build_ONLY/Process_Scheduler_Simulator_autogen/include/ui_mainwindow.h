/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_processTable;
    QHBoxLayout *horizontalLayout_2;
    QTableWidget *processTable;
    QVBoxLayout *verticalLayout;
    QFormLayout *layout_inputForms;
    QLabel *label_ArrivalTime;
    QLineEdit *lineEdit_ArrivalTime;
    QLabel *label_BurstTime;
    QLineEdit *lineEdit_BurstTime;
    QLabel *label_Priority;
    QLineEdit *lineEdit_Priority;
    QPushButton *pushButton_InsertProcess;
    QVBoxLayout *layout_input;
    QHBoxLayout *layout_combobox;
    QComboBox *comboBox;
    QCheckBox *checkBox_instantSimulation;
    QHBoxLayout *layout_quantum;
    QLabel *label_Quantum;
    QSpinBox *spinBox_Quantum;
    QHBoxLayout *layout_buttons;
    QPushButton *pushButton_StartSimulation;
    QPushButton *pushButton_reset;
    QLabel *label_ProgramMessages;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_Statistics;
    QTextEdit *textEdit_Statistics;
    QLabel *label_simulationTime;
    QLabel *label;
    QGraphicsView *GanttChartWindow;
    QMenuBar *menubar;
    QMenu *menuCPU_Task_Scheduler_Simulator;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1286, 766);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy1);
        horizontalLayout_3 = new QHBoxLayout(centralwidget);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);
        verticalLayout_2->setContentsMargins(20, 20, 20, 20);
        label_processTable = new QLabel(centralwidget);
        label_processTable->setObjectName("label_processTable");
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        label_processTable->setFont(font);

        verticalLayout_2->addWidget(label_processTable);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(10, 10, 10, 10);
        processTable = new QTableWidget(centralwidget);
        if (processTable->columnCount() < 4)
            processTable->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        processTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        processTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        processTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        processTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        processTable->setObjectName("processTable");
        processTable->setMaximumSize(QSize(4000, 16777215));
        processTable->setFrameShadow(QFrame::Shadow::Raised);
        processTable->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        processTable->setColumnCount(4);
        processTable->horizontalHeader()->setCascadingSectionResizes(true);
        processTable->horizontalHeader()->setStretchLastSection(true);
        processTable->verticalHeader()->setCascadingSectionResizes(true);

        horizontalLayout_2->addWidget(processTable);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(10, 10, 10, 10);
        layout_inputForms = new QFormLayout();
        layout_inputForms->setObjectName("layout_inputForms");
        layout_inputForms->setLabelAlignment(Qt::AlignmentFlag::AlignCenter);
        layout_inputForms->setFormAlignment(Qt::AlignmentFlag::AlignCenter);
        layout_inputForms->setContentsMargins(5, 5, 5, 5);
        label_ArrivalTime = new QLabel(centralwidget);
        label_ArrivalTime->setObjectName("label_ArrivalTime");

        layout_inputForms->setWidget(0, QFormLayout::ItemRole::LabelRole, label_ArrivalTime);

        lineEdit_ArrivalTime = new QLineEdit(centralwidget);
        lineEdit_ArrivalTime->setObjectName("lineEdit_ArrivalTime");

        layout_inputForms->setWidget(0, QFormLayout::ItemRole::FieldRole, lineEdit_ArrivalTime);

        label_BurstTime = new QLabel(centralwidget);
        label_BurstTime->setObjectName("label_BurstTime");

        layout_inputForms->setWidget(1, QFormLayout::ItemRole::LabelRole, label_BurstTime);

        lineEdit_BurstTime = new QLineEdit(centralwidget);
        lineEdit_BurstTime->setObjectName("lineEdit_BurstTime");

        layout_inputForms->setWidget(1, QFormLayout::ItemRole::FieldRole, lineEdit_BurstTime);

        label_Priority = new QLabel(centralwidget);
        label_Priority->setObjectName("label_Priority");

        layout_inputForms->setWidget(2, QFormLayout::ItemRole::LabelRole, label_Priority);

        lineEdit_Priority = new QLineEdit(centralwidget);
        lineEdit_Priority->setObjectName("lineEdit_Priority");

        layout_inputForms->setWidget(2, QFormLayout::ItemRole::FieldRole, lineEdit_Priority);

        pushButton_InsertProcess = new QPushButton(centralwidget);
        pushButton_InsertProcess->setObjectName("pushButton_InsertProcess");

        layout_inputForms->setWidget(3, QFormLayout::ItemRole::FieldRole, pushButton_InsertProcess);


        verticalLayout->addLayout(layout_inputForms);

        layout_input = new QVBoxLayout();
        layout_input->setObjectName("layout_input");
        layout_input->setContentsMargins(10, 10, 10, 10);
        layout_combobox = new QHBoxLayout();
        layout_combobox->setObjectName("layout_combobox");
        layout_combobox->setContentsMargins(5, 5, 5, 5);
        comboBox = new QComboBox(centralwidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");

        layout_combobox->addWidget(comboBox);

        checkBox_instantSimulation = new QCheckBox(centralwidget);
        checkBox_instantSimulation->setObjectName("checkBox_instantSimulation");

        layout_combobox->addWidget(checkBox_instantSimulation);


        layout_input->addLayout(layout_combobox);

        layout_quantum = new QHBoxLayout();
        layout_quantum->setObjectName("layout_quantum");
        layout_quantum->setContentsMargins(5, 5, 5, 5);
        label_Quantum = new QLabel(centralwidget);
        label_Quantum->setObjectName("label_Quantum");

        layout_quantum->addWidget(label_Quantum);

        spinBox_Quantum = new QSpinBox(centralwidget);
        spinBox_Quantum->setObjectName("spinBox_Quantum");

        layout_quantum->addWidget(spinBox_Quantum);


        layout_input->addLayout(layout_quantum);

        layout_buttons = new QHBoxLayout();
        layout_buttons->setObjectName("layout_buttons");
        layout_buttons->setContentsMargins(5, 5, 5, 5);
        pushButton_StartSimulation = new QPushButton(centralwidget);
        pushButton_StartSimulation->setObjectName("pushButton_StartSimulation");

        layout_buttons->addWidget(pushButton_StartSimulation);

        pushButton_reset = new QPushButton(centralwidget);
        pushButton_reset->setObjectName("pushButton_reset");

        layout_buttons->addWidget(pushButton_reset);


        layout_input->addLayout(layout_buttons);

        label_ProgramMessages = new QLabel(centralwidget);
        label_ProgramMessages->setObjectName("label_ProgramMessages");
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(false);
        label_ProgramMessages->setFont(font1);
        label_ProgramMessages->setStyleSheet(QString::fromUtf8("color:red;"));
        label_ProgramMessages->setAlignment(Qt::AlignmentFlag::AlignCenter);

        layout_input->addWidget(label_ProgramMessages);


        verticalLayout->addLayout(layout_input);


        horizontalLayout_2->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_4->setContentsMargins(5, 5, 5, 5);
        label_Statistics = new QLabel(centralwidget);
        label_Statistics->setObjectName("label_Statistics");
        label_Statistics->setFont(font);

        verticalLayout_4->addWidget(label_Statistics);

        textEdit_Statistics = new QTextEdit(centralwidget);
        textEdit_Statistics->setObjectName("textEdit_Statistics");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(textEdit_Statistics->sizePolicy().hasHeightForWidth());
        textEdit_Statistics->setSizePolicy(sizePolicy2);
        textEdit_Statistics->setReadOnly(true);

        verticalLayout_4->addWidget(textEdit_Statistics);


        horizontalLayout->addLayout(verticalLayout_4);

        label_simulationTime = new QLabel(centralwidget);
        label_simulationTime->setObjectName("label_simulationTime");
        label_simulationTime->setFont(font);
        label_simulationTime->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);

        horizontalLayout->addWidget(label_simulationTime);


        verticalLayout_2->addLayout(horizontalLayout);

        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setFont(font);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(label);

        GanttChartWindow = new QGraphicsView(centralwidget);
        GanttChartWindow->setObjectName("GanttChartWindow");

        verticalLayout_2->addWidget(GanttChartWindow);


        horizontalLayout_3->addLayout(verticalLayout_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1286, 21));
        menuCPU_Task_Scheduler_Simulator = new QMenu(menubar);
        menuCPU_Task_Scheduler_Simulator->setObjectName("menuCPU_Task_Scheduler_Simulator");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuCPU_Task_Scheduler_Simulator->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_processTable->setText(QCoreApplication::translate("MainWindow", "Process Table", nullptr));
        QTableWidgetItem *___qtablewidgetitem = processTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "PID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = processTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Priority", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = processTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Arrival Time", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = processTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Remaining Burst Time", nullptr));
        label_ArrivalTime->setText(QCoreApplication::translate("MainWindow", "Arrival Time", nullptr));
        label_BurstTime->setText(QCoreApplication::translate("MainWindow", "Burst Time", nullptr));
        label_Priority->setText(QCoreApplication::translate("MainWindow", "Priority", nullptr));
        pushButton_InsertProcess->setText(QCoreApplication::translate("MainWindow", "Insert Process", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "First-Come, First Serve", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Round-Robin", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Priority (Preemptive)", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Priority (Non-Preemptive)", nullptr));
        comboBox->setItemText(4, QCoreApplication::translate("MainWindow", "Shortest Job First (Preemptive)", nullptr));
        comboBox->setItemText(5, QCoreApplication::translate("MainWindow", "Shortest Job First (Non-Preemptive)", nullptr));

        checkBox_instantSimulation->setText(QCoreApplication::translate("MainWindow", "Instant Simulation", nullptr));
        label_Quantum->setText(QCoreApplication::translate("MainWindow", "Time Quantum", nullptr));
        pushButton_StartSimulation->setText(QCoreApplication::translate("MainWindow", "Start Simulation", nullptr));
        pushButton_reset->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        label_ProgramMessages->setText(QString());
        label_Statistics->setText(QCoreApplication::translate("MainWindow", "Statistics", nullptr));
        label_simulationTime->setText(QCoreApplication::translate("MainWindow", "Simulation Time:", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Gantt Chart", nullptr));
        menuCPU_Task_Scheduler_Simulator->setTitle(QCoreApplication::translate("MainWindow", "CPU Task Scheduler Simulator", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

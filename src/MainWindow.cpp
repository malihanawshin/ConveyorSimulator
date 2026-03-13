#include "MainWindow.h"

#include <QHeaderView>
#include <QVBoxLayout>
#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QWidget>
#include <QGroupBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), engine("config/routing.json") {
    setupUi();
    refreshUi();
}

void MainWindow::setupUi() {
    auto* central = new QWidget(this);

    auto* mainLayout = new QVBoxLayout();
    auto* statusGrid = new QGridLayout();
    auto* buttonLayout = new QHBoxLayout();
    auto* historyLayout = new QVBoxLayout();

    auto* overviewGroup = new QGroupBox("Machine Overview", this);
    auto* controlsGroup = new QGroupBox("Operator Controls", this);
    auto* historyGroup = new QGroupBox("Alarm and Event History", this);

    machineStateLabel = new QLabel(this);
    conveyorLabel = new QLabel(this);
    gateLabel = new QLabel(this);
    sensorLabel = new QLabel(this);
    blockedTicksLabel = new QLabel(this);
    alarmLabel = new QLabel(this);
    

    startButton = new QPushButton("Start Machine", this);
    typeAButton = new QPushButton("Process TypeA", this);
    typeBButton = new QPushButton("Process TypeB", this);
    jamButton = new QPushButton("Simulate Jam Fault", this);
    resetButton = new QPushButton("Reset", this);

    historyTable = new QTableWidget(this);
    historyTable->setColumnCount(3);
    historyTable->setHorizontalHeaderLabels({"Timestamp", "Type", "Message"});
    historyTable->horizontalHeader()->setStretchLastSection(true);
    historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    historyTable->setSelectionMode(QAbstractItemView::SingleSelection);

    machineStateLabel->setAlignment(Qt::AlignCenter);
    conveyorLabel->setAlignment(Qt::AlignCenter);
    gateLabel->setAlignment(Qt::AlignCenter);
    sensorLabel->setAlignment(Qt::AlignCenter);
    blockedTicksLabel->setAlignment(Qt::AlignCenter);
    alarmLabel->setAlignment(Qt::AlignCenter);
   
    machineStateLabel->setMinimumHeight(40);
    conveyorLabel->setMinimumHeight(40);
    gateLabel->setMinimumHeight(40);
    sensorLabel->setMinimumHeight(40);
    blockedTicksLabel->setMinimumHeight(40);
    alarmLabel->setMinimumHeight(40);

    startButton->setMinimumHeight(36);
    typeAButton->setMinimumHeight(36);
    typeBButton->setMinimumHeight(36);
    jamButton->setMinimumHeight(36);
    resetButton->setMinimumHeight(36);

    statusGrid->setHorizontalSpacing(10);
    statusGrid->setVerticalSpacing(10);
    statusGrid->setContentsMargins(8, 8, 8, 8);

    statusGrid->addWidget(machineStateLabel, 0, 0);
    statusGrid->addWidget(conveyorLabel,     0, 1);
    statusGrid->addWidget(gateLabel,         1, 0);
    statusGrid->addWidget(sensorLabel,       1, 1);
    statusGrid->addWidget(blockedTicksLabel, 2, 0);
    statusGrid->addWidget(alarmLabel,        2, 1);

    statusGrid->setColumnStretch(0, 1);
    statusGrid->setColumnStretch(1, 1);

    overviewGroup->setLayout(statusGrid);

    buttonLayout->setSpacing(10);
    buttonLayout->setContentsMargins(8, 8, 8, 8);
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(typeAButton);
    buttonLayout->addWidget(typeBButton);
    buttonLayout->addWidget(jamButton);
    buttonLayout->addWidget(resetButton);

    controlsGroup->setLayout(buttonLayout);

    historyLayout->setContentsMargins(8, 8, 8, 8);
    historyLayout->addWidget(historyTable);
    historyGroup->setLayout(historyLayout);

    QString groupStyle =
        "QGroupBox { "
        "font-weight: bold; "
        "border: 1px solid #b0b0b0; "
        "border-radius: 6px; "
        "margin-top: 10px; "
        "padding-top: 10px; "
        "} "
        "QGroupBox::title { "
        "subcontrol-origin: margin; "
        "left: 10px; "
        "padding: 0 4px 0 4px; "
        "}";

    overviewGroup->setStyleSheet(groupStyle);
    controlsGroup->setStyleSheet(groupStyle);
    historyGroup->setStyleSheet(groupStyle);

    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->addWidget(overviewGroup);
    mainLayout->addWidget(controlsGroup);
    mainLayout->addWidget(historyGroup, 1);

    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(typeAButton, &QPushButton::clicked, this, &MainWindow::onTypeAClicked);
    connect(typeBButton, &QPushButton::clicked, this, &MainWindow::onTypeBClicked);
    connect(jamButton, &QPushButton::clicked, this, &MainWindow::onJamClicked);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);

    central->setLayout(mainLayout);
    setCentralWidget(central);
    setWindowTitle("Conveyor Sorting Cell HMI");
    resize(950, 700);
}



void MainWindow::refreshUi() {
    SimulationSnapshot snapshot = engine.getSnapshot();

    machineStateLabel->setText(QString("Machine State: %1").arg(QString::fromStdString(snapshot.machineState)));
    updateMachineStateStyle(snapshot.machineState);

    conveyorLabel->setText(QString("Conveyor Running: %1").arg(snapshot.conveyorRunning ? "YES" : "NO"));
    updateConveyorLabelStyle(snapshot.conveyorRunning);
    
    gateLabel->setText(QString("Gate Position: %1").arg(QString::fromStdString(snapshot.gatePosition)));
    updateGateLabelStyle(snapshot.gatePosition);
    
    blockedTicksLabel->setText(QString("Sensor Blocked Ticks: %1").arg(snapshot.sensorBlockedTicks));
    updateBlockedTicksLabelStyle(snapshot.sensorBlockedTicks);

    alarmLabel->setText(QString("Latest Alarm: %1").arg(QString::fromStdString(snapshot.latestAlarm)));
    updateAlarmLabelStyle(snapshot.latestAlarm);

    sensorLabel->setText(QString("Sensor Blocked: %1").arg(snapshot.sensorBlocked ? "YES" : "NO"));
    updateSensorLabelStyle(snapshot.sensorBlocked);

    refreshHistoryTable();
}

void MainWindow::refreshHistoryTable() {
    const auto& history = engine.getEventHistory();

    historyTable->setRowCount(0);

    for (int i = 0; i < static_cast<int>(history.size()); ++i) {
        historyTable->insertRow(i);

        auto* timestampItem = new QTableWidgetItem(QString::fromStdString(history[i].timestamp));
        auto* typeItem = new QTableWidgetItem(QString::fromStdString(history[i].type));
        auto* messageItem = new QTableWidgetItem(QString::fromStdString(history[i].message));

        QColor backgroundColor;
        QColor foregroundColor;

        if (history[i].type == "ALARM") {
            backgroundColor = QColor(255, 220, 220);   // light red
            foregroundColor = QColor(120, 0, 0);       // dark red text
        } else {
            backgroundColor = QColor(220, 235, 255);   // light blue
            foregroundColor = QColor(0, 40, 90);       // dark blue text
        }

        timestampItem->setBackground(QBrush(backgroundColor));
        typeItem->setBackground(QBrush(backgroundColor));
        messageItem->setBackground(QBrush(backgroundColor));

        timestampItem->setForeground(QBrush(foregroundColor));
        typeItem->setForeground(QBrush(foregroundColor));
        messageItem->setForeground(QBrush(foregroundColor));

        historyTable->setItem(i, 0, timestampItem);
        historyTable->setItem(i, 1, typeItem);
        historyTable->setItem(i, 2, messageItem);
    }

    historyTable->scrollToBottom();
}

void MainWindow::updateMachineStateStyle(const std::string& machineState) {
    QString style;

    if (machineState == "Running") {
        style = "QLabel { "
                "background-color: #d4edda; "
                "color: #155724; "
                "border: 1px solid #155724; "
                "border-radius: 8px; "
                "padding: 8px; "
                "font-weight: bold; "
                "}";
    } else if (machineState == "Starting") {
        style = "QLabel { "
                "background-color: #fff3cd; "
                "color: #856404; "
                "border: 1px solid #856404; "
                "border-radius: 8px; "
                "padding: 8px; "
                "font-weight: bold; "
                "}";
    } else if (machineState == "Fault" || machineState == "EmergencyStop") {
        style = "QLabel { "
                "background-color: #f8d7da; "
                "color: #721c24; "
                "border: 1px solid #721c24; "
                "border-radius: 8px; "
                "padding: 8px; "
                "font-weight: bold; "
                "}";
    } else {
        style = "QLabel { "
                "background-color: #e2e3e5; "
                "color: #383d41; "
                "border: 1px solid #383d41; "
                "border-radius: 8px; "
                "padding: 8px; "
                "font-weight: bold; "
                "}";
    }

    machineStateLabel->setStyleSheet(style);
}

void MainWindow::updateConveyorLabelStyle(bool isRunning) {
    QString style;

    if (isRunning) {
        style = "QLabel { "
                "background-color: #d4edda; "
                "color: #155724; "
                "border: 1px solid #155724; "
                "border-radius: 8px; "
                "padding: 8px; "
                "font-weight: bold; "
                "}";
    } else {
        style = "QLabel { "
                "background-color: #e2e3e5; "
                "color: #383d41; "
                "border: 1px solid #383d41; "
                "border-radius: 8px; "
                "padding: 8px; "
                "font-weight: bold; "
                "}";
    }

    conveyorLabel->setStyleSheet(style);
}

void MainWindow::updateGateLabelStyle(const std::string& gatePosition) {
    QString style;

    if (gatePosition == "LEFT") {
        style = "QLabel { "
                "background-color: #d1ecf1; "
                "color: #0c5460; "
                "border: 1px solid #0c5460; "
                "border-radius: 8px; "
                "padding: 8px; "
                "font-weight: bold; "
                "}";
    } else if (gatePosition == "RIGHT") {
        style = "QLabel { "
                "background-color: #e2d6f3; "
                "color: #4b2e83; "
                "border: 1px solid #4b2e83; "
                "border-radius: 8px; "
                "padding: 8px; "
                "font-weight: bold; "
                "}";
    } else {
        style = "QLabel { "
                "background-color: #e2e3e5; "
                "color: #383d41; "
                "border: 1px solid #383d41; "
                "border-radius: 8px; "
                "padding: 8px; "
                "font-weight: bold; "
                "}";
    }

    gateLabel->setStyleSheet(style);
}


void MainWindow::updateSensorLabelStyle(bool isBlocked) {
    QString style;

    if (isBlocked) {
        style = "QLabel { "
                "background-color: #fff3cd; "
                "color: #856404; "
                "border: 1px solid #856404; "
                "border-radius: 8px; "
                "padding: 8px; "
                "font-weight: bold; "
                "}";
    } else {
        style = "QLabel { "
                "background-color: #d1ecf1; "
                "color: #0c5460; "
                "border: 1px solid #0c5460; "
                "border-radius: 8px; "
                "padding: 8px; "
                "font-weight: bold; "
                "}";
    }

    sensorLabel->setStyleSheet(style);
}

void MainWindow::updateAlarmLabelStyle(const std::string& latestAlarm) {
    QString style;

    if (latestAlarm != "No active alarms") {
        style = "QLabel { "
                "background-color: #f8d7da; "
                "color: #721c24; "
                "border: 1px solid #721c24; "
                "border-radius: 8px; "
                "padding: 8px; "
                "font-weight: bold; "
                "}";
    } else {
        style = "QLabel { "
                "background-color: #d4edda; "
                "color: #155724; "
                "border: 1px solid #155724; "
                "border-radius: 8px; "
                "padding: 8px; "
                "font-weight: bold; "
                "}";
    }

    alarmLabel->setStyleSheet(style);
}

void MainWindow::updateBlockedTicksLabelStyle(int blockedTicks) {
    QString style;

    if (blockedTicks >= 5) {
        style = "QLabel { "
                "background-color: #f8d7da; "
                "color: #721c24; "
                "border: 1px solid #721c24; "
                "border-radius: 8px; "
                "padding: 8px; "
                "font-weight: bold; "
                "}";
    } else if (blockedTicks > 0) {
        style = "QLabel { "
                "background-color: #fff3cd; "
                "color: #856404; "
                "border: 1px solid #856404; "
                "border-radius: 8px; "
                "padding: 8px; "
                "font-weight: bold; "
                "}";
    } else {
        style = "QLabel { "
                "background-color: #d4edda; "
                "color: #155724; "
                "border: 1px solid #155724; "
                "border-radius: 8px; "
                "padding: 8px; "
                "font-weight: bold; "
                "}";
    }

    blockedTicksLabel->setStyleSheet(style);
}

void MainWindow::onStartClicked() {
    engine.startupStep();
    refreshUi();
}

void MainWindow::onTypeAClicked() {
    engine.processNormalItemStep(ItemType::TypeA);
    refreshUi();
}

void MainWindow::onTypeBClicked() {
    engine.processNormalItemStep(ItemType::TypeB);
    refreshUi();
}

void MainWindow::onJamClicked() {
    engine.processJamFaultStep();
    refreshUi();
}

void MainWindow::onResetClicked() {
    engine.resetStep();
    refreshUi();
}

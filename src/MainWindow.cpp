#include "MainWindow.h"

#include <QHeaderView>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), engine("config/routing.json") {
    setupUi();
    refreshUi();
}

void MainWindow::setupUi() {
    auto* central = new QWidget(this);
    auto* layout = new QVBoxLayout();

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

    machineStateLabel->setAlignment(Qt::AlignCenter);
    machineStateLabel->setMinimumHeight(40);

    historyTable = new QTableWidget(this);
    historyTable->setColumnCount(3);
    historyTable->setHorizontalHeaderLabels({"Timestamp", "Type", "Message"});
    historyTable->horizontalHeader()->setStretchLastSection(true);
    historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    historyTable->setSelectionMode(QAbstractItemView::SingleSelection);

    layout->addWidget(machineStateLabel);
    layout->addWidget(conveyorLabel);
    layout->addWidget(gateLabel);
    layout->addWidget(sensorLabel);
    layout->addWidget(blockedTicksLabel);
    layout->addWidget(alarmLabel);

    layout->addWidget(startButton);
    layout->addWidget(typeAButton);
    layout->addWidget(typeBButton);
    layout->addWidget(jamButton);
    layout->addWidget(resetButton);

    layout->addWidget(historyTable);

    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(typeAButton, &QPushButton::clicked, this, &MainWindow::onTypeAClicked);
    connect(typeBButton, &QPushButton::clicked, this, &MainWindow::onTypeBClicked);
    connect(jamButton, &QPushButton::clicked, this, &MainWindow::onJamClicked);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);

    central->setLayout(layout);
    setCentralWidget(central);
    setWindowTitle("Conveyor Sorting Cell HMI");
    resize(800, 500);
}

void MainWindow::refreshUi() {
    SimulationSnapshot snapshot = engine.getSnapshot();

    machineStateLabel->setText(QString("Machine State: %1").arg(QString::fromStdString(snapshot.machineState)));
    updateMachineStateStyle(snapshot.machineState);

    conveyorLabel->setText(QString("Conveyor Running: %1").arg(snapshot.conveyorRunning ? "YES" : "NO"));
    gateLabel->setText(QString("Gate Position: %1").arg(QString::fromStdString(snapshot.gatePosition)));
    sensorLabel->setText(QString("Sensor Blocked: %1").arg(snapshot.sensorBlocked ? "YES" : "NO"));
    blockedTicksLabel->setText(QString("Sensor Blocked Ticks: %1").arg(snapshot.sensorBlockedTicks));
    alarmLabel->setText(QString("Latest Alarm: %1").arg(QString::fromStdString(snapshot.latestAlarm)));

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

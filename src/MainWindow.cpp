#include "MainWindow.h"

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

    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(typeAButton, &QPushButton::clicked, this, &MainWindow::onTypeAClicked);
    connect(typeBButton, &QPushButton::clicked, this, &MainWindow::onTypeBClicked);
    connect(jamButton, &QPushButton::clicked, this, &MainWindow::onJamClicked);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);

    central->setLayout(layout);
    setCentralWidget(central);
    setWindowTitle("Conveyor Sorting Cell HMI");
    resize(420, 320);
}

void MainWindow::refreshUi() {
    SimulationSnapshot snapshot = engine.getSnapshot();

    machineStateLabel->setText(QString("Machine State: %1").arg(QString::fromStdString(snapshot.machineState)));
    conveyorLabel->setText(QString("Conveyor Running: %1").arg(snapshot.conveyorRunning ? "YES" : "NO"));
    gateLabel->setText(QString("Gate Position: %1").arg(QString::fromStdString(snapshot.gatePosition)));
    sensorLabel->setText(QString("Sensor Blocked: %1").arg(snapshot.sensorBlocked ? "YES" : "NO"));
    blockedTicksLabel->setText(QString("Sensor Blocked Ticks: %1").arg(snapshot.sensorBlockedTicks));
    alarmLabel->setText(QString("Latest Alarm: %1").arg(QString::fromStdString(snapshot.latestAlarm)));
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

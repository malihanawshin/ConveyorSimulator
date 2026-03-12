#pragma once

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>

#include "SimulationEngine.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onStartClicked();
    void onTypeAClicked();
    void onTypeBClicked();
    void onJamClicked();
    void onResetClicked();

private:
    SimulationEngine engine;

    QLabel* machineStateLabel;
    QLabel* conveyorLabel;
    QLabel* gateLabel;
    QLabel* sensorLabel;
    QLabel* blockedTicksLabel;
    QLabel* alarmLabel;

    QPushButton* startButton;
    QPushButton* typeAButton;
    QPushButton* typeBButton;
    QPushButton* jamButton;
    QPushButton* resetButton;

    void setupUi();
    void refreshUi();
};

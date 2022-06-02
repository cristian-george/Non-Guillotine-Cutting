#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CuttingProblem.h"
#include "GeneticAlgorithm.h"

class CuttingProblem : public QMainWindow
{
    Q_OBJECT

public:
    CuttingProblem(QWidget *parent = Q_NULLPTR);

    virtual void paintEvent(QPaintEvent* event);
    void drawPiece(const Piece& piece, const QPoint& origin,
        const QColor& color);

private slots:
    void on_btn_Start_clicked();

    void on_btn_Clear_clicked();

private:
    Ui::MainWindow ui;
    int scale = 50;
    bool startBtnPressed = false;
};

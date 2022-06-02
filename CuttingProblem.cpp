#include "CuttingProblem.h"
#include <QPainter>
#include <QMessageBox>

CuttingProblem::CuttingProblem(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void CuttingProblem::drawPiece(const Piece& piece, const QPoint& origin,
    const QColor& color)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(color);

    int length = scale * piece.Length;
    int width = scale * piece.Width;

    int X = origin.x() + scale * piece.X;
    int Y = origin.y() + scale * piece.Y;

    QPoint topLeft(X - length / 2,
        Y - width / 2);
    QPoint bottomRight(X + length / 2,
        Y + width / 2);

    painter.drawRect(QRect(topLeft, bottomRight));
}

void CuttingProblem::paintEvent(QPaintEvent* event)
{
    if (startBtnPressed == true)
    {
        QPainter painter(this);
        painter.setPen(QPen(Qt::black, 2));

        QPointF center(size().width() / 2, size().height() / 2);

        int length = scale * GeneticAlgorithm::BestIndividual.PaperLength;
        int width = scale * GeneticAlgorithm::BestIndividual.PaperWidth;

        QPoint topLeft(center.x() - length / 2,
            center.y() - width / 2);
        QPoint bottomRight(center.x() + length / 2,
            center.y() + width / 2);

        /* Draw rectangle */
        painter.setBrush(Qt::white);
        painter.drawRect(QRect(topLeft, bottomRight));

        std::vector<Piece> pieces =
            GeneticAlgorithm::BestIndividual.Pieces;

        /* Draw pieces */
        std::random_device rd;
        std::mt19937_64 generator(rd());
        std::uniform_int_distribution<int> distrib(0, 255);

        std::unordered_map<int, QColor> colors;
        for (const auto& piece : pieces)
        {
            if (piece.IsCut)
            {
                if (colors.find(piece.Type) == colors.end())
                {
                    int r = distrib(generator);
                    int g = distrib(generator);
                    int b = distrib(generator);

                    QColor color(r, g, b);
                    colors[piece.Type] = color;
                }

                drawPiece(piece, topLeft, colors[piece.Type]);
            }
        }
    }
}

void CuttingProblem::on_btn_Start_clicked()
{
    QString text;
    text = ui.txtEdit_PopSize->toPlainText();
    if (text.length() == 0)
    {
        QMessageBox::critical(this, "Error", "Population size field is empty!");
        return;
    }

    int populationSize = text.toInt();

    text = ui.txtEdit_NoGen->toPlainText();
    if (text.length() == 0)
    {
        QMessageBox::critical(this, "Error", "Number of generations field is empty!");
        return;
    }

    int numberOfGenerations = text.toInt();

    GeneticAlgorithm::Start(populationSize, numberOfGenerations);
    std::ofstream fout("solution.txt");
    fout << GeneticAlgorithm::BestIndividual << std::endl;

    int fitness = GeneticAlgorithm::BestIndividual.Fitness;
    ui.lbl_fitness->setText(QString(("Fitness: " + std::to_string(fitness)).c_str()));

    startBtnPressed = true;
    update();
}

void CuttingProblem::on_btn_Clear_clicked()
{
    startBtnPressed = false;
    ui.lbl_fitness->setText("");
    update();
}
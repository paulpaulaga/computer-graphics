#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    solution()
{
    ui->setupUi(this);
    ui->dotX->setPlaceholderText("X");
    ui->dotY->setPlaceholderText("Y");
    ui->dotNumber->setPlaceholderText("Номер");

    // set up table
    dotNumber = 0;
    QPen blackPen(Qt::black);
    QStringList dth;
    dth<<"X"<<"Y";
    ui->dotTable->setRowCount(1000);
    ui->dotTable->setColumnCount(2);
    ui->dotTable->setColumnWidth(0, (ui->dotTable->width() - ui->dotTable->verticalHeader()->width()) / 2 - 1);
    ui->dotTable->setColumnWidth(1, (ui->dotTable->width() - ui->dotTable->verticalHeader()->width()) / 2 - 1);
    ui->dotTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->dotTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->dotTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->dotTable->setFocusPolicy(Qt::NoFocus);
    ui->dotTable->setHorizontalHeaderLabels(dth);
    ui->dotTable->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Return)
    {
        if (ui->dotY->hasFocus() || ui->dotX->hasFocus())
            this->on_addButton_clicked();
        else if (ui->dotNumber->hasFocus())
            this->on_deleteButton_clicked();
    }
}

void MainWindow::on_solveButton_clicked()
{
    double tx1, tx2, tx3;
    double ty1, ty2, ty3;
    double x, y, r, tx, ty;
    bool tx1Ok, tx2Ok, tx3Ok;
    bool ty1Ok, ty2Ok, ty3Ok;

    tx1 = ui->fTriX->text().toDouble(&tx1Ok);
    tx2 = ui->sTriX->text().toDouble(&tx2Ok);
    tx3 = ui->tTriX->text().toDouble(&tx3Ok);

    ty1 = ui->fTriY->text().toDouble(&ty1Ok);
    ty2 = ui->sTriY->text().toDouble(&ty2Ok);
    ty3 = ui->tTriY->text().toDouble(&ty3Ok);

    if (tx1Ok && tx2Ok && tx3Ok && ty1Ok && ty2Ok && ty3Ok)
    {
        ui->drawWidget->isSolved(false);
        // add to Solution
        this->solution.setTriangle(tx1, ty1, tx2, ty2, tx3, ty3);
        // add to draw
        ui->drawWidget->updateTriangle(tx1, ty1, tx2, ty2, tx3, ty3);

        if (this->solution.solve(x, y, r, tx, ty) == 0)
        {
            cout << "YESSS" << endl;
            ui->drawWidget->isSolved(true);
            ui->drawWidget->isComplete(true);
            ui->drawWidget->addSolution(x, y, r, tx, ty);
            ui->drawWidget->update();
            QString ans = QString("ANSWER: X = %1; Y = %2; R = %3").arg(QString::number(x), QString::number(y), QString::number(r));
            ui->statusbar->showMessage(ans,40000);
        }
        else
        {
            ui->statusbar->showMessage("NO SOLUTION",500);
        }

    }
    else
        ui->statusbar->showMessage("Неверные координаты треугольника", 500);
}


void MainWindow::on_showButton_clicked()
{
    double tx1, tx2, tx3;
    double ty1, ty2, ty3;
    bool tx1Ok, tx2Ok, tx3Ok;
    bool ty1Ok, ty2Ok, ty3Ok;

    tx1 = ui->fTriX->text().toDouble(&tx1Ok);
    tx2 = ui->sTriX->text().toDouble(&tx2Ok);
    tx3 = ui->tTriX->text().toDouble(&tx3Ok);

    ty1 = ui->fTriY->text().toDouble(&ty1Ok);
    ty2 = ui->sTriY->text().toDouble(&ty2Ok);
    ty3 = ui->tTriY->text().toDouble(&ty3Ok);

    if (tx1Ok && tx2Ok && tx3Ok && ty1Ok && ty2Ok && ty3Ok && this->dotNumber > 0)
    {
        ui->drawWidget->updateTriangle(tx1, ty1, tx2, ty2, tx3, ty3);

        // pair<double, double> center = solution.triangleCenter({tx1, ty1}, {tx2, ty2}, {tx3, ty3});
        // ui->drawWidget->addPoint(center.first, center.second);
        ui->drawWidget->isComplete(true);
        ui->drawWidget->update();
    }
    else
        ui->statusbar->showMessage("Неверные координаты треугольника или нет точек", 500);
}

void MainWindow::on_addButton_clicked()
{
    QString xText = ui->dotX->text();
    QString yText = ui->dotY->text();
    bool xOk, yOk;
    double x, y;
    x = xText.toDouble(&xOk);
    y = yText.toDouble(&yOk);
    if (xOk && yOk)
    {
        // add to Solution
        this->solution.addDot(x, y);

        // add to table
        ui->dotTable->setItem(this->dotNumber, 0, new QTableWidgetItem(xText));
        ui->dotTable->setItem(this->dotNumber, 1, new QTableWidgetItem(yText));

        // empty lineedits
        ui->dotX->setText("");
        ui->dotY->setText("");

        // add to draw
        ui->drawWidget->addPoint(x, y);

        this->dotNumber++;
    }
    else
        ui->statusbar->showMessage("Неверные координаты", 500);

}

void MainWindow::on_deleteButton_clicked()
{
    QString nText = ui->dotNumber->text();
    bool nOk;
    int n;
    n = nText.toInt(&nOk);
    if (nOk && n > 0 && n <= this->dotNumber)
    {
        // delete from Solution
        this->solution.deleteDot(n - 1);

        // delete from table
        ui->dotTable->removeRow(n - 1);

        // delete from draw
        ui->drawWidget->deletePoint(n - 1);

        // empty lineedit
        ui->dotNumber->setText("");

        this->dotNumber--;
    }
    else
        ui->statusbar->showMessage("Неверный номер", 500);
}


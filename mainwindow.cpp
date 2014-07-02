#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMetaEnum>
#include <QKeyEvent>
#include <QMessageBox>
#include <QTooltip>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    editState(NEW_NUMBER),
    lastValue("")
{
    ui->setupUi(this);
    ui->label->clear();

    operatorMap.insert(ADD,'+');
    operatorMap.insert(MINUS,'-');
    operatorMap.insert(MULTIPLY,'*');
    operatorMap.insert(DIVIDE,'/');

    foreach (QPushButton* pushButton, this->findChildren<QPushButton*>()) {
        QString uiObjName = pushButton->objectName();
        if (uiObjName.at(uiObjName.size()-1).isDigit())
            //Digit Button
        {
            connect(pushButton,SIGNAL(clicked()),SLOT(onDigitNumberClicked()));
        }
    }

    connect(ui->pushButton_Sym_Dot,SIGNAL(clicked()),SLOT(onDotClicked()));
    connect(ui->pushButton_Sym_Opposite,SIGNAL(clicked()),SLOT(onOppositeClicked()));
    connect(ui->pushButton_Sym_Reciprocal,SIGNAL(clicked()),SLOT(onReciprocalClicked()));
    connect(ui->pushButton_Sym_Sqrt,SIGNAL(clicked()),SLOT(onSqrtClicked()));
    connect(ui->pushButton_Sym_Back,SIGNAL(clicked()),SLOT(onBackSpace()));

    connect(ui->pushButton_Sym_Add,SIGNAL(clicked()),SLOT(onAddClicked()));
    connect(ui->pushButton_Sym_Minus,SIGNAL(clicked()),SLOT(onMinusClicked()));
    connect(ui->pushButton_Sym_Multiply,SIGNAL(clicked()),SLOT(onMultiplyClicked()));
    connect(ui->pushButton_Sym_Divide,SIGNAL(clicked()),SLOT(onDivideClicked()));

    connect(ui->pushButton_Sym_Equal,SIGNAL(clicked()),SLOT(onEqualClicked()));

    connect(ui->pushButton_Sym_C,SIGNAL(clicked()),SLOT(onCClicked()));
    connect(ui->pushButton_Sym_CE,SIGNAL(clicked()),SLOT(onCEClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onDigitNumberClicked()
{
    QPushButton* pushButton = (QPushButton*)sender();
    QChar digit = pushButton->objectName().at(pushButton->objectName().size()-1);

    if (editState == NEW_NUMBER)
    {
        currentValue = digit;
        if (digit != '0')
            editState = CONTINUE;
    }
    else if(editState == CONTINUE)
    {
        currentValue.append(digit);
    }

    updateShowingValue();
}

void MainWindow::onDotClicked()
{
    if (currentValue.contains('.'))
        return;

    editState = CONTINUE;
    currentValue.append('.');
    updateShowingValue();
}

void MainWindow::onOppositeClicked()
{
    if (currentValue.at(0) == '-')
    {
        currentValue.remove(0,1);
    }
    else if (currentValue.at(0) != '0')
    {
        currentValue.push_front('-');
    }
    updateShowingValue();
}

void MainWindow::onReciprocalClicked()
{
    double res = 0;
    arithmeticCompute(DIVIDE, 1, currentValue.toDouble(), res);
    currentValue = QString::number(res,'g',ui->lcdNumber->digitCount()-2);
    updateShowingValue();
}

void MainWindow::onSqrtClicked()
{
    double src = currentValue.toDouble();
    if (src<0)
    {
        QToolTip::showText(this->mapToGlobal(ui->pushButton_Sym_Sqrt->rect().bottomLeft()),
                           tr("<font color=\"#FF0000\">Current number is less than 0, couldn't compute the square root!</font>"),this);
        //        QMessageBox::warning(this,tr("Warning"),tr("Current number is less than 0, couldn't compute the square root!"));

        return;
    }
    double res = sqrt(src);
    currentValue = QString::number(res,'g',ui->lcdNumber->digitCount()-2);
    updateShowingValue();
}

void MainWindow::onBackSpace()
{
    if (currentValue == "0")
        return;

    if (currentValue.size()==1)
    {
        currentValue = "0";
        editState = NEW_NUMBER;
    }
    else
        currentValue.remove(currentValue.size()-1,1);

    updateShowingValue();
}

void MainWindow::onAddClicked()
{
    executeLastArithmeticCompute();
    currentOperator = ADD;
    updateShowingValue();
}

void MainWindow::onMinusClicked()
{
    executeLastArithmeticCompute();
    currentOperator = MINUS;
    updateShowingValue();
}

void MainWindow::onMultiplyClicked()
{
    executeLastArithmeticCompute();
    currentOperator = MULTIPLY;
    updateShowingValue();
}

void MainWindow::onDivideClicked()
{
    executeLastArithmeticCompute();
    currentOperator = DIVIDE;
    updateShowingValue();
}

void MainWindow::onEqualClicked()
{
    executeLastArithmeticCompute();
    currentValue = lastValue;
    lastValue.clear();
    updateShowingValue();
}

void MainWindow::onCClicked()
{
    currentValue = "0";
    editState = NEW_NUMBER;
    updateShowingValue();
}

void MainWindow::onCEClicked()
{
    currentValue = "0";
    lastValue = "";
    editState = NEW_NUMBER;
    updateShowingValue();
}

void MainWindow::updateShowingValue()
{
    int indexOfEnum = this->metaObject()->indexOfEnumerator("State");
    QMetaEnum metaEnum = this->metaObject()->enumerator(indexOfEnum);

    qDebug()<<"State:"<<metaEnum.valueToKey(editState)<<"\tCurrent Value:"<<currentValue;
    ui->lcdNumber->display(currentValue);
    if (!lastValue.isEmpty())
        ui->label->setText(lastValue + " " + operatorMap.value(currentOperator));
    else
        ui->label->clear();
}

void MainWindow::ignoreZeros()
{
    while (currentValue.contains('.') && currentValue.endsWith('0'))
    {
        currentValue.remove(currentValue.size()-1,1);
    }
}

bool MainWindow::arithmeticCompute(MainWindow::ArithmeticOperators oper, double val1, double val2, double &res)
{
    switch (oper) {
    case ADD:
        res = val1 + val2;
        break;
    case MINUS:
        res = val1 - val2;
        break;
    case MULTIPLY:
        res = val1 * val2;
        break;
    case DIVIDE:
        res = val1 / val2;
        break;
    }
    return qQNaN() != res;
}

bool MainWindow::executeLastArithmeticCompute()
{
    ignoreZeros();
    if (!lastValue.isEmpty())
    {
        double res = 0;
        bool isValid = arithmeticCompute(currentOperator,lastValue.toDouble(),currentValue.toDouble(),res);
        lastValue = QString::number(res,'g',ui->lcdNumber->digitCount()-2);
    }
    else
        lastValue = currentValue;

    editState = NEW_NUMBER;

    return true;
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    QFont font;

//    int widthScale  = static_cast<int>(this->width() * 100/210.);
//    int heightScale = static_cast<int>(this->height()* 100/270.);

//    foreach (QPushButton* pushButton, this->findChildren<QPushButton*>()) {
//        font = pushButton->font();
//        font.setStretch(qMin(widthScale,heightScale));
//        pushButton->setFont(font);
//    }


//    font = ui->label->font();
//    font.setStretch(ui->label->height()*100/15.);
//    ui->label->setFont(font);
}


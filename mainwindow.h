#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_ENUMS(State)
public:
    enum State{
        NEW_NUMBER,
        CONTINUE
    };

    enum ArithmeticOperators{
        ADD,
        MINUS,
        MULTIPLY,
        DIVIDE
    };

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onDigitNumberClicked();

    void onDotClicked();
    void onOppositeClicked();
    void onReciprocalClicked();
    void onSqrtClicked();

    void onAddClicked();
    void onMinusClicked();
    void onMultiplyClicked();
    void onDivideClicked();

    void onEqualClicked();

private:
    void updateShowingValue();
    void ignoreZeros();
    bool arithmeticCompute(ArithmeticOperators oper, double val1, double val2, double &res);
    bool executeLastArithmeticCompute();

private:
    Ui::MainWindow *ui;
    QString lastValue;
    QString currentValue;

    State editState;
    ArithmeticOperators currentOperator;

    QMap<ArithmeticOperators,QChar> operatorMap;
};

#endif // MAINWINDOW_H

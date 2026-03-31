#include "ui/calculator_ui.hpp"
#include <QApplication>

int main(int argc, char **argv) {

    QApplication app(argc, argv);
    app.setApplicationName("QtCalculator");

    CalculatorUI window;
    window.show();

    return app.exec();
}

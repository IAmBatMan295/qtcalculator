#include "calculator_ui.hpp"

QString Build_Calculator_Style_Sheet() {
    return R"(
        QWidget#RootWidget {
            background-color: #000000;
        }

        QFrame#HistorySection,
        QFrame#ExpressionSection,
        QFrame#KeypadSection {
            background-color: #000000;
            border: 1px solid #ffffff;
            border-radius: 0px;
        }

        QTextEdit#HistoryList {
            background-color: #000000;
            color: #8a8a8a;
            border: none;
            outline: none;
            font-size: 20px;
            padding: 4px;
        }

        QTextEdit#HistoryList QScrollBar:vertical {
            background: #000000;
            width: 14px;
            margin: 0px 0px 0px 8px;
            border: none;
        }

        QTextEdit#HistoryList QScrollBar::handle:vertical {
            background: #ffffff;
            min-height: 28px;
            border: none;
        }

        QTextEdit#HistoryList QScrollBar::add-line:vertical,
        QTextEdit#HistoryList QScrollBar::sub-line:vertical,
        QTextEdit#HistoryList QScrollBar::add-page:vertical,
        QTextEdit#HistoryList QScrollBar::sub-page:vertical {
            background: #000000;
            border: none;
            height: 0px;
        }

        QLineEdit#ExpressionDisplay {
            background-color: #000000;
            color: #ffffff;
            border: none;
            border-radius: 0px;
            padding: 0px 8px 0px 8px;
            font-size: 30px;
            font-weight: 600;
        }

        QLabel#StatusLabel {
            color: #8a8a8a;
            min-height: 18px;
            border: none;
        }

        QPushButton {
            border: 1px solid #ffffff;
            border-radius: 0px;
            background-color: #000000;
            color: #ffffff;
            font-size: 24px;
            font-weight: 600;
            padding: 12px;
        }

        QPushButton[button_role="number"] {
            background-color: #000000;
            color: #ffffff;
        }

        QPushButton[button_role="operator"] {
            background-color: #1f1f1f;
            color: #ffffff;
            font-size: 30px;
        }

        QPushButton#ModButton {
            font-size: 24px;
        }

        QPushButton[button_role="function"] {
            background-color: #2f2f2f;
            color: #ffffff;
        }

        QPushButton[button_role="equal"] {
            background-color: #ffffff;
            color: #000000;
        }

        QPushButton[button_role="ac"] {
            background-color: #5a0000;
            color: #ffffff;
        }

        QPushButton[button_role="clear"] {
            background-color: #8a4b00;
            color: #ffffff;
        }

        QPushButton:pressed {
            border: 1px solid #ffffff;
            background-color: #3f3f3f;
        }

        QPushButton#CornerIconButton {
            padding: 1px;
            font-size: 22px;
            font-family: "Symbols Nerd Font Mono", "Noto Sans Mono", monospace;
            background-color: #1f1f1f;
            color: #ffffff;
        }
    )";
}

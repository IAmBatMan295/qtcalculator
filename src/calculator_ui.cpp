#include "calculator_ui.hpp"

#include "math_expression.hpp"

#include <QApplication>
#include <QEvent>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>

#include <exception>
#include <string>
#include <vector>

CalculatorUI::CalculatorUI(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("QtCalculator");
    setMinimumSize(440, 760);

    qApp->installEventFilter(this);

    Build_User_Interface();
    Connect_User_Interface_Signals();

    Refresh_Mode_Dependent_User_Interface();
    Refresh_Expression_Display();
    Show_Status_Message("", false);
}

bool CalculatorUI::eventFilter(QObject *watched_object, QEvent *event) {
    Q_UNUSED(watched_object);

    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        return true;
    }

    return QMainWindow::eventFilter(watched_object, event);
}

void CalculatorUI::Build_User_Interface() {
    root_widget_ = new QWidget(this);
    root_widget_->setObjectName("RootWidget");
    setCentralWidget(root_widget_);

    QVBoxLayout *root_layout = new QVBoxLayout(root_widget_);
    root_layout->setContentsMargins(10, 10, 10, 10);
    root_layout->setSpacing(10);

    history_panel_ = new HistoryPanel(root_widget_);
    display_panel_ = new DisplayPanel(root_widget_);
    keypad_panel_ = new KeypadPanel(root_widget_);

    root_layout->addWidget(history_panel_, 2);
    root_layout->addWidget(display_panel_, 0);
    root_layout->addWidget(keypad_panel_, 4);

    root_widget_->setStyleSheet(Build_Calculator_Style_Sheet());
}

void CalculatorUI::Connect_User_Interface_Signals() {
    connect(display_panel_, &DisplayPanel::Expansion_Toggle_Requested, this, &CalculatorUI::Handle_Expansion_Toggle_Request);
    connect(display_panel_, &DisplayPanel::Expansion_Button_Size_Changed, history_panel_, &HistoryPanel::Set_Clear_Button_Size);

    connect(history_panel_, &HistoryPanel::Clear_History_Requested, this, [this]() {
        const QMessageBox::StandardButton user_choice =
            QMessageBox::question(this, "Clear History", "Clear all history entries?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (user_choice == QMessageBox::Yes) {
            history_panel_->Clear_History();
        }
    });

    connect(keypad_panel_, &KeypadPanel::Insert_Chunk_Requested, this, &CalculatorUI::Handle_Insert_Chunk_Request);
    connect(keypad_panel_, &KeypadPanel::Function_Group_Requested, this, &CalculatorUI::Handle_Function_Request);

    connect(keypad_panel_, &KeypadPanel::Clear_All_Requested, this, &CalculatorUI::Handle_Clear_All_Request);
    connect(keypad_panel_, &KeypadPanel::Backspace_Requested, this, &CalculatorUI::Handle_Backspace_Request);
    connect(keypad_panel_, &KeypadPanel::Evaluate_Requested, this, &CalculatorUI::Evaluate_Current_Expression);

    connect(keypad_panel_, &KeypadPanel::Inverse_Mode_Toggle_Requested, this, &CalculatorUI::Handle_Inverse_Mode_Toggle_Request);
    connect(keypad_panel_, &KeypadPanel::Rad_Deg_Mode_Toggle_Requested, this, &CalculatorUI::Handle_Rad_Deg_Mode_Toggle_Request);
}

void CalculatorUI::Handle_Insert_Chunk_Request(const QString &visible_chunk, const QString &backend_chunk) {
    expression_state_.Append_Chunk(visible_chunk, backend_chunk);
    Refresh_Expression_Display();
    Show_Status_Message("", false);
}

void CalculatorUI::Handle_Function_Request(const QString &function_group_name) {
    const FunctionTokenMapper::FunctionNamePair function_name_pair = FunctionTokenMapper::Resolve_Function_Names(function_group_name, is_inverse_mode_enabled_);

    const QString visible_chunk = function_name_pair.display_function_name + "(";
    const QString backend_chunk = FunctionTokenMapper::Build_Backend_Function_Token(function_name_pair.backend_function_name, is_radian_mode_enabled_);

    expression_state_.Append_Chunk(visible_chunk, backend_chunk);
    Refresh_Expression_Display();
    Show_Status_Message("", false);
}

void CalculatorUI::Handle_Clear_All_Request() {
    expression_state_.Clear();
    Refresh_Expression_Display();
    Show_Status_Message("", false);
}

void CalculatorUI::Handle_Backspace_Request() {
    expression_state_.Remove_Last_Chunk();
    Refresh_Expression_Display();
    Show_Status_Message("", false);
}

void CalculatorUI::Evaluate_Current_Expression() {
    const QString backend_expression = expression_state_.Get_Backend_Expression();
    if (backend_expression.isEmpty()) {
        Show_Status_Message("Nothing to evaluate.", true);
        return;
    }

    try {
        std::string expression_string = backend_expression.toStdString();
        std::vector<Token> tokenized_expression = Tokenize_Expression(expression_string);
        std::vector<Token> postfix_expression = To_Postfix(tokenized_expression);
        const double evaluation_result = Eval_Expression(postfix_expression);

        const QString visible_expression = expression_state_.Get_Visible_Expression();
        const QString result_text = Format_Result_Text(evaluation_result);

        history_panel_->Add_History_Entry(visible_expression + " = " + result_text);
        history_panel_->Scroll_To_Latest();

        expression_state_.Set_Result_Text(result_text);
        Refresh_Expression_Display();
        Show_Status_Message("", false);
    } catch (const std::exception &runtime_error) {
        Show_Status_Message(QString::fromStdString(runtime_error.what()), true);
    }
}

void CalculatorUI::Handle_Expansion_Toggle_Request() {
    is_scientific_panel_visible_ = !is_scientific_panel_visible_;
    keypad_panel_->Set_Scientific_Visible(is_scientific_panel_visible_);
    display_panel_->Set_Expansion_Checked(is_scientific_panel_visible_);
}

void CalculatorUI::Handle_Inverse_Mode_Toggle_Request() {
    is_inverse_mode_enabled_ = !is_inverse_mode_enabled_;
    Refresh_Mode_Dependent_User_Interface();
}

void CalculatorUI::Handle_Rad_Deg_Mode_Toggle_Request() {
    is_radian_mode_enabled_ = !is_radian_mode_enabled_;
    Refresh_Mode_Dependent_User_Interface();
}

void CalculatorUI::Refresh_Mode_Dependent_User_Interface() {
    keypad_panel_->Set_Inverse_Mode_Enabled(is_inverse_mode_enabled_);
    keypad_panel_->Set_Rad_Mode_Enabled(is_radian_mode_enabled_);
    history_panel_->Set_Clear_Button_Size(display_panel_->Get_Expansion_Button_Size());
}

void CalculatorUI::Refresh_Expression_Display() { display_panel_->Set_Expression_Text(expression_state_.Get_Visible_Expression()); }

QString CalculatorUI::Format_Result_Text(double result_value) const { return QString::number(result_value, 'g', 15); }

void CalculatorUI::Show_Status_Message(const QString &status_text, bool is_error) const { display_panel_->Set_Status_Message(status_text, is_error); }

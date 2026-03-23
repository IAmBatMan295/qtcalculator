#include "calculator_ui.hpp"

#include <QGridLayout>
#include <QPushButton>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QWidget>

KeypadPanel::KeypadPanel(QWidget *parent) : QFrame(parent) {
    setObjectName("KeypadSection");
    setFrameShape(QFrame::NoFrame);

    QVBoxLayout *keypad_section_layout = new QVBoxLayout(this);
    keypad_section_layout->setContentsMargins(8, 8, 8, 8);
    keypad_section_layout->setSpacing(8);

    scientific_panel_widget_ = new QWidget(this);
    QGridLayout *scientific_grid_layout = new QGridLayout(scientific_panel_widget_);
    scientific_grid_layout->setContentsMargins(0, 0, 0, 0);
    scientific_grid_layout->setSpacing(6);

    QPushButton *sqrt_button = Create_Key_Button("√", "function");
    QPushButton *pi_button = Create_Key_Button("π", "function");
    QPushButton *power_button = Create_Key_Button("^", "operator");
    QPushButton *factorial_button = Create_Key_Button("!", "operator");

    rad_mode_button_ = Create_Key_Button("Mode : RAD", "function");
    rad_mode_button_->setCheckable(true);

    sin_button_ = Create_Key_Button("sin", "function");
    cos_button_ = Create_Key_Button("cos", "function");
    tan_button_ = Create_Key_Button("tan", "function");

    inverse_mode_button_ = Create_Key_Button("INV", "function");
    inverse_mode_button_->setCheckable(true);

    QPushButton *constant_e_button = Create_Key_Button("E", "function");
    ln_button_ = Create_Key_Button("ln", "function");
    log_button_ = Create_Key_Button("log", "function");

    scientific_grid_layout->addWidget(sqrt_button, 0, 0, 1, 2);
    scientific_grid_layout->addWidget(pi_button, 0, 2, 1, 2);
    scientific_grid_layout->addWidget(power_button, 0, 4, 1, 2);
    scientific_grid_layout->addWidget(factorial_button, 0, 6, 1, 2);

    scientific_grid_layout->addWidget(rad_mode_button_, 1, 0, 1, 2);
    scientific_grid_layout->addWidget(sin_button_, 1, 2, 1, 2);
    scientific_grid_layout->addWidget(cos_button_, 1, 4, 1, 2);
    scientific_grid_layout->addWidget(tan_button_, 1, 6, 1, 2);

    scientific_grid_layout->addWidget(inverse_mode_button_, 2, 0, 1, 2);
    scientific_grid_layout->addWidget(constant_e_button, 2, 2, 1, 2);
    scientific_grid_layout->addWidget(ln_button_, 2, 4, 1, 2);
    scientific_grid_layout->addWidget(log_button_, 2, 6, 1, 2);

    QWidget *base_panel_widget = new QWidget(this);
    QGridLayout *base_grid_layout = new QGridLayout(base_panel_widget);
    base_grid_layout->setContentsMargins(0, 0, 0, 0);
    base_grid_layout->setSpacing(6);

    QPushButton *all_clear_button = Create_Key_Button("AC", "ac");
    QPushButton *left_parenthesis_button = Create_Key_Button("(", "number");
    QPushButton *right_parenthesis_button = Create_Key_Button(")", "number");
    QPushButton *mod_button = Create_Key_Button("MOD", "operator");
    mod_button->setObjectName("ModButton");
    QPushButton *divide_button = Create_Key_Button("÷", "operator");

    QPushButton *seven_button = Create_Key_Button("7", "number");
    QPushButton *eight_button = Create_Key_Button("8", "number");
    QPushButton *nine_button = Create_Key_Button("9", "number");
    QPushButton *multiply_button = Create_Key_Button("×", "operator");

    QPushButton *four_button = Create_Key_Button("4", "number");
    QPushButton *five_button = Create_Key_Button("5", "number");
    QPushButton *six_button = Create_Key_Button("6", "number");
    QPushButton *subtract_button = Create_Key_Button("-", "operator");

    QPushButton *one_button = Create_Key_Button("1", "number");
    QPushButton *two_button = Create_Key_Button("2", "number");
    QPushButton *three_button = Create_Key_Button("3", "number");
    QPushButton *add_button = Create_Key_Button("+", "operator");

    QPushButton *zero_button = Create_Key_Button("0", "number");
    QPushButton *decimal_button = Create_Key_Button(".", "number");
    QPushButton *clear_character_button = Create_Key_Button("C", "clear");
    QPushButton *equal_button = Create_Key_Button("=", "equal");

    base_grid_layout->addWidget(all_clear_button, 0, 0, 1, 2);
    base_grid_layout->addWidget(left_parenthesis_button, 0, 2, 1, 1);
    base_grid_layout->addWidget(right_parenthesis_button, 0, 3, 1, 1);
    base_grid_layout->addWidget(mod_button, 0, 4, 1, 2);
    base_grid_layout->addWidget(divide_button, 0, 6, 1, 2);

    base_grid_layout->addWidget(seven_button, 1, 0, 1, 2);
    base_grid_layout->addWidget(eight_button, 1, 2, 1, 2);
    base_grid_layout->addWidget(nine_button, 1, 4, 1, 2);
    base_grid_layout->addWidget(multiply_button, 1, 6, 1, 2);

    base_grid_layout->addWidget(four_button, 2, 0, 1, 2);
    base_grid_layout->addWidget(five_button, 2, 2, 1, 2);
    base_grid_layout->addWidget(six_button, 2, 4, 1, 2);
    base_grid_layout->addWidget(subtract_button, 2, 6, 1, 2);

    base_grid_layout->addWidget(one_button, 3, 0, 1, 2);
    base_grid_layout->addWidget(two_button, 3, 2, 1, 2);
    base_grid_layout->addWidget(three_button, 3, 4, 1, 2);
    base_grid_layout->addWidget(add_button, 3, 6, 1, 2);

    base_grid_layout->addWidget(zero_button, 4, 0, 1, 2);
    base_grid_layout->addWidget(decimal_button, 4, 2, 1, 2);
    base_grid_layout->addWidget(clear_character_button, 4, 4, 1, 2);
    base_grid_layout->addWidget(equal_button, 4, 6, 1, 2);

    for (int column_index = 0; column_index < 8; ++column_index) {
        scientific_grid_layout->setColumnStretch(column_index, 1);
        base_grid_layout->setColumnStretch(column_index, 1);
    }
    for (int scientific_row_index = 0; scientific_row_index < 3; ++scientific_row_index) {
        scientific_grid_layout->setRowStretch(scientific_row_index, 1);
    }
    for (int base_row_index = 0; base_row_index < 5; ++base_row_index) {
        base_grid_layout->setRowStretch(base_row_index, 1);
    }

    scientific_panel_widget_->setVisible(false);
    keypad_section_layout->addWidget(scientific_panel_widget_);
    keypad_section_layout->addWidget(base_panel_widget);

    Connect_Insert_Button(sqrt_button, "√(", "sqrt(");
    Connect_Insert_Button(pi_button, "π", "PI");
    Connect_Insert_Button(power_button, "^", "^");
    Connect_Insert_Button(factorial_button, "!", "!");
    Connect_Insert_Button(constant_e_button, "E", "E");

    Connect_Insert_Button(left_parenthesis_button, "(", "(");
    Connect_Insert_Button(right_parenthesis_button, ")", ")");
    Connect_Insert_Button(mod_button, "%", "%");
    Connect_Insert_Button(divide_button, "÷", "/");
    Connect_Insert_Button(seven_button, "7", "7");
    Connect_Insert_Button(eight_button, "8", "8");
    Connect_Insert_Button(nine_button, "9", "9");
    Connect_Insert_Button(multiply_button, "×", "*");
    Connect_Insert_Button(four_button, "4", "4");
    Connect_Insert_Button(five_button, "5", "5");
    Connect_Insert_Button(six_button, "6", "6");
    Connect_Insert_Button(subtract_button, "-", "-");
    Connect_Insert_Button(one_button, "1", "1");
    Connect_Insert_Button(two_button, "2", "2");
    Connect_Insert_Button(three_button, "3", "3");
    Connect_Insert_Button(add_button, "+", "+");
    Connect_Insert_Button(zero_button, "0", "0");
    Connect_Insert_Button(decimal_button, ".", ".");

    connect(sin_button_, &QPushButton::clicked, this, [this]() { emit Function_Group_Requested("sin"); });
    connect(cos_button_, &QPushButton::clicked, this, [this]() { emit Function_Group_Requested("cos"); });
    connect(tan_button_, &QPushButton::clicked, this, [this]() { emit Function_Group_Requested("tan"); });
    connect(ln_button_, &QPushButton::clicked, this, [this]() { emit Function_Group_Requested("ln"); });
    connect(log_button_, &QPushButton::clicked, this, [this]() { emit Function_Group_Requested("log"); });

    connect(rad_mode_button_, &QPushButton::clicked, this, &KeypadPanel::Rad_Deg_Mode_Toggle_Requested);
    connect(inverse_mode_button_, &QPushButton::clicked, this, &KeypadPanel::Inverse_Mode_Toggle_Requested);

    connect(all_clear_button, &QPushButton::clicked, this, &KeypadPanel::Clear_All_Requested);
    connect(clear_character_button, &QPushButton::clicked, this, &KeypadPanel::Backspace_Requested);
    connect(equal_button, &QPushButton::clicked, this, &KeypadPanel::Evaluate_Requested);
}

void KeypadPanel::Set_Scientific_Visible(bool is_visible) { scientific_panel_widget_->setVisible(is_visible); }

void KeypadPanel::Set_Rad_Mode_Enabled(bool is_radian_mode_enabled) {
    is_radian_mode_enabled_ = is_radian_mode_enabled;
    Refresh_Mode_Dependent_Button_Text();
}

void KeypadPanel::Set_Inverse_Mode_Enabled(bool is_inverse_mode_enabled) {
    is_inverse_mode_enabled_ = is_inverse_mode_enabled;
    Refresh_Mode_Dependent_Button_Text();
}

QPushButton *KeypadPanel::Create_Key_Button(const QString &button_text, const QString &button_role) const {
    QPushButton *key_button = new QPushButton(button_text);
    key_button->setProperty("button_role", button_role);
    key_button->setFocusPolicy(Qt::NoFocus);
    key_button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    return key_button;
}

void KeypadPanel::Connect_Insert_Button(QPushButton *button, const QString &visible_chunk, const QString &backend_chunk) {
    connect(button, &QPushButton::clicked, this, [this, visible_chunk, backend_chunk]() { emit Insert_Chunk_Requested(visible_chunk, backend_chunk); });
}

void KeypadPanel::Refresh_Mode_Dependent_Button_Text() {
    rad_mode_button_->setText(is_radian_mode_enabled_ ? "Mode : RAD" : "Mode : DEG");
    rad_mode_button_->setChecked(!is_radian_mode_enabled_);

    inverse_mode_button_->setChecked(is_inverse_mode_enabled_);

    sin_button_->setText(is_inverse_mode_enabled_ ? "asin" : "sin");
    cos_button_->setText(is_inverse_mode_enabled_ ? "acos" : "cos");
    tan_button_->setText(is_inverse_mode_enabled_ ? "atan" : "tan");

    ln_button_->setText(is_inverse_mode_enabled_ ? "exp" : "ln");
    log_button_->setText(is_inverse_mode_enabled_ ? "pow10" : "log");
}

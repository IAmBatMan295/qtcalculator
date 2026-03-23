#include "calculator_ui.hpp"

#include <QFont>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QResizeEvent>
#include <QVBoxLayout>

DisplayPanel::DisplayPanel(QWidget *parent) : QFrame(parent) {
    setObjectName("ExpressionSection");
    setFrameShape(QFrame::NoFrame);

    QVBoxLayout *display_layout = new QVBoxLayout(this);
    display_layout->setContentsMargins(8, 8, 8, 8);
    display_layout->setSpacing(6);

    QWidget *expression_container_widget = new QWidget(this);
    QGridLayout *expression_overlay_layout = new QGridLayout(expression_container_widget);
    expression_overlay_layout->setContentsMargins(0, 0, 0, 0);
    expression_overlay_layout->setSpacing(0);

    expansion_toggle_button_ = new QPushButton("󰞙", expression_container_widget);
    expansion_toggle_button_->setObjectName("CornerIconButton");
    expansion_toggle_button_->setProperty("button_role", "function");
    expansion_toggle_button_->setCheckable(true);
    expansion_toggle_button_->setFocusPolicy(Qt::NoFocus);
    expansion_toggle_button_->setToolTip("Expand or collapse scientific keys");

    expression_line_edit_ = new QLineEdit(expression_container_widget);
    expression_line_edit_->setObjectName("ExpressionDisplay");
    expression_line_edit_->setReadOnly(true);
    expression_line_edit_->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    expression_line_edit_->setPlaceholderText("0");

    expression_overlay_layout->addWidget(expression_line_edit_, 0, 0);
    expression_overlay_layout->addWidget(expansion_toggle_button_, 0, 0, Qt::AlignLeft | Qt::AlignBottom);
    expansion_toggle_button_->raise();

    status_message_label_ = new QLabel(this);
    status_message_label_->setObjectName("StatusLabel");
    status_message_label_->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    display_layout->addWidget(expression_container_widget);
    display_layout->addWidget(status_message_label_);

    connect(expansion_toggle_button_, &QPushButton::clicked, this, &DisplayPanel::Expansion_Toggle_Requested);

    Update_Expansion_Button_Size();
}

void DisplayPanel::Set_Expression_Text(const QString &expression_text) { expression_line_edit_->setText(expression_text); }

void DisplayPanel::Set_Status_Message(const QString &status_text, bool is_error) {
    status_message_label_->setText(status_text);
    if (is_error) {
        status_message_label_->setStyleSheet("color: #ff6b6b; border: none;");
    } else {
        status_message_label_->setStyleSheet("color: #8a8a8a; border: none;");
    }
}

void DisplayPanel::Set_Expansion_Checked(bool is_checked) { expansion_toggle_button_->setChecked(is_checked); }

int DisplayPanel::Get_Expansion_Button_Size() const { return expansion_button_size_; }

void DisplayPanel::resizeEvent(QResizeEvent *resize_event) {
    QFrame::resizeEvent(resize_event);
    Update_Expansion_Button_Size();
}

void DisplayPanel::Update_Expansion_Button_Size() {
    const int calculated_button_size = qMax(expression_line_edit_->fontMetrics().height(), (expression_line_edit_->height() * 3) / 5);
    if (calculated_button_size != expansion_button_size_) {
        expansion_button_size_ = calculated_button_size;
        emit Expansion_Button_Size_Changed(expansion_button_size_);
    }

    expansion_toggle_button_->setFixedSize(expansion_button_size_, expansion_button_size_);

    QFont corner_button_font = expansion_toggle_button_->font();
    corner_button_font.setPixelSize((expansion_button_size_ * 3) / 5);
    expansion_toggle_button_->setFont(corner_button_font);

    const int text_vertical_margin = qMax(0, ((expression_line_edit_->height() - expression_line_edit_->fontMetrics().height()) / 2) - 1);
    expression_line_edit_->setTextMargins(expansion_button_size_ + 10, text_vertical_margin, 0, text_vertical_margin);
}

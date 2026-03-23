#include "calculator_ui.hpp"

#include <QFont>
#include <QGridLayout>
#include <QPushButton>
#include <QTextCursor>
#include <QTextEdit>
#include <QVBoxLayout>

HistoryPanel::HistoryPanel(QWidget *parent) : QFrame(parent) {
    setObjectName("HistorySection");
    setFrameShape(QFrame::NoFrame);

    QVBoxLayout *history_layout = new QVBoxLayout(this);
    history_layout->setContentsMargins(8, 8, 8, 8);
    history_layout->setSpacing(6);

    QWidget *history_container_widget = new QWidget(this);
    QGridLayout *history_overlay_layout = new QGridLayout(history_container_widget);
    history_overlay_layout->setContentsMargins(0, 0, 0, 0);
    history_overlay_layout->setSpacing(0);

    history_text_edit_ = new QTextEdit(history_container_widget);
    history_text_edit_->setObjectName("HistoryList");
    history_text_edit_->setReadOnly(true);
    history_text_edit_->setAcceptRichText(false);
    history_text_edit_->setUndoRedoEnabled(false);
    history_text_edit_->setTextInteractionFlags(Qt::TextSelectableByMouse);
    history_text_edit_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    history_text_edit_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    clear_history_button_ = new QPushButton("", history_container_widget);
    clear_history_button_->setObjectName("CornerIconButton");
    clear_history_button_->setProperty("button_role", "function");
    clear_history_button_->setFocusPolicy(Qt::NoFocus);
    clear_history_button_->setToolTip("Clear history");

    history_overlay_layout->addWidget(history_text_edit_, 0, 0);
    history_overlay_layout->addWidget(clear_history_button_, 0, 0, Qt::AlignLeft | Qt::AlignTop);
    history_layout->addWidget(history_container_widget);

    connect(clear_history_button_, &QPushButton::clicked, this, &HistoryPanel::Clear_History_Requested);
}

void HistoryPanel::Add_History_Entry(const QString &history_text) {
    QTextCursor history_cursor = history_text_edit_->textCursor();
    history_cursor.movePosition(QTextCursor::End);
    history_text_edit_->setTextCursor(history_cursor);
    history_text_edit_->setAlignment(Qt::AlignRight);
    history_text_edit_->insertPlainText(history_text + "\n");
}

void HistoryPanel::Scroll_To_Latest() {
    QTextCursor history_cursor = history_text_edit_->textCursor();
    history_cursor.movePosition(QTextCursor::End);
    history_text_edit_->setTextCursor(history_cursor);
}

void HistoryPanel::Clear_History() { history_text_edit_->clear(); }

void HistoryPanel::Set_Clear_Button_Size(int button_size) {
    const int clear_button_size = (button_size * 3) / 2;
    clear_history_button_->setFixedSize(clear_button_size, clear_button_size);

    QFont clear_button_font = clear_history_button_->font();
    clear_button_font.setPixelSize((clear_button_size * 3) / 5);
    clear_history_button_->setFont(clear_button_font);
}

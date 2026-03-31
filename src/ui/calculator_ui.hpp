#pragma once

#include <QFrame>
#include <QMainWindow>
#include <QString>
#include <QVector>

class QLabel;
class QLineEdit;
class QPushButton;
class QResizeEvent;
class QTextEdit;
class QWidget;

class ExpressionState {
  public:
    struct ExpressionChunk {
        QString visible_chunk;
        QString backend_chunk;
    };

    void Append_Chunk(const QString &visible_chunk, const QString &backend_chunk);
    void Remove_Last_Chunk();
    void Clear();
    void Set_Result_Text(const QString &result_text);

    QString Get_Visible_Expression() const;
    QString Get_Backend_Expression() const;

  private:
    QString visible_expression_;
    QString backend_expression_;
    QVector<ExpressionChunk> chunk_history_;
};

class FunctionTokenMapper {
  public:
    struct FunctionNamePair {
        QString display_function_name;
        QString backend_function_name;
    };

    static FunctionNamePair Resolve_Function_Names(const QString &function_group_name, bool is_inverse_mode_enabled);
    static QString Build_Backend_Function_Token(const QString &backend_function_name, bool is_radian_mode_enabled);

  private:
    static bool Is_Trig_Function(const QString &backend_function_name);
    static bool Is_Inverse_Trig_Function(const QString &backend_function_name);
};

class HistoryPanel : public QFrame {
    Q_OBJECT

  public:
    explicit HistoryPanel(QWidget *parent = nullptr);

    void Add_History_Entry(const QString &history_text);
    void Scroll_To_Latest();
    void Clear_History();
    void Set_Clear_Button_Size(int button_size);

  signals:
    void Clear_History_Requested();

  private:
    QTextEdit *history_text_edit_ = nullptr;
    QPushButton *clear_history_button_ = nullptr;
};

class DisplayPanel : public QFrame {
    Q_OBJECT

  public:
    explicit DisplayPanel(QWidget *parent = nullptr);

    void Set_Expression_Text(const QString &expression_text);
    void Set_Status_Message(const QString &status_text, bool is_error);
    void Set_Expansion_Checked(bool is_checked);
    int Get_Expansion_Button_Size() const;

  signals:
    void Expansion_Toggle_Requested();
    void Expansion_Button_Size_Changed(int button_size);

  protected:
    void resizeEvent(QResizeEvent *resize_event) override;

  private:
    void Update_Expansion_Button_Size();

    QPushButton *expansion_toggle_button_ = nullptr;
    QLineEdit *expression_line_edit_ = nullptr;
    QLabel *status_message_label_ = nullptr;
    int expansion_button_size_ = 24;
};

class KeypadPanel : public QFrame {
    Q_OBJECT

  public:
    explicit KeypadPanel(QWidget *parent = nullptr);

    void Set_Scientific_Visible(bool is_visible);
    void Set_Rad_Mode_Enabled(bool is_radian_mode_enabled);
    void Set_Inverse_Mode_Enabled(bool is_inverse_mode_enabled);

  signals:
    void Insert_Chunk_Requested(const QString &visible_chunk, const QString &backend_chunk);
    void Function_Group_Requested(const QString &function_group_name);
    void Clear_All_Requested();
    void Backspace_Requested();
    void Evaluate_Requested();
    void Inverse_Mode_Toggle_Requested();
    void Rad_Deg_Mode_Toggle_Requested();

  private:
    QPushButton *Create_Key_Button(const QString &button_text, const QString &button_role) const;
    void Connect_Insert_Button(QPushButton *button, const QString &visible_chunk, const QString &backend_chunk);
    void Refresh_Mode_Dependent_Button_Text();

    QWidget *scientific_panel_widget_ = nullptr;

    QPushButton *rad_mode_button_ = nullptr;
    QPushButton *inverse_mode_button_ = nullptr;
    QPushButton *sin_button_ = nullptr;
    QPushButton *cos_button_ = nullptr;
    QPushButton *tan_button_ = nullptr;
    QPushButton *ln_button_ = nullptr;
    QPushButton *log_button_ = nullptr;

    bool is_radian_mode_enabled_ = true;
    bool is_inverse_mode_enabled_ = false;
};

QString Build_Calculator_Style_Sheet();

class CalculatorUI : public QMainWindow {
    Q_OBJECT

  public:
    explicit CalculatorUI(QWidget *parent = nullptr);

  protected:
    bool eventFilter(QObject *watched_object, QEvent *event) override;

  private:
    void Build_User_Interface();
    void Connect_User_Interface_Signals();

    void Handle_Insert_Chunk_Request(const QString &visible_chunk, const QString &backend_chunk);
    void Handle_Function_Request(const QString &function_group_name);

    void Handle_Clear_All_Request();
    void Handle_Backspace_Request();
    void Evaluate_Current_Expression();

    void Handle_Expansion_Toggle_Request();
    void Handle_Inverse_Mode_Toggle_Request();
    void Handle_Rad_Deg_Mode_Toggle_Request();
    void Refresh_Mode_Dependent_User_Interface();

    void Refresh_Expression_Display();
    QString Format_Result_Text(double result_value) const;
    void Show_Status_Message(const QString &status_text, bool is_error) const;

    QWidget *root_widget_ = nullptr;

    HistoryPanel *history_panel_ = nullptr;
    DisplayPanel *display_panel_ = nullptr;
    KeypadPanel *keypad_panel_ = nullptr;

    ExpressionState expression_state_;

    bool is_scientific_panel_visible_ = false;
    bool is_inverse_mode_enabled_ = false;
    bool is_radian_mode_enabled_ = true;
};

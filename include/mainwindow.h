#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>
#include <QPushButton>
#include "passwordgenerator.h"
#include "logger.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onGenerateClicked();
    void onClearClicked();
    void onAboutClicked();
    void onCommandEntered();
    void onCommandTextChanged(const QString& text);

private:
    void setupUi();
    void updateLog();
    void showAlert(const QString& title, const QString& message);
    void processCommand(const QString& cmd);
    void highlightCommand();
    void applyCommandSyntaxHighlight(const QString& text);

    // UI 컴포넌트
    QLineEdit *m_lengthEdit;
    QCheckBox *m_uppercaseCheck;
    QCheckBox *m_numbersCheck;
    QCheckBox *m_symbolsCheck;
    QPushButton *m_generateBtn;
    QPushButton *m_clearBtn;
    QPushButton *m_aboutBtn;
    QTextEdit *m_logView;
    QTextEdit *m_commandLine;  // QLineEdit에서 QTextEdit로 변경

    // 로직
    PasswordGenerator m_generator;
    Logger m_logger;

    // 상태
    bool m_passwordGenerated;
    int m_createCount;
    bool m_exceptSimilar;
    bool m_useCustomSymbols;
    QString m_customSymbols;
};

#endif // MAINWINDOW_H

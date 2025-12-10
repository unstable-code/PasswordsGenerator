#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QClipboard>
#include <QApplication>
#include <QInputDialog>
#include <QListWidget>
#include <QDialog>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_passwordGenerated(false)
    , m_createCount(1)
    , m_exceptSimilar(false)
    , m_useCustomSymbols(false)
{
    setWindowTitle("비밀번호 생성기");

    m_logger.info("passwdGen 초기화... [비밀번호 생성기] 윈도우 생성됨");

    setupUi();

    // Java 원본과 동일하게 설정 (600x150에서 pack()으로 자동 조정됨)
    // Qt에서는 적절한 크기로 조정
    setFixedSize(780, 380);  // 크기 고정 (리사이즈 불가)

    // 최소화/최대화 버튼 비활성화
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Java의 BorderLayout 구조를 Qt로 재현
    // North, West, Center, East, South
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    // ===== North: 헤더 (환영 메시지 + 설명을 한 줄로) =====
    QWidget *headerWidget = new QWidget(this);
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    QLabel *welcomeLabel = new QLabel("비밀번호 생성기에 오신것을 환영합니다!    아래 규칙을 설정하여 조건에 맞는 비밀번호를 생성하세요", this);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    headerLayout->addWidget(welcomeLabel);
    mainLayout->addWidget(headerWidget);

    // ===== 중간 영역: West, Center, East를 QHBoxLayout으로 구성 =====
    QHBoxLayout *middleLayout = new QHBoxLayout();

    // --- West: 비밀번호 생성 길이 입력 ---
    QWidget *passCntWidget = new QWidget(this);
    QHBoxLayout *passCntLayout = new QHBoxLayout(passCntWidget);
    passCntLayout->setContentsMargins(0, 0, 0, 0);
    passCntLayout->setSpacing(2);  // 라벨과 필드 간격 줄이기
    QLabel *placeholderText = new QLabel("비밀번호 생성 길이:", this);
    m_lengthEdit = new QLineEdit("5", this);
    m_lengthEdit->setMaximumWidth(50);
    passCntLayout->addWidget(placeholderText);
    passCntLayout->addWidget(m_lengthEdit);
    middleLayout->addWidget(passCntWidget);

    // --- Center: 체크박스 3개 ---
    QWidget *contextWidget = new QWidget(this);
    QHBoxLayout *contextLayout = new QHBoxLayout(contextWidget);
    contextLayout->setContentsMargins(0, 0, 0, 0);
    m_uppercaseCheck = new QCheckBox("대소문자 구분", this);
    m_numbersCheck = new QCheckBox("숫자 포함", this);
    m_symbolsCheck = new QCheckBox("특수문자 포함", this);
    contextLayout->addWidget(m_uppercaseCheck);
    contextLayout->addWidget(m_numbersCheck);
    contextLayout->addWidget(m_symbolsCheck);
    middleLayout->addWidget(contextWidget);

    // --- East: 생성, 지우기 버튼 ---
    QWidget *actionWidget = new QWidget(this);
    QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
    actionLayout->setContentsMargins(0, 0, 0, 0);
    m_generateBtn = new QPushButton("생성", this);
    m_clearBtn = new QPushButton("지우기", this);
    actionLayout->addWidget(m_generateBtn);
    actionLayout->addWidget(m_clearBtn);
    middleLayout->addWidget(actionWidget);

    mainLayout->addLayout(middleLayout);

    // ===== South: 푸터 (로그 출력 + 커맨드 라인) =====
    QWidget *footerWidget = new QWidget(this);
    QVBoxLayout *footerLayout = new QVBoxLayout(footerWidget);
    footerLayout->setContentsMargins(0, 0, 0, 0);

    // --- 로그 출력 (JScrollPane 스타일) - stretch로 확장 ---
    m_logView = new QTextEdit(this);
    m_logView->setReadOnly(true);
    m_logView->setMinimumHeight(200);  // 최소 높이 조정
    footerLayout->addWidget(m_logView, 1);  // stretch factor 1로 공간 확장

    // --- 커맨드 라인 (> 프롬프트 + 입력(stretch) + 더보기(우측)) - 하단 고정 ---
    QWidget *commandLineWidget = new QWidget(this);
    QHBoxLayout *cmdLayout = new QHBoxLayout(commandLineWidget);
    cmdLayout->setContentsMargins(0, 0, 0, 0);
    QLabel *cmdPrompt = new QLabel("> ", this);
    m_commandLine = new QTextEdit(this);
    m_commandLine->setMaximumHeight(25);  // 한 줄 높이
    m_commandLine->setMinimumHeight(25);  // 최소 높이도 설정
    m_commandLine->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_commandLine->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_commandLine->setAcceptRichText(false);
    m_aboutBtn = new QPushButton("더보기", this);
    cmdLayout->addWidget(cmdPrompt);
    cmdLayout->addWidget(m_commandLine, 1);  // stretch factor 1로 양 옆 채우기
    cmdLayout->addWidget(m_aboutBtn);  // 우측에 고정
    footerLayout->addWidget(commandLineWidget, 0);  // stretch factor 0으로 크기 고정

    mainLayout->addWidget(footerWidget, 1);  // 푸터가 남은 공간 모두 차지

    // 시그널 연결
    connect(m_generateBtn, &QPushButton::clicked, this, &MainWindow::onGenerateClicked);
    connect(m_clearBtn, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(m_aboutBtn, &QPushButton::clicked, this, &MainWindow::onAboutClicked);
    connect(m_commandLine, &QTextEdit::textChanged, this, &MainWindow::highlightCommand);

    // QTextEdit에서 Enter 키 처리를 위해 installEventFilter 사용
    m_commandLine->installEventFilter(this);

    updateLog();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_commandLine && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            if (!(keyEvent->modifiers() & Qt::ShiftModifier)) {
                onCommandEntered();
                return true;  // 이벤트 소비
            }
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::onGenerateClicked()
{
    bool ok;
    int length = m_lengthEdit->text().trimmed().toInt(&ok);

    if (!ok || length <= 0 || length > 256) {
        m_logger.error(QString("비밀번호를 생성하는 도중 예외 발생: %1 (은)는 비밀번호 길이를 생성할 수 있는 올바른 숫자가 아닙니다.")
                      .arg(m_lengthEdit->text()));
        updateLog();
        showAlert("비밀번호 생성기 - 오류",
                 "비밀번호 길이 필드에는 자연수만 입력해야 합니다. 비밀번호 길이 필드를 다시 한번 확인하시길 바랍니다.");
        return;
    }

    // 정책 검증
    if (!m_generator.validatePolicy(length, m_uppercaseCheck->isChecked(),
                                   m_numbersCheck->isChecked(), m_symbolsCheck->isChecked())) {
        m_logger.warning("비밀번호 정책이 규정에 부합하지 않음");
        updateLog();
        showAlert("비밀번호 생성기 - 경고",
                 "현재 설정된 비밀번호 정책은 대부분의 사이트에서 허용하지 않으며 보안에 치명적이므로 이 비밀번호를 사용하는 것은 권장하지 않습니다.");
    }

    QString customSymbols = m_useCustomSymbols ? m_customSymbols : QString();

    if (m_createCount == 1) {
        // 단일 비밀번호 생성
        QString password = m_generator.generate(length, m_uppercaseCheck->isChecked(),
                                               m_numbersCheck->isChecked(), m_symbolsCheck->isChecked(),
                                               m_exceptSimilar, customSymbols);

        if (password.isEmpty()) {
            m_logger.error("비밀번호 생성 실패");
            updateLog();
            showAlert("비밀번호 생성기 - 오류", "비밀번호를 생성할 수 없습니다.");
            return;
        }

        m_logger.info("비밀번호가 성공적으로 생성되었습니다.");
        updateLog();

        QApplication::clipboard()->setText(password);
        m_passwordGenerated = true;

        if (password.length() > 20) {
            showAlert("비밀번호 생성기",
                     "생성된 비밀번호가 클립보드에 저장되었으므로 이제 비밀번호 필드에 붙여넣기만 하면 됩니다.");
        } else {
            showAlert("비밀번호 생성기",
                     QString("생성된 비밀번호는: %1 입니다. 이 비밀번호는 이미 클립보드에 저장되었으므로 스크린 샷을 따로 촬영하지 않아도 됩니다.").arg(password));
        }
    } else {
        // 다중 비밀번호 생성
        QStringList passwords;
        for (int i = 0; i < m_createCount; ++i) {
            QString pwd = m_generator.generate(length, m_uppercaseCheck->isChecked(),
                                             m_numbersCheck->isChecked(), m_symbolsCheck->isChecked(),
                                             m_exceptSimilar, customSymbols);
            if (!pwd.isEmpty()) {
                passwords.append(pwd);
            }
        }

        if (passwords.isEmpty()) {
            m_logger.error("비밀번호 생성 실패");
            updateLog();
            return;
        }

        m_logger.info("비밀번호가 성공적으로 생성되었습니다.");
        updateLog();

        // 선택 다이얼로그
        QDialog dialog(this);
        dialog.setWindowTitle("비밀번호 생성 결과");
        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        layout->addWidget(new QLabel("생성된 비밀번호 중 하나를 선택하세요:"));
        QListWidget *listWidget = new QListWidget(&dialog);
        listWidget->addItems(passwords);
        listWidget->setCurrentRow(0);
        layout->addWidget(listWidget);

        QPushButton *okBtn = new QPushButton("승인", &dialog);
        layout->addWidget(okBtn);

        connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

        if (dialog.exec() == QDialog::Accepted && listWidget->currentItem()) {
            QString selected = listWidget->currentItem()->text();
            QApplication::clipboard()->setText(selected);
            m_passwordGenerated = true;

            if (selected.length() > 20) {
                showAlert("비밀번호 생성기",
                         "선택한 비밀번호가 클립보드에 저장되었으므로 이제 비밀번호 필드에 붙여넣기만 하면 됩니다.");
            } else {
                showAlert("비밀번호 생성기",
                         QString("선택한 비밀번호는: %1 입니다. 이 비밀번호는 클립보드에 저장되었으므로 스크린 샷을 따로 촬영하지 않아도 됩니다.").arg(selected));
            }
        }
    }
}

void MainWindow::onClearClicked()
{
    if (!m_passwordGenerated) {
        m_logger.error("클립보드를 지울 수 없습니다. 먼저 비밀번호를 생성하시길 바랍니다.");
        updateLog();
        showAlert("비밀번호 생성기 - 오류",
                 "비밀번호를 한 번도 복사하지 않았거나 이미 클립보드가 비워진 상태입니다.");
    } else {
        QApplication::clipboard()->clear();
        m_passwordGenerated = false;
        m_logger.clear();
        m_logger.info("클립보드 지우기 완료");
        updateLog();
        showAlert("비밀번호 생성기",
                 "생성된 비밀번호가 클립보드에서 제거되었습니다. 만약 비밀번호가 유실되었을 경우 해당 홈페이지 비밀번호 찾기 기능을 이용하시길 바랍니다.");
    }
}

void MainWindow::onAboutClicked()
{
    m_logger.info("개발자 정보 창 활성화");
    updateLog();
    QMessageBox::information(this, "개발자에 관하여",
                            "개발자: unstable-code\n"
                            "GitHub: https://github.com/unstable-code/PasswordsGenerator");
}

void MainWindow::onCommandEntered()
{
    QString cmd = m_commandLine->toPlainText().trimmed();
    if (!cmd.isEmpty()) {
        processCommand(cmd);
        m_commandLine->clear();
    }
}

void MainWindow::processCommand(const QString& cmd)
{
    QStringList parts = cmd.toLower().split(' ', Qt::SkipEmptyParts);
    if (parts.isEmpty()) return;

    if (parts[0] == "help") {
        QString help = QString("도움말\n"
                              "  help - 이 도움말 표시\n"
                              "  count <1-16> - 생성할 갯수 (현재 %1개)\n"
                              "  exceptsym <on/off> - 비슷한 문자 제외 (상태: %2)\n"
                              "  unisym <on [symbols]/off> - 특수문자 간략화 (상태: %3)\n"
                              "  verbose <on/off> - INFO 로그 표시 (상태: %4)\n"
                              "  clear - 버퍼 청소\n"
                              "  exit - 이 프로그램 종료")
                      .arg(m_createCount)
                      .arg(m_exceptSimilar ? "true" : "false")
                      .arg(m_useCustomSymbols ? "true" : "false")
                      .arg(m_logger.debugMode() ? "true" : "false");
        m_logger.note(help);
    }
    else if (parts[0] == "count") {
        if (parts.size() != 2) {
            m_logger.error("비밀번호 생성 수를 제공하지 않았습니다.");
        } else {
            bool ok;
            int count = parts[1].toInt(&ok);
            if (ok && count > 0 && count <= 16) {
                m_createCount = count;
                m_logger.note("비밀번호 생성 수가 성공적으로 변경됨");
            } else {
                m_logger.error(QString("%1 (은)는 비밀번호 생성 수를 설정할 수 있는 올바른 숫자가 아닙니다.").arg(parts[1]));
            }
        }
    }
    else if (parts[0] == "verbose") {
        if (parts.size() == 2 && (parts[1] == "on" || parts[1] == "off")) {
            m_logger.setDebugMode(parts[1] == "on");
        } else {
            m_logger.error("verbose 명령어는 on 또는 off 인자가 필요합니다.");
        }
    }
    else if (parts[0] == "exceptsym") {
        if (parts.size() == 2 && (parts[1] == "on" || parts[1] == "off")) {
            m_exceptSimilar = (parts[1] == "on");
            m_logger.note(QString("비슷한 문자 제외 기능이 %1되었음").arg(m_exceptSimilar ? "활성화" : "비활성화"));
        } else {
            m_logger.error("exceptsym 명령어는 on 또는 off 인자가 필요합니다.");
        }
    }
    else if (parts[0] == "unisym") {
        if (parts.size() < 2 || (parts[1] != "on" && parts[1] != "off")) {
            m_logger.error("unisym on: 명령어를 실행하는 도중 예외가 발생하였습니다. java.lang.NullPointerException: Command not found.");
        } else if (parts[1] == "off") {
            m_useCustomSymbols = false;
            m_customSymbols.clear();
            m_symbolsCheck->setText("특수문자 포함");  // 원래대로 변경
            m_logger.note("특수문자 선별기능이 비활성화 되었음");
        } else if (parts[1] == "on") {
            if (parts.size() != 3) {
                m_useCustomSymbols = false;
                m_symbolsCheck->setText("특수문자 포함");
                m_logger.error("unisym on: 명령어를 실행하는 도중 예외가 발생하였습니다. java.lang.NullPointerException: The parameter options are ambiguous.");
            } else {
                // Java 로직: 숫자, 알파벳, 공백, DEL 문자 제거 및 중복 제거
                QString input = parts[2];
                QString cleaned;
                bool isEdited = false;
                bool hasExceptSimilar = false;

                // 문자 필터링
                for (int i = 0; i < input.length(); ++i) {
                    QChar ch = input[i];
                    int code = ch.unicode();

                    // 소문자 알파벳 제거 (a-z: 97-122)
                    if (code >= 97 && code <= 122) {
                        isEdited = true;
                        continue;
                    }
                    // 제어 문자 및 공백 제거 (0-32)
                    if (code >= 0 && code <= 32) {
                        isEdited = true;
                        continue;
                    }
                    // 숫자 제거 (0-9: 48-57)
                    if (code >= 48 && code <= 57) {
                        isEdited = true;
                        continue;
                    }
                    // DEL 문자 제거 (127)
                    if (code == 127) {
                        isEdited = true;
                        continue;
                    }

                    // 중복 검사
                    if (cleaned.contains(ch)) {
                        isEdited = true;
                        continue;
                    }

                    // exceptSimilar 검사 (Java: similarSymbol = "1l|Ii!joO0;:9gqxX.,")
                    if (m_exceptSimilar) {
                        QString similarSymbol = "1l|Ii!joO0;:9gqxX.,";
                        if (similarSymbol.contains(ch)) {
                            hasExceptSimilar = true;
                        }
                    }

                    cleaned += ch;
                }

                if (hasExceptSimilar) {
                    m_logger.warning("비슷한 문자 제외기능이 활성화 되어 있으므로 1l|Ii!joO0;:9gqxX., 문자들은 비밀번호 조합시 사용하지 않을 예정입니다.");
                }

                if (cleaned.isEmpty()) {
                    m_useCustomSymbols = false;
                    m_symbolsCheck->setText("특수문자 포함");
                    m_logger.error(QString("unisym on %1: 명령어를 실행하는 도중 예외가 발생하였습니다. java.lang.NullPointerException: 두번째 파라미터에는 특수문자만 제공해야 합니다.").arg(input));
                } else {
                    m_customSymbols = cleaned;
                    m_useCustomSymbols = true;
                    m_symbolsCheck->setText("일부 특수문자 포함");

                    if (isEdited && !hasExceptSimilar) {
                        m_logger.note(QString("특수문자 선별기능이 활성화 되었지만 적합하지 않은 문자를 제외하고 다음 문자만 적용됨: %1").arg(cleaned));
                    } else if (isEdited && hasExceptSimilar) {
                        m_logger.note(QString("특수문자 선별기능이 활성화 되었지만 적합하지 않은 문자를 제외하고도 다음 문자가 무시될 수 있음: %1").arg(cleaned));
                    } else if (!isEdited && hasExceptSimilar) {
                        m_logger.note(QString("특수문자 선별기능이 활성화 되었지만 다음 문자가 무시될 수 있음: %1").arg(cleaned));
                    } else {
                        m_logger.note(QString("특수문자 선별기능이 활성화 되었으며 다음 문자만 표시됨: %1").arg(cleaned));
                    }
                }
            }
        }
    }
    else if (parts[0] == "clear") {
        m_logger.clear();
    }
    else if (parts[0] == "exit") {
        QApplication::quit();
    }
    else {
        m_logger.error(QString("%1: 명령어를 찾을 수 없습니다.").arg(parts[0]));
    }

    updateLog();
}

void MainWindow::updateLog()
{
    m_logView->clear();
    QString html;

    for (const LogEntry& entry : m_logger.logs()) {
        QString color;
        switch (entry.level) {
            case LogLevel::Error: color = "red"; break;
            case LogLevel::Warning: color = "orange"; break;
            case LogLevel::Info: color = "gray"; break;
            case LogLevel::Note: color = "black"; break;
        }
        // \n을 <br>로 변환
        QString msg = entry.toString().toHtmlEscaped();
        msg.replace("\n", "<br>&nbsp;&nbsp;");
        html += QString("<span style='color:%1'><b>%2</b></span><br>").arg(color, msg);
    }

    m_logView->setHtml(html);

    // 스크롤을 맨 아래로
    QTextCursor cursor = m_logView->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_logView->setTextCursor(cursor);
}

void MainWindow::showAlert(const QString& title, const QString& message)
{
    m_logger.info(QString("알럿이 활성화됨: [%1]").arg(title));
    updateLog();
    QMessageBox::information(this, title, message);
    m_logger.info(QString("알럿이 비활성화됨: [%1]").arg(title));
    updateLog();
}

void MainWindow::onCommandTextChanged(const QString& text)
{
    // 사용하지 않음 - highlightCommand()가 직접 호출됨
}

void MainWindow::highlightCommand()
{
    // Java의 토큰별 색상 강조 구현
    QString text = m_commandLine->toPlainText();

    // 커서 위치 저장
    QTextCursor cursor = m_commandLine->textCursor();
    int cursorPos = cursor.position();

    // 블록 시그널로 무한 루프 방지
    m_commandLine->blockSignals(true);

    // 텍스트를 다시 설정하고 포맷 적용
    cursor.select(QTextCursor::Document);
    cursor.removeSelectedText();

    applyCommandSyntaxHighlight(text);

    // 커서 위치 복원
    cursor = m_commandLine->textCursor();
    cursor.setPosition(qMin(cursorPos, m_commandLine->toPlainText().length()));
    m_commandLine->setTextCursor(cursor);

    m_commandLine->blockSignals(false);
}

void MainWindow::applyCommandSyntaxHighlight(const QString& text)
{
    QStringList validCommands = {"help", "count", "exceptsym", "unisym", "verbose", "clear", "exit"};
    QStringList toggleKeywords = {"on", "off"};

    QTextCursor cursor = m_commandLine->textCursor();
    cursor.movePosition(QTextCursor::Start);

    QStringList tokens = text.split(' ', Qt::KeepEmptyParts);

    for (int i = 0; i < tokens.size(); ++i) {
        QString token = tokens[i];
        QString tokenLower = token.toLower();

        QTextCharFormat format;

        if (i == 0) {
            // 첫 번째 토큰: 명령어
            if (validCommands.contains(tokenLower)) {
                format.setForeground(QColor(0, 150, 0));  // 초록색 (GREEN)
            } else if (!token.isEmpty()) {
                format.setForeground(Qt::red);  // 빨간색 (RED)
            }
        } else {
            // 나머지 토큰: 인자
            if (toggleKeywords.contains(tokenLower)) {
                format.setForeground(QColor(255, 140, 0));  // 주황색 (ORANGE)
            } else {
                // 숫자 검증 (1-16)
                bool ok;
                int num = tokenLower.toInt(&ok);
                if (ok && num >= 1 && num <= 16) {
                    format.setForeground(Qt::blue);  // 파란색 (BLUE)
                } else if (!token.isEmpty()) {
                    // 그 외 (특수문자 또는 잘못된 인자)
                    if (i == 1 && tokens[0].toLower() == "unisym" && tokenLower == "on") {
                        format.setForeground(QColor(255, 140, 0));  // on은 주황색
                    } else if (i == 2 && tokens[0].toLower() == "unisym") {
                        // unisym의 세 번째 인자는 특수문자 - 검정색으로 유지
                        format.setForeground(Qt::black);
                    } else {
                        format.setForeground(Qt::red);  // 잘못된 인자 - 빨간색
                    }
                }
            }
        }

        cursor.insertText(token, format);

        // 공백 추가 (마지막 토큰이 아닌 경우)
        if (i < tokens.size() - 1) {
            cursor.insertText(" ");
        }
    }
}

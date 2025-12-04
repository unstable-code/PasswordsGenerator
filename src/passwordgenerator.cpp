#include "passwordgenerator.h"
#include <QSet>
#include <algorithm>

PasswordGenerator::PasswordGenerator()
    : m_similarSymbols("1l|Ii!joO0;:9gqxX.,")
    , m_gen(m_rd())
{
}

QString PasswordGenerator::generate(int length,
                                   bool includeUppercase,
                                   bool includeNumbers,
                                   bool includeSymbols,
                                   bool exceptSimilar,
                                   const QString& customSymbols)
{
    if (length <= 0 || length > 256) {
        return QString();
    }

    QString charset = buildCharset(includeUppercase, includeNumbers,
                                  includeSymbols, exceptSimilar, customSymbols);

    if (charset.isEmpty()) {
        return QString();
    }

    QString password;
    password.reserve(length);
    QChar previousChar;

    std::uniform_int_distribution<> dist(0, charset.length() - 1);

    while (password.length() < length) {
        QChar ch = charset[dist(m_gen)];

        // 연속된 문자 방지
        if (ch != previousChar) {
            password.append(ch);
            previousChar = ch;
        }
    }

    return password;
}

bool PasswordGenerator::validatePolicy(int length,
                                      bool includeUppercase,
                                      bool includeNumbers,
                                      bool includeSymbols) const
{
    // 대부분의 사이트 정책: 8자 이상, 대소문자/숫자/특수문자 모두 포함
    return length >= 8 && includeUppercase && includeNumbers && includeSymbols;
}

QString PasswordGenerator::buildCharset(bool includeUppercase,
                                       bool includeNumbers,
                                       bool includeSymbols,
                                       bool exceptSimilar,
                                       const QString& customSymbols)
{
    QString charset;

    // 소문자 (항상 포함)
    for (char c = 'a'; c <= 'z'; ++c) {
        charset.append(QChar(c));
    }

    // 대문자
    if (includeUppercase) {
        for (char c = 'A'; c <= 'Z'; ++c) {
            charset.append(QChar(c));
        }
    }

    // 숫자
    if (includeNumbers) {
        for (char c = '0'; c <= '9'; ++c) {
            charset.append(QChar(c));
        }
    }

    // 특수문자
    if (includeSymbols) {
        if (!customSymbols.isEmpty()) {
            charset.append(customSymbols);
        } else {
            // ASCII 특수문자: 33-47, 58-64, 91-96, 123-126
            for (char c = 33; c <= 47; ++c) charset.append(QChar(c));
            for (char c = 58; c <= 64; ++c) charset.append(QChar(c));
            for (char c = 91; c <= 96; ++c) charset.append(QChar(c));
            for (char c = 123; c <= 126; ++c) charset.append(QChar(c));
        }
    }

    // 비슷한 문자 제외
    if (exceptSimilar) {
        QString filtered;
        for (const QChar& ch : charset) {
            if (!m_similarSymbols.contains(ch)) {
                filtered.append(ch);
            }
        }
        charset = filtered;
    }

    return charset;
}

#ifndef PASSWORDGENERATOR_H
#define PASSWORDGENERATOR_H

#include <QString>
#include <QVector>
#include <random>

class PasswordGenerator {
public:
    PasswordGenerator();

    // 비밀번호 생성
    QString generate(int length,
                    bool includeUppercase,
                    bool includeNumbers,
                    bool includeSymbols,
                    bool exceptSimilar,
                    const QString& customSymbols = QString());

    // 정책 검증
    bool validatePolicy(int length,
                       bool includeUppercase,
                       bool includeNumbers,
                       bool includeSymbols) const;

    // 비슷한 문자 목록
    QString similarSymbols() const { return m_similarSymbols; }

private:
    QString m_similarSymbols;
    std::random_device m_rd;
    std::mt19937 m_gen;

    QString buildCharset(bool includeUppercase,
                        bool includeNumbers,
                        bool includeSymbols,
                        bool exceptSimilar,
                        const QString& customSymbols);
};

#endif // PASSWORDGENERATOR_H

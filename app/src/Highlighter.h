#ifndef UTEXT_HIGHLIGHTER_H
#define UTEXT_HIGHLIGHTER_H

// WARN
#include <QDebug>

#include <QSyntaxHighlighter>
#include <QRegularExpression>

class Highlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) override;

private:
    QBrush m_brownBrush;
    QColor m_brownColor;
    QBrush m_yellowBrush;
    QColor m_yellowColor;
    QBrush m_blueBrush;
    QColor m_blueColor;

    struct HighlightingRule {
        QRegularExpression m_pattern;
        QTextCharFormat m_format;
    };
    QVector<HighlightingRule> m_highlightingRules;

    QRegularExpression m_commentStartExpression;
    QRegularExpression m_commentEndExpression;

    QTextCharFormat m_keywordFormat;
    QTextCharFormat m_operatorFormat;
    QTextCharFormat m_classFormat;
    QTextCharFormat m_singleLineCommentFormat;
    QTextCharFormat m_multiLineCommentFormat;
    QTextCharFormat m_quotationFormat;
    QTextCharFormat m_functionFormat;
    QTextCharFormat m_variableFormat;
    QTextCharFormat m_preprocFormat;
};

#endif // UTEXT_HIGHLIGHTER_H

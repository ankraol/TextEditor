#include "Highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {
    HighlightingRule rule;

    // Setup custom colors
    m_brownColor.setRgb(132, 84, 76);
    m_brownBrush.setStyle(Qt::BrushStyle::SolidPattern);
    m_brownBrush.setColor(m_brownColor);
    m_yellowColor.setRgb(168, 150, 100);
    m_yellowBrush.setStyle(Qt::BrushStyle::SolidPattern);
    m_yellowBrush.setColor(m_yellowColor);
    m_blueColor.setRgb(68, 104, 116);
    m_blueBrush.setStyle(Qt::BrushStyle::SolidPattern);
    m_blueBrush.setColor(m_blueColor);

    m_keywordFormat.setForeground(Qt::blue);
    m_keywordFormat.setFontWeight(QFont::Bold);

    m_operatorFormat.setForeground(Qt::darkMagenta);
    m_operatorFormat.setFontWeight(QFont::Bold);

    const QString keywordPatterns[] = {
        QStringLiteral("\\bchar\\b"), QStringLiteral("\\bclass\\b"), QStringLiteral("\\bconst\\b"),
        QStringLiteral("\\bdouble\\b"), QStringLiteral("\\benum\\b"), QStringLiteral("\\bexplicit\\b"),
        QStringLiteral("\\bfriend\\b"), QStringLiteral("\\binline\\b"), QStringLiteral("\\bint\\b"),
        QStringLiteral("\\blong\\b"), QStringLiteral("\\bnamespace\\b"), QStringLiteral("\\boperator\\b"),
        QStringLiteral("\\bprivate\\b"), QStringLiteral("\\bprotected\\b"), QStringLiteral("\\bpublic\\b"),
        QStringLiteral("\\bshort\\b"), QStringLiteral("\\bsignals\\b"), QStringLiteral("\\bsigned\\b"),
        QStringLiteral("\\bslots\\b"), QStringLiteral("\\bstatic\\b"), QStringLiteral("\\bstruct\\b"),
        QStringLiteral("\\btemplate\\b"), QStringLiteral("\\btypedef\\b"), QStringLiteral("\\btypename\\b"),
        QStringLiteral("\\bunion\\b"), QStringLiteral("\\bunsigned\\b"), QStringLiteral("\\bvirtual\\b"),
        QStringLiteral("\\bvoid\\b"), QStringLiteral("\\bvolatile\\b"), QStringLiteral("\\bbool\\b")
    };

    const QString operatorsPatterns[] = {
        QStringLiteral("\\bif\\b"), QStringLiteral("\\bfor\\b"), QStringLiteral("\\bwhile\\b"),
        QStringLiteral("\\bswitch\\b"), QStringLiteral("\\bbreak\\b"), QStringLiteral("\\breturn\\b")
    };

    m_variableFormat.setForeground(m_blueBrush);
    rule.m_pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+\\b"));
    rule.m_format = m_variableFormat;
    m_highlightingRules.append(rule);

    for (const QString &pattern : keywordPatterns) {
        rule.m_pattern = QRegularExpression(pattern);
        rule.m_format = m_keywordFormat;
        m_highlightingRules.append(rule);
    }

    for (const QString &pattern : operatorsPatterns) {
        rule.m_pattern = QRegularExpression(pattern);
        rule.m_format = m_operatorFormat;
        m_highlightingRules.append(rule);
    }

    m_classFormat.setFontWeight(QFont::Bold);
    m_classFormat.setForeground(Qt::darkGreen);
    rule.m_pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
    rule.m_format = m_classFormat;
    m_highlightingRules.append(rule);

    m_quotationFormat.setFontWeight(QFont::Bold);
    m_quotationFormat.setForeground(m_brownBrush);
    rule.m_pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.m_format = m_quotationFormat;
    m_highlightingRules.append(rule);

    // functionFormat.setFontItalic(true);
    m_functionFormat.setForeground(m_yellowBrush);
    rule.m_pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.m_format = m_functionFormat;
    m_highlightingRules.append(rule);

    m_singleLineCommentFormat.setForeground(Qt::darkGray);
    rule.m_pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.m_format = m_singleLineCommentFormat;
    m_highlightingRules.append(rule);

    m_multiLineCommentFormat.setForeground(Qt::darkGray);
    m_commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    m_commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));

    m_preprocFormat.setForeground(Qt::darkMagenta);
    rule.m_pattern = QRegularExpression(QStringLiteral("#[A-Za-z]+"));
    rule.m_format = m_preprocFormat;
    m_highlightingRules.append(rule);
}

void Highlighter::highlightBlock(const QString &text) {
    for (const HighlightingRule &rule : qAsConst(m_highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.m_pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.m_format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(m_commentStartExpression);
    while (startIndex >= 0) {
        QRegularExpressionMatch match = m_commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else
            commentLength = endIndex - startIndex + match.capturedLength();
        setFormat(startIndex, commentLength, m_multiLineCommentFormat);
        startIndex = text.indexOf(m_commentStartExpression, startIndex + commentLength);
    }
}

// struct Highlighter::HighlightingRule {
//     QRegularExpression pattern;
//     QTextCharFormat format;
// };

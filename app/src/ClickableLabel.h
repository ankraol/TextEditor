#ifndef UTEXT_CLICKABLELABEL_H
#define UTEXT_CLICKABLELABEL_H

#include <iostream>

#include <QLabel>
#include <QWidget>
#include <QString>
#include <QEvent>
#include <QRect>
#include <QMouseEvent>
#include <Qt>
#include <QFont>
#include <QSizePolicy>
#include <QDebug>

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    ClickableLabel(QString text, QWidget *parent = Q_NULLPTR, Qt::AlignmentFlag = Qt::AlignCenter);

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent* event) override;
    QRect getTextComponentRectangle() const;

    QPoint m_mouseCoord;
};

#endif //UTEXT_CLICKABLELABEL_H

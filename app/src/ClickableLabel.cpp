#include "ClickableLabel.h"

ClickableLabel::ClickableLabel(QString text, QWidget *parent, Qt::AlignmentFlag flag) : QLabel(parent){
    setMouseTracking(true);
    setContentsMargins(10,10,10,10);
    setAlignment(Qt::AlignHCenter | flag);
//    setContentsMargins(10,10,10,10);
//    setSizePolicy(QSizePolicy::Maximum , QSizePolicy::Maximum);
    setText(text);
}

void ClickableLabel::mousePressEvent(QMouseEvent *event) {
    emit clicked();
}

QRect ClickableLabel::getTextComponentRectangle() const {
    if(frameWidth() < 0) {
        throw std::runtime_error("Negative frame width.");
    }
    int effectiveIndent = indent();
    int trueMargin = margin();
    if(effectiveIndent < 0) {
        if(frameWidth() == 0 || margin() > 0) { // (1)
            effectiveIndent = 0;
        } else if(frameWidth() > 0) {
            QFontMetrics fm(font());
            effectiveIndent = fm.horizontalAdvance(QChar('x')) / 2;
        }
        if(frameWidth() > 0 && margin() < 0) { // (2)
            trueMargin = 0;
        }
    }

    QFontMetrics fm(font());
    QRect bRect = fm.boundingRect(text());

//    int x1, x2, y1, y2;
//    bRect.getCoords(&x1, &y1, &x2, &y2);
//    std::cout << "Bounding Rectangle coordinates -> " << x1  << ' ' << y1 << " " << x2 << " " << y2 << '\n';

    bRect.setWidth(fm.horizontalAdvance(text()));

    int indentOffset = effectiveIndent + trueMargin + frameWidth();
    int offsetX = 0;
    int offsetY = 0;
    if(alignment() & Qt::AlignHCenter) {
        offsetX = rect().width() / 2 - bRect.width() / 2;
    } else if(alignment() & Qt::AlignRight) {
        offsetX = rect().width() - bRect.width() - indentOffset;
    } else if(alignment() & Qt::AlignJustify) {
        offsetX = trueMargin + frameWidth();
    } else if(alignment() & Qt::AlignLeft) {
        offsetX = indentOffset;
    }
    if(alignment() & Qt::AlignVCenter) {
        offsetY = rect().height() / 2 - bRect.height() / 2;
    } else if(alignment() & Qt::AlignBottom) {
        offsetY = rect().height() - bRect.height() - indentOffset;
    } else if(alignment() & Qt::AlignTop) {
        offsetY = indentOffset;
    }

    bRect.moveTopLeft(rect().topLeft());
    bRect.setX(bRect.x() + offsetX);
    bRect.setWidth(bRect.width() + offsetX);

    qDebug() << text();
    qDebug() << bRect.y();
    qDebug() << bRect.height();
    qDebug() << offsetY;

    bRect.setY(bRect.y() + offsetY);
    bRect.setHeight(bRect.height() + offsetY);

    qDebug() << bRect.y();
    qDebug() << bRect.height();

    return bRect;
}

void ClickableLabel::mouseMoveEvent(QMouseEvent* event) {
    int x1, x2, y1, y2;

    QRect bRect = getTextComponentRectangle();
    bRect.getCoords(&x1, &y1, &x2, &y2);

    m_mouseCoord = event->pos();
    std::cout << "Rectangla coordinates -> " << x1  << ' ' << y1 << " " << x2 << " " << y2 << '\n';
    qDebug() << "Mouse coordinates -> " << m_mouseCoord;

    if(bRect.contains(event->pos())) {
        setStyleSheet("QLabel { color : blue; font : 14pt 'PT Mono'; font-weight : bold; }");
        // Mouse pointer over text.
    } else {
        setStyleSheet("QLabel { color : black; font : 14pt 'PT Mono'; font-weight : bold; }");
        // Mouse pointer outside text.
    }
    QLabel::mouseMoveEvent(event);
}

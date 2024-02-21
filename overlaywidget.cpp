#include "overlaywidget.h"
 #include <QPainter>
#include <QPalette>

OverlayWidget::OverlayWidget(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_TransparentForMouseEvents);  // Make the widget ignore mouse events
    setPalette(QPalette(QColor(255, 255, 255, 0)));  // Set a transparent background

    // Uncomment if OverlayWidget is a standalone window
    // setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
}

void OverlayWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::red);  // Example: Set pen for drawing, customize as needed

    // Custom painting code here
    // For example, draw a rectangle or other shapes
    painter.drawRect(10, 10, 100, 100);  // Example rectangle, replace with your drawing logic
}
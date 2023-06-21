#pragma once

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QWheelEvent>

#include <QPointF>
#include <QLineF>
#include <QPainterPath>
#include <QVector>

#include <QMenu>
#include <QAction>

#include "poly.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QPointF offset;
    QPointF press_point;
    double scale_min = 1.0 * 0.1;
    double scale_max = 1000.0;
    double scale_size;
    double scale_step = 0.2;

    QMenu *m_contextMenu;

    QAction *nodeAction;
    QAction *edgeAction;
    QAction *eleAction;
    QAction *allAction;
    QAction *polyAction;

    QAction *centerAction;
    QAction *picAction;

private:
    void empty();

    vector<node> nodes;
    vector<edge> edges;
    vector<ele> eles;
    vector<hole> holes;
};

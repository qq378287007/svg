#include <QFileDialog>
#include <QDebug>
#include <QtMath>

#include "widget.h"

static QVector<QColor> colors{
    Qt::red,
            Qt::green,
            Qt::blue,
            Qt::cyan,
            Qt::magenta,
            Qt::yellow,
            Qt::darkRed,
            Qt::darkGreen,
            Qt::darkBlue,
            Qt::darkCyan,
            Qt::darkMagenta,
            Qt::darkYellow,
            Qt::black,
            Qt::darkGray,
            Qt::gray,
            Qt::lightGray,
};

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    offset = QPointF(0.0, 0.0);
    scale_size = 1.0;

    m_contextMenu = new QMenu(this);

    nodeAction = new QAction("导入.node文件", this);
    m_contextMenu->addAction(nodeAction);

    edgeAction = new QAction("导入.edge文件", this);
    m_contextMenu->addAction(edgeAction);

    eleAction = new QAction("导入.ele文件", this);
    m_contextMenu->addAction(eleAction);

    allAction = new QAction("导入all文件", this);
    m_contextMenu->addAction(allAction);

    polyAction = new QAction("导入.poly文件", this);
    m_contextMenu->addAction(polyAction);

    centerAction = new QAction("图像居中", this);
    m_contextMenu->addAction(centerAction);

    picAction = new QAction("导出图像", this);
    m_contextMenu->addAction(picAction);

    connect(nodeAction, &QAction::triggered, [&]()
    {
        QString fileName = QFileDialog::getOpenFileName(this, "导入.node文件");
        if(!fileName.isEmpty()){
            empty();
            readNode(fileName.toStdString().c_str(), nodes);
            update();
        } });

    connect(edgeAction, &QAction::triggered, [&]()
    {
        QString fileName = QFileDialog::getOpenFileName(this, "导入.edge文件");
        if(!fileName.isEmpty()){
            //empty();
            //readEdge(fileName);
            update();
        } });

    connect(eleAction, &QAction::triggered, [&]()
    {
        QString fileName = QFileDialog::getOpenFileName(this, "导入.ele文件");
        if(!fileName.isEmpty()){
            //empty();
            //readEle(fileName);
            update();
        } });

    connect(allAction, &QAction::triggered, [&]()
    {
        QString fileName = QFileDialog::getOpenFileName(this, "导入all文件");
        if(!fileName.isEmpty()){
            //empty();
            //readAll(fileName);
            update();
        } });

    connect(polyAction, &QAction::triggered, [&]()
    {
        QString fileName = QFileDialog::getOpenFileName(this, "导入.poly文件");
        if(!fileName.isEmpty()){
            //empty();
            //readPoly(fileName);
            update();
        } });

    connect(centerAction, &QAction::triggered, [&]()
    {
        int number = nodes.size();
        if(number>0){
            double min_x=999.9;
            double max_x=-999.9;
            double min_y=999.9;
            double max_y=-999.9;
            for(int i=0; i<number; i++){
                double x = nodes[i].m_X;
                double y = nodes[i].m_Y;
                if(x<min_x)
                    min_x = x;
                else if(x>max_x)
                    max_x = x;
                if(y<min_y)
                    min_y = y;
                else if(y>max_y)
                    max_y = y;
            }

            double center_x = (max_x+min_x)/2;
            double center_y = (max_y+min_y)/2;
            for(int i=0; i<number; i++){
                nodes[i].m_X -= center_x;
                nodes[i].m_Y -= center_y;
            }

            for(unsigned long long i=0; i<holes.size(); i++){
                holes[i].m_X -= center_x;
                holes[i].m_Y -= center_y;
            }

            offset = rect().center();
            scale_size = qMin(width()/(max_x-min_x), height()/(max_y-min_y))*0.8;
            update();
        }
    });

    connect(picAction, &QAction::triggered, [&]()
    {
        QPixmap pix = grab();
        pix.save("out.png"); });
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    paint.translate(offset);
    paint.scale(scale_size, scale_size);
    paint.rotate(0);

    paint.save();
    paint.setPen(Qt::NoPen);
    paint.setBrush(Qt::black);
    for (unsigned long long i = 0; i < eles.size(); i++)
    {
        int x = eles[i].m_X;
        int y = eles[i].m_Y;
        int z = eles[i].m_Z;
        QPointF p1 = QPointF(nodes[x - 1].m_X, nodes[x - 1].m_Y);
        QPointF p2 = QPointF(nodes[y - 1].m_X, nodes[y - 1].m_Y);
        QPointF p3 = QPointF(nodes[z - 1].m_X, nodes[z - 1].m_Y);

        QPointF points[3] = {p1, p2, p3};
        paint.drawPolygon(points, 3);
    }
    paint.restore();

    for (unsigned long long i = 0; i < edges.size(); i++)
    {
        paint.setPen(QPen(colors[edges[i].m_BoundMark % colors.size()], 5 / scale_size));
        int node1 = edges[i].m_X;
        int node2 = edges[i].m_Y;
        paint.drawLine(QLineF(nodes[node1 - 1].m_X, nodes[node1 - 1].m_Y,
                nodes[node2 - 1].m_X, nodes[node2 - 1].m_Y));
    }

    for (unsigned long long i = 0; i < nodes.size(); i++)
    {
        paint.setPen(QPen(colors[nodes[i].m_BoundMark % colors.size()], 7 / scale_size));
        paint.drawPoint(QPointF(nodes[i].m_X, nodes[i].m_Y));
    }

    for (unsigned long long i = 0; i < holes.size(); i++)
    {
        paint.setPen(QPen(Qt::black, 7 / scale_size));
        paint.drawPoint(QPointF(holes[i].m_X, holes[i].m_Y));
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        press_point = event->pos();
    else if (event->button() == Qt::RightButton)
        m_contextMenu->exec(event->globalPos());
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        offset += event->pos() - press_point;
        press_point = event->pos();
        update();
    }
}

void Widget::wheelEvent(QWheelEvent *event)
{
    QPoint p0 = event->pos();
    if (event->delta() > 0)
    {
        if (scale_size + scale_step <= scale_max)
        {
            offset = p0 + (offset - p0) * (scale_size + scale_step) / scale_size; // 基于当前鼠标点放大
            scale_size += scale_step;
            update();
        }
    }
    else
    {
        if (scale_size - scale_step >= scale_min)
        {
            offset = p0 + (offset - p0) * (scale_size - scale_step) / scale_size; // 基于当前鼠标点缩小
            scale_size -= scale_step;
            update();
        }
    }
}

void Widget::empty()
{
    nodes.clear();
    edges.clear();
    eles.clear();
    holes.clear();
}

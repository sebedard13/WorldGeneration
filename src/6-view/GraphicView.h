#pragma once

#include <qgraphicsview.h>
#include <QWheelEvent>

class GraphicView : public QGraphicsView {
private:
    unsigned int scaleNb = 2;
public:
    explicit GraphicView(QWidget *parent);

    void wheelEvent(QWheelEvent *event) override;

    void resetScale() { scaleNb = 2; };
};


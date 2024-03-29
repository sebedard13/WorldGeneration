﻿#pragma once

#include <QAction>
#include <qboxlayout.h>
#include <QFrame>
#include <QTabWidget>
#include <QStatusBar>
#include <QToolBar>
#include <QMenuBar>
#include <QPushButton>

#include "MapScreen.h"
#include "../5-controller/MapView.h"
#include "tab/Generate.h"
#include "tab/TabPerlinNoise.h"
#include "menu/MenuBar.h"
#include "tab/TabRandomVoronoi.h"
#include "tab/TabDiamondSquare.h"
#include "tab/TabPlatec.h"

class ViewRootClass {

public:

    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    TabPerlinNoise *tab0;
    TabRandomVoronoi *tab1;
    TabDiamondSquare *tab2;
    TabPlatec *tab3;
    QFrame *frame1;
    QVBoxLayout *verticalLayout_4;
    QPushButton *btnGenerate;
    MapScreen *mainMapScreen;
    std::shared_ptr<MapView> mapView;

    MenuBar *menuBar;

    QStatusBar *statusBar;

    std::vector<std::unique_ptr<Generate>> tabs{};

    void setupUi(QMainWindow *ViewRootClass);

    void retranslateUi(QMainWindow *ViewRootClass);

};


class ViewRoot : public QMainWindow {
Q_OBJECT

public:
    ViewRoot(QWidget *parent = nullptr);

    ~ViewRoot();

    void clickGenerate();

    std::shared_ptr<MapView> getMainMapView() {
        return ui.mapView;
    }

    void retranslate() {
        ui.retranslateUi(this);
    }

private:
    ViewRootClass ui;
};
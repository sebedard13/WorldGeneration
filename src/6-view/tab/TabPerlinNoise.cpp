#include "TabPerlinNoise.h"

#include "../ViewUtils.h"

#include "../../5-controller/Controller.h"
#include "../../5-controller/commands/DoAlgo.h"
#include "../../4-domain/algos/perlin/AlgoPerlinNoise.h"

void TabPerlinNoise::setupUi() {
    sizeContainer = new QWidget();
    sizeContainer->setObjectName("sizeContainer");
    sizeContainerLayout = generateInputLayout(sizeContainer);
    sizeLabel = new QLabel();
    sizeLabel->setObjectName("sizeLabel");
    sizeContainerLayout->addWidget(sizeLabel);
    sizeComboBox = new QComboBox();
    sizeComboBox->addItem(QString());
    sizeComboBox->addItem(QString());
    sizeComboBox->addItem(QString());
    sizeComboBox->addItem(QString());
    sizeComboBox->addItem(QString());
    sizeComboBox->setCurrentIndex(1);
    sizeComboBox->setObjectName("sizeComboBox");
    sizeContainerLayout->addWidget(sizeComboBox);
    mainLayout->addWidget(sizeContainer);

    seedInput = new InputSeed();
    mainLayout->addWidget(seedInput);

    cellSizeInput = new InputInteger("labelCellSize", 24, 1, 5000);
    mainLayout->addWidget(cellSizeInput);

    octavesInput = new InputInteger("labelOctaves", 1, 1, 24);
    mainLayout->addWidget(octavesInput);

    persistenceInput = new InputDouble("labelPersistence", 0.8);
    mainLayout->addWidget(persistenceInput);
}

void TabPerlinNoise::retranslateUi() {
    title->setText(ViewsUtils::local("algoPerlinNoiseName"));
    sizeLabel->setText(ViewsUtils::local("labelSize"));
    sizeComboBox->setItemText(0, "100x100");
    sizeComboBox->setItemText(1, "500x500");
    sizeComboBox->setItemText(2, "1000x1000");
    sizeComboBox->setItemText(3, "2500x2500");
    sizeComboBox->setItemText(4, "5000x5000");

    seedInput->retranslateUi();
    cellSizeInput->retranslateUi();
    octavesInput->retranslateUi();
    persistenceInput->retranslateUi();
}

void GeneratePerlinNoise::handleGenerate() {
    constexpr int sizes[5]{100, 500, 1000, 2500, 5000};
    const int sizeIndex = tabPerlinNoise->sizeComboBox->currentIndex();

    auto algo = std::make_unique<AlgoPerlinNoise>(sizes[sizeIndex], sizes[sizeIndex],
            tabPerlinNoise->cellSizeInput->getValue(),
            tabPerlinNoise->octavesInput->getValue(),
            tabPerlinNoise->persistenceInput->getValue(),
            tabPerlinNoise->seedInput->getValue()
    );

    Controller::execute(std::make_unique<DoAlgo>(std::move(algo)));
}

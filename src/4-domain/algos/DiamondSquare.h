#pragma once

#include "Algo.h"
#include "../../2-infrastructure/RandomEngine.h"
#include "../Mapf.h"

class DiamondSquare: public Algo {
public:
    DiamondSquare(const DiamondSquare &other) = delete;

    DiamondSquare(DiamondSquare &&other) noexcept = delete;

    DiamondSquare &operator=(const DiamondSquare &other) = delete;

    DiamondSquare &operator=(DiamondSquare &&other) noexcept = delete;

    DiamondSquare(unsigned size, unsigned seed, double roughness);

    std::unique_ptr<Map> run() override;

    ~DiamondSquare() override = default;

    RandomEngine rand = RandomEngine();

    std::unique_ptr<Mapf> execute();
private:
    const double roughness;
    const unsigned size;


};

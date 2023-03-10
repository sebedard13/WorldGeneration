cmake_minimum_required(VERSION 3.24)
project(WorldGeneration)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(WorldGeneration
        src/main.cpp

        src/4-domain/algos/RandomVoronoi.cpp src/4-domain/algos/Algo.h src/4-domain/algos/perlin/AlgoPerlinNoise.cpp src/4-domain/algos/perlin/AlgoPerlinNoise.h src/4-domain/algos/DistanceFromCenter.cpp src/4-domain/algos/DistanceFromCenter.h src/4-domain/algos/perlin/PerlinNoise.cpp src/3-services/KeyBinding.cpp src/3-services/KeyBinding.h src/3-services/Localization.cpp src/2-infrastructure/SetTimeout.cpp src/2-infrastructure/SetTimeout.h src/2-infrastructure/ThreadController.cpp src/4-domain/Map.cpp src/5-controller/commands/DoAlgo.cpp src/5-controller/commands/DoAlgo.h src/5-controller/commands/SaveImage.cpp src/5-controller/commands/SaveImage.h src/5-controller/Controller.cpp src/5-controller/MapView.cpp src/6-view/GraphicView.cpp src/6-view/MapScreen.cpp src/6-view/ViewRoot.cpp src/6-view/ViewUtils.h src/6-view/tab/TabPerlinNoise.cpp src/6-view/tab/input/InputDouble.cpp src/6-view/tab/input/InputInteger.cpp src/6-view/menu/MenuBar.cpp src/4-domain/Coord.cpp src/4-domain/Coord.h src/6-view/tab/TabRandomVoronoi.cpp src/6-view/tab/TabRandomVoronoi.h src/4-domain/algos/DiamondSquare.cpp src/4-domain/algos/DiamondSquare.h src/4-domain/Mapf.cpp src/4-domain/Mapf.h src/6-view/tab/TabDiamondSquare.cpp src/6-view/tab/TabDiamondSquare.h src/6-view/tab/Tab.h src/6-view/tab/Tab.cpp src/4-domain/algos/platec/Plate.cpp
        src/4-domain/algos/platec/Lithosphere.h src/4-domain/algos/platec/Lithosphere.cpp src/4-domain/algos/platec/GenerationController.h src/4-domain/algos/platec/GenerationController.cpp src/4-domain/algos/platec/Platec.cpp src/4-domain/algos/platec/Platec.h
        src/4-domain/Mapi.cpp src/4-domain/Mapi.h
        src/4-domain/algos/platec/PlatecMap.cpp src/4-domain/algos/platec/PlatecMap.h src/4-domain/Output.h src/2-infrastructure/RandomEngine.cpp src/2-infrastructure/RandomEngine.h src/6-view/tab/TabPlatec.cpp src/6-view/tab/TabPlatec.h src/6-view/tab/input/InputSeed.h src/6-view/tab/input/InputSeed.cpp)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property(TARGET WorldGeneration PROPERTY WIN32_EXECUTABLE true)
endif ()

target_link_libraries(WorldGeneration
        PRIVATE Qt::Core
        PRIVATE Qt::Gui
        PRIVATE Qt::Widgets
        )


#Qt6_DIR = Qt/{version}/{compiler}/lib/cmake/Qt6
#the_qt_path = Qt/{version}/{compiler}
cmake_path(GET Qt6_DIR PARENT_PATH the_qt_path)
cmake_path(GET the_qt_path PARENT_PATH the_qt_path)
cmake_path(GET the_qt_path PARENT_PATH the_qt_path)

install(DIRECTORY data/
        DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )

install(DIRECTORY ${the_qt_path}/plugins/platforms/
        DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms
        )

install(FILES $<TARGET_RUNTIME_DLLS:WorldGeneration>
        DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )

#gcc compiler dll
install(DIRECTORY ${the_qt_path}/bin/
        DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>
        FILES_MATCHING PATTERN "libgcc_s_seh-1.dll"
        )
install(DIRECTORY ${the_qt_path}/bin/
        DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>
        FILES_MATCHING PATTERN "libstdc++-6.dll"
        )
install(DIRECTORY ${the_qt_path}/bin/
        DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>
        FILES_MATCHING PATTERN "libwinpthread-1.dll"
        )

#Output all dll miss by previous install
install(TARGETS WorldGeneration
        COMPONENT WorldGeneration
        RUNTIME_DEPENDENCIES
        PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
        POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
        DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
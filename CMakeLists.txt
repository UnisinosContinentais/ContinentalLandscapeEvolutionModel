cmake_minimum_required(VERSION 3.5)
project(ContinentalLandscapeEvolutionModel VERSION 1.0.0 LANGUAGES CXX)

add_definitions(-DMAKE_CONTINENTALLANDSCAPEEVOLUTIONMODELPLUGIN_LIB)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Load custom configuration file for CMake (if any).
if(EXISTS ${CMAKE_SOURCE_DIR}/CMakeLists.config)
    include(${CMAKE_SOURCE_DIR}/CMakeLists.config)
endif()

#Make sure that custom modules like FindRapidJSON are found
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

##############################################
# Declare dependencies
find_package(SHP REQUIRED)
include_directories(${SHP_INCLUDE_DIR})

find_package(ContinentalDataManagement REQUIRED)
include_directories(${ContinentalDataManagement_INCLUDE_DIRS})

find_package(ContinentalHydroTools REQUIRED)
include_directories(${ContinentalHydroTools_INCLUDE_DIRS})

find_package(Qt5Core CONFIG REQUIRED)

find_package(OpenMP)

##############################################
# Create target and set properties

add_library(ContinentalLandscapeEvolutionModel
    src/Version.cpp
    src/ProcessLandscapeEvolutionModel.cpp
    src/service/SlopeCalculator.cpp
    src/service/LithologyDefinionCalculator.cpp
    src/constant/LandscapeEvolutionModelConstant.cpp
    src/domain/Direction.cpp
    src/domain/DrainageNetwork.cpp
    src/domain/EnumDirection.cpp
    src/domain/EnumDrainageNetworkLimit.cpp
    src/domain/HeuristicSinkRemovalProcessingMode.cpp
    src/domain/PositionMatrix.cpp
    src/domain/Raster.cpp
    src/domain/SinkDestroyConfig.cpp
    src/domain/StreamDefinitionConfig.cpp
    src/domain/SimulationLandscapeEvolutionModelConfig.cpp
    src/domain/GrainDispersionConfig.cpp
    src/domain/StreamDefinitionThresholdType.cpp
    src/domain/definition/SlopeTypes.cpp
    src/domain/definition/SlopeUnits.cpp
    src/domain/definition/GrainNames.cpp
    src/dto/LandscapeEvolutionModelInput.cpp
    src/exception/LandscapeEvolutionModelIsNotValidInputCommandException.cpp
    src/service/DifusionAlgorithmService.cpp
    src/service/DirectionCalculatorService.cpp
    src/service/EroderAlgorithmService.cpp
    src/service/HydroToolsAlgorithmService.cpp
    src/service/UpliftAlgorithmService.cpp
    src/util/RasterFileUtil.cpp
    src/util/ProcessLandscapeEvolutionModelLogUtil.cpp
)

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(ContinentalLandscapeEvolutionModel::ContinentalLandscapeEvolutionModel ALIAS ContinentalLandscapeEvolutionModel)

#Set target properties
target_include_directories(ContinentalLandscapeEvolutionModel
    PUBLIC
       $<INSTALL_INTERFACE:include>
       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(ContinentalLandscapeEvolutionModel PRIVATE cxx_auto_type)
target_compile_options(ContinentalLandscapeEvolutionModel PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)

target_link_libraries(ContinentalLandscapeEvolutionModel
    PUBLIC Qt5::Core
    PUBLIC SHP::SHP
    PUBLIC ContinentalDataManagement::ContinentalDataManagement
    PUBLIC OpenMP::OpenMP_CXX
    PRIVATE ContinentalHydroTools::ContinentalHydroTools
)

##############################################
# Installation instructions

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/ContinentalLandscapeEvolutionModel)

install(TARGETS ContinentalLandscapeEvolutionModel
    EXPORT ContinentalLandscapeEvolutionModel-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

#This is required so that the exported target has the name ContinentalLandscapeEvolutionModel and not ContinentalLandscapeEvolutionModel
set_target_properties(ContinentalLandscapeEvolutionModel PROPERTIES EXPORT_NAME ContinentalLandscapeEvolutionModel)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#Export the targets to a script
install(EXPORT ContinentalLandscapeEvolutionModel-targets
  FILE
    ContinentalLandscapeEvolutionModelTargets.cmake
  NAMESPACE
    ContinentalLandscapeEvolutionModel::
  DESTINATION
    ${INSTALL_CONFIGDIR}
)

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/ContinentalLandscapeEvolutionModelConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/ContinentalLandscapeEvolutionModelConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ContinentalLandscapeEvolutionModelConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

#Install the config, configversion and custom find modules
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ContinentalLandscapeEvolutionModelConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ContinentalLandscapeEvolutionModelConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)



##############################################
## Exporting from the build tree
# configure_file(
#     ${CMAKE_CURRENT_LIST_DIR}/cmake/FindRapidJSON.cmake
#     ${CMAKE_CURRENT_BINARY_DIR}/FindRapidJSON.cmake
#     COPYONLY)

export(EXPORT ContinentalLandscapeEvolutionModel-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/ContinentalLandscapeEvolutionModelTargets.cmake NAMESPACE ContinentalLandscapeEvolutionModel::)

add_subdirectory(test)
add_subdirectory(console)






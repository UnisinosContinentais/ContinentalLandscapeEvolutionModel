add_executable(ContinentalLandscapeEvolutionModelConsole src/main.cpp)
target_compile_features(ContinentalLandscapeEvolutionModelConsole PRIVATE cxx_auto_type)

target_link_libraries(ContinentalLandscapeEvolutionModelConsole ContinentalLandscapeEvolutionModel::ContinentalLandscapeEvolutionModel)

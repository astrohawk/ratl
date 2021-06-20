
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

pybind11_add_module(ratl_pybind ratl_pybind.cpp)

target_link_libraries(ratl_pybind PRIVATE ratl Gist)

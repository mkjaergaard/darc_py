find_package(PythonLibs)
find_package(Boost COMPONENTS python thread)

include_directories(${PYTHON_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_library(darc_py-pybindings SHARED darc_py.cpp)
target_link_libraries(darc_py-pybindings ${Boost_LIBRARIES} ${catkin_LIBRARIES})
set_target_properties(darc_py-pybindings
  PROPERTIES
  OUTPUT_NAME darc_py
  PREFIX ""
  LIBRARY_OUTPUT_DIRECTORY ${CATKIN_DEVEL_PREFIX}/${PYTHON_INSTALL_DIR}
  )

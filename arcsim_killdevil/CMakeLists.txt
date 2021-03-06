PROJECT(arcsim)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "-omp -g  ${CMAKE_CXX_FLAGS}")
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# BOOST
SET(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE(Boost QUIET REQUIRED filesystem system timer thread)
IF(Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
  MESSAGE(STATUS "Found Boost @ ${Boost_INCLUDE_DIR}")
ELSE(Boost_FOUND)
  MESSAGE(SEND_ERROR "Cannot find boost")
ENDIF(Boost_FOUND)

# GLEW
#FIND_PACKAGE(GLEW QUIET REQUIRED)
#IF(GLEW_FOUND)
#  INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})
#  MESSAGE(STATUS "Found GLEW @ ${GLEW_INCLUDE_DIR}")
#ELSE(GLEW_FOUND)
#  MESSAGE(SEND_ERROR "Cannot find GLEW")
#ENDIF(GLEW_FOUND)

# glut
#FIND_PACKAGE(GLUT QUIET REQUIRED)
#IF(GLUT_FOUND)
#  INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})
#  MESSAGE(STATUS "Found GLUT @ ${GLUT_INCLUDE_DIR}")
#ELSE(GLUT_FOUND)
#  MESSAGE(SEND_ERROR "Cannot find GLUT")
#ENDIF(GLUT_FOUND)

# glu
#FIND_PACKAGE(OpenGL QUIET REQUIRED)
#IF(OPENGL_FOUND)
#  INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
#  MESSAGE(STATUS "Found OPENGL @ ${OPENGL_INCLUDE_DIR}")
#ELSE(OPENGL_FOUND)
#  MESSAGE(SEND_ERROR "Cannot find OPENGL")
#ENDIF(OPENGL_FOUND)

# png
#FIND_PACKAGE(PNG QUIET REQUIRED)
#IF(PNG_FOUND)
#    ADD_DEFINITIONS(${PNG_DEFINITIONS})
#    MESSAGE(STATUS "Found PNG @ ${PNG_INCLUDE_DIR}")
#ELSE(PNG_FOUND)
#    MESSAGE(SEND_ERROR "Cannot find PNG")
#ENDIF(PNG_FOUND)

# z
#FIND_PACKAGE(ZLIB QUIET REQUIRED)
#IF(ZLIB_FOUND)
#    INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})
#    MESSAGE(STATUS "Found ZLIB @ ${ZLIB_INCLUDE_DIR}")
#ELSE(ZLIB_FOUND)
#    MESSAGE(SEND_ERROR "Cannot find ZLIB")
#ENDIF(ZLIB_FOUND)

#lapack
FIND_PACKAGE(LAPACK QUIET REQUIRED)
IF(LAPACK_FOUND)
    MESSAGE(STATUS "Found LAPACK ")
ELSE(LAPACK_FOUND)
    MESSAGE(SEND_ERROR "Cannot find LAPACK")
ENDIF(LAPACK_FOUND)

# cblas
FIND_PACKAGE(CBLAS QUIET REQUIRED)
IF(CBLAS_FOUND)
    INCLUDE_DIRECTORIES(${CBLAS_INCLUDE_DIR})
    MESSAGE(STATUS "Found CBLAS ")
ELSE(CBLAS_FOUND)
    MESSAGE(SEND_ERROR "Cannot find CBLAS")
ENDIF(CBLAS_FOUND)



LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/dependencies/lib)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/dependencies/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/pcgsolver/)

FILE(GLOB_RECURSE arcsim_SRC  adaptiveCloth/*.cc  adaptiveCloth/*.c
adaptiveCloth/*.cpp dependencies/jsoncpp/src/lib_json/*.cpp)
FILE(GLOB_RECURSE arcsim_HEADER  adaptiveCloth/*.hpp adaptiveCloth/*.h
pcgsolver/*.h dependencies/include/json/*.h)

list(REMOVE_ITEM arcsim_SRC ${CMAKE_SOURCE_DIR}/adaptiveCloth/main.cpp)
list(REMOVE_ITEM arcsim_SRC ${CMAKE_SOURCE_DIR}/adaptiveCloth/display.cpp)
list(REMOVE_ITEM arcsim_SRC ${CMAKE_SOURCE_DIR}/adaptiveCloth/displayphysics.cpp)
list(REMOVE_ITEM arcsim_SRC ${CMAKE_SOURCE_DIR}/adaptiveCloth/displayreplay.cpp)
list(REMOVE_ITEM arcsim_SRC ${CMAKE_SOURCE_DIR}/adaptiveCloth/displaytesting.cpp)

ADD_LIBRARY(arcsim ${arcsim_SRC} ${arcsim_HEADER})
ADD_EXECUTABLE(arcsimEXEC ${CMAKE_SOURCE_DIR}/adaptiveCloth/main.cpp )
TARGET_LINK_LIBRARIES(arcsimEXEC arcsim ${CBLAS_LIBRARIES} ${LAPACK_LIBRARIES} ${Boost_LIBRARIES} gomp alglib )

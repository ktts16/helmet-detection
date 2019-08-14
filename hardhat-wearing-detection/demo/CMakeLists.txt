cmake_minimum_required(VERSION 2.8.9)
project (TestLibrary)

#For the shared library:
# set ( PROJECT_LINK_LIBS libtestStudent.so )
# link_directories( ~/exploringBB/extras/cmake/studentlib_shared/build )
set ( PROJECT_LINK_LIBS libcaffe.so )
link_directories( ~/github/mini-caffe/build )

#For the static library:
#set ( PROJECT_LINK_LIBS libtestStudent.a )
#link_directories( ~/exploringBB/extras/cmake/studentlib_static/build )

# include_directories(~/exploringBB/extras/cmake/studentlib_shared/include)
include_directories(~/github/mini-caffe/include)

find_package( OpenCV REQUIRED )

# add_executable(libtest libtest.cpp)
# target_link_libraries(libtest ${PROJECT_LINK_LIBS} )
add_executable(main main.cpp)
target_link_libraries(main ${PROJECT_LINK_LIBS} )
target_link_libraries(main ${OpenCV_LIBS} )
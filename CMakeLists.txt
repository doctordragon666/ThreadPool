cmake_minimum_required(VERSION 3.22)
project(ThreadPool)
# 获取指定目录下的所有内容，并将获取到的内容保存到变量 ALL_ITEMS 
add_executable(ThreadPool main.cpp ThreadCond.cpp ThreadMutex.cpp ThreadPool.cpp)
target_link_libraries(ThreadPool pthread)
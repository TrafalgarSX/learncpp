cmake_minimum_required(VERSION 3.18) 

project(learncpp VERSION 1.0.0)

message("CMAKE_C_COMPILER=${CMAKE_C_COMPILER}")
message("CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")

#version standard
Set(CMAKE_C_STANDARD 11)
Set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#print some message  eg. variable 
message(CMAKE_CXX_STANDARD="${CMAKE_CXX_STANDARD}")

set(MY_DEV_LIB_PATH "D:/workspace/dev")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(ARCH_TYPE "${CMAKE_SYSTEM_PROCESSOR}")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(ARCH_TYPE "${CMAKE_SYSTEM_PROCESSOR}")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
  set(ARCH_TYPE "${CMAKE_ANDROID_ARCH_ABI}")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  message("64bit")
  set(BUILD_BIT "x64")
else()
  # 暂时用不到32位
  message("32bit")
  set(BUILD_BIT "x86")
endif()


#static lib output path  windows: .a .lib   unix: .a
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${ARCH_TYPE}/$<IF:$<CONFIG:Debug>,Debug,Release>/lib")
# 目标链接库的存放位置 unix: .so
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${ARCH_TYPE}/$<IF:$<CONFIG:Debug>,Debug,Release>/lib")
# windows: .exe .dll   unix: 可执行文件
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${ARCH_TYPE}/$<IF:$<CONFIG:Debug>,Debug,Release>/bin")

# 通用宏定义
list(APPEND EXTRA_DEFINES)
# 编译选项 目前只支持gcc和clang, msvc用CMAKE_CXX_FLAGS
list(APPEND EXTRA_COMPILE_FLAGS)
# 依赖的库的路径
list(APPEND EXTRA_LIB_PATH)
# 依赖的库
list(APPEND EXTRA_LIB)

list(APPEND EXTRA_INCLUDE  
                           "${PROJECT_SOURCE_DIR}/include" # 本工程的头文件
                           "${MY_DEV_LIB_PATH}/include" # 第三方库的头文件
                           )

list(APPEND EXTRA_SRC "${PROJECT_SOURCE_DIR}/socket_server.c")

#link lib 后面应该完善，操作系统和编译器组合判断
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  message("windows")
# 根据编译器的不同，设置不同的编译选项和依赖库
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message("mingw")
    # 方便生成compile_commands.json
    list(APPEND EXTRA_INCLUDE "D:/workspace/dev/vcpkg/installed/${BUILD_BIT}-windows/include")
    list(APPEND EXTRA_LIB_PATH "${MY_DEV_LIB_PATH}/unixlib")
    list(APPEND EXTRA_COMPILE_FLAGS "-Wall")

    list(APPEND EXTRA_LIB gtest pthread)

  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message("clang")

  elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message("msvc")
    # set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "D:/workspace/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_PREFIX_PATH "D:/workspace/dev/vcpkg/installed/${BUILD_BIT}-windows")
    set(VCPKG_FEATURE_FLAGS "versions")

    # 第三方库的头文件（vcpkg安装的）
    list(APPEND EXTRA_INCLUDE "${CMAKE_PREFIX_PATH}/include")
    # 第三方库的路径（vcpkg安装的）
    # list(APPEND EXTRA_LIB_PATH "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin")
    # 区分debug和release, 多配置编译器不能直接使用CMAKE_BUILD_TYPE
    list(APPEND EXTRA_LIB_PATH "${CMAKE_PREFIX_PATH}-static/$<$<CONFIG:Debug>:debug/>lib")
    list(APPEND EXTRA_LIB_PATH "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>lib")
    # 自己编译的库的路径(msvc编译器)
    list(APPEND EXTRA_LIB_PATH "${MY_DEV_LIB_PATH}/msvclib")

    # list(APPEND EXTRA_DEFINES "_LOG_")
    # list(APPEND EXTRA_COMPILE_FLAGS "$<IF:$<CONFIG:Debug>,MTd,MT>")
    list(APPEND EXTRA_COMPILE_FLAGS "/MD$<$<CONFIG:Debug>:d>")

    # 动态库
    # find_package(CURL CONFIG REQUIRED)

    find_package(ZLIB REQUIRED)
    find_package(gflags REQUIRED)
    find_package(glog CONFIG REQUIRED)
    find_package(libuv CONFIG REQUIRED)
    find_package(fmt CONFIG REQUIRED)

    list(APPEND EXTRA_LIB fmt::fmt)
  endif()

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  message("unix")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
  list(APPEND EXTRA_LIB_PATH "${MY_DEV_LIB_PATH}/android-lib/${ANDROID_ABI}")
  list(APPEND EXTRA_LIB curl)
endif()
message("EXTRA_COMPILE_FLAGS: ${EXTRA_COMPILE_FLAGS}")
message("EXTRA_SRC: ${EXTRA_SRC}")

add_library(common INTERFACE)

# target_link_options(${PROJECT_NAME} PUBLIC ${EXTRA_LINK_FLAGS})
# 编译选项
target_compile_options(common INTERFACE ${EXTRA_COMPILE_FLAGS})
# 宏定义
target_compile_definitions(common INTERFACE ${EXTRA_DEFINES})
# 工程下的头文件的路径
target_include_directories(common INTERFACE ${EXTRA_INCLUDE})
# 第三方库的路径
target_link_directories(common INTERFACE ${EXTRA_LIB_PATH})
# 第三方库
target_link_libraries(common INTERFACE ${EXTRA_LIB})

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE common)
# 源文件
target_sources(${PROJECT_NAME} PUBLIC main.cpp)


add_executable(usefmt usefmt.cpp)

target_link_libraries(usefmt PRIVATE common)

add_executable(testtiming testtiming.cpp)

target_link_libraries(testtiming PRIVATE common)

add_executable(structureBinding structureBinding.cpp)

target_link_libraries(structureBinding PRIVATE common)

add_executable(testoptional testOptional.cpp)

target_link_libraries(testoptional PRIVATE common)

add_executable(testvariant testVariant.cpp)

target_link_libraries(testvariant PRIVATE common)

add_executable(testasync testasync.cpp)

target_link_libraries(testasync PRIVATE common)

add_executable(teststringview teststringview.cpp)

target_link_libraries(teststringview PRIVATE common)

add_executable(testArg testArg.cpp)

add_executable(newArray newArray.cpp)

add_executable(testIter testIter.cpp)

add_executable(testEmplace testEmplace.cpp)

add_executable(testdefault testdefault.cpp)

add_executable(testfstream testfstream.cpp)

add_executable(sm9TestData sm9TestData.cpp hexUtil.cpp)
target_link_libraries(sm9TestData PRIVATE common)

add_executable(testRWLock testrwlock.cpp ReadWriteLock.c)
target_link_libraries(testRWLock PRIVATE common)

add_executable(testShareMemory testsharememory.cpp)
target_link_libraries(testShareMemory PRIVATE common)

add_executable(testShareMemoryAccept acceptmemoryinfo.cpp)
target_link_libraries(testShareMemoryAccept PRIVATE common)

add_executable(testSearchVector searchTime.cpp)
target_link_libraries(testSearchVector PRIVATE common)

add_executable(stdAlgorithms stdAlgorithms.cpp)
target_link_libraries(stdAlgorithms PRIVATE common)

add_executable(testThreadLocal testThreadLocal.cpp)
target_link_libraries(testThreadLocal PRIVATE common)

add_executable(testConst testConst.cpp)
target_link_libraries(testConst PRIVATE common)

add_executable(testSwap testSwap.cpp)
target_link_libraries(testSwap PRIVATE common)

add_executable(testClassScope testClassScope.cpp)
target_link_libraries(testClassScope PRIVATE common)

add_executable(testTryCatch testTryCatch.cpp)
target_link_libraries(testTryCatch PRIVATE common)

add_executable(testUsing testUsing.cpp)
target_link_libraries(testUsing PRIVATE common)

add_executable(testClass testClass.cpp)
target_link_libraries(testClass PRIVATE common)

add_executable(testMultiClass testMultiClass.cpp)
target_link_libraries(testMultiClass PRIVATE common)

add_executable(testUnion testUnion.cpp)
target_link_libraries(testUnion PRIVATE common)

add_executable(testMemberPointer testMemberPointer.cpp)
target_link_libraries(testMemberPointer PRIVATE common)

add_executable(testRTTI testRTTI.cpp)
target_link_libraries(testRTTI PRIVATE common)

add_executable(testNew testNew.cpp)
target_link_libraries(testNew PRIVATE common)

add_executable(testTemplate testTemplate.cpp)
target_link_libraries(testTemplate PRIVATE common)

add_executable(order order.cpp)
target_link_libraries(order PRIVATE common)

add_executable(testExecCodePage testExecCodePage.cpp)
target_link_libraries(testExecCodePage PRIVATE common)

add_executable(testRLvalue testRLvalue.cpp)
target_link_libraries(testRLvalue PRIVATE common)

add_executable(testRemoveRf testRemoveRf.cpp)
target_link_libraries(testRemoveRf PRIVATE common)

add_executable(testFilesystem testFilesystem.cpp)
target_link_libraries(testFilesystem PRIVATE common)

add_executable(testInitialize testInitialize.cpp)
target_link_libraries(testInitialize PRIVATE common)

add_executable(testVirtual testVirtual.cpp)
target_link_libraries(testVirtual PRIVATE common)


# copy need multi dll to exe dir 
add_custom_command(TARGET usefmt POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/fmt$<$<CONFIG:Debug>:d>.dll"
    $<TARGET_FILE_DIR:usefmt>)

if(1)
  message("test")
endif()

include(CTest)

# add_test(NAME first COMMAND gtest_server)
# add_test(NAME second COMMAND gtest_client)


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

list(APPEND EXTRA_SRC)

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
    list(APPEND EXTRA_COMPILE_FLAGS "/MD$<$<CONFIG:Debug>:d>"  "/utf-8")

    # 动态库
    # find_package(CURL CONFIG REQUIRED)

    find_package(ZLIB REQUIRED)
    find_package(gflags REQUIRED)
    find_package(glog CONFIG REQUIRED)
    find_package(libuv CONFIG REQUIRED)
    find_package(fmt CONFIG REQUIRED)
    find_package(jsoncpp CONFIG REQUIRED)
    find_package(nlohmann_json CONFIG REQUIRED)
    find_package(OpenSSL REQUIRED)
    find_package(asio CONFIG REQUIRED)
    # vcpkg  Qt5Cored.lib
    find_package(Qt5 COMPONENTS Core REQUIRED)
    # vcpkg  Qt5Network.lib
    find_package(Qt5 COMPONENTS Network REQUIRED)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    find_package(Python COMPONENTS Interpreter Development)
    find_package(spdlog CONFIG REQUIRED)
    list(APPEND EXTRA_LIB fmt::fmt)
    list(APPEND EXTRA_LIB spdlog::spdlog)
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

list(APPEND _targets "usefmt" "testtiming" "structureBinding" "testoptional" 
    "testvariant" "testasync" "teststringview" "testArg" "newArray" "testIter" 
    "testEmplace" "testdefault" "testfstream" "testRWLock" 
    "testShareMemory" 
    "stdAlgorithms" "testThreadLocal" "testConst" "testSwap" 
    "testClassScope" "testTryCatch" "testUsing" "testClass" "testMultiClass"
    "testUnion" "testMemberPointer" "testRTTI" "testNew" "testTemplate" 
    "order" "testExecCodePage" "testRLvalue" "testRemoveRf" "testFilesystem"
    "testInitialize" "testVirtual" "testTypeTraits" "testCryptFile"
    "testNumber" "testPromise" "testFuncObj" "testThread" "testPackage_task"
    "cast_check" 
    )

foreach(_target IN LISTS _targets)

    add_executable(${_target})
    target_link_libraries(${_target} PRIVATE common)
    target_sources(${_target} PUBLIC "${_target}.cpp")

endforeach()

add_executable(testQtBase)
target_link_libraries(testQtBase PRIVATE common Qt5::Core Qt5::Network)
target_sources(testQtBase PUBLIC "testQtBase.cpp")

add_executable(testAsio)
target_link_libraries(testAsio PRIVATE common asio::asio)
target_sources(testAsio PUBLIC "testAsio.cpp")

add_executable(asioClient)
target_link_libraries(asioClient PRIVATE common asio::asio)
target_sources(asioClient PUBLIC "asioClient.cpp" "hexUtil.cpp")

add_executable(asioServer)
target_link_libraries(asioServer PRIVATE common asio::asio)
target_sources(asioServer PUBLIC "asioServer.cpp"  "hexUtil.cpp")


add_executable(testSpdlog)
target_link_libraries(testSpdlog PRIVATE common spdlog::spdlog)
target_compile_definitions(testSpdlog PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
target_sources(testSpdlog PUBLIC "testSpdlog.cpp" )

add_executable(testSpdlogPrint)
target_link_libraries(testSpdlogPrint PRIVATE common spdlog::spdlog)
target_sources(testSpdlogPrint PUBLIC "testSpdlogPrint.cpp" )

add_executable(testNlohmanJson)
target_link_libraries(testNlohmanJson PRIVATE common nlohmann_json::nlohmann_json fmt::fmt)
target_sources(testNlohmanJson PUBLIC "testNlohmannJson.cpp" )

get_target_property(TESTSPDLOG_DEFINITIONS testSpdlogPrint COMPILE_DEFINITIONS)

message(STATUS "testSpdlog compile definitions: ${TESTSPDLOG_DEFINITIONS}")

# add_executable(pybindTest)
# target_link_libraries(pybindTest PUBLIC )
# target_sources(pytbindTest PUBLIC pybindTest.cpp)


# copy need multi dll to exe dir 
add_custom_command(TARGET usefmt POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/fmt$<$<CONFIG:Debug>:d>.dll"
    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/jsoncpp.dll"
    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/libssl-3-x64.dll"

    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/Qt5Core$<$<CONFIG:Debug>:d>.dll"
    # QtCore dependencies
    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/double-conversion.dll"  
    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/zlib$<$<CONFIG:Debug>:d>1.dll"
    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/pcre2-16$<$<CONFIG:Debug>:d>.dll"
    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/zst$<$<CONFIG:Debug>:d>.dll"
    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/Qt5Network$<$<CONFIG:Debug>:d>.dll"

    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/spdlog$<$<CONFIG:Debug>:d>.dll"
    $<TARGET_FILE_DIR:usefmt>)

if(1)
  message("test")
endif()

include(CTest)

# add_test(NAME first COMMAND gtest_server)
# add_test(NAME second COMMAND gtest_client)


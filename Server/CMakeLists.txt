cmake_minimum_required(VERSION 3.12)
project(Server)

FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(X11 REQUIRED)
FIND_PACKAGE(PkgConfig REQUIRED)
FIND_PACKAGE(Threads REQUIRED)
FIND_PACKAGE(OpenSSL REQUIRED)
PKG_CHECK_MODULES(JSON REQUIRED jsoncpp)

INCLUDE_DIRECTORIES(${JSON_INCLUDE_DIRS} ${X11_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIRS})
LINK_DIRECTORIES(${JSON_LIBRARY_DIRS} ${X11_LIBRARY_DIRS} ${OpenCV_LIBRARY_DIRS} ${OPENSSL_LIBRARY_DIRS})
ADD_DEFINITIONS(${JSON_CFLAGS_OTHER} ${X11_CFLAGS_OTHER} ${OpenCV_CFLAGS_OTHER} ${OPENSSL_CFLAGS_OTHER})

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp
        src/Request/Request.h src/Request/Request.cpp
        src/Request/FileRequest.h src/Request/FileRequest.cpp
        src/Request/MouseEventRequest.h src/Request/MouseEventRequest.cpp
        src/Request/ErrorRequest.h src/Request/ErrorRequest.cpp
        src/Request/HandInRequest.h src/Request/HandInRequest.cpp
        src/Request/HandUpRequest.h src/Request/HandUpRequest.cpp
        src/Request/OKRequest.h src/Request/OKRequest.cpp
        src/Request/LoginRequest.h src/Request/LoginRequest.cpp
        src/Request/QuitRequest.h src/Request/QuitRequest.cpp
        src/TransmissionControlProtocolSerial.h src/TransmissionControlProtocolSerial.cpp
        src/Setting.h src/Setting.cpp
        src/Client.h src/Client.cpp
        src/ClientManager.h src/ClientManager.cpp
        src/RequestHandler.cpp src/RequestHandler.h)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${JSON_LIBRARIES}
        ${X11_LIBRARIES}
        ${OpenCV_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT})
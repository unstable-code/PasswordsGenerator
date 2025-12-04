cmake_minimum_required(VERSION 3.16)
project(PasswordGenerator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt6 패키지 찾기
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# 소스 파일
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/passwordgenerator.cpp
    src/logger.cpp
)

# 헤더 파일
set(HEADERS
    include/mainwindow.h
    include/passwordgenerator.h
    include/logger.h
)

# 실행 파일 생성
if(APPLE)
    # macOS 앱 번들
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE
        ${SOURCES}
        ${HEADERS}
    )
elseif(WIN32)
    # Windows GUI 앱
    add_executable(${PROJECT_NAME} WIN32
        ${SOURCES}
        ${HEADERS}
    )
else()
    # Linux
    add_executable(${PROJECT_NAME}
        ${SOURCES}
        ${HEADERS}
    )
endif()

# 인클루드 디렉토리
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Qt6 라이브러리 링크
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::Gui
    Qt6::Core
)

# 릴리스 최적화
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(UNIX)
        target_link_options(${PROJECT_NAME} PRIVATE -s)  # Strip symbols
    endif()
endif()

# 설치
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

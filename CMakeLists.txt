make_minimum_required(VERSION 3.16.3)

project(wordgen)

add_executable(${PROJECT_NAME} src/main.cpp)


add_subdirectory("src")

target_link_directories(${PROJECT_NAME} PRIVATE
                            "conf"
                            "src"
)

target_include_directories(${PROJECT_NAME} PUBLIC
                            "conf"
                            "src"
)

target_link_libraries(${PROJECT_NAME} src)
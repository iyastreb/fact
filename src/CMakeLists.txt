add_executable(fact fact.cpp)

add_executable(encoder encoder.cpp)

add_custom_target(run
    COMMAND encoder
    DEPENDS encoder
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

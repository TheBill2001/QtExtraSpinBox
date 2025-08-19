qt_add_executable(${PROJECT_NAME}ExampleApp
    main.cpp
)

target_link_libraries(${PROJECT_NAME}ExampleApp PRIVATE Qt6::Widgets)

if(${PROJECT_NAME}_BUILD_SHARED)
    target_link_libraries(${PROJECT_NAME}ExampleApp PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})
elseif(${PROJECT_NAME}_BUILD_STATIC)
    target_link_libraries(${PROJECT_NAME}ExampleApp PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}Static)
else()
    message(FATAL_ERROR "${PROJECT_NAME}ExampleApp: No ${PROJECT_NAME} target found!")
endif()

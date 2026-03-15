qt_import_plugins(main_gui INCLUDE Qt::QWaylandIntegrationPlugin)
target_link_libraries(main_gui PRIVATE game_lib Qt6::Core Qt6::Gui Qt6::Widgets Qt6::WaylandClient)

target_sources(main_gui PRIVATE
        main.cxx
        MainWindow.h
)
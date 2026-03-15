target_link_libraries(main_console PRIVATE game_lib Qt6::Core)

target_sources(main_console PRIVATE
        main.cxx
)

cmake_minimum_required(VERSION 3.15...4.2)

set( INC_FILES
    inc/graphics/Window.hpp
)

set( SRC_FILES 
    src/Window.cpp
)

set( IMGUI_INC_FILES
    ../externals/imgui/imconfig.h
    ../externals/imgui/imgui.h
    ../externals/imgui/imgui_internal.h
    ../externals/imgui/misc/freetype/imgui_freetype.h
)

set( IMGUI_SRC_FILES
    ../externals/imgui/imgui.cpp
    ../externals/imgui/imgui_demo.cpp
    ../externals/imgui/imgui_draw.cpp
    ../externals/imgui/imgui_tables.cpp
    ../externals/imgui/imgui_widgets.cpp
    ../externals/imgui/misc/freetype/imgui_freetype.cpp    
)

set( IMGUI_BACKEND_FILES
    ../externals/imgui/backends/imgui_impl_sdl3.h
    ../externals/imgui/backends/imgui_impl_sdl3.cpp
    ../externals/imgui/backends/imgui_impl_sdlrenderer3.h
    ../externals/imgui/backends/imgui_impl_sdlrenderer3.cpp
)

set( IMGUI_FILES ${IMGUI_INC_FILES} ${IMGUI_SRC_FILES} ${IMGUI_BACKEND_FILES} )

set( ALL_FILES
    ${INC_FILES}
    ${SRC_FILES}
    ${IMGUI_FILES}
    ../.clang-format
)

source_group(imgui FILES ${IMGUI_SRC_FILES} ${IMGUI_INC_FILES} )
source_group(imgui/backend FILES ${IMGUI_BACKEND_FILES} )

add_library( graphics STATIC ${ALL_FILES} )
add_library( rast::graphics ALIAS graphics )
target_compile_features(graphics PUBLIC cxx_std_23)
target_compile_definitions(graphics PRIVATE IMGUI_ENABLE_FREETYPE)
target_include_directories(graphics PUBLIC inc ../externals/imgui ../externals/imgui/backends)
target_link_libraries(graphics PUBLIC Freetype::Freetype SDL3::SDL3)

# Warning level 4 and treat warnings as errors.
target_compile_options(graphics
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra -Wpedantic -Werror>
)

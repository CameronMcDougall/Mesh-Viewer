
add_library(imgui
  imconfig.h
  imgui.h
  imgui.cpp
  imgui_draw.cpp
  imgui_internal.h
  stb_textedit.h)

target_include_directories(imgui PUBLIC "${PROJECT_SOURCE_DIR}/vendor/imgui")
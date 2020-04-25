@echo Project dependencies will be installed with vcpkg.

cd vcpkg

@echo Installing vcpkg...

call bootstrap-vcpkg.bat

@echo Installing dependencies...

vcpkg.exe install docopt:x64-windows glad:x64-windows glfw3:x64-windows glm:x64-windows ^
nlohmann-json:x64-windows opengl:x64-windows spdlog:x64-windows stb:x64-windows

@echo Done.

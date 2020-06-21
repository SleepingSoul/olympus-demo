@echo Project dependencies will be installed with vcpkg.

@echo Installing vcpkg...

call %~dp0vcpkg\bootstrap-vcpkg.bat

@echo Installing dependencies...

%~dp0vcpkg\vcpkg.exe install docopt:x64-windows glad:x64-windows glfw3:x64-windows glm:x64-windows ^
nlohmann-json:x64-windows opengl:x64-windows spdlog:x64-windows stb:x64-windows boost-circular-buffer:x64-windows ^
curl:x64-windows opencv:x64-windows

@echo Done.

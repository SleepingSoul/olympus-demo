@echo Project dependencies will be installed, but only for release configuration. Use this scipt only on buldmachine.

@echo Installing vcpkg...

call %~dp0..\vcpkg\bootstrap-vcpkg.bat

@echo Fixing triplets...

copy /y %~dp0x64-windows-donly.cmake %~dp0..\vcpkg\triplets\x64-windows-d.cmake

@echo Installing debug only libs...

%~dp0..\vcpkg\vcpkg.exe install docopt:x64-windows-d glad:x64-windows-d glfw3:x64-windows glm:x64-windows ^
nlohmann-json:x64-windows opengl:x64-windows spdlog:x64-windows stb:x64-windows boost-circular-buffer:x64-windows-d ^
curl:x64-windows-d opencv:x64-windows-d

@echo Done.

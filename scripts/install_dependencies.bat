@echo Project dependencies will be installed with vcpkg.
@echo off

set SCRIPTS_PATH=%cd%

cd ..\vcpkg


@echo on
@echo Installing vcpkg...
@echo off

call bootstrap-vcpkg.bat

@echo on
@echo Done.
@echo Installing dependencies...
@echo off

vcpkg.exe install opengl:x64-windows
vcpkg.exe install glfw3:x64-windows
vcpkg.exe install glm:x64-windows
vcpkg.exe install glad:x64-windows
vcpkg.exe install stb:x64-windows
vcpkg.exe install spdlog:x64-windows
vcpkg.exe install nlohmann-json:x64-windows
vcpkg.exe install imgui:x64-windows

@echo on
@echo Done.
@echo Creating a package...
@echo off

vcpkg.exe integrate project

cd scripts\buildsystems\tmp

del *.nuspec

@echo on
copy %SCRIPTS_PATH%\vcpkg.nuget.nuspec %cd%
@echo off

cd ..

del *.nupkg

cd ..\..\downloads\tools\nuget-4.6.2-windows

nuget.exe pack ..\..\..\scripts\buildsystems\tmp\vcpkg.nuget.nuspec

move olympus_dependencies*.nupkg ..\..\..\scripts\buildsystems

cd ..\..\..\scripts\buildsystems

@echo on
@echo -------------------------------------------------------------------------------------------
@echo -------------------------------------------------------------------------------------------
@echo -- Now, please, go to NuGet console of your 'olympus' -------------------------------------
@echo -- project and execute the following script: ----------------------------------------------
@echo -- Install-Package olympus_dependencies -Source "%cd%"
@echo -------------------------------------------------------------------------------------------

cd %SCRIPTS_PATH%

@echo on
@echo Done.

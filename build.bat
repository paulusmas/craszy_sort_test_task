call "%VS90COMNTOOLS%vsvars32.bat"

devenv Solution.sln /Build "Debug|Win32"
devenv Solution.sln /Build "Release|Win32"

::devenv Solution.sln /Build "Debug|x64"
::devenv Solution.sln /Build "Release|x64"
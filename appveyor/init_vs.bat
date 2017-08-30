if "%VC_VER%" == "vc14" (
	echo "Configuring for Visual Studio 2015"
	"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64\vcvars64.bat"
) else if "%VC_VER%" == "vc15" (
	echo "Configuring for Visual Studio 2017"
	"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" %ARCH%
) else (
	echo "Unknown Visual Studio version, aborting"
	exit /b 1
)
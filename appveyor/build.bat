if "%VC_VER%" == "vc14" (
	"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64\vcvars64.bat"
) else if "%VC_VER%" == "vc15" (
	"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" %ARCH%
) else (
	echo "Unknown Visual Studio version, aborting"
	exit 1
)

wget https://github.com/OSTC/php-sdk-binary-tools/archive/%PHP_SDK_BINARY_TOOLS_VER%.zip --no-check-certificate -q -O php-sdk-binary-tools-%PHP_SDK_BINARY_TOOLS_VER%.zip

7z x -y php-sdk-binary-tools-%PHP_SDK_BINARY_TOOLS_VER%.zip -oC:\projects

move C:\projects\php-sdk-binary-tools-%PHP_SDK_BINARY_TOOLS_VER% C:\projects\php-sdk

C:\projects\php-sdk\bin\phpsdk_setvars.bat

git clone https://github.com/php/php-src C:\projects\php-src -b PHP-%PHP_VER% --depth=1

mkdir C:\projects\php-src\ext\pocketmine_chunkutils

xcopy C:\projects\pocketmine_chunkutils C:\projects\php-src\ext\pocketmine_chunkutils /s /e /y /q

phpsdk_deps -u -t %VC_VER% -b %PHP_VER% -a %ARCH% -f -d C:\projects\php-src\deps

cd C:\projects\php-src

buildconf.bat

configure.bat --disable-all --enable-cli --enable-zts --enable-pocketmine-chunkutils=shared --with-config-file-scan-dir=C:\projects\pocketmine_chunkutils\bin\modules.d --with-prefix=C:\projects\pocketmine_chunkutils\bin --with-php-build=deps --enable-debug-pack

nmake

nmake install

copy php.ini-development C:\projects\pocketmine_chunkutils\bin\php.ini

copy C:\projects\php-src\x64\Release_TS\php_pocketmine_chunkutils.pdb C:\projects\pocketmine_chunkutils\bin\ext\php_pocketmine_chunkutils.pdb

mkdir C:\projects\pocketmine_chunkutils\bin\modules.d

echo extension=php_pocketmine_chunkutils.dll > C:\projects\pocketmine_chunkutils\bin\modules.d\php.ini
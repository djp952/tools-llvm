@ECHO OFF
SETLOCAL

SET CONFIGURATION=%1
SET PLATFORM=%2

IF "%CONFIGURATION%"=="" GOTO ARGUMENT_ERROR
IF "%PLATFORM%"=="" GOTO ARGUMENT_ERROR

msbuild tools-llvm.sln /t:Rebuild /p:Configuration=%CONFIGURATION% /p:Platform=%PLATFORM%
SET BUILD_STATUS=%ERRORLEVEL%
IF NOT %BUILD_STATUS%==0 GOTO BUILD_FAILURE

..\external-opencover\tools\OpenCover.Console.exe -register:user -target:"%VS140COMNTOOLS%..\IDE\CommonExtensions\Microsoft\TestWindow\vstest.console.exe" -targetargs:"/Settings:default.runsettings /Logger:trx /inIsolation out\%PLATFORM%\%CONFIGURATION%\zuki.tools.llvm.clang.test.dll" -filter:"+[zuki.tools.llvm.clang]*" -mergebyhash -output:"out\%PLATFORM%\%CONFIGURATION%\codecoverage.xml"
..\external-reportgenerator\tools\ReportGenerator.exe -reports:"out\%PLATFORM%\%CONFIGURATION%\codecoverage.xml" -targetdir:"out\%PLATFORM%\%CONFIGURATION%\codecoverage"
GOTO EXIT

:ARGUMENT_ERROR
ECHO Usage: codecoverage.bat CONFIGURATION PLATFORM
GOTO EXIT

:BUILD_FAILURE
ECHO %CONFIGURATION% %PLATFORM% build failed
GOTO EXIT

:exit

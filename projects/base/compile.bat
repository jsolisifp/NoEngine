if not exist "../../libraries/noengine/lib/NoEngine.lib" (
	echo ERROR: Compile NoEngine library first
) else (
	cd ../../projects/base
	cl /I "../../libraries/noengine/include" src/*.cpp lib/*.lib ../../libraries/noengine/lib/*.lib /Fo:obj/ /Fe:bin/main.exe
)

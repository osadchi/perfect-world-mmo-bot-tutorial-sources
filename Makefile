g++ := i686-w64-mingw32-g++
etc := -std=c++17 -pthread -lws2_32 -lgdi32 `C:/msys64/mingw32/bin/wx-config --cxxflags` `C:/msys64/mingw32/bin/wx-config --libs std,stc`
o := -O3 -g
build := build
all: $(build)/pwbot_gui.exe $(build)/pwbot_lib.dll

$(build)/pwbot_gui.exe: pwbot_gui.cpp pwbot_config.h pwbot.h
	$(g++) $(o) pwbot_gui.cpp -o $(build)/pwbot_gui.exe $(etc) -Wl,--subsystem,console

$(build)/pwbot_lib.dll: pwbot_lib.cpp pwbot.h pwbot_config.h
	$(g++) $(o) pwbot_lib.cpp -shared -o $(build)/pwbot_lib.dll $(etc)

clean:
	rm -rf $(build)/*.o $(build)/*.exe $(build)/*.dll
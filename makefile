
CXX = g++

# Update these paths to match your installation
# You may also need to update the linker option rpath, which sets where to look for
# the SDL2 libraries at runtime to match your install
SDL_LIB = -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer \
	-Wl,-rpath=/usr/local/lib -lpython2.7
SDL_INCLUDE = -I/usr/local/include

# choose one of those 
BUILD_FLAGS=-ggdb -g3
#BUILD_FLAGS= -O2 -DNDEBUG 


# You may need to change -std=c++11 to -std=c++0x if your compiler is a bit older
# run python2.7-config --cflags to get the python flags you need
CXXFLAGS = $(BUILD_FLAGS) \
	-Wall -Werror \
	-c -std=c++0x \
	$(SDL_INCLUDE) -I src -I/usr/include/python2.7 -I/usr/include/python2.7 \
	-fno-strict-aliasing \
	-fwrapv -fstack-protector \
	--param=ssp-buffer-size=4 -Wformat -Wformat-security 


LDFLAGS = $(SDL_LIB)
PROG = turkey


all: $(PROG)

$(PROG): main.o model.o view.o fps.o scripting.o position.o component.o \
	intro_component.o game_component.o sprite.o game_state.o game_obj.o utils.o
	$(CXX) $^ $(LDFLAGS) -o $@

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

model.o: src/model/model.cpp src/model/model.h src/model/position.h
	$(CXX) $(CXXFLAGS) $< -o $@

view.o: src/view/view.cpp src/view/view.h src/view/fps.h
	$(CXX) $(CXXFLAGS) $< -o $@

utils.o: src/view/utils.cpp src/view/utils.h
	$(CXX) $(CXXFLAGS) $< -o $@

fps.o: src/view/fps.cpp src/view/fps.h 
	$(CXX) $(CXXFLAGS) $< -o $@

scripting.o: src/shared/scripting.cpp src/shared/scripting.h
	$(CXX) $(CXXFLAGS) $< -o $@


position.o: src/model/position.cpp src/model/position.h
	$(CXX) $(CXXFLAGS) $< -o $@

sprite.o: src/view/sprite.cpp src/view/sprite.h
	$(CXX) $(CXXFLAGS) $< -o $@

component.o: src/view/component.cpp src/view/component.h 
	$(CXX) $(CXXFLAGS) $< -o $@

intro_component.o: src/view/intro_component/intro_component.cpp src/view/intro_component/intro_component.h src/view/component.h # src/control/control.h
	$(CXX) $(CXXFLAGS) $< -o $@

game_component.o: src/view/game_component/game_component.cpp src/view/game_component/game_component.h src/view/component.h # src/control/control.h
	$(CXX) $(CXXFLAGS) $< -o $@

game_state.o: src/model/game_state.cpp src/model/game_state.h
	$(CXX) $(CXXFLAGS) $< -o $@

game_obj.o: src/model/game_obj.cpp src/model/game_obj.h
	$(CXX) $(CXXFLAGS) $< -o $@


.PHONY: clean
clean:
	rm -f *.o && rm -f $(PROG)	
	find . -name '*~' -exec rm {} \; # clean up emacs temporary files
	find . -name '*.pyc' -exec rm {} \; # clean up compiled python scripts


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

$(PROG): main.o model.o view.o fps.o scripting.o base_view.o \
	intro_view.o game_view.o sprite.o game_state.o intro_state.o game_obj.o \
	utils.o font_manager.o collision.o camera.o collision_type.o particle_system.o \
	sound_manager.o image_manager.o
	$(CXX) $^ $(LDFLAGS) -o $@

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

model.o: src/model/model.cpp src/model/model.h # src/model/position.h
	$(CXX) $(CXXFLAGS) $< -o $@

view.o: src/view/view.cpp src/view/view.h src/view/fps.h
	$(CXX) $(CXXFLAGS) $< -o $@

font_manager.o: src/view/font_manager.cpp src/view/font_manager.h src/view/utils.h
	$(CXX) $(CXXFLAGS) $< -o $@

sound_manager.o: src/view/sound_manager.cpp src/view/sound_manager.h src/view/utils.h
	$(CXX) $(CXXFLAGS) $< -o $@

image_manager.o: src/view/image_manager.cpp src/view/image_manager.h src/view/utils.h
	$(CXX) $(CXXFLAGS) $< -o $@

utils.o: src/view/utils.cpp src/view/utils.h
	$(CXX) $(CXXFLAGS) $< -o $@

fps.o: src/view/fps.cpp src/view/fps.h 
	$(CXX) $(CXXFLAGS) $< -o $@

scripting.o: src/shared/scripting.cpp src/shared/scripting.h
	$(CXX) $(CXXFLAGS) $< -o $@


# position.o: src/model/position.cpp src/model/position.h
# 	$(CXX) $(CXXFLAGS) $< -o $@

collision.o: src/model/collision.cpp src/model/collision.h
	$(CXX) $(CXXFLAGS) $< -o $@

collision_type.o: src/model/collision_type.cpp src/model/collision_type.h
	$(CXX) $(CXXFLAGS) $< -o $@

sprite.o: src/view/sprite.cpp src/view/sprite.h
	$(CXX) $(CXXFLAGS) $< -o $@

base_view.o: src/view/base_view.cpp src/view/base_view.h 
	$(CXX) $(CXXFLAGS) $< -o $@

intro_view.o: \
	src/view/intro_view.cpp \
	src/view/intro_view.h \
	src/view/base_view.h 
	$(CXX) $(CXXFLAGS) $< -o $@

game_view.o: \
	src/view/game_view.cpp \
	src/view/game_view.h \
	src/view/base_view.h 
	$(CXX) $(CXXFLAGS) $< -o $@

game_state.o: \
	src/model/game_state.cpp \
	src/model/game_state.h 
	$(CXX) $(CXXFLAGS) $< -o $@

intro_state.o: \
	src/model/intro_state.cpp \
	src/model/intro_state.h 
	$(CXX) $(CXXFLAGS) $< -o $@

particle_system.o: \
	src/model/particle_system.cpp \
	src/model/particle_system.h 
	$(CXX) $(CXXFLAGS) $< -o $@

game_obj.o: \
	src/model/game_obj.cpp \
	src/model/game_obj.h
	$(CXX) $(CXXFLAGS) $< -o $@

camera.o: \
	src/model/camera.cpp \
	src/model/camera.h
	$(CXX) $(CXXFLAGS) $< -o $@


# generate TAGS file for emacs
TAGS:
	 find src -type f -iname "*.h" -or -iname "*.cpp" | etags -

.PHONY: clean
clean:
	rm -f *.o && rm -f $(PROG)	
	rm -f TAGS
	find . -name '*~' -exec rm {} \; # clean up emacs temporary files
	find . -name '*.pyc' -exec rm {} \; # clean up compiled python scripts

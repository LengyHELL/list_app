SRC_DIR := src
OBJ_DIR := obj
OBJ_DIR_WIN32 := obj_win32

EXE 			:= list_app
SRC 			:= $(wildcard $(SRC_DIR)/*.cpp)
OBJ 			:= $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJ_WIN32	:= $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR_WIN32)/%.o)

.PHONY: all win32 clean


all: CXX				:= g++-9
all: CPPFLAGS		:= -MMD -MP
all: CXXFLAGS		:= -std=c++17 -Wall -g
all: LDLIBS			:= -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

all: $(EXE).out
	@echo "Done."

$(EXE).out: $(OBJ)
	@echo "Linking ..."
	@$(CXX) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	 @echo "Compiling $@ ..."
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@


win32: CXX				:= i686-w64-mingw32-g++
win32: CPPFLAGS		:= -Idev/SDL2-2.0.12/i686-w64-mingw32/include -MMD -MP
win32: CXXFLAGS		:= -std=c++17 -Wall -g
win32: LDFLAGS		:= -Ldev/SDL2-2.0.12/i686-w64-mingw32/lib
win32: LDLIBS			:= -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

win32: $(EXE).exe
	@echo "Done."

$(EXE).exe: $(OBJ_WIN32)
	@echo "Linking ..."
	@$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR_WIN32)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR_WIN32)
	@echo "Compiling $@ ..."
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR_WIN32):
	mkdir -p $@



-include $(OBJ:.o=.d)



clean:
	$(RM) -rv $(OBJ_DIR)
	$(RM) -rv $(EXE).out
	$(RM) -rv $(OBJ_DIR_WIN32)
	$(RM) -rv $(EXE).exe

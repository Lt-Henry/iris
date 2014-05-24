COLOR_NONE=\x1b[0m
COLOR_COMPILE=\x1b[32;01m
COLOR_BUILD=\x1b[35;01m
COLOR_CLEAN=\x1b[31;01m

#application name
APP_NAME=iris

#compilation flags
COMPILE_FLAGS=-std=c++0x

#libraries
LIBS=

#objects
OBJECTS=main.o




$(APP_NAME): $(OBJECTS)
	@echo -e '$(COLOR_BUILD)* Building [$@]$(COLOR_NONE)'
	g++ -o $(APP_NAME) $(OBJECTS) $(LIBS)


%.o : %.cpp
	@echo -e '$(COLOR_COMPILE)* Compiling [$<]$(COLOR_NONE)'
	g++ -c $< $(COMPILE_FLAGS)

%.o : %.c
	@echo -e '$(COLOR_COMPILE)* Compiling [$<]$(COLOR_NONE)'
	g++ -c $< $(COMPILE_FLAGS)

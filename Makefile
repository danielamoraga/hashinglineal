APP_PATH = ./app

compile:
	 g++ -g -std=c++20 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic main.cpp -o $(APP_PATH)

run: compile
	$(APP_PATH)

debug:
	gdb $(APP_PATH)

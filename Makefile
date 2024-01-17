SHELL := /bin/bash
TARGET = ircserv
OBJ_PATH = obj
CXX = c++
sanitizer = -fsanitize=address -fno-omit-frame-pointer
CXXFLAGS = -Wall -Wextra -Werror -std=c++98  -g3

pwd = $(shell pwd):/home/vscode/src

SRCS = main.cpp Server.cpp Client.cpp Command.cpp Commands_utils.cpp Channelcmds.cpp Channel.cpp servError.cpp chcmdsutils.cpp

OBJS = $(patsubst %.cpp,$(OBJ_PATH)/%.o,$(SRCS))

all: $(TARGET)

$(OBJ_PATH)/%.o: %.cpp | $(OBJ_PATH)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)
	@printf "$(TARGET) Ready!\n"

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(TARGET)

re: clean all

push: fclean
	git add .
	git commit -m " $(shell date +'%Y-%m-%d %H:%M:%S') by $(shell whoami)"
	git push -u origin master

.PHONY: all clean fclean re push

leaks: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET) 6667 pass

docker:
	docker run -it --cap-add=SYS_PTRACE --security-opt seccomp=unconfined --security-opt apparmor=unconfined --name 42-valgrind$(shell date '+%H%M%S') --network host --rm -v $(pwd) valgrind "/bin/zsh"

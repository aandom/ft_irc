SHELL := /bin/bash
TARGET = ircserv
OBJ_PATH = obj
SRC_PATH  = src/
CXX = c++
sanitizer = -fsanitize=address -fno-omit-frame-pointer
CXXFLAGS = -Wall -Wextra -Werror -std=c++98  -g3 # $(sanitizer)
leak = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

pwd = $(shell pwd):/home/vscode/src

SRCS = src/main.cpp src/Server.cpp src/Client.cpp src/Command.cpp src/Commands_utils.cpp src/Channelcmds.cpp src/Channel.cpp src/servError.cpp src/chcmdsutils.cpp src/init_socket.cpp

# OBJS 	  = $(patsubst src%.cpp,$(OBJ_PATH)/%.o,$(SRCS))
OBJS      = $(patsubst $(SRC_PATH)%.cpp,$(OBJ_PATH)/%.o,$(SRCS))

all: $(TARGET)

$(OBJ_PATH)/%.o: $(SRC_PATH)%.cpp | $(OBJ_PATH)
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

server: $(TARGET)
	$(leak) ./$(TARGET) 6667 pass

docker:
	docker run -it --cap-add=SYS_PTRACE --security-opt seccomp=unconfined --security-opt apparmor=unconfined  --network host --name 42-valgrind$(shell date '+%H%M%S') --rm -v $(pwd) valgrind "/bin/zsh"

.PHONY: all clean fclean re push docker server
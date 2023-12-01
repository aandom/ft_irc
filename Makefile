TARGET = ircserv
OBJ_PATH = obj
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = main.cpp server.cpp

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
	git commit -m "Updated on $(shell date +'%Y-%m-%d %H:%M:%S') by $(shell whoami)"
	git push -u origin master

.PHONY: all clean fclean re push
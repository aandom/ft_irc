# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aandom <aandom@student.abudhabi42.ae>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/25 09:44:11 by aandom            #+#    #+#              #
#    Updated: 2023/11/25 09:47:32 by aandom           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRCS = 
OBJS = 
CPPFLAGS = -Wall -Wextra -Werror -std=c++98


%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(NAME): $(OBJ)
		 $(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

all: $(NAME)

clean: 
		rm -f $(OBJ)

fclean: clean
		rm -rf $(NAME)

re: fclean all

.PHONY: all clean re
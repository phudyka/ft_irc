# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/05 15:51:03 by phudyka           #+#    #+#              #
#    Updated: 2024/03/06 11:36:31 by phudyka          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

TARGET = ircserv

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = src/main.cpp src/ft_irc.cpp src/utils.cpp \

OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean fclean re

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
 
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean all
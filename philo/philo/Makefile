# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mchampag <mchampag@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/23 09:25:26 by mchampag          #+#    #+#              #
#    Updated: 2023/08/17 20:06:49 by mchampag         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Project name
NAME = philo

# Flags
AR = ar
CC = gcc
ARFLAG = rcs
CFLAG = -Wall -Wextra -Werror -g
#-fsanitize=thread
LDFLAGS = -phtread -lpthread

# Files sources
S = src/
SRC_FILE = main.c parsing.c print.c routine.c sleep_time.c
SRC = $(addprefix $S, $(SRC_FILE))

# Object conversion
O = obj/
OBJ= $(SRC:$S%=$O%.o)
$O%.o: $S%
	@printf "$R■$W "
	@$(CC) $(CFLAG) -c $< -o $@

# Main rule
all: init $(NAME)
	@echo "$G$(NAME) compiled$W"
	
# Initialise librairies and make object folder
init:
	@mkdir -p $O
	@echo "$GLibrary's initialisation$W"

# Create executable
$(NAME): $(OBJ)
	@$(CC) $(CFLAG) -o $@ $^

# Clean
REMOVE = rm -rf

clean:
	@$(REMOVE) $O

fclean: clean
	@$(REMOVE) $(NAME)

re:	fclean all

#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tsankola <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/20 20:58:24 by tsankola          #+#    #+#              #
#    Updated: 2023/05/20 20:58:26 by tsankola         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = philo

NAME_BONUS = philo_bonus

CC = cc

CFLAGS = -Wall -Wextra -Werror -Ofast

SFLAGS = -fsanitize=address -g3

SRCDIR = src

SRCDIR_BONUS = src_bonus

OBJDIR = obj

OBJDIR_BONUS = obj

INCDIR = include

INCDIR_BONUS = include_bonus

_SRC =	philo_main.c parsing.c philosopher_actions.c philosopher_actions2.c \
		timer.c printer.c print_buffer.c
SRC = $(patsubst %, $(SRCDIR)/%, $(_SRC))

_OBJ = $(patsubst %.c, %.o, $(_SRC))
OBJ = $(patsubst %, $(OBJDIR)/%, $(_OBJ))

_SRC_BONUS = $(patsubst %.c, %_bonus.c, $(_SRC))
SRC_BONUS = $(patsubst %, $(SRCDIR)/%, $(_SRC_BONUS))

_OBJ_BONUS = $(patsubst %.c, %.o, $(_SRC_BONUS))
OBJ_BONUS = $(patsubst %, $(OBJDIR)/%, $(_OBJ_BONUS))

_INC = philo.h
INC = $(patsubst %, $(INCDIR)/%, $(_INC))

_INC_BONUS = $(patsubst %.h, %_bonus.h, $(_INC))
INC_BONUS = $(patsubst %, $(INCDIR_BONUS)/%, $(_INC_BONUS))

.PHONY: all bonus test clean

all: $(NAME)

sanitizer: $(OBJ) $(INC)
	$(CC) $(CFLAGS) $(SFLAGS) $(OBJ) -o $(NAME)

suppress: $(SRC) $(INC)
	$(CC) $(SRC) -I$(INCDIR) -o $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INC) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -I$(INCDIR) -o $@

$(OBJDIR_BONUS)/%.o: $(SRCDIR_BONUS)/%.c $(INC_BONUS) | $(OBJDIR_BONUS)
	$(CC) $(CFLAGS) -c $< -I$(INCDIR_BONUS) -o $@

# Build
$(NAME): .mandatory

.mandatory: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@touch .mandatory
	@rm -f .bonus

bonus: .bonus

.bonus: $(OBJ_BONUS)
	$(CC) $(CFLAGS) $(OBJ_BONUS) -o $(NAME_BONUS)
	@touch .bonus
	@rm -f .mandatory

clean:
	rm -f $(OBJ)
	rm -f $(OBJ_BONUS)
	rmdir $(OBJDIR) 2> /dev/null || true
	rm -f $(patsubst %, %~, $(SRC) $(SRC_BONUS))
	rm -f $(patsubst %, %~, $(INC) $(INC_BONUS))

fclean: clean
	rm -f $(NAME)
	@rm -f .bonus
	@rm -f .mandatory

re: fclean all

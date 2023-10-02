# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/23 16:00:31 by isojo-go          #+#    #+#              #
#    Updated: 2023/10/01 16:43:44 by isojo-go         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# --- Makefile structure ---

## Program Name:
NAME	=	libclasses.a
EXT		=	.cpp

## Folder Structure:
SRCDIR		=	src
OBJDIR		=	obj

## Libraries:
LIBS	=	# N/A

## Header Files (dedicated and from libraries):
HEADERS	=	-I ./inc

## Source / Object Files:
SRC		=	$(wildcard $(SRCDIR)/*$(EXT))
OBJ		=	$(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)

## Compilation flags:
CC			=	c++
AR			=	ar rcs
CFLAGS		=	-Wall -Wextra -Werror -std=c++98 -pedantic -Wshadow
RM			=	rm -rf
DEBUG		=

# *************************************************************************** #

# --- Makefile Extras ---

## Colors:
DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;31m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

# *************************************************************************** #

# --- Makefile instructions ---

all:		$(NAME)

$(NAME):	$(OBJ)
			@$(AR) $(NAME) $(OBJ)
			@echo "$(GREEN)$(NAME) compiled!$(DEF_COLOR)"

$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
			@mkdir -p $(OBJDIR)
			@printf "$(BLUE)Compiling: $< ...$(DEF_COLOR)"
			@$(CC) $(DEBUG) $(CFLAGS) -o $@ -c $< $(HEADERS)
			@echo "$(GREEN)Done!$(DEF_COLOR)"

clean:
			@$(RM) $(OBJ) $(OBJDIR)
			@echo "$(YELLOW)$(NAME) object files removed!$(DEF_COLOR)"

fclean:
			@if [ -d $(OBJDIR) ]; then \
				$(RM) $(OBJ) $(OBJDIR); \
				echo "$(YELLOW)$(NAME) object files removed!$(DEF_COLOR)"; \
			fi
			@if [ -f $(NAME) ]; then \
				$(RM) $(NAME); \
				echo "$(RED)$(NAME) removed!$(DEF_COLOR)"; \
			fi

re:			fclean all

.PHONY:		all clean fclean re

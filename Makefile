# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/23 16:00:17 by isojo-go          #+#    #+#              #
#    Updated: 2023/09/12 15:57:47 by isojo-go         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# --- Makefile structure ---

## Program Name:
NAME	=	webserv
EXT		=	.cpp

## Folder Structure:
SRCDIR		=	src
OBJDIR		=	obj
BINDIR		=	bin

## Libraries / Classes:
LIBCLASS	=	./classes
LIBS		=	$(LIBCLASS)/libclasses.a

## Header Files (dedicated and from libraries):
HEADERS	=	-I ./inc -I $(LIBCLASS)/inc

## Source / Object Files:
SRC		=	$(wildcard $(SRCDIR)/*$(EXT))
OBJ		=	$(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)


## Compilation flags:
CC			=	c++
CFLAGS		=	-Wall -Wextra -Werror -std=c++98 -pedantic -Wshadow
RM			=	rm -rf
DEBUG		=	-DDEBUG=1

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

$(NAME):	libclasses $(OBJ)
			@mkdir -p $(BINDIR)
			@$(CC) $(DEBUG) $(CFLAGS) $(OBJ) $(LIBS) $(HEADERS) -o ./$(BINDIR)/$(NAME)
			@echo "$(GREEN)$(NAME) compiled!$(DEF_COLOR)"

$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
			@mkdir -p $(OBJDIR)
			@printf "$(BLUE)Compiling: $< ...$(DEF_COLOR)"
			@$(CC) $(DEBUG) $(CFLAGS) -o $@ -c $< $(HEADERS)
			@echo "$(GREEN)Done!$(DEF_COLOR)"

libclasses:
			@$(MAKE) -C $(LIBCLASS)

clean:
			@$(RM) $(OBJ) $(OBJDIR)
			@echo "$(YELLOW)$(NAME) object files removed!$(DEF_COLOR)"
			@$(MAKE) -C $(LIBCLASS) clean

fclean:
			@$(MAKE) -C $(LIBCLASS) fclean
			@if [ -d $(OBJDIR) ]; then \
				$(RM) $(OBJ) $(OBJDIR); \
				echo "$(YELLOW)$(NAME) object files removed!$(DEF_COLOR)"; \
			fi
			@$(RM) $(NAME) $(BINDIR)
			@echo "$(RED)$(NAME) removed!$(DEF_COLOR)"

re:			fclean all

.PHONY:		all clean fclean re libclasses

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lnicosia <lnicosia@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/22 09:45:49 by lnicosia          #+#    #+#              #
#    Updated: 2022/05/30 11:33:46 by lnicosia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ls

MAKEFILE = Makefile

LIB_DIR = .
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .
INCLUDES_DIR = includes
LIBFT_DIR = $(LIB_DIR)/libft
LIBMFT_DIR = $(LIB_DIR)/libmft
INSTALL_DIR = install
SED = sed
ROOT = sudo

#LIBS
LIBFT = $(LIBFT_DIR)/libft.a
LIBMFT = $(LIBMFT_DIR)/libmft.a

ACL_TEST := $(shell ld -lacl 2>/dev/null; echo $$?; rm a.out)

ifeq ($(ACL_TEST), 1)
	ACL =
else
	ACL = -lacl
endif

LDLIBS = $(LIBFT) $(ACL)

LDFLAGS = -L $(LIBFT_DIR) -lm

LIB_RAW =

SRC_RAW =	main.c ft_ls.c parse_option_line.c analyze_file.c \
			print_file.c get_ls_time.c print_size.c sort_files.c compare.c \
			print_dlist_col.c print_dlist.c set_color.c link.c \

HEADERS =	ls.h directory.h options.h ls_padding.h ls_colors.h

#
# Creation of files path
#

SRC = $(addprefix $(SRC_DIR)/, $(SRC_RAW))
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_RAW:.c=.o))

INCLUDES = $(addprefix $(INCLUDES_DIR)/, $(HEADERS))

RESOURCES =

OPTI_FLAGS = -O3

CFLAGS =	-Wall -Wextra -Werror -Wpadded -Wconversion -I $(INCLUDES_DIR) \
	  	-I $(LIBFT_DIR)/includes -I $(BMP_PARSER_DIR)/includes \
		-I $(LIBMFT_DIR)/includes -I $(GLAD_DIR)/include \
		$(OPTI_FLAGS) \
		#-fsanitize=address -g3 \

#
# Setting right flags and files dependencies to link external libs
# according to user's os
#

ifeq ($(OS), Windows_NT)
else
	UNAME_S = $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
		OPTI_FLAGS += -flto
		SED = gsed
    else
		CFLAGS += -Wno-misleading-indentation
		OPTI_FLAGS += -flto
    endif
endif

#
# Color declarations
#

RED := "\e[0;31m"
GREEN := "\e[0;32m"
YELLOW := "\e[0;33m"
BLUE := "\e[0;34m"
MAGENTA := "\e[0;35m"
CYAN := "\e[0;36m"
RESET :="\e[0m"

#
# Rules
#

all:
	@printf $(CYAN)"[INFO] Building libft..\n"$(RESET)
	@make --no-print-directory -C $(LIBFT_DIR)
	@printf $(CYAN)"[INFO] Building ft_ls..\n"$(RESET)
	@make --no-print-directory $(BIN_DIR)/$(NAME)


$(LIBFT):
	@git submodule update --init
	@make --no-print-directory -C $(LIBFT_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDES)
	@printf $(YELLOW)"Compiling $<\n"$(RESET)
	gcc -c $< -o $@ $(CFLAGS)

$(NAME): $(LIBFT) $(OBJ_DIR) $(OBJ)
	@printf $(CYAN)"[INFO] Linking ${BIN_DIR}/${NAME}\n"$(RESET)
	gcc $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS) $(LDLIBS)
	@printf ${GREEN}"[INFO] Compiled $(BIN_DIR)/$(NAME) with success!\n"
	@printf ${RESET}

clean:
	@make --no-print-directory clean -C $(LIBFT_DIR)
	@printf ${CYAN}"[INFO] Removing objs\n"${RESET}
	rm -rf $(OBJ_DIR)

fclean:
	@make --no-print-directory fclean -C $(LIBFT_DIR)
	@printf ${CYAN}"[INFO] Removing objs\n"${RESET}
	rm -rf $(OBJ_DIR)
	@printf ${CYAN}"[INFO] Removing $(BIN_DIR)/$(NAME)\n"$(RESET)
	rm -rf $(BIN_DIR)/$(NAME)

re: fclean all

.PHONY: fclean all clean libft

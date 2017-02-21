NAME = ft_select

CC = gcc

CFLAGS = -Wall -Wextra -Werror -g

MFLAGS =

INCLUDE_DIR = ./includes

DIR_OBJ = ./obj/

DIR_LIB = ./libft/

SRC_DIR = ./srcs/

MAIN = main.c

FILES = nmatch.c \
		delete.c \
		get_params.c \
		left_move.c \
		right_move.c \
		vertical_moves.c \
		read_stdin.c \
		search.c \
		select.c \
		signals.c \
		term.c \
		update_window.c

OBJS = $(patsubst %.c, $(DIR_OBJ)%.o, $(FILES)) $(patsubst %.c, $(DIR_OBJ)%.o, $(MAIN))

DEP_OBJ = src_msg \
		  $(OBJS)

RM = /bin/rm -rf

all: mkdirobj $(DEP_OBJ)
	@ make -C $(DIR_LIB)
	@ /bin/echo -n "Archiving object in $(NAME):"
	@ $(CC) -o $(NAME) $(OBJS) $(MFLAGS) -L $(DIR_LIB) -lft -ltermcap
	@ echo " \033[32mAll done!\033[0m"

$(DIR_OBJ)%.o: $(SRC_DIR)%.c
	@ /bin/echo -n "    $(notdir $@)"
	@ $(CC) $(CFLAGS) -c -o $@ $< -I $(INCLUDE_DIR)
	@ echo " \033[32mOK\033[0m"

mkdirobj:
	@ mkdir -p $(DIR_OBJ)

clean:
	@ make clean -C $(DIR_LIB)
	@ /bin/echo -n "Removing object files:"
	@ $(RM) $(DIR_OBJ)
	@ echo " \033[32mdone\033[0m"

fclean: clean
	@ make fclean -C $(DIR_LIB)
	@ /bin/echo -n "Removing library:"
	@ $(RM) $(NAME)
	@ echo " \033[32mdone\033[0m"

re: fclean all

src_msg:
	@ echo " src functions:"

.PHONY : all clean fclean re

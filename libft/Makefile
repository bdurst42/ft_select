NAME =	libft.a

AR =		ar
CC =		gcc
AFLAGS =	rcus
CFLAGS =	-Wall -Werror -Wextra -Ofast

DIR_OBJ =	./obj/
PATH_INC =	./include/
PATH_SUBD =	./ascii/ \
			./io/ \
			./list/ \
			./c_list/ \
			./memory/ \
			./string/
VPATH =	$(PATH_SUBD)
INC_PATH =	$(patsubst %, -I %, $(PATH_INC))

SRC_ASCII =		ft_isalnum.c \
				ft_isalpha.c \
				ft_isascii.c \
				ft_isdigit.c \
				ft_isprint.c \
				ft_tolower.c \
				ft_toupper.c

SRC_IO =		ft_fputchar.c \
				ft_fputendl.c \
				ft_fputnbr.c \
				ft_fputstr.c \
				ft_putchar.c \
				ft_putendl.c \
				ft_putnbr.c \
				ft_putstr.c \
				gnl.c

SRC_LIST =		ft_create_node.c \
				ft_clear_list.c \
				ft_node_push_back.c \
				ft_node_push_front.c \
				ft_node_push_after.c \
				ft_go_start_list.c \
				ft_go_end_list.c \
				ft_list_size.c \
				ft_add_list.c \
				ft_bubblesort_list.c \
				ft_insertsort_list.c \
				ft_list_sort_rev.c \
				ft_pop_node.c

SRC_C_LIST =	ft_c_node_push_back.c \
				ft_c_node_push_front.c \
				ft_c_node_push_after.c \
				ft_c_list_size.c

SRC_MEMORY =	ft_bzero.c \
				ft_memalloc.c \
				ft_memccpy.c \
				ft_memchr.c \
				ft_memcmp.c \
				ft_memcpy.c \
				ft_memdel.c \
				ft_memmove.c \
				ft_memset.c \
				ft_malloc_tab_char.c \
				ft_free_tab.c

SRC_STRING =	ft_atoi.c \
				ft_itoa.c \
				ft_nbrlen.c \
				ft_strcat.c \
				ft_strchr.c \
				ft_strclr.c \
				ft_strcmp.c \
				ft_strcpy.c \
				ft_strdel.c \
				ft_strdup.c \
				ft_strequ.c \
				ft_striter.c \
				ft_striteri.c \
				ft_strjoin.c \
				ft_strlcat.c \
				ft_strlen.c \
				ft_strmap.c \
				ft_strmapi.c \
				ft_strncat.c \
				ft_strncmp.c \
				ft_strncpy.c \
				ft_strnequ.c \
				ft_strnew.c \
				ft_strnstr.c \
				ft_strrchr.c \
				ft_strsplit.c \
				ft_strstr.c \
				ft_strsub.c \
				ft_perror.c \
				ft_strtrim.c \
				ft_exiterror.c \
				ft_strfind.c

OBJ_ASCII =		$(patsubst %.c, $(DIR_OBJ)%.o, $(SRC_ASCII))
OBJ_IO =		$(patsubst %.c, $(DIR_OBJ)%.o, $(SRC_IO))
OBJ_LIST =		$(patsubst %.c, $(DIR_OBJ)%.o, $(SRC_LIST))
OBJ_C_LIST =	$(patsubst %.c, $(DIR_OBJ)%.o, $(SRC_C_LIST))
OBJ_MEMORY =	$(patsubst %.c, $(DIR_OBJ)%.o, $(SRC_MEMORY))
OBJ_STRING =	$(patsubst %.c, $(DIR_OBJ)%.o, $(SRC_STRING))
DEP_OBJ =	ascii_msg \
			$(OBJ_ASCII) \
			io_msg \
			$(OBJ_IO) \
			list_msg \
			$(OBJ_LIST) \
			c_list_msg \
			$(OBJ_C_LIST) \
			memory_msg \
			$(OBJ_MEMORY) \
			string_msg \
			$(OBJ_STRING)
OBJ =	$(OBJ_ASCII) \
		$(OBJ_IO) \
		$(OBJ_LIST) \
		$(OBJ_C_LIST) \
		$(OBJ_MEMORY) \
		$(OBJ_STRING)

all: $(NAME)

$(NAME): assemble mkobjdir $(DEP_OBJ)
	@ /bin/echo -n "Archiving objects in $(NAME):"
	@ $(AR) $(AFLAGS) $(NAME) $(OBJ)
	@ echo " \033[32mAll done!\033[0m"

$(DIR_OBJ)%.o: %.c
	@ /bin/echo -n "	$(notdir $@)"
	@ $(CC) $(CFLAGS) -c -o $@ $< $(INC_PATH)
	@ echo " \033[32mOK\033[0m"

clean:
	@ /bin/echo -n "Removing object files:"
	@ rm -rf $(DIR_OBJ)
	@ echo " \033[32mdone\033[0m"

fclean: clean
	@ /bin/echo -n "Removing library:"
	@ rm -rf $(NAME)
	@ echo " \033[32mdone\033[0m"

re: fclean all

mkobjdir:
	@ mkdir -p $(DIR_OBJ)

assemble:
	@ echo "Assembling:"

ascii_msg:
	@ echo "  ASCII functions:"

io_msg:
	@ echo "  I/O functions:"

list_msg:
	@ echo "  list functions:"

c_list_msg:
	@ echo "  c_list functions:"

memory_msg:
	@ echo "  memory functions:"

string_msg:
	@ echo "  string functions:"

.PHONY: clean fclean re

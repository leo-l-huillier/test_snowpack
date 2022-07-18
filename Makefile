##
## technical test, 2022
## 
## File description:
## Makefile
##

SRC	=	main.c 				\
		lib.c 				\
		server.c 			\
		handle_client.c 	\
		client.c 			\
		send_and_receive.c 	\

BUILDDIR = build

OBJ = $(patsubst %.c, $(BUILDDIR)/%.o, $(SRC))

$(BUILDDIR)/%.o: 	src/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

CFLAGS	=	-Wall -Wextra -pthread

CPPFLAGS	=	-I./include

NAME	=	snowpack

all:	$(NAME)

$(NAME):	$(OBJ)
	gcc -o $(NAME) $(OBJ) $(CFLAGS) -lm

debug: CPPFLAGS += -g3
debug: re

clean:
	@$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all clean

.PHONY: fclean all clean debug re

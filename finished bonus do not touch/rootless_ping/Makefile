# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/10 17:08:53 by ndelhota          #+#    #+#              #
#    Updated: 2026/04/20 19:15:15 by ndelhota         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := ft_ping 

ROOT_RULES = all re $(NAME)

CURRENT_CMD = $(if $(MAKECMDGOALS), $(MAKECMDGOALS), all)

ifneq ($(filter $(ROOT_RULES), $(CURRENT_CMD)),)
	ifneq ($(shell id -u), 0)
       		$(error "file must be compiled with root privileges to set capabilities and allow rootless ft_ping")
	endif
endif

SRC := main.c \
       srcs/utils/string_display.c \
       srcs/utils/string_display_2.c \
       srcs/init/init.c \
       srcs/init/socket_init.c \
       srcs/init/retrieve_args.c \
       srcs/init/retrieve_args_utils.c \
       srcs/init/sig_init.c \
       srcs/core/core_loop.c \
       srcs/core/create_send.c \
       srcs/core/receive_pong.c \
       srcs/core/parse_received.c \
       srcs/end/ft_end.c \

CC := cc

CFLAGS := -g -Wall -Werror -Wextra

OBJDIR = obj
OBJ = $(SRC:.c=.o)
OBJ := $(addprefix $(OBJDIR)/, $(OBJ))


all : $(OBJ) $(NAME)

$(NAME) : $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME) -lm -lcap
	@chown 1000:1000 $(NAME)
	@chown -R 1000:1000 $(OBJDIR)
	@setcap cap_net_raw=+ep ft_ping	

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c -o $@ $^

clean :
	@rm -rf $(OBJDIR) 

fclean : clean 
	@rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re

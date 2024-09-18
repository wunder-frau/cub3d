NAME = cub3D
SRC = main.c vector.c parser_config.c error_handling.c free.c
OBJ = $(SRC:.c=.o)
CC = gcc
MLX42_PATH = MLX42
LIBFT_PATH = libft
LIBFT = $(LIBFT_PATH)/libft.a
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address -I $(MLX42_PATH)/include -I $(LIBFT_PATH)
LDFLAGS = -L $(MLX42_PATH)/build -lmlx42 -lglfw -ldl -L $(LIBFT_PATH) -lft

# Color definitions for terminal output
RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
RESET = \033[0m

all: $(LIBFT) $(NAME)

$(LIBFT):
	@echo "$(BLUE)🔄 Building libft...$(RESET)"
	$(MAKE) -C $(LIBFT_PATH)

$(NAME): $(OBJ) $(LIBFT)
	@echo "$(BLUE)🔄 Building MLX42...$(RESET)"
	cd $(MLX42_PATH) && mkdir -p build && cd build && cmake .. && make
	@echo "$(BLUE)🚀 Compiling $(SRC)$(RESET)"
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(YELLOW)🧹 Cleaning up...$(RESET)"
	$(MAKE) -C $(LIBFT_PATH) clean
	rm -f $(OBJ)

fclean: clean
	@echo "$(RED)🚮 Removing executable: $(NAME)$(RESET)"
	@rm -rf $(MLX42_PATH)/build
	$(MAKE) -C $(LIBFT_PATH) fclean
	rm -f $(NAME)

re: fclean all
	@echo "$(BLUE)🔄 Rebuilding the project: $(NAME)$(RESET)"

.PHONY: all clean fclean re

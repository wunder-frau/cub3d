NAME = cub3D
SRC = main.c
CC = gcc
MLX42_PATH = MLX42
CFLAGS = -Wall -Wextra -Werror -I $(MLX42_PATH)/include
LDFLAGS = -L $(MLX42_PATH)/build -lmlx42 -lglfw -ldl

RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
RESET = \033[0m

all: $(NAME)

$(NAME): $(SRC)
	@echo "$(BLUE)🚀 Compiling $(SRC)$(RESET)"
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(NAME)

clean:
	@echo "$(YELLOW)🧹 Cleaning up...$(RESET)"
	rm -f $(NAME)

fclean: clean
	@echo "$(RED)🚮 Removing executable: $(NAME)$(RESET)"
	@rm -f $(NAME)

re: fclean all
	@echo "$(BLUE)🔄 Rebuild the project: $(NAME)$(RESET)"

.PHONY: all clean fclean re

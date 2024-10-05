
NAME = cub3d

MLX42_PATH = MLX42
LIBFT_PATH = libft
LIBFT = $(LIBFT_PATH)/libft.a
CC = gcc

# Detect the operating system
UNAME_S := $(shell uname -s)

# Default CFLAGS and LDFLAGS
# CFLAGS = -g -fsanitize=address -I $(MLX42_PATH)/include -I $(LIBFT_PATH)
# LDFLAGS = -fsanitize=address
CFLAGS = -g -I $(MLX42_PATH)/include -I $(LIBFT_PATH)
# LDFLAGS = -fsanitize=address

# Adjust CFLAGS and LDFLAGS based on OS
ifeq ($(UNAME_S),Linux)
    LDFLAGS += -L $(MLX42_PATH)/build -lmlx42 -L $(LIBFT_PATH) -lft -ldl -lglfw -lm
    GLFW_INSTALL_CMD = sudo apt-get install -y libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev
else ifeq ($(UNAME_S),Darwin)
    # For macOS, specify the path to the GLFW library and required frameworks
    CFLAGS += -I /opt/homebrew/include
    LDFLAGS += -L $(MLX42_PATH)/build -lmlx42 -L $(LIBFT_PATH) -lft -L /opt/homebrew/lib -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
    GLFW_INSTALL_CMD = true  # No-op command to prevent syntax errors
endif

OBJ_PATH = build/
SRC_PATH = src/
VEC_PATH = vector/

SRC_FILES = main.c parser_config.c error_handling.c free.c process_map.c map_validation.c cub3d.c player_position.c allocate.c
VEC_FILES = ft_realloc.c vector_create.c vector_free.c vector_methods.c vector_push_back.c vector_copy.c vector_print.c

CUB3D_SRC = $(addprefix $(SRC_PATH), $(SRC_FILES)) \
            $(addprefix $(VEC_PATH), $(VEC_FILES))

OBJS = $(addprefix $(OBJ_PATH), $(SRC_FILES:.c=.o)) \
       $(addprefix $(OBJ_PATH)vector/, $(VEC_FILES:.c=.o))

RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
PURPLE = \033[0;35m
CYAN = \033[0;36m
RESET = \033[0m

SUCCESS_EMOJI = ✅
CLEAN_EMOJI = 🧹
BUILD_EMOJI = 🔨
REMOVE_EMOJI = 🗑️
REBUILD_EMOJI = ♻️
SUBMODULE_EMOJI = 🧩

all: $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_PATH)
	@echo "$(BUILD_EMOJI) $(GREEN)libft built!$(RESET)"

$(NAME): $(LIBFT) $(OBJ_PATH) $(OBJS)
	@echo "$(BUILD_EMOJI) $(BLUE)Building MLX42...$(RESET)"
ifeq ($(UNAME_S),Linux)
	@if ! pkg-config --exists glfw3; then \
		echo "$(BUILD_EMOJI) $(YELLOW)Installing GLFW...$(RESET)"; \
		$(GLFW_INSTALL_CMD); \
	fi
endif
	@cd $(MLX42_PATH) && mkdir -p build && cd build && cmake .. && make
	@echo "$(BUILD_EMOJI) $(GREEN)MLX42 built!$(RESET)"
	@echo "$(BUILD_EMOJI) $(BLUE)Compiling $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)
	@echo "$(SUCCESS_EMOJI) $(GREEN)$(NAME) built successfully!$(RESET)"

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(BUILD_EMOJI) $(BLUE)Compiled: $(RESET)$<"

$(OBJ_PATH)vector/%.o: $(VEC_PATH)%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(BUILD_EMOJI) $(BLUE)Compiled: $(RESET)$<"

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH) \
		$(OBJ_PATH)vector

clean:
	@rm -f $(OBJS)
	@rm -rf $(OBJ_PATH)
	@$(MAKE) -C $(LIBFT_PATH) clean
	@echo "$(CLEAN_EMOJI) $(PURPLE)Object files and build directories removed!$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_PATH) fclean
	@echo "$(REMOVE_EMOJI) $(RED)All build artifacts removed!$(RESET)"

re: fclean all
	@echo "$(REBUILD_EMOJI) $(YELLOW)Rebuild complete!$(RESET)"

.PHONY: all clean fclean re

# # NAME = cub3d

# # MLX42_PATH = MLX42
# # LIBFT_PATH = libft
# # LIBFT = $(LIBFT_PATH)/libft.a
# # CC = gcc
# # CFLAGS = -g -fsanitize=address -I $(MLX42_PATH)/include -I $(LIBFT_PATH)
# # LDFLAGS = -L $(MLX42_PATH)/build -lmlx42 -lglfw -ldl -L $(LIBFT_PATH) -lft -fsanitize=address

# # OBJ_PATH = build/
# # SRC_PATH = src/
# # VEC_PATH = vector/

# # SRC_FILES = main.c parser_config.c error_handling.c free.c process_map.c map_validation.c cub3d.c player_position.c
# # VEC_FILES = ft_realloc.c vector_create.c vector_free.c vector_methods.c vector_push_back.c vector_copy.c vector_print.c

# # CUB3D_SRC = $(addprefix $(SRC_PATH), $(SRC_FILES)) \
# #             $(addprefix $(VEC_PATH), $(VEC_FILES))

# # CUB3D_OBJ = $(CUB3D_SRC:.c=.o)
# # OBJS = $(addprefix $(OBJ_PATH), $(SRC_FILES:.c=.o)) \
# #        $(addprefix $(OBJ_PATH)vector/, $(VEC_FILES:.c=.o))

# # RED = \033[0;31m
# # GREEN = \033[0;32m
# # YELLOW = \033[0;33m
# # BLUE = \033[0;34m
# # PURPLE = \033[0;35m
# # CYAN = \033[0;36m
# # RESET = \033[0m

# # SUCCESS_EMOJI = ✅
# # CLEAN_EMOJI = 🧹
# # BUILD_EMOJI = 🔨
# # REMOVE_EMOJI = 🗑️
# # REBUILD_EMOJI = ♻️
# # SUBMODULE_EMOJI = 🧩

# # all: $(NAME)

# # $(LIBFT):
# # 	@$(MAKE) -C $(LIBFT_PATH)
# # 	@echo "$(BUILD_EMOJI) $(GREEN)libft built!$(RESET)"

# # $(NAME):  $(LIBFT) $(OBJ_PATH) $(OBJS)
# # 	@echo "$(BUILD_EMOJI) $(BLUE)Building MLX42...$(RESET)"
# # 	cd $(MLX42_PATH) && mkdir -p build && cd build && cmake .. && make
# # 	@echo "$(BUILD_EMOJI) $(GREEN)MLX42 built!$(RESET)"
# # 	@echo "$(BUILD_EMOJI) $(BLUE)Compiling $(NAME)...$(RESET)"
# # 	@$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)
# # 	@echo "$(SUCCESS_EMOJI) $(GREEN)$(NAME) built successfully!$(RESET)"

# # $(OBJ_PATH)%.o: $(SRC_PATH)%.c
# # 	@mkdir -p $(dir $@)
# # 	@$(CC) $(CFLAGS) -c $< -o $@
# # 	@echo "$(BUILD_EMOJI) $(BLUE)Compiled: $(RESET)$<"

# # $(OBJ_PATH):
# # 	@mkdir -p $(OBJ_PATH) \
# # 		$(OBJ_PATH)vector

# # clean:
# # 	@rm -f $(OBJS)
# # 	@rm -rf $(OBJ_PATH)
# # 	@$(MAKE) -C $(LIBFT_PATH) clean
# # 	@echo "$(CLEAN_EMOJI) $(PURPLE)Object files and build directories removed!$(RESET)"

# # fclean: clean
# # 	@rm -f $(NAME)
# # 	@$(MAKE) -C $(LIBFT_PATH) fclean
# # 	@echo "$(REMOVE_EMOJI) $(RED)All build artifacts removed!$(RESET)"

# # re: fclean all
# # 	@echo "$(REBUILD_EMOJI) $(YELLOW)Rebuild complete!$(RESET)"

# # .PHONY: all clean fclean re

# # Makefile for cub3d

# # NAME = cub3d

# # MLX42_PATH = MLX42
# # LIBFT_PATH = libft
# # LIBFT = $(LIBFT_PATH)/libft.a
# # CC = gcc

# # # Detect the operating system
# # UNAME_S := $(shell uname -s)

# # # Default CFLAGS and LDFLAGS
# # CFLAGS = -g -I $(MLX42_PATH)/include -I $(LIBFT_PATH)
# # LDFLAGS = -L $(MLX42_PATH)/build -lmlx42 -L $(LIBFT_PATH) -lft 

# # # Adjust CFLAGS and LDFLAGS based on OS
# # ifeq ($(UNAME_S),Linux)
# #     CFLAGS += $(shell pkg-config --cflags glfw3)
# #     LDFLAGS += $(shell pkg-config --static --libs glfw3) -ldl -lm -no-pie
# #     GLFW_INSTALL_CMD = sudo apt-get install -y libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev
# # else ifeq ($(UNAME_S),Darwin)
# #     CFLAGS += $(shell pkg-config --cflags glfw3)
# #     LDFLAGS += $(shell pkg-config --static --libs glfw3) -lm
# #     GLFW_INSTALL_CMD = brew install glfw
# # endif

# # OBJ_PATH = build/
# # SRC_PATH = src/
# # VEC_PATH = src/vector/

# # SRC_FILES = main.c parser_config.c error_handling.c free.c process_map.c map_validation.c cub3d.c player_position.c
# # VEC_FILES = ft_realloc.c vector_create.c vector_free.c vector_methods.c vector_push_back.c vector_copy.c vector_print.c

# # CUB3D_SRC = $(addprefix $(SRC_PATH), $(SRC_FILES)) \
# #             $(addprefix $(VEC_PATH), $(VEC_FILES))

# # OBJS = $(addprefix $(OBJ_PATH), $(SRC_FILES:.c=.o)) \
# #        $(addprefix $(OBJ_PATH)vector/, $(VEC_FILES:.c=.o))

# # # Color variables and emojis
# # RED = \033[0;31m
# # GREEN = \033[0;32m
# # YELLOW = \033[0;33m
# # BLUE = \033[0;34m
# # PURPLE = \033[0;35m
# # CYAN = \033[0;36m
# # RESET = \033[0m

# # SUCCESS_EMOJI = ✅
# # CLEAN_EMOJI = 🧹
# # BUILD_EMOJI = 🔨
# # REMOVE_EMOJI = 🗑️
# # REBUILD_EMOJI = ♻️
# # SUBMODULE_EMOJI = 🧩

# # all: $(NAME)

# # $(LIBFT):
# # 	@$(MAKE) -C $(LIBFT_PATH)
# # 	@echo "$(BUILD_EMOJI) $(GREEN)libft built!$(RESET)"

# # glfw_check:
# # 	@if ! pkg-config --exists glfw3; then \
# # 		echo "$(BUILD_EMOJI) $(YELLOW)Installing GLFW...$(RESET)"; \
# # 		$(GLFW_INSTALL_CMD); \
# # 	fi

# # $(NAME): glfw_check $(LIBFT) $(OBJ_PATH) $(OBJS)
# # 	@echo "$(BUILD_EMOJI) $(BLUE)Building MLX42...$(RESET)"
# # 	cd $(MLX42_PATH) && mkdir -p build && cd build && cmake .. && make
# # 	@echo "$(BUILD_EMOJI) $(GREEN)MLX42 built!$(RESET)"
# # 	@echo "$(BUILD_EMOJI) $(BLUE)Compiling $(NAME)...$(RESET)"
# # 	@$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)
# # 	@echo "$(SUCCESS_EMOJI) $(GREEN)$(NAME) built successfully!$(RESET)"

# # $(OBJ_PATH)%.o: $(SRC_PATH)%.c
# # 	@mkdir -p $(dir $@)
# # 	@$(CC) $(CFLAGS) -c $< -o $@
# # 	@echo "$(BUILD_EMOJI) $(BLUE)Compiled: $(RESET)$<"

# # $(OBJ_PATH)vector/%.o: $(VEC_PATH)%.c
# # 	@mkdir -p $(dir $@)
# # 	@$(CC) $(CFLAGS) -c $< -o $@
# # 	@echo "$(BUILD_EMOJI) $(BLUE)Compiled: $(RESET)$<"

# # $(OBJ_PATH):
# # 	@mkdir -p $(OBJ_PATH) \
# # 		$(OBJ_PATH)vector

# # clean:
# # 	@rm -f $(OBJS)
# # 	@rm -rf $(OBJ_PATH)
# # 	@$(MAKE) -C $(LIBFT_PATH) clean
# # 	@echo "$(CLEAN_EMOJI) $(PURPLE)Object files and build directories removed!$(RESET)"

# # fclean: clean
# # 	@rm -f $(NAME)
# # 	@$(MAKE) -C $(LIBFT_PATH) fclean
# # 	@echo "$(REMOVE_EMOJI) $(RED)All build artifacts removed!$(RESET)"

# # re: fclean all
# # 	@echo "$(REBUILD_EMOJI) $(YELLOW)Rebuild complete!$(RESET)"

# # .PHONY: all clean fclean re glfw_check

# NAME = cub3d

# MLX42_PATH = MLX42
# LIBFT_PATH = libft
# LIBFT = $(LIBFT_PATH)/libft.a
# CC = gcc

# # Detect the operating system
# UNAME_S := $(shell uname -s)

# # Default CFLAGS and LDFLAGS
# CFLAGS = -g -fsanitize=address -I $(MLX42_PATH)/include -I $(LIBFT_PATH)
# LDFLAGS = -L $(MLX42_PATH)/build -lmlx42 -L $(LIBFT_PATH) -lft -fsanitize=address

# # Adjust CFLAGS and LDFLAGS based on OS
# ifeq ($(UNAME_S),Linux)
#     LDFLAGS += -ldl -lglfw
#     GLFW_INSTALL_CMD = sudo apt-get install -y libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev
# else ifeq ($(UNAME_S),Darwin)
#     # For macOS, specify the path to the GLFW library and required frameworks
#     CFLAGS += -I /opt/homebrew/include
#     LDFLAGS += -L /opt/homebrew/lib -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
#     GLFW_INSTALL_CMD = true  # No-op command to prevent syntax errors
# endif

# OBJ_PATH = build/
# SRC_PATH = src/
# VEC_PATH = vector/

# SRC_FILES = main.c parser_config.c error_handling.c free.c process_map.c map_validation.c cub3d.c player_position.c
# VEC_FILES = ft_realloc.c vector_create.c vector_free.c vector_methods.c vector_push_back.c vector_copy.c vector_print.c

# CUB3D_SRC = $(addprefix $(SRC_PATH), $(SRC_FILES)) \
#             $(addprefix $(VEC_PATH), $(VEC_FILES))

# OBJS = $(addprefix $(OBJ_PATH), $(SRC_FILES:.c=.o)) \
#        $(addprefix $(OBJ_PATH)vector/, $(VEC_FILES:.c=.o))

# RED = \033[0;31m
# GREEN = \033[0;32m
# YELLOW = \033[0;33m
# BLUE = \033[0;34m
# PURPLE = \033[0;35m
# CYAN = \033[0;36m
# RESET = \033[0m

# SUCCESS_EMOJI = ✅
# CLEAN_EMOJI = 🧹
# BUILD_EMOJI = 🔨
# REMOVE_EMOJI = 🗑️
# REBUILD_EMOJI = ♻️
# SUBMODULE_EMOJI = 🧩

# all: $(NAME)

# $(LIBFT):
# 	@$(MAKE) -C $(LIBFT_PATH)
# 	@echo "$(BUILD_EMOJI) $(GREEN)libft built!$(RESET)"

# $(NAME): $(LIBFT) $(OBJ_PATH) $(OBJS)
# 	@echo "$(BUILD_EMOJI) $(BLUE)Building MLX42...$(RESET)"
# ifeq ($(UNAME_S),Linux)
# 	@if ! pkg-config --exists glfw3; then \
# 		echo "$(BUILD_EMOJI) $(YELLOW)Installing GLFW...$(RESET)"; \
# 		$(GLFW_INSTALL_CMD); \
# 	fi
# endif
# 	cd $(MLX42_PATH) && mkdir -p build && cd build && cmake .. && make
# 	@echo "$(BUILD_EMOJI) $(GREEN)MLX42 built!$(RESET)"
# 	@echo "$(BUILD_EMOJI) $(BLUE)Compiling $(NAME)...$(RESET)"
# 	@$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)
# 	@echo "$(SUCCESS_EMOJI) $(GREEN)$(NAME) built successfully!$(RESET)"

# $(OBJ_PATH)%.o: $(SRC_PATH)%.c
# 	@mkdir -p $(dir $@)
# 	@$(CC) $(CFLAGS) -c $< -o $@
# 	@echo "$(BUILD_EMOJI) $(BLUE)Compiled: $(RESET)$<"

# $(OBJ_PATH)vector/%.o: $(VEC_PATH)%.c
# 	@mkdir -p $(dir $@)
# 	@$(CC) $(CFLAGS) -c $< -o $@
# 	@echo "$(BUILD_EMOJI) $(BLUE)Compiled: $(RESET)$<"

# $(OBJ_PATH):
# 	@mkdir -p $(OBJ_PATH) \
# 		$(OBJ_PATH)vector

# clean:
# 	@rm -f $(OBJS)
# 	@rm -rf $(OBJ_PATH)
# 	@$(MAKE) -C $(LIBFT_PATH) clean
# 	@echo "$(CLEAN_EMOJI) $(PURPLE)Object files and build directories removed!$(RESET)"

# fclean: clean
# 	@rm -f $(NAME)
# 	@$(MAKE) -C $(LIBFT_PATH) fclean
# 	@echo "$(REMOVE_EMOJI) $(RED)All build artifacts removed!$(RESET)"

# re: fclean all
# 	@echo "$(REBUILD_EMOJI) $(YELLOW)Rebuild complete!$(RESET)"

# .PHONY: all clean fclean re

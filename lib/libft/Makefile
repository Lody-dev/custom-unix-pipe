# --- Library Name ---
NAME    = libft.a

# --- Compiler & Flags ---
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -Iincludes -g

# --- Directories ---
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = includes

# --- Files ---
SRC     = $(wildcard $(SRC_DIR)/*.c)
OBJ     = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
HEADER  = $(wildcard $(INC_DIR)/*.h)

# --- Default Target ---
all: $(NAME)

# --- Build Static Library ---
$(NAME): $(OBJ)
	@echo "📚 Creating static library: $(NAME)"
	@ar rcs $(NAME) $(OBJ)
	@echo "✅ libft.a ready!"

# --- Compile each .c to .o ---
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER) | $(OBJ_DIR)
	@echo "🧩 Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# --- Ensure obj/ folder exists ---
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# --- Cleanup ---
clean:
	@echo "🧹 Removing object files..."
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "🗑️  Removing library..."
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

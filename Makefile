# --- Project Name ---
NAME    = custom-unix-pipe

# --- Compiler & Flags ---
CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -Iincludes -I$(LIBFT_DIR)

# --- Directories ---
SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = lib/libft

# --- Files ---
SRC     = $(wildcard $(SRC_DIR)/*.c)
OBJ     = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
HEADER  = $(wildcard includes/*.h)

# --- Libft ---
LIBFT   = $(LIBFT_DIR)/libft.a

# --- Default Target ---
all: $(LIBFT) $(NAME)

# --- Build main executable ---
$(NAME): $(OBJ)
	@echo "🔧 Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)
	@echo "✅ Build complete!"

# --- Compile each .c to .o ---
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER) | $(OBJ_DIR)
	@echo "🧩 Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# --- Ensure obj/ folder exists ---
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# --- Build libft first ---
$(LIBFT):
	@echo "📚 Building libft..."
	@make --no-print-directory -C $(LIBFT_DIR)

# --- Clean object files ---
clean:
	@echo "🧹 Removing object files..."
	@rm -rf $(OBJ_DIR)
	@make --no-print-directory -C $(LIBFT_DIR) clean

# --- Full cleanup ---
fclean: clean
	@echo "🗑️  Removing executable and libft..."
	@rm -f $(NAME)
	@make --no-print-directory -C $(LIBFT_DIR) fclean

# --- Rebuild everything ---
re: fclean all

.PHONY: all clean fclean re

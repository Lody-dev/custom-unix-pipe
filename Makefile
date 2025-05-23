NAME    := pipex
CFLAGS  := -Wall -Wextra -Werror
LIBFT   := ./lib/libft

HEADERS := -I $(LIBFT)
LIBS    := $(LIBFT)/libft.a -ldl -lglfw -pthread -lm

SRCS    := $(shell find ./src -name "*.c")
OBJS    := $(SRCS:.c=.o)

.PHONY: all libft clean fclean re

all: libft $(NAME)
	@echo "🚀 Build complete: $(NAME)"

libft:
	@make --no-print-directory -C $(LIBFT) > /dev/null 2>&1
	@echo "📚 libft compiled"

%.o: %.c
	@echo "⚙️ Compiling: $<"
	@gcc $(CFLAGS) -c $< -o $@ $(HEADERS)

$(NAME): $(OBJS)
	@echo "🔗 Linking: $(NAME)"
	@gcc $(OBJS) $(LIBS) -o $(NAME) $(HEADERS)
	@echo "✅ Executable created: $(NAME)"

clean:
	@make --no-print-directory clean -C $(LIBFT)
	@rm -f $(LIBFT)/*.a
	@rm -f $(OBJS)
	@echo "🧹 Cleaned object files and libraries"

fclean: clean
	@rm -f $(NAME)
	@echo "🗑️ Executable removed"

re: fclean all

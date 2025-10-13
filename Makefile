NAME    := custom-unix-pipe
CFLAGS  := -Wall -Wextra -Werror -g
LIBFT   := ./lib/libft

HEADERS := -I $(LIBFT)
LIBS    := $(LIBFT)/libft.a 

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
	@cc $(CFLAGS) -c $< -o $@ $(HEADERS)

$(NAME): $(OBJS)
	@echo "🔗 Linking: $(NAME)"
	@cc $(OBJS) $(LIBS) -o $(NAME) $(HEADERS)
	@echo "✅ Executable created: $(NAME)"

clean:
	@make --no-print-directory clean -C $(LIBFT)
	@rm -f $(LIBFT)/*.a
	@rm -f $(OBJS) $(OBJS_BONUS)
	@echo "🧹 Cleaned object files and libraries"

fclean: clean
	@rm -f $(NAME) $(NAME_BONUS)
	@echo "🗑️ Executable removed"

re: fclean all

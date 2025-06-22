NAME    := pipex
NAME_BONUS := pipex_bonus
CFLAGS  := -Wall -Wextra -Werror -g
LIBFT   := ./lib/libft

HEADERS := -I $(LIBFT)
LIBS    := $(LIBFT)/libft.a 

SRCS    := $(shell find ./src -name "*.c")
OBJS    := $(SRCS:.c=.o)

SRCS_BONUS    := $(shell find ./src_bonus -name "*.c")
OBJS_BONUS    := $(SRCS_BONUS:.c=.o)

.PHONY: all libft clean fclean re

all: libft $(NAME)
	@echo "🚀 Build complete: $(NAME)"

bonus: libft $(NAME_BONUS) 
	@echo "🚀 Bonus build complete: $(NAME)"
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

$(NAME_BONUS): $(OBJS_BONUS)
	@echo "🔗 Linking: $(NAME_BONUS)"
	@cc $(OBJS_BONUS) $(LIBS) -o $(NAME_BONUS) $(HEADERS)
	@echo "✅ Executable created: $(NAME_BONUS)"
clean:
	@make --no-print-directory clean -C $(LIBFT)
	@rm -f $(LIBFT)/*.a
	@rm -f $(OBJS) $(OBJS_BONUS)
	@echo "🧹 Cleaned object files and libraries"

fclean: clean
	@rm -f $(NAME) $(NAME_BONUS)
	@echo "🗑️ Executable removed"

re: fclean all

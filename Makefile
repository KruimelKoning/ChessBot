GREEN =\033[0;32m
RED = \033[0;31m
NO_COLOUR =\033[0m

NAME	:= chessbot
CC		:= clang++
CFLAGS	:= -Wall -Wextra -pedantic -std=c++20 -Ofast -march=native -mtune=native -fno-rtti
MAKEFLAGS += --no-print-directory
VALGRIND_FLAGS := --track-fds=yes --leak-check=full \
	--show-leak-kinds=all --track-origins=yes
# CFLAGS := -Wall -Wextra -pedantic -std=c89 -O3 -flto -march=native
INC_DIRS := include
MAIN_SRCDIR := src
MAIN_OBJDIR := build


MODULES := main evaluate
SRCDIR := $(addprefix $(MAIN_SRCDIR)/, $(MODULES))
OBJDIR := $(addprefix $(MAIN_OBJDIR)/, $(MODULES))

SRC_MAIN := $(shell find $(MAIN_SRCDIR)/main -iname "*.cpp")
SRC_EVAL := $(shell find $(MAIN_SRCDIR)/evaluate -iname "*.cpp")

SRC := $(SRC_MAIN) $(SRC_EVAL)
OBJ := $(patsubst $(MAIN_SRCDIR)/%.cpp,$(MAIN_OBJDIR)/%.o, $(SRC))
HEADERS = $(foreach dir, $(INC_DIRS), -I$(dir)) $(addprefix -I,$(SRCDIR))

.PHONY: all clean flcean re run rerun valgrind

all: $(NAME)

$(MAIN_OBJDIR):
	@mkdir -p $(MAIN_OBJDIR)

$(OBJDIR): | $(MAIN_OBJDIR)
	@mkdir -p $(OBJDIR)

$(OBJ) : $(MAIN_OBJDIR)/%.o: $(MAIN_SRCDIR)/%.cpp | $(OBJDIR)
	@$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

$(NAME): $(OBJ)
	@$(CC) $(OBJ) -o $(NAME)
	@echo "${GREEN}built ${NAME}... ${NO_COLOUR}"

clean:
	@echo "${RED}cleaning ${NAME} build files...${NO_COLOUR}"
	@rm -rf build/

fclean: clean
	@echo "${RED}removing ${NAME}...${NO_COLOUR}"
	@rm -f $(NAME)

re: fclean all

valgrind: $(NAME)
	valgrind $(VALGRIND_FLAGS) ./$(NAME)

run: $(NAME)
	@echo "${GREEN}running ${NAME} ... ${NO_COLOUR}"
	@./$(NAME)

rerun: fclean run

NAME = ircserve

SRC = src/Channel.cpp src/Client.cpp src/main.cpp src/Server.cpp\
		src/commands/Invite.cpp src/commands/Join.cpp src/commands/Kick.cpp src/commands/Mode.cpp\
		src/commands/Nick.cpp src/commands/Part.cpp src/commands/Pass.cpp src/commands/PrivateMsg.cpp\
		src/commands/Quit.cpp src/commands/Topic.cpp src/commands/User.cpp
OBJ_DIR = obj
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

COLOR_COMPILATION = \033[38;2;0;200;100m
COLOR_CLEAN = \033[38;2;200;100;100m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@printf "$(COLOR_COMPILATION)✔ Compilation complete!$(RESET)\n"

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@printf "$(COLOR_CLEAN)✖ Objects cleaned!$(RESET)\n"

fclean: clean
	@rm -f $(NAME)
	@printf "$(COLOR_CLEAN)✖ Binary removed!$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re

SETTINGS_DIR := make_settings

include $(SETTINGS_DIR)/src.mk
include $(SETTINGS_DIR)/settings.mk
include $(SETTINGS_DIR)/colors.mk

all:
	@$(MAKE) $(NAME)

# Compilation
$(NAME): $(LIBS) $(OBJ)
	@echo "$(COLOR_GREEN)Creating ls executable...$(COLOR_RESET)"
	@$(CXX) -o $@ $(OBJ) $(LIBS) $(LFLAGS) $(CFLAGS)

$(OBJ): $(ODIR)/%.o: $(SDIR)/%.c
	@mkdir -p $(@D)
	@echo "$(COLOR_LBLUE)Compiling...	$(COLOR_BLUE)$<$(COLOR_RESET)"
	@$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)

$(LIBS):
	@$(MAKE) -C libkm

# Clean up
.PHONY: clean fclean re

clean:
	@echo "$(COLOR_YELLOW)clean $(NAME)... $(COLOR_RESET)"
	@$(MAKE) clean -C libkm
	@printf "$(COLOR_RED)"
	$(RM) -r $(ODIR)
	@printf "$(COLOR_RESET)"

fclean: clean
	@echo "$(COLOR_YELLOW)force clean $(NAME)... $(COLOR_RESET)"
	@$(MAKE) fclean -C libkm
	@printf "$(COLOR_RED)"
	$(RM) $(NAME)
	@printf "$(COLOR_RESET)"

re: fclean
	@$(MAKE) re -C libkm
	@$(MAKE) all

# phony
.PHONY: debug fsanitize

# Debugging
debug: fclean
	@echo "$(COLOR_YELLOW)Building $(NAME) debug... $(COLOR_RESET)"
	@$(MAKE) debug -C libkm
	@$(MAKE) DEBUG=1

leaks: fclean
	@echo "$(COLOR_YELLOW)Building $(NAME) leaks... $(COLOR_RESET)"
	@$(MAKE) leaks -C libkm
	@$(MAKE) LEAKS=1

fsanitize: fclean
	@echo "$(COLOR_YELLOW)Building $(NAME) fsanitize... $(COLOR_RESET)"
	@$(MAKE) fsanitize -C libkm
	@$(MAKE) FSANITIZE=1

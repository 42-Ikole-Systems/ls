SETTINGS_DIR := make_settings

include $(SETTINGS_DIR)/src.mk
include $(SETTINGS_DIR)/settings.mk
include $(SETTINGS_DIR)/colors.mk

all:
	@$(MAKE) $(NAME)

# Compilation
$(NAME): $(LIBS) $(OBJ)
	@echo "$(COLOR_GREEN)Creating ls executable...$(COLOR_RESET)"
	@$(CXX) -o $@ $(OBJ) $(LIBS) $(LFLAGS)

$(OBJ): $(ODIR)/%.o: $(SDIR)/%.c
	@mkdir -p $(@D)
	@echo "$(COLOR_LBLUE)Compiling...	$(COLOR_BLUE)$<$(COLOR_RESET)"
	@$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)

$(LIBS):
	@$(MAKE) -C libkm

# Clean up
.PHONY: clean fclean re

clean:
	@$(MAKE) clean -C libkm
	@printf "$(COLOR_RED)"
	$(RM) -r $(ODIR)
	@printf "$(COLOR_RESET)"

fclean: clean
	@$(MAKE) fclean -C libkm
	@printf "$(COLOR_RED)"
	$(RM) $(NAME)
	@printf "$(COLOR_RESET)"

re: fclean
	@$(MAKE) re -C libkm
	@$(MAKE) all

# Debugging
.PHONY: debug fsanitize

debug:
	@$(MAKE) debug -C libkm
	@$(MAKE) re DEBUG=1

leaks:
	@$(MAKE) leaks -C libkm
	@$(MAKE) re LEAKS=1

fsanitize:
	@$(MAKE) fsanitize -C libkm
	@$(MAKE) re FSANITIZE=1

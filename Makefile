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

# testing

$(TEST)/bin/%: $(TEST)/%.c
	@echo "$(COLOR_LBLUE)Compiling tests... $(COLOR_BLUE)$<$(COLOR_RESET)"
	@$(CC) $(CFLAGS) $(IFLAGS) $< $(OBJ) -o $@ -lcriterion

test: fclean $(NAME) $(TEST)/bin $(TESTBIN)
	@for test in $(TESTBIN) ; do echo "\n$(COLOR_YELLOW)TEST:$(COLOR_RESET) $$test" && ./$$test ; done

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
	$(RM) $(NAME).a
	$(RM) -r $(TEST)/bin/*
	@printf "$(COLOR_RESET)"

re: fclean
	@$(MAKE) re -C libkm
	@$(MAKE) all

# Debugging
.PHONY: debug fsanitize

debug:
	@$(MAKE) debug -C libkm
	@$(MAKE) test DEBUG=1

leaks:
	@$(MAKE) leaks -C libkm
	@$(MAKE) test LEAKS=1

fsanitize:
	@$(MAKE) fsanitize -C libkm
	@$(MAKE) test FSANITIZE=1

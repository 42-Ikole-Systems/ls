ifndef SRC_MK
SRC_MK = 1

SRC :=	main.c \
		parsing/flags.c \
		parsing/parse.c \
		utility/list.c \
		logic/file.c \
		logic/operands.c \
		logic/sort.c

LIB_LOCATIONS = libkm/
LIB_NAMES = km

LIBS  = $(addprefix -L, $(LIB_LOCATIONS))
LIBS += $(addprefix -l, $(LIB_NAMES))

endif

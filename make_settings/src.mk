ifndef SRC_MK
SRC_MK = 1

SRC :=	main.c \
		parsing/flags.c \
		parsing/parse.c \
		utility/file.c \
		logic/file.c \
		logic/operands.c \
		logic/sort.c

LIBKM_LOCATION = libkm/
LIBKM = $(LIBKM_LOCATION)libkm.a
LIBKM_LIB = -L$(LIBKM_LOCATION) -lkm

endif

APPLICATION = game_of_life

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Modules and packages:
USEMODULE += xtimer
USEMODULE += random

USEPKG += u8glib

# Display settings:
CFLAGS += -DU8G_STDOUT_ANSI

include $(RIOTBASE)/Makefile.include

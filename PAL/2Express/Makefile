# Colorful output
color_off = \033[0m
color_green = \033[0;32m
color_cyan = \033[0;36m
color_yellow = \033[0;33m

define echo_green
    $(call echo_custom_color,$(color_green),$(1))
endef

define echo_cyan
	$(call echo_custom_color,$(color_cyan),$(1))
endef

define echo_warning
	$(call echo_custom_color,$(color_yellow),$(1))
endef

define echo_custom_color
	@printf "$(1)$(2)$(color_off)\n"
endef

# Before and after job output functions
define before_job
	$(eval TO_WRITE = $(strip $(1)))
	$(eval WRITE_LENGTH = $(shell expr \( 11 + \( "X$(TO_WRITE)" : ".*" \) \) || echo 0 ))
	$(eval TO_WRITE = ╔$(shell f=0; while [ $$((f+=1)) -le 8 ]; do printf ═; done;) $(TO_WRITE))
	$(eval TO_WRITE = $(TO_WRITE) $(shell f=$(WRITE_LENGTH); while [ $$((f+=1)) -le $(COLUMNS) ]; do printf ═; done;)╗)
	$(call echo_cyan,$(TO_WRITE))
	$(eval TO_WRITE = "")
	$(eval WRITE_LENGTH = 0)
endef

define after_job
	$(eval TO_WRITE = $(strip $(1)))
	$(eval WRITE_LENGTH = $(shell expr \( 11 + \( "X$(TO_WRITE)" : ".*" \) \) || echo 0 ))
	$(eval TO_WRITE = ╚$(shell f=0; while [ $$((f+=1)) -le 8 ]; do printf ═; done;) $(TO_WRITE))
	$(eval TO_WRITE = $(TO_WRITE) $(shell f=$(WRITE_LENGTH); while [ $$((f+=1)) -le $(COLUMNS) ]; do printf ═; done;)╝)
	$(call echo_green,$(TO_WRITE))
	$(eval TO_WRITE = "")
	$(eval WRITE_LENGTH = 0)
endef

# Make this makefile self-documented with target `help`
.PHONY: help
.DEFAULT_GOAL := help

help:
	@grep -Eh '^[0-9a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

compile: ## Compile the main file
	g++ -std=c++11 -pipe -Wall -O3 -c *.cpp
	g++ -std=c++11 *.o -o main

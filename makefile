all: setup generate
	

setup:
	@cd src_setup && $(MAKE)

generate:
	@cd src_generate && $(MAKE)
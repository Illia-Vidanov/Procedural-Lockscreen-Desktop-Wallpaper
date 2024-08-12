all: setup generate
	

setup:
	$(MAKE) -f src/setup/makefile

generate:
	$(MAKE) -f src/generate/makefile
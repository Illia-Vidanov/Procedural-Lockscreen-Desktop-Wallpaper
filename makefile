all: setup generate
	

setup:
	@make -f makefiles/MakeSetup

generate:
	@make -f makefiles/MakeGenerate
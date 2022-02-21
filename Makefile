build:
	gcc tema1.c list.c list.h -g -o tema
	
clean:
	rm tema

checker:
	python3 checker.py

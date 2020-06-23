build:
	gcc ./src/*.cpp -o Allegro -I include -L lib -l SDL2-2.0.0

debug:
	gcc -g ./src/*.cpp -o Debug -I include -L lib -l SDL2-2.0.0

run:
	./Allegro

clean:
	rm Allegro

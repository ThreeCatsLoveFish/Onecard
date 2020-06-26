objects = build/Card.o build/Game.o build/Player.o build/PlayerCard.o build/server.o build/main.o
edit: $(objects)
	gcc -o OneCardServer.exe $(objects) -lws2_32
build/main.o: build/main.c build/main.h
	gcc -c build/main.c -o build/main.o
build/Card.o: build/Card.c build/Card.h
	gcc -c build/Card.c -o build/Card.o
build/PlayerCard.o: build/PlayerCard.c build/PlayerCard.h build/Card.h
	gcc -c build/PlayerCard.c -o build/PlayerCard.o
build/Player.o: build/Player.c build/Player.h build/PlayerCard.h build/Card.h
	gcc -c build/Player.c -o build/Player.o
build/Game.o: build/Game.c build/Game.h build/Player.h build/PlayerCard.h build/Card.h
	gcc -c build/Game.c -o build/Game.o
build/server.o: build/server.c build/server.h
	gcc -c build/server.c -o build/server.o
clean:
	rm edit $(objects)
cprompt: obj/main.o obj/callext.o obj/cprapp.o obj/cprexec.o obj/cpr_parser.o obj/cprtypes.o obj/OutsideCall.o obj/rpnlist.o
	g++ $(CFLAGS) -ldl -o cprompt obj/main.o obj/callext.o obj/cprapp.o obj/cprexec.o obj/cpr_parser.o obj/cprtypes.o obj/OutsideCall.o obj/rpnlist.o

obj/main.o: src/main.cpp
	gcc $(CFLAGS) -c src/main.cpp -o obj/main.o

obj/callext.o: src/callext.cpp
	gcc $(CFLAGS) -c src/callext.cpp -o obj/callext.o

obj/cprapp.o: src/cprapp.cpp
	gcc $(CFLAGS) -c src/cprapp.cpp -o obj/cprapp.o

obj/cprexec.o: src/cprexec.cpp
	gcc $(CFLAGS) -c src/cprexec.cpp -o obj/cprexec.o

obj/cpr_parser.o: src/cpr_parser.cpp
	gcc $(CFLAGS) -c src/cpr_parser.cpp -o obj/cpr_parser.o

obj/cprtypes.o: src/cprtypes.cpp
	gcc $(CFLAGS) -c src/cprtypes.cpp -o obj/cprtypes.o

obj/OutsideCall.o: src/OutsideCall.s
	gcc $(CFLAGS) -c src/OutsideCall.s -o obj/OutsideCall.o

obj/rpnlist.o: src/rpnlist.cpp
	gcc $(CFLAGS) -c src/rpnlist.cpp -o obj/rpnlist.o


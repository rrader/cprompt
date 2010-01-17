cprompt: obj/main.o obj/callext.o obj/cprapp.o obj/cprexec.o obj/cpr_parser.o obj/cprtypes.o obj/OutsideCall.o obj/rpnlist.o
	g++ $(CFLAGS) -ldl -w -o bin/CPrompt obj/main.o obj/callext.o obj/cprapp.o obj/cprexec.o obj/cpr_parser.o obj/cprtypes.o obj/OutsideCall.o obj/rpnlist.o

obj/main.o: src/main.cpp
	g++ $(CFLAGS) -w -c src/main.cpp -o obj/main.o

obj/callext.o: src/callext.cpp
	g++ $(CFLAGS) -w -c src/callext.cpp -o obj/callext.o

obj/cprapp.o: src/cprapp.cpp
	g++ $(CFLAGS) -w -c src/cprapp.cpp -o obj/cprapp.o

obj/cprexec.o: src/cprexec.cpp
	g++ $(CFLAGS) -w -c src/cprexec.cpp -o obj/cprexec.o

obj/cpr_parser.o: src/cpr_parser.cpp
	g++ $(CFLAGS) -w -c src/cpr_parser.cpp -o obj/cpr_parser.o

obj/cprtypes.o: src/cprtypes.cpp
	g++ $(CFLAGS) -w -c src/cprtypes.cpp -o obj/cprtypes.o

obj/OutsideCall.o: src/OutsideCall.s
	g++ $(CFLAGS) -w -c src/OutsideCall.s -o obj/OutsideCall.o

obj/rpnlist.o: src/rpnlist.cpp
	g++ $(CFLAGS) -w -c src/rpnlist.cpp -o obj/rpnlist.o


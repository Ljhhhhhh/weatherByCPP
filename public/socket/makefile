all: demo01 demo02 demo03 demo04 demo05 demo06 demo07 demo08 demo10 \
	demo11 demo12 demo13 demo14 demo15 demo16 demo17 demo19 demo20 \
	demo21 client tcpselect tcppoll tcpepoll

demo01: demo01.cpp
	g++ -g -o demo01 demo01.cpp -lm -lc

demo02: demo02.cpp
	g++ -g -o demo02 demo02.cpp -lm -lc

demo03: demo03.cpp
	g++ -g -o demo03 demo03.cpp -lm -lc

demo04: demo04.cpp
	g++ -g -o demo04 demo04.cpp -lm -lc

demo05: demo05.cpp
	g++ -g -o demo05 demo05.cpp ../_public.cpp -lm -lc

demo06: demo06.cpp
	g++ -g -o demo06 demo06.cpp ../_public.cpp -lm -lc

demo07: demo07.cpp
	g++ -g -o demo07 demo07.cpp ../_public.cpp -lm -lc

demo08: demo08.cpp
	g++ -g -o demo08 demo08.cpp ../_public.cpp -lm -lc

demo10: demo10.cpp
	g++ -g -o demo10 demo10.cpp ../_public.cpp -lm -lc

demo11: demo11.cpp
	g++ -g -o demo11 demo11.cpp ../_public.cpp -lm -lc

demo12: demo12.cpp
	g++ -g -o demo12 demo12.cpp ../_public.cpp -lm -lc

demo13: demo13.cpp
	g++ -g -o demo13 demo13.cpp ../_public.cpp -lm -lc

demo14: demo14.cpp
	g++ -g -o demo14 demo14.cpp ../_public.cpp -lm -lc

demo15: demo15.cpp
	g++ -g -o demo15 demo15.cpp ../_public.cpp -lpthread -lm -lc

demo16: demo16.cpp
	g++ -g -o demo16 demo16.cpp ../_public.cpp -lm -lc

demo17: demo17.cpp
	g++ -g -o demo17 demo17.cpp ../_public.cpp -lm -lc

demo19: demo19.cpp
	g++ -g -o demo19 demo19.cpp ../_public.cpp -lm -lc

demo20: demo20.cpp
	g++ -g -o demo20 demo20.cpp ../_public.cpp -lm -lc

demo21: demo21.cpp
	g++ -g -o demo21 demo21.cpp ../_public.cpp -lm -lc

client:client.cpp
	g++ -g -o client client.cpp ../_public.cpp -lm -lc

tcpselect:tcpselect.cpp
	g++ -g -o tcpselect tcpselect.cpp ../_public.cpp -lm -lc

tcppoll:tcppoll.cpp
	g++ -g -o tcppoll tcppoll.cpp ../_public.cpp -lm -lc

tcpepoll:tcpepoll.cpp
	g++ -g -o tcpepoll tcpepoll.cpp ../_public.cpp -lm -lc

clean:
	rm -f demo01 demo02 demo03 demo04 demo05 demo06 demo07 demo08 demo10 
	rm -f demo11 demo12 demo13 demo14 demo15 demo16 demo17 demo19 demo20
	rm -f demo21 client tcpselect tcppoll tcpepoll

default:
	g++ main.c CANlib.cpp -g -o main -I /home/karachatzis/Downloads/linuxcan/include -I. -L/home/karachatzis/Downloads/linuxcan/canlib -lcanlib

stubbed:
	g++ main.c CANlib.cpp -g stubs.c -o main -I /home/karachatzis/Downloads/linuxcan/include -I.

all: 
	g++ -o healthcenterserver healthcenterserver.cpp -lsocket -lnsl -lresolv
	g++ -o patient1 patient1.cpp -lsocket -lnsl -lresolv
	g++ -o patient2 patient2.cpp -lsocket -lnsl -lresolv
	g++ -o doctor1 doctor1.cpp -lsocket -lnsl -lresolv
	g++ -o doctor2 doctor2.cpp -lsocket -lnsl -lresolv
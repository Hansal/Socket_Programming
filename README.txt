****************************************************************************************************
NAME: 	HANSAL KAVAN SHAH
USC ID: 3352-8628-13
****************************************************************************************************

MY WORK:
I have created a TCP connection between the health center server and two patients (patient1 and patient2). 
Once the connection is established, the patient must authenticate himself by providing the username and
Password. Once authenticated, the health center server will send a list of appointment and the patient 
needs to confirm any one from the available list. This concludes PHASE 2. In the next phase, I created 
an UDP connection for the communication between the patient and doctor. The patient will send the type 
of insurance he has and in return the doctor will send an estimated cost.
***************************************************************************************************************

I have 5 cpp files and they are as follows:
	1.healthcenterserver.cpp
		In this file, I have created a server using TCP socket. The server first authenticates the patient and only
		then send the list of available appointment to the patient. Block of codes on how to create socket, bind it, 
		assign addresses to it, is taken from http://www.linuxhowtos.org/data/6/server.c The server has a 
		STATIC PORT NUMBER= 21813 and IP address is assigned using LOOPBACK (127.0.0.1). Also the server
		is listening continuously so the user needs to break out of the infinite loop i.e. (Ctrl+C).
		
	2.doctor1.cpp and 3.doctor2.cpp
		In this file, I have created an UDP socket which sends an estimated cost to the patient. Block of codes 
		on UDP socket have been referred from http://www.programminglogic.com/sockets-programming-in-c-using-udp-datagrams/
		Doctor1 has a port number 41813 while Doctor2 has a port number 42813. The doctor is listening continuously so 
		the user needs to break out of the infinite loop i.e.(Ctrl+C).
	
	4. patient1.cpp and 5.patient2.cpp
		In this file, the patient communicates to the server via TCP sockets while it communicates to the doctor via UDP
		sockets. The patient needs to authenticate himself and get the list of available appointment. Then he needs to 
		tell the doctor about the insurance and in return the doctor will give him the estimated cost. Block of codes is
		used from http://mech.math.msu.su/~vvb/Java/samples/Invert/client.cpp
*******************************************************************************************************************************

HOW TO RUN MY PROGRAM:
	1. In availabilities.txt, please change the port number of doctor1 to 41813 and doctor2 to 42813.

	2. On the putty terminal execute the make file by typing "make -f Makefile.mak" (without quotes). This will compile all 
	 the 5 cpp files. Along with this, open 4 more terminals (1 each for 1 .cpp file).
	
	3. It is necessary to run the healthcenterserver first so that it reads the database of the clients. The executable file 
	 name is "healthcenterserver" (without quotes).
		Note: Here the server is in infinite loop that is the server will passively listen always to any client. In order 
		to break from the loop, type "Ctrl+c" (without quotes).		
	
	4. Now run the doctor file. This can be done by typing "doctor1" or "doctor2" (without quotes).
		Note: The doctor1 and doctor2 needs to be opened in two different terminals. The doctor will remain idle
		(that is in listening state) for PHASE 1 and 2. It is possible to execute the patient file before the doc. 
		But since doctor#.cpp is behaving like a server, it is a good practise to run the doctor before the patient.
	
	5. The patient must now be executed using "patient1" or "patient2" (without quotes).
		Note: Incase the patient enters an appointment slot that is not mentioned, the program will ask him to enter 
		again and again	till he enters the correct index value.	
		
	6. Sometimes due to zombie process or because the port is in use the bind process gives error. So we need to wait for
	 a while and retry.

	7.Observe all the cout message.	
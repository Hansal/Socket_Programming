#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<arpa/inet.h>
using namespace std;

//function to read the file
vector <string> readfile(string s)
{   string line,l;
    ifstream myfile (s.c_str(), ios::binary);
    if (myfile.is_open())
    {   int i=0;
        vector<string> lines(15);
        while (getline(myfile, line))
        {
            lines[i] = line;
            i++;
        }
        myfile.close();
        return lines;
    }
    else
    {
        cout<<"Error in file:"<<endl;
     }
};

// function to split the contents in file
vector<string> split(vector<string> sp)
{
    size_t pos=0; vector<string> token(50);
    static const size_t npos=-1;
    int x=0,y=0;
    while(!sp[y].empty())
     {
         while((pos=sp[y].find_first_of(" "))!= npos)
        {
         token[x]=sp[y].substr(0,pos);
         sp[y].erase(0, pos+1);
         x=x+1;
        }

     token[x]=sp[y];
     x=x+1;
     y=y+1;
    }
    return token;
};


int main() {
    
    vector<string> u1=readfile("patient2.txt");     
     vector<string> u=split(u1);
     
	 //the code for creating socket, binding it and filling the address is taken partially from
	 //http://mech.math.msu.su/~vvb/Java/samples/Invert/client.cpp
	 
    // Create a socket
    int s0 = socket(AF_INET, SOCK_STREAM, 0);
    if (s0 < 0) {
        cout<<"Error in creating a socket"<<endl; exit(1);
    }

    // Fill in the address of server
    struct sockaddr_in peeraddr;
    int peeraddr_len;
    memset(&peeraddr, 0, sizeof(peeraddr));
    char* peerHost = "localhost";

    // Get the server address
    struct hostent *host = gethostbyname(peerHost);
    if (host == NULL) {
        cout<<"Error in binding a socket"<<endl; exit(1);
    }
    peeraddr.sin_family = AF_INET;
    short peerPort = 21813;
     peeraddr.sin_port = htons(peerPort);     

    // Write resolved IP address of a server to the address structure
    memmove(&(peeraddr.sin_addr.s_addr), host->h_addr_list[0], 4);

    // Connect to a remote server
    int res = connect(s0, (struct sockaddr*) &peeraddr, sizeof(peeraddr));
    if (res < 0) {
        cout<<"Error in connect"<<endl; exit(1);
    }
    struct sockaddr_in client;
    socklen_t cli=sizeof(client);
     if((getsockname(s0, (struct sockaddr *)&client, &cli))!=0) // to get the port number and IP address
     {cout<<"error in getsock "<<endl;
     }
     int port=(int)ntohs(client.sin_port);
     string ip=inet_ntoa(client.sin_addr);
    
    cout<<"Phase 1: Patient 2 has TCP port number "<<port<<" and IP address "<<ip<<endl;

    char buffer[1024], buffer1[1024], apoint[1024];
    
    cout<<"Phase 1: Authentication request from Patient 2 with username ";
    cout<<u[0]<<" and password "<<u[1];
    cout<<" has been sent to the Health Center Server."<<endl;
    memset(buffer,0,1024);
    int y=write(s0, "authenticate", 13);
    if (y<0)
    {cout<<"error in write"<<endl;
    }    
    int n=read(s0,buffer,1024);
    
    memset(buffer,0,1024);
     y=write(s0, u[0].c_str(), u[0].length()); //Sending the username to server
    if (y<0)
    {cout<<"error in writing username"<<endl;
    }    
    n=read(s0,buffer,1024);
    
    memset(buffer1,0,1024);   
    int z=write(s0, u[1].c_str(), u[1].length()); //Sending the password to server
    if (z<0)
    {cout<<"error in writing password"<<endl;
    }

    n=read(s0,buffer1,1024);
    
    
    cout<<"Phase 1: Patient 2 authentication result: "<<buffer1<<"."<<endl;
    
    string str=string (buffer1); //Need to exit the program if Authentication failed
    if (str=="Failure")    
    {
     close(s0);
     exit(1);
     }
	 else
	 cout<<"Phase 1: End of Phase 1 for Patient 2."<<endl;
     
     //     PHASE 2 BEGINS HERE*************************************************************************************
     memset(buffer,0,1024);memset(buffer1,0,1024);memset(apoint,0,1024);
     n=write(s0,"available",1024);
     if (n<0){cout<<"error in writing available"<<endl;
     }
     
     n=read(s0, apoint,1024);
     if (n<0){cout<<"error in reading the appointments"<<endl;
     }
     cout<<"Phase 2: The following appointments are available for patient 2:"<<endl<<apoint<<endl;
    char num[1024]; int inum; int dummy =1;
    while(dummy!=0){ // this while loop will ask the patient to enter the correct index number:
	
		cout<<"Please enter the preferred appointment index and press enter:"<<endl;	
		cin>>num;
		string list1 = string(apoint);
		size_t npos=-1;
		size_t pos=0; vector<string> index_array(50);
		int x=0, count=0;
		while((pos=list1.find_first_of("\n"))!= npos)
        {
         index_array[x]=list1.substr(0,pos);
         list1.erase(0, pos+1);
         x=x+1;  count=count+1;
        }
        index_array[x]=list1;
        x=x+1;
        count=count+1;        
        vector<string> list=split(index_array);
		string number=string(num);
        for (int j=0;j<3*(count-1);j=j+3)
		{	if (number==list[j])
            {
			dummy=0;
            }
		}
      if (dummy==1)            
      {
      cout<<"You have enteed an invalid entry, Please try again."<<endl;// from the following appointments and press enter"<<endl<<apoint<<endl;
      }
    }
    
    n=write(s0,num,1024); //sending the appointment number to the server
     if (n<0){cout<<"error in writing num"<<endl;
     }
     memset(buffer,0,1024);
      n=read(s0, buffer,1024);
      if (n<0){cout<<"error in reading failure of appointment"<<endl;
     } 
      int flag=buffer[0]-'0'; //Logic to recieve the corresponding doctor and port number if available.
      n=write(s0,"for synch",1024);
      if (!flag)
        { cout<<" Phase 2: The requested appointment from Patient 2 is not available. Exiting..."<<endl; 
        exit(1);
        }
      else
        {
        n=read(s0, buffer1,1024);
                if (n<0){cout<<"error in reading the doc name and port"<<endl;
                        }
        cout<<"Phase 2:The requested Appointment is available and reserved to patient 2.";
        cout<<"The assigned doctor and port number is "<<buffer1<<"."<<endl;
      }
     n=write(s0,"for synch",1024);
     
     memset(buffer,0,1024);
      n=read(s0,buffer,1024);
      if(n<0)
     {cout<<"Error recieving port"<<endl;
     }
     n=write(s0,"for synch",1024);
     memset(buffer1,0,1024);
     n=read(s0,buffer1,1024);
     if(n<0)
     {cout<<"Error recieving doc_name"<<endl;
     }
	 close(s0);// close the TCP connection 
     
     // PHASE 3: BEGIN HERE***********************************************
     
     int clientSockudp, udpport=atoi(buffer);
    char buffer_udp[1024];
    struct sockaddr_in serverAddr;
    socklen_t addr_size = sizeof serverAddr;
    struct sockaddr_in client_udp;
    socklen_t cli_udp=sizeof(client_udp);
    
    //reading insurance file
    vector<string> patient_insurance=readfile("patient2insurance.txt");
    
    //Create UDP socket
    clientSockudp = socket(PF_INET, SOCK_DGRAM, 0);

    // Fill in the address of UDP server
    memset((char *) &serverAddr, 0,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(udpport);
    serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); 
    char ip_udp_server[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(serverAddr.sin_addr), ip_udp_server, INET_ADDRSTRLEN); 
       
    memset(buffer_udp,0,1024);
    // sending the insurance details to the doctor
     n=sendto(clientSockudp,patient_insurance[0].c_str(),1024,0,(struct sockaddr *)&serverAddr,addr_size);
    if (n<0)
    {   cout<<"error in sending which insurance :"<<endl;
    }
    // to get the port number of the client udp
     if((getsockname(clientSockudp, (struct sockaddr *)&client_udp, &cli_udp))!=0)
     {cout<<"error in getsock "<<endl;
     }
     int cli_udpport=(int)ntohs(client_udp.sin_port);
     cout<<"Phase 3: Patient 2 has a dynamic UDP port number "<<cli_udpport<<" and IP address "<<ip<<endl; 
     cout<<"Phase 3: The cost estimation request from Patient 2 with insurance plan "<<patient_insurance[0];
     cout<<" has been sent to the doctor with port number "<<buffer<<" and IP address "<<ip_udp_server<<endl;
     
     n = recvfrom(clientSockudp,buffer_udp,1024,0,NULL, NULL);// for synchronization
     // sending the username to the doctor
     n=sendto(clientSockudp,u[0].c_str(),1024,0,(struct sockaddr *)&serverAddr,addr_size);
    if (n<0)
    {   cout<<"error in sending udername :"<<endl;
    exit(0);
    }
    
    //Receive message from server the insurance amount
    n = recvfrom(clientSockudp,buffer_udp,1024,0,NULL, NULL);
    if (n<0)
    {   cout<<"error in recieving the amount :"<<endl;
    }
    cout<<"Phase 3: Patient 2 receives "<<buffer_udp<<"$ estimation cost from doctor with port number ";
    cout<<buffer<<" and name "<<buffer1<<"."<<endl;    
    cout<<"Phase 3: End of Phase 3 for Patient 2."<<endl;
    	
    return 0;
    
    // g++ -o patient2 patient2.cpp -lsocket -lnsl -lresolv
}   
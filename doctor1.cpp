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
    size_t pos=0; vector<string> token(20);
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

int main(){
  int udpSock,udpport=41813;
  char buffer[1024], buffer1[1024], buffer2[1024];
  struct sockaddr_in serverAddr, clientAddr;
   socklen_t cli = sizeof clientAddr;  
  
  //reading and splitting of doc1 file
  vector<string> doc=readfile("doc1.txt");     
     vector<string> doc_file=split(doc);
    
	//Creation of UDP socket and filling the address is referred from
	//http://www.programminglogic.com/sockets-programming-in-c-using-udp-datagrams/
	
  //Create UDP socket
  udpSock = socket(PF_INET, SOCK_DGRAM, 0);

  // This is to fill the ip address and port number of doctor. Note that I have used LOOPBACK ADDRESS
    memset((char *) &serverAddr, 0,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(udpport);
    serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(serverAddr.sin_addr), str, INET_ADDRSTRLEN);
    cout<<"Phase 3: Doctor 1 has a static UDP port "<<udpport<<" and IP address "<<str<<endl;    

  //Bind socket
  int n=bind(udpSock, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if (n<0)
    {cout<<"error in bind "<<endl;
    exit(1);
    }  

  while(1){// infinite loop begin
  
    // Try to receive any incoming UDP datagram.
    n = recvfrom(udpSock,buffer,1024,0,(struct sockaddr *)&clientAddr, &cli);
    if (n<0)
    {cout<<"error in receiving which insurance "<<endl;
    }
    
    // to get the port number and IP address of the UDP client.   
    int port_cli_udp=(int)ntohs(clientAddr.sin_port);
    string ip_udp=inet_ntoa(clientAddr.sin_addr);
    n=sendto(udpSock,"synchronization",1024,0,(struct sockaddr *)&clientAddr,cli);
    
     n = recvfrom(udpSock,buffer1,1024,0,(struct sockaddr *)&clientAddr, &cli);
    if (n<0)
    {cout<<"error in receiving doc_name "<<endl;
    }
    
    cout<<"Phase 3: Doctor 1 receives the request from the patient with port number "<<port_cli_udp;
    cout<<" and the insurance plan "<<buffer<<"."<<endl; 
    
    //to send the amount of insurance
    for (int k=0; k<6; k+=2)
     {
        if(buffer==doc_file[k])
        {   int n=sendto(udpSock,doc_file[k+1].c_str(),1024,0,(struct sockaddr *)&clientAddr,cli);
            if(n<0)
                {cout<<"error in sending the insurance amount "<<endl;
                }
                cout<<"Phase 3: Doctor 1 has sent estimated price "<<doc_file[k+1]<<"$ to patient with port number "<<port_cli_udp<<"."<<endl;
        }
        
     }
  } // infinite loop end

  
  //g++ -o doctor1 doctor1.cpp -lsocket -lnsl -lresolv
  return 0;
}
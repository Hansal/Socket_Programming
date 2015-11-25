#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <typeinfo>
#include <cstring>

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

struct apoint
{
    int index,port;
    string day, time, name;

};

int main()
{
    int sock_new,n,sock,y;
    int port=21813;
    struct sockaddr_in my_addr;
    struct sockaddr_in client_addr;
    char buffer[1024], buffer1[1024], buffer2[1024];
    vector<string> users1=readfile("users.txt"); //read the users.txt and save in users1
	vector<string> users=split(users1); //split the contents of users.txt and save in users
   
      vector<string> availl=readfile("availabilities.txt"); //read the availabilities.txt and save in availl
      vector<string> avail= split(availl); //split the contents of availabilities.txt and save in avail    
      struct apoint h;
      vector<apoint> final_avail;
      char num='10'; int inum;
      inum=num-'0';
    for (int x=0;x<30;x+=5)   // storing the entries in structure created above
    {
        h.index=atoi(avail[x].c_str());
        h.day=avail[x+1];
        h.time=avail[x+2];
        h.name=avail[x+3];
        h.port=atoi(avail[x+4].c_str());
        final_avail.push_back(h);
    }
	
	//the code for creating socket, binding it and filling the address is taken from
	//http://www.linuxhowtos.org/data/6/server.c
	
	
	//assigning the static port number and LOOPBACK IP address to the server
    memset((char *) &my_addr, 0,sizeof(my_addr));
    my_addr.sin_family = AF_INET;   /* address family */
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    char ip_server[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(my_addr.sin_addr), ip_server, INET_ADDRSTRLEN);
    cout<<"Phase1: The Health Center Server has port number "<<port<<" and IP address "<<ip_server<<endl;
	
	// creating a Parent socket
    sock = socket(AF_INET, SOCK_STREAM, NULL);
    if (sock<0)
    {   cout<<"error in socket"<<endl;
		
        exit(1);
    }
 
	//Binding the socket to the given port number
	if ((bind(sock,(struct sockaddr *) & my_addr, sizeof(my_addr))<0))
     {  cout<<"error in bind"<<endl;
		close(sock);
        exit(1);
     }
   
	//Defining the backlog to be 5 at the max
     if (y=listen(sock, 5)<0)
     {	cout<<"error in listen"<<endl;
        exit(1);
     }
	 socklen_t cli=sizeof(client_addr);
	 
	 //infinite loop, because the server needs to listen passively
     for (;;) 
    {
        
		//Accepting the connection from the client and creating a child socket
			if ((sock_new=accept(sock,(struct sockaddr *)& client_addr, &cli))<0)
			{cout<<"unsuccessful in connecting the server :"<<endl;
            }
			//getting the port number and IP address of the client on the server side
            int port_cli=(int)ntohs(client_addr.sin_port);
            string ip_client=inet_ntoa(client_addr.sin_addr);    
            
			memset(buffer,0,1024);
			n= read(sock_new,buffer,1024); // to read Authentication
            if (n<0)
            {   cout<<"error in reading authentication: "<<endl;
            }
            string s2(buffer);
            n=write(sock_new,buffer,1024);// for Synchronization

			memset(buffer1,0,1024);   
			n= read(sock_new,buffer1,1024); //To read Username
			if (n<0)
			{   cout<<"error in reading username: "<<endl;
			}
        
			string username_client(buffer1);
			n=write(sock_new,buffer1,1024);// for Synchronization
        
			memset(buffer2,0,1024);   
			n= read(sock_new,buffer2,1024); // To read Password
			if (n<0)
			{   cout<<"error in reading password: "<<endl;
			}
			string password_client(buffer2);          
			cout<<"Phase 1: The Health Center Server has received request from a  patient with username ";
			cout<<username_client<<" and password "<<password_client<<"."<<endl; 
        
			//n=write(sock_new,buffer2,1024); 
          // cout<<password_client.length()<<" "<<users[1].length()<<endl;
		
		//Logic to compare the username & password and approve or disapprove the patient
		string res;
		if(s2=="authenticate")
		{
          int h=0;
            while(!users[h].empty())
            { string result;
                if (username_client.compare(users[2*h])==0)
                {
                    if (password_client.compare(users[2*h+1])==0)
                    {
                        cout<<"Phase 1: The Health Center Server sends the response success to patient with username "<<username_client<<"."<<endl;
                        result="Success";
                        res="0";
                        n=write(sock_new,result.c_str(),1024);
                    }
                    else
                    {
                        cout<<"Phase 1: The Health Center Server sends the response failure to patient with username "<<username_client<<"."<<endl;
                         result="Failure";
                         res="1";
                         n=write(sock_new,result.c_str(),1024);
                         close(sock_new);
                         
                         
                    }
                }
                h=h+1; //to go to the next line of the users.txt
            }
        }
        else{ cout<<"did not write authenticate"<<endl;
            }
          
       
    
    
        // PHASE 2 BEGINS HERE***********************************************************************
	if(res=="0")
	{    
		memset(buffer,0,1024);memset(buffer1,0,1024);memset(buffer2,0,1024);
		char result[1024] = {0};
		for (int z=0;z<6;z++)
		//int z=0;
		//while(!avail[z].empty())  //Logic to send only the first 3 columns of the availabilitites
		{	
			if((final_avail.at(z).index)!=inum)
			{
			char index[1024]={0};
			strcat(result,index);
			snprintf(index, sizeof(index), "%d", final_avail.at(z).index);
			const char *day=(final_avail.at(z).day).c_str();
			const char *time=(final_avail.at(z).time).c_str();
			strcat(result,index);
			strcat(result," ");
			strcat(result,day);
			strcat(result," ");
			strcat(result,time);
			strcat(result,"\n");
			}
		//z++;
		}
		n= read(sock_new,buffer,1024);
        if (n<0)
        {   cout<<"error in reading available: "<<endl;
        }
        if(buffer=="available");
			{	cout<<"Phase 2: The Health Center Server, receives a request for available time slots from patients";
				cout<<" with port number "<<port_cli<<" and IP address "<<ip_client<<endl;
				n=write(sock_new,result,1024);
				if(n<0)
				{cout<<"error in write while sending appointment"<<endl;
				}
			}
        cout<<"Phase 2: The Health Center Server sends available time slots to patient with username "<<username_client<<"."<<endl;
        
        n= read(sock_new,buffer1,1024); //To read the index number given by the patient
        if (n<0)
        {   cout<<"error in reading num: "<<endl;
        }
        cout<<"Phase 2: The Health Center Server receives a request for appointment "<<buffer1;
        cout<<" from patient with port number "<<port_cli<<" and username "<<username_client<<"."<<endl;
        if(inum== buffer1[0]-'0')
        {   n=write(sock_new,"0",1024);
        
        cout<<"Phase 2: The Health Center Server rejects the following appointment "<<buffer1[0];
        cout<<" to patient with username "<<username_client<<"."<<endl;
        }
        else
        {
        n=write(sock_new,"1",1024);
        cout<<"Phase 2: The Health Center Server confirms the following appointment "<<buffer1[0];
        cout<<" to patient with username "<<username_client<<"."<<endl;
        }
        inum=buffer1[0]-'0';
        n= read(sock_new,buffer1,1024);// for synchronization
        char result1[1024]={0}; char port_doc[1024], name_doc[1024];
		for (int y=0;y<6;y++)  //Logic to send the corresponding Doctor and port number
		{
        if (final_avail.at(y).index==inum)
        {           
            char port[1024]={0};            strcat(result1,port);
            const char *name=(final_avail.at(y).name).c_str();
            snprintf(port, sizeof(port), "%d", final_avail.at(y).port);   
            strncpy(port_doc, port, 1024);     
            strncpy(name_doc, name, 1024); 
            strcat(result1,name);
            strcat(result1," ");
            strcat(result1,port);
            strcat(result1,"\n");
        }
		}
		n=write(sock_new,result1,1024);
		if(n<0)
            {cout<<"error in write while sending doc name and port"<<endl;
            }            
        n= read(sock_new,buffer1,1024);// for synch
     
		n=write(sock_new,port_doc,1024);
		if(n<0)
		{cout<<"Error in sending port"<<endl;
		}
		n= read(sock_new,buffer1,1024);// for synch
		n=write(sock_new,name_doc,1024);
		if(n<0)
		{cout<<"Error in sending name of doc"<<endl;
		}
    }
	close(sock_new);	
    }   //infinite loop end;
    // g++ -o server server.cpp -lsocket -lnsl -lresolv
    return 0;
}

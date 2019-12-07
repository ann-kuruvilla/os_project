#include<bits/stdc++.h>
#include "headerfile.h"
#define MAX_SIZE 9000
struct addr{
	string ip;
	string port;
	string nodeId;
};
struct table
{
	string nodeId;
	string ip;
	string port;
	struct addr nSet[4];
	struct addr lSet[4];
	struct addr rTable[4][16];
}node;
map<char,int> h;
char buffer[MAX_SIZE];
vector<string> filter(string s)
{
	string j="";
	vector<string> v;
	for(int i=0;i<s.length();i++)
	{
		if(s[i]!='#')
		j+=s[i];
		else if(s[i]=='#'||(i+1)==s.length())
		{
			v.push_back(j);
			j="";
		}
	}
	return v;
}
void convert()
{
	for(int i=0;i<16;i++)
	{
		if(i>9)
		{
			h.insert(make_pair('a'+(i-10),i));
		}
		else
		{
			h.insert(make_pair('0'+i,i));
		}
	}
}
string getIp()
{
	struct ifaddrs * ifAddrStruct=NULL;
	struct ifaddrs * ifa=NULL;
	void * tmpAddrPtr=NULL;
	string ip;
	getifaddrs(&ifAddrStruct);
	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
	{
        	if (!ifa->ifa_addr)
		{
            		continue;
        	}
        	if (ifa->ifa_addr->sa_family == AF_INET)
		{ 
            		tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            		char addressBuffer[INET_ADDRSTRLEN];
            		inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
			if(ifa->ifa_name[0] == 'e')
			{
				ip = string(addressBuffer);
				if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
				return ip;
			}
			else if(ifa->ifa_name[0] == 'w')
			{
				ip = string(addressBuffer);
				if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
				return ip;
			}
        	}
    	}
	return ip;
}
string nodeIdCall(string port,string ip)
{
	unsigned char *buff;
	char tt[3];
	int size=port.length();
	size+=ip.length();
	buff=(unsigned char *)malloc(size+1);
	string g=port+ip;	
	for(int i=0;i<size;i++)
	{
		buff[i]=(unsigned char)g[i];
	}
	buff[size]='\0';
	const unsigned char *t=buff;
	char unsigned result[SHA_DIGEST_LENGTH];
	SHA1(t,size, result);
	string kk="";
	for(int i = 0; i < SHA_DIGEST_LENGTH; i++)
	{
		sprintf(tt,"%02x",result[i]);
		string k(tt);
		kk+=k;
	}
	return kk.substr(0,4);		
}
addr path(string id)
{	
	addr temp;
	if(isLeaf(id,temp))
	{
	}
}
void *create_server(void *arg)
{
	int port=stoi(node.port);
	int listenfd = 0;
	struct sockaddr_in serv_addr; 
   	listenfd = socket(AF_INET, SOCK_STREAM, 0);
   	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port); 
	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
	listen(listenfd, 10); 
	for(int i=0;i<4;i++)
	{
		node.nSet[i].nodeId="$";
		node.nSet[i].ip="$";
		node.nSet[i].port="$";
		node.lSet[i].nodeId="$";
		node.lSet[i].ip="$";
		node.lSet[i].port="$";
	}
	addr temp;
	temp.ip=node.ip;
	temp.nodeId=node.nodeId;
	temp.port=node.port;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<16;j++)
		{
			node.rTable[i][j].nodeId="$";
			node.rTable[i][j].ip="$";
			node.rTable[i][j].port="$";
		}
		node.rTable[i][h[node.nodeId[i]]]=temp;
	}
	/*for(int i=0;i<4;i++)
	{
		for(int j=0;j<16;j++)
		{
			cout<<j<<" "<<node.rTable[i][j].nodeId<<" "<<node.rTable[i][j].port<<" "<<node.rTable[i][j].ip<<endl;
		}
	}*/
	while(1)
	{
		int connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		if(connfd==-1)
		{
			cout<<endl<<"Error to connect\n";
		} 
		memset(buffer, 0, sizeof(buffer));
		recv(connfd,buffer,sizeof(buffer),0);
		string msg(buffer);
		vector<string> rev= filter(msg);
		if(rev[0]=="join")
		{
			string id=rev[1],ip=rev[2],port=rev[3];
			addr temp=path(id);
		}
		close(connfd);	
	}
	
}
void sendTo(string data,string i,string p)
{
	struct sockaddr_in serv_addr1;
	int argc=3,sockfd1;
	char *argv[3]={"Self"};
	argv[1]=(char *)malloc(i.length());
	strcpy(argv[1],i.c_str());
	argv[2]=(char *)malloc(p.length());
	strcpy(argv[2],p.c_str());
	
	if(argc != 3)
	{
		printf("\n Usage: %s <ip of server> \n",argv[0]);
		return ;
	} 
	if((sockfd1 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Error : Could not create socket \n");
		return ;
	} 
	memset(&serv_addr1, '0', sizeof(serv_addr1)); 
	serv_addr1.sin_family = AF_INET;
	int port=stoi(p);
	serv_addr1.sin_port = htons(port); 
	printf("Server address used is: %s\n", argv[1]);
	if(inet_pton(AF_INET, argv[1], &serv_addr1.sin_addr)<=0)
	{
		printf("\ninet_pton error occured\n");
		return;
	} 
	if( connect(sockfd1, (struct sockaddr *)&serv_addr1, sizeof(serv_addr1)) < 0)
	{
		printf("\n Error : Connect Failed \n");
		return;
	} 
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer,data.c_str());
	send(sockfd1,buffer,sizeof(buffer),0);
	close(sockfd1);
}
int main()
{
	pthread_t t1;
	string comm;
	convert();
	while(1)
	{
		getline(cin,comm);
		stringstream str(comm);
		string ss;
		str >> ss;
		if(ss=="port")
		{
			str >> ss;
			node.port=ss;
			string y=getIp();
			if(y!="")
			node.ip=y;
			else
			node.ip="127.0.0.1";
			node.nodeId=nodeIdCall(node.port,node.ip);
			cout<<"Ip :"<<node.ip<<endl;
			cout<<"Port :"<<node.port<<endl;
			cout<<"....Node Id generated after SHA hashing.....\n";
			cout<<"Node Id :"<<node.nodeId<<endl<<endl;	
			pthread_create(&t1,NULL,create_server,NULL);
		}
		else if(ss=="join")
		{
			str >> ss;
			string nodeId="join#"+node.nodeId+"#"+node.ip+"#"+node.port;
			str >> ss;
			string ip=ss;
			str >> ss;
			string port=ss;
			sendTo(nodeId,ip,port);
		}
	}
}

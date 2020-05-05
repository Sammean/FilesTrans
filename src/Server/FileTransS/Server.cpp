#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define CHUNK_SIZE (64*1024) 
#ifdef  _WIN32
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#else
#include<unistd.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#define SOCKET int
#endif
#include<stdlib.h>
#include <stdio.h>
#define BUFSIZE 262144 
/*int init_socket()
{
#ifdef _WIN32
	//1������汾
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (HIBYTE(wsaData.wVersion) != 2 || LOBYTE(wsaData.wVersion) != 2)
	{
		printf("����汾ʧ�ܣ�\n");
		return -1;
	}
	else
		printf("�����ɹ���\n");
#endif
	return 0;
}
SOCKET socket_create(char *&ip,char *&cfilename,int port)
{
	if (init_socket() == -1)return 0;
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		printf("����socketʧ�ܣ�\n");
		return 0;
	}
	else printf("����socket�ɹ���\n");
	int num = 0;
	sockaddr_in sockAddr = { 0 };
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.S_un.S_addr = inet_addr(ip);
	sockAddr.sin_port = htons(port);
	int r = bind(serverSocket, (SOCKADDR*)&sockAddr, sizeof sockAddr);
	if (r == -1)
	{
		printf("��ʧ�ܣ�\n");
		return 0;
	}
	else printf("�󶨳ɹ���\n");
	listen(serverSocket, 10);
	if (r == -1)
	{
		printf("����ʧ�ܣ�\n");
		return -1;
	}
	else printf("�����ɹ���\n");
	return serverSocket;
}
SOCKET socket_accept(SOCKET serverSocket)
{
#ifdef _WIN32
	int nSize = 0;
#else
	int nSize = 1;
#endif
	nSize = sizeof(SOCKADDR_IN);
	SOCKADDR_IN clientAddr;
	SOCKET clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr,&nSize);//����  �൱��scanf
	if (clientSocket == SOCKET_ERROR)
	{
		printf("�ͻ��˳���\n");
		return 0;
	}
	else {
		printf("�ͻ������ӳɹ���\n");
		return clientSocket;
	}
}
*/
int tcpip(char* ip, char* cfilename, int port)
{
#ifdef _WIN32


	//1������汾
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (HIBYTE(wsaData.wVersion) != 2 || LOBYTE(wsaData.wVersion) != 2)
	{
		printf("����汾ʧ�ܣ�\n");
		return -1;
	}
	else printf("�����ɹ���\n");
	//2������socket
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		printf("����socketʧ�ܣ�\n");
		return -1;
	}
	else printf("����socket�ɹ���\n");
	//3������Э���ַ��
	char inputip[20] = { 0 };
	//	findIP(ip, sizeof(ip));
	int num = 0;
	strcpy(inputip, ip);
	while (1)
	{
		if (strcmp(ip, inputip) == 0)
			break;
		else
			printf("IP��ַ�뱾�������ϣ�����֪��������ַ���밴1\n");
		int i = 0;
		scanf("%d", &i);
		if (i == 1)
			printf("%s", ip);
		printf("�����뱾��IP��ַ");
	}

	sockaddr_in sockAddr = { 0 };
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.S_un.S_addr = inet_addr(ip);
	sockAddr.sin_port = htons(port);
	//4����
	int r = bind(serverSocket, (SOCKADDR*)&sockAddr, sizeof sockAddr);
	if (r == -1)
	{
		printf("��ʧ�ܣ�\n");
		return -1;
	}
	else printf("�󶨳ɹ���\n");
	//5������
	listen(serverSocket, 10);
	if (r == -1)
	{
		printf("����ʧ�ܣ�\n");
		return -1;
	}
	else printf("�����ɹ���\n");
	//6������
	SOCKADDR clientAddr;
	int nSize = sizeof(SOCKADDR);
	SOCKET clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &nSize);//����  �൱��scanf
	if (clientSocket == SOCKET_ERROR)
	{
		printf("�ͻ��˳���\n");
		return -2;
	}
	printf("�ͻ������ӳɹ���\n");
	//7������
	char fileName[256] = { 0 };
	r = recv(clientSocket, fileName, 255, NULL);
	if (r > 0)
	{
		printf("���յ��ļ�����%s\n", fileName);
	}
	int s = strlen(fileName);
	int j = 0;
	for (int i = 0; i < s; i++)
	{
		if (fileName[i] == '//')
			j++;
	}//ͳ��\�ĸ���
	j /= 2;
	int nj = 0;
	char changer[256] = { 0 };
	changer[0] = '//';
	for (int i = 0; i < s; i++)
	{
		if (nj == j)
		{
			strcat(changer, fileName + i);
			break;
		}
		else if (fileName[i] == '//')
			nj++;
	}
	strcat(cfilename, changer);
	printf("%s\n", cfilename);
	int FileLen = 0;
	int nCurrentPos = 0; //�ϵ�λ��              
	FILE *PosFile= fopen("PosFile.temp", "rb");
	if(PosFile)   
	{                
		fread((char*)&nCurrentPos,sizeof(nCurrentPos),1,PosFile);        //��ȡ�ϵ�λ��                
		printf("The File Pos is %d\n",nCurrentPos);                
		nCurrentPos=nCurrentPos+1;                                //�Ӷϵ����һ�鿪ʼ                
		fclose(PosFile);
		send(clientSocket,(char*)&nCurrentPos,sizeof(nCurrentPos),0);        //���Ͷϵ�ֵ                   
	}        
	else        
	{                
		send(clientSocket,(char*)&nCurrentPos,sizeof(nCurrentPos),0);        //�޶ϵ��ļ�nCurrentPosΪ0                        
	}        
	if(recv(clientSocket,(char*)&FileLen,sizeof(FileLen),0)!=0)//�����ļ�����        
	{                
		int nChunkCount;                
		FILE* file=fopen(cfilename,"rb");                
		nChunkCount=FileLen/CHUNK_SIZE;                //�����ļ�����                
		if(FileLen%nChunkCount!=0)                
		{                        
			nChunkCount++;                
		}                
		if(file)                
		{                        
			fseek(file,nCurrentPos*CHUNK_SIZE,SEEK_SET);        //�ļ�ָ�������ϵ㴦                        
			char *date = new char[CHUNK_SIZE];                        
			for(int i=nCurrentPos;i<nChunkCount;i++)        //�Ӷϵ㴦��ʼд���ļ�                        
			{                                
				printf("Recv The Chunk is %d\n",i);                                
				int nLeft;                                
				if(i+1==nChunkCount)//���һ�� 
				nLeft=FileLen-CHUNK_SIZE*(nChunkCount-1);
				else                                        
					nLeft=CHUNK_SIZE;                                
				int idx=0;                                
				while(nLeft>0)                                
				{                                        
					int ret=recv(clientSocket,&date[idx],nLeft,0);                                        
					if(ret==SOCKET_ERROR)                                        
					{                                                
						printf("Recv Date Error");                                                
						return 0;                                        
					}                                        
					idx+=ret;                                        
					nLeft-=ret;                                
				}                                
				fwrite(date,CHUNK_SIZE,1,file);                                
				FILE *PosFile=fopen("PosFile.temp","wb");                //���ϵ�д��PosFile.temp�ļ�                                
				int seekpos=i+1;                                
				if(PosFile);   
				{                                        
					fwrite((char*)&seekpos,sizeof(seekpos),1,PosFile);                                        
					fclose(PosFile);                                
				}                        
			}                        
			fclose(file);                        
			delete[] date;                
		}                        
		if(DeleteFile("PosFile.temp")!=0)                
		{                        
			printf("�ļ��������");                
		}        
	}
	//int size = 0;
	//r = recv(clientSocket, (char*)&size, 4, NULL);
	//if (r > 0)
	//{
	//	printf("���յ��ļ���С��%d\n", size);
	//}
	//FILE *fp = fopen(cfilename, "wb");
	//int count = 0;
	//char buff[1024];
	//while (1)
	//{
	//	memset(buff, 0, 1024);
	//	r = recv(clientSocket, buff, 1024, NULL);
	//	if (r > 0)
	//	{
	//		count += r;
	//		fwrite(buff, 1, r, fp);
	//		if (count > size)
	//		{
	//			printf("�ļ�������ϣ�\n");
	//			break;
	//		}
	//	}
	//}
	//fclose(fp);
	return 1;
#else
	struct sockaddr_in client_addr;
	bzero(&client_addr, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = htons(ip);
	client_addr.sin_port = htons(port);

	// ����socket�����ɹ�������socket������ 
	int client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket_fd < 0)
	{
		perror("Create Socket Failed:");
		exit(1);
	}

	// �󶨿ͻ��˵�socket�Ϳͻ��˵�socket��ַ�ṹ �Ǳ��� 
	if (-1 == (bind(client_socket_fd, (struct sockaddr*)&client_addr, sizeof(client_addr))))
	{
		perror("Client Bind Failed:");
		exit(1);
	}

	// ����һ���������˵�socket��ַ�ṹ�����÷������Ǳߵ�IP��ַ���˿ڶ�����г�ʼ�������ں�������� 
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	if (inet_pton(AF_INET, ip, &server_addr.sin_addr) == 0)
	{
		perror("Server IP Address Error:");
		exit(1);
	}
	server_addr.sin_port = htons(SERVER_PORT);
	socklen_t server_addr_length = sizeof(server_addr);

	// ��������������ӣ����ӳɹ���client_socket_fd�����˿ͻ��˺ͷ�������һ��socket���� 
	if (connect(client_socket_fd, (struct sockaddr*)&server_addr, server_addr_length) < 0)
	{
		perror("Can Not Connect To Server IP:");
		exit(0);
	}

	// �����ļ��� ���ŵ�������buffer�еȴ����� 
	char file_name[FILE_NAME_MAX_SIZE + 1];
	bzero(file_name, FILE_NAME_MAX_SIZE + 1);
	char buffer[BUFFER_SIZE];
	bzero(buffer, BUFFER_SIZE);
	strncpy(buffer, cfilename, strlen(cfilename) > BUFFER_SIZE ? BUFFER_SIZE : strlen(cfilename));

	// �����������buffer�е����� 
	if (send(client_socket_fd, buffer, BUFFER_SIZE, 0) < 0)
	{
		perror("Send File Name Failed:");
		exit(1);
	}

	// ���ļ���׼��д�� 
	FILE *fp = fopen(cfilename, "w");
	if (NULL == fp)
	{
		printf("File:\t%s Can Not Open To Write\n", cfilename);
		exit(1);
	}

	// �ӷ������������ݵ�buffer�� 
	// ÿ����һ�����ݣ��㽫��д���ļ��У�ѭ��ֱ���ļ������겢д��Ϊֹ 
	bzero(buffer, BUFFER_SIZE);
	int length = 0;
	while ((length = recv(client_socket_fd, buffer, BUFFER_SIZE, 0)) > 0)
	{
		if (fwrite(buffer, sizeof(char), length, fp) < length)
		{
			printf("File:\t%s Write Failed\n", cfilename);
			break;
		}
		bzero(buffer, BUFFER_SIZE);
	}

	// ���ճɹ��󣬹ر��ļ����ر�socket 
	printf("Receive File:\t%s From Server IP Successful!\n", cfilename);
	close(fp);
	close(client_socket_fd);
	return 1;
#endif
}
/*int recv_work(char *ip,char *cfilename,int port)
{
	SOCKET clientSocket = socket_create(ip,cfilename,port);
	if (clientSocket == 0)return 0;
	SOCKET serverSocket = socket_accept(clientSocket);
	if (serverSocket == 0)return 0;
	char fileName[30] = { 0 };
	int r = recv(clientSocket, fileName, 29, NULL);
	if (r > 0)
	{
		printf("���յ��ļ�����%s\n", fileName);
	}
	strcat(cfilename, fileName);
	int size = 0;
	r = recv(clientSocket, (char*)&size, 4, NULL);
	if (r > 0)
	{
		printf("���յ��ļ���С��%d\n", size);
	}
	FILE *fp = fopen(cfilename, "wb");
	int count = 0;
	char buff[1024];
	while (1)
	{
		memset(buff, 0, 1024);
		r = recv(clientSocket, buff, 1024, NULL);
		if (r > 0)
		{
			count += r;
			fwrite(buff, 1, r, fp);
			if (count >=size)
			{
				printf("�ļ�������ϣ�\n");
				break;
			}
		}
	}
	fclose(fp);
	free(buff);

#ifdef  _WIN32
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
#else
	closesocket(clientSocket);
	closesocket(serverSocket);
#endif
	return 1;

}*/
int main(int arg, char *args[])
{
	//if (arg !=4)// �������������Ϊ4��main�����˳�
	//{
	//	printf("usage:server port\n");
	//	return EXIT_FAILURE;
	//}

	char ip[20] = { 0 };
	char cfilename[256] = { 0 };
	char argv[6] = { 0 };
	printf("������:�磨D://file// 192.168.184.1 9999��\n");
	scanf("%s", cfilename);
	scanf("%s", ip);
	scanf("%s", argv);
	//strcpy(cfilename, args[1]);
	//strcpy(ip, args[2]);
	//strcpy(argv, args[3]);
	if (tcpip(ip, cfilename, atoi(argv)) == 1)
		printf("���ճɹ�\n");
	else printf("����ʧ��\n");
	return 0;
}
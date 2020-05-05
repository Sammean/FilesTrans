#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define CHUNK_SIZE (64*1024) 
#include<stdio.h>
#include <stdlib.h>
#include<WinSock2.h>
#include<string.h>
#pragma comment(lib,"ws2_32.lib")

int main() {

	//1������汾
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (HIBYTE(wsaData.wVersion) != 2 || LOBYTE(wsaData.wVersion) != 2)
	{
		printf("����汾ʧ�ܣ�\n");
		return 1;
	}
	else printf("�����ɹ���\n");

	//2������socket
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		printf("����socketʧ�ܣ�\n");
		return -1;
	}
	else printf("����socket�ɹ���\n");

	//3������Э���ַ��
	char ip[20] = { 0 };
	char fileName[256] = { 0 };
	char port[6] = { 0 };//�˿ں�
	int num = 0;
	printf("������:�磨D://file//swift.bmp 192.168.184.1 9999��\n");
	scanf("%s", fileName);
	scanf("%s", ip);
	scanf("%s", port);
	num = atoi(port);
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.S_un.S_addr = inet_addr(ip);//ip��ַ
	sockAddr.sin_port = htons(num);

	//4������
 	int r = connect(clientSocket, (SOCKADDR*)&sockAddr, sizeof sockAddr);
	if (r == SOCKET_ERROR)
	{
		printf("���ӷ���˳���\n");
		return -2;
	}
	else printf("��������ӳɹ���\n");

	//5������

	r = send(clientSocket, fileName, strlen(fileName), NULL);
	if (r > 0)
	{
		printf("�ļ������ͳɹ���%s\n", fileName);
	}
	FILE* fp = fopen(fileName, "rb");
	int size;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	r = send(clientSocket, (char*)&size, 4, NULL);
	if (r > 0)
	{
		printf("�����ļ���С��%d\n", size);
	}
	char buff[1024];
	while (1)
	{              
		int nCurrentPos = 0;//���ܶϵ�ֵ                        
		if(recv(clientSocket,(char*)&nCurrentPos,sizeof(nCurrentPos),0)==SOCKET_ERROR)                        
		{                                
			printf("The Clinet Socket is Closed\n");                                
			break;                        
		}
		else                        
		{                                
			printf("The Currentpos is The %d\n",nCurrentPos);                                      
			printf("Get File Proc is ok\n");
			FILE *file=fopen(fileName, "rb");
			int nChunkCount = 0;        //�ļ�����        
			if (file)
			{
				fseek(file, 0, SEEK_END);
				int FileLen = ftell(file);
				rewind(file);
				if (nCurrentPos != 0)
				{
					fseek(file,nCurrentPos*CHUNK_SIZE, SEEK_SET);        //�ļ�ָ�������ϵ㴦                        
						printf("file seek is %d\n",nCurrentPos * CHUNK_SIZE);
				}
				nChunkCount = FileLen / CHUNK_SIZE;                                //�ļ�����                
				if (FileLen%nChunkCount != 0)nChunkCount++;
				send(clientSocket, (char*)&FileLen, sizeof(FileLen), 0);                //�����ļ�����                
				char *date = new char[CHUNK_SIZE];
				for (int i = nCurrentPos; i < nChunkCount; i++)                //�Ӷϵ㴦�ֿ鷢��                
				{
					printf("send the count %d\n",i );
					int nLeft;
					if (i + 1 == nChunkCount)                                //���һ��                                
						nLeft = FileLen - CHUNK_SIZE * (nChunkCount - 1);
					else
						nLeft = CHUNK_SIZE;
					int idx = 0;
					fread(date, CHUNK_SIZE,1,file);
					while (nLeft > 0)
					{
						int ret = send(clientSocket, &date[idx], nLeft, 0);
						if (ret == SOCKET_ERROR)
						{
							printf("Send The Date Error \n");
							break;
						}
						nLeft -= ret;
						idx += ret;
					}
				}
				fclose(file);
				delete[] date;
			}
			else
			{
				printf("open the file error\n");
			}
		}                
	}
	//�ر�
	closesocket(clientSocket);
	WSACleanup();
	fclose(fp);
	system("pause");
	return 0;
}


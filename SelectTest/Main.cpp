#include <Winsock.h>
#include <string>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")
using namespace std;


#define BUFSIZE 64
#define PORT 9999

int main()
{
	WSAData wsaData;
	SOCKET sHost;
	sockaddr_in addrServ;
	char buf[BUFSIZE];
	int retVal;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "WSAStartupʧ��!" << endl;
		return -1;
	}

	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sHost)
	{
		cout << "socket() ����!" << endl;
		WSACleanup();
		return -1;
	}

	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(PORT);
	addrServ.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	retVal = connect(sHost, (LPSOCKADDR)&addrServ, sizeof(addrServ));
	if (SOCKET_ERROR == retVal)
	{
		cout << "connect ����!" << endl;
		closesocket(sHost);
		WSACleanup();
		return -1;
	}

	while (true)
	{
		cout << "����Ҫ����������������" << endl;
		//         string msg;
		//         getline(cin, msg);
		char msg[BUFSIZE];
		cin.getline(msg, BUFSIZE);
		ZeroMemory(buf, BUFSIZE);
		strcpy_s(buf, msg);
		retVal = send(sHost, buf, strlen(buf), 0);
		if (SOCKET_ERROR == retVal)
		{
			cout << "����ʧ��" << endl;
			closesocket(sHost);
			WSACleanup();
			return -1;
		}

		retVal = recv(sHost, buf, sizeof(buf) + 1, 0);
		cout << "�ӷ������˽��գ�" << buf << endl;
		if (strcmp(buf, "quit") == 0)
		{
			cout << "quit" << endl;
			break;
		}
	}

	closesocket(sHost);
	WSACleanup();


	return 0;
}
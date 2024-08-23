#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <iostream>

#pragma warning(disable : 4996)
#define MY_PORT 8464 


DWORD WINAPI SClint(LPVOID client_socket);

int nclients = 0;

int main(int argc, char* argv[])
{
	setlocale(0, "");
	char buff[1024];
	printf("Сервер\n");


	if (WSAStartup(0x0202, (WSADATA*)&buff[0]))
	{
		printf("Ошибка WSAStartup %d\n", WSAGetLastError());
		return -1;
	}

	SOCKET mysocket;

	if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Ошибка сокета %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(MY_PORT);
	local_addr.sin_addr.s_addr = 0;

	if (bind(mysocket, (sockaddr*)&local_addr, sizeof(local_addr)))
	{
		printf("Ошибка bind %d\n", WSAGetLastError());
		closesocket(mysocket);
		WSACleanup();
		return -1;
	}

	if (listen(mysocket, 0x100))
	{
		printf("Ошибка listen %d\n", WSAGetLastError());
		closesocket(mysocket);
		WSACleanup();
		return -1;
	}
	printf("Ожидание \n");

	SOCKET client_socket;
	sockaddr_in client_addr;

	int client_addr_size = sizeof(client_addr);
	while ((client_socket = accept(mysocket, (sockaddr*)&client_addr, &client_addr_size)))
	{
		nclients++;
		HOSTENT* hst;
		hst = gethostbyaddr((char*)&client_addr.sin_addr.s_addr, 4, AF_INET);
		printf("+%s [%s] подключен новый клиент!\n",
			(hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr));
		if (nclients) printf("%d клиентов о нлайн\n", nclients);else printf("Нет подключенных юзеров\n");
			DWORD thID;
		CreateThread(NULL, NULL, SClint, &client_socket, NULL, &thID);
	}
	return 0;
}

DWORD WINAPI SClint(LPVOID client_socket)
{
	SOCKET my_sock;
	my_sock = ((SOCKET*)client_socket)[0];
	char buff[20 * 1024];
	send(my_sock, "\nВведи номер задания:\n1 - выслать 5\n2 - выслать 4\n3 - выслать 2\n4 - выслать 1 \nquit - выйти из клиента\r\n", sizeof("\nВведи номер задания:\n1 - выслать 5\n2 - выслать 4\n3 - выслать 2\n4 - выслать 1 \nquit - выйти из клиента\r\n"), 0);
	int bytes_recv;



	setlocale(0, "");
	SetConsoleCP(1251);
		

	
	while ((bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR)

	{
		if (!strcmp(&buff[0], "1\n"))
		{
			
			printf("5\n");
			send(my_sock, "2\n", bytes_recv, 0);
		}
		if (!strcmp(&buff[0], "2\n"))
		{

			printf("4\n");

		}
		if (!strcmp(&buff[0], "3\n"))
		{

			printf("2\n");
;
		}
		if (!strcmp(&buff[0], "4\n"))
		{

			printf("1\n");

		}
		send(my_sock, &buff[0], bytes_recv, 0);
	}


	nclients--;
	printf(" -disconnect\n"); if (nclients) printf("%d клиентов о нлайн\n", nclients);else printf("Нет подключенных юзеров\n");
		closesocket(my_sock);
	return 0;
}
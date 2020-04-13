#include<iostream>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

void Error(const char message[]);
int main() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)!=0) {
		Error("�׽�������ʧ��");
	}
	//1. �����׽���
	SOCKET server = socket(PF_INET, SOCK_STREAM, 0);
	if (server == INVALID_SOCKET) {
		Error("Creat socket Error.");
	}
	//2.���׽���
	SOCKADDR_IN serAddr;
	memset(&serAddr, 0, sizeof(serAddr));	//��ʼ��
	serAddr.sin_family = AF_INET;			//�󶨵�ַ��
	serAddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP��ַ
	serAddr.sin_port = htons(8080);			//�󶨶˿�
	if (bind(server, (SOCKADDR*)&serAddr, sizeof(serAddr))==SOCKET_ERROR)
	{
		Error("bind Error.");
	}
	//3.��������˿�
	if (listen(server, 5)) {
		Error("listen Error.");
	}
	//4.�ȴ��ͻ�������
	SOCKADDR_IN client;
	memset(&client, 0, sizeof(client));	//��ʼ��
	char buf[20]{ 0 };
	int client_len = sizeof(client);
	while (true) {
	//5.�ȴ�����
		std::cout << "\n�ȴ��ͻ�������.....\n";
		SOCKET Clien = accept(server, (SOCKADDR*)&client, &client_len);
		if (Clien == SOCKET_ERROR) {
			Error("accept Error.");
		}
		std::cout << "�¼���ͻ��˵�IP��ַΪ��" << inet_ntoa(client.sin_addr) << " �˿ں�Ϊ�� "
			<< ntohs(client.sin_port) << std::endl;
		std::cout << "����������: ";
		std::cin >> buf;
		if (send(Clien, buf, sizeof(buf), 0) ==SOCKET_ERROR) {
			Error("����ʧ��\a\n");
		}
		closesocket(Clien);
	}
	closesocket(server);
	WSACleanup();
}

void Error(const char message[]) {
	fputs(message, stderr);
	fputs("\n", stderr);
	exit(0);
}
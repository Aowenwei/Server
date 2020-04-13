#include<iostream>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

void Error(const char message[]);
int main() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)!=0) {
		Error("套接字启动失败");
	}
	//1. 创建套接字
	SOCKET server = socket(PF_INET, SOCK_STREAM, 0);
	if (server == INVALID_SOCKET) {
		Error("Creat socket Error.");
	}
	//2.绑定套接字
	SOCKADDR_IN serAddr;
	memset(&serAddr, 0, sizeof(serAddr));	//初始化
	serAddr.sin_family = AF_INET;			//绑定地址族
	serAddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址
	serAddr.sin_port = htons(8080);			//绑定端口
	if (bind(server, (SOCKADDR*)&serAddr, sizeof(serAddr))==SOCKET_ERROR)
	{
		Error("bind Error.");
	}
	//3.监听网络端口
	if (listen(server, 5)) {
		Error("listen Error.");
	}
	//4.等待客户端连接
	SOCKADDR_IN client;
	memset(&client, 0, sizeof(client));	//初始化
	char buf[20]{ 0 };
	int client_len = sizeof(client);
	while (true) {
	//5.等待连接
		std::cout << "\n等待客户端连接.....\n";
		SOCKET Clien = accept(server, (SOCKADDR*)&client, &client_len);
		if (Clien == SOCKET_ERROR) {
			Error("accept Error.");
		}
		std::cout << "新加入客户端的IP地址为：" << inet_ntoa(client.sin_addr) << " 端口号为： "
			<< ntohs(client.sin_port) << std::endl;
		std::cout << "请输入数据: ";
		std::cin >> buf;
		if (send(Clien, buf, sizeof(buf), 0) ==SOCKET_ERROR) {
			Error("发送失败\a\n");
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
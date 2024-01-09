#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>// 소켓프로그램 할때 하는 선언문

#pragma comment(lib, "ws2_32")

using namespace std;

int main()
{
	srand((unsigned int)time(nullptr));

	WSAData wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (Result != 0)
	{
		cout << "Winsock dll error : " << GetLastError() << endl; // GetLastError() << 에러코드가 00000 숫자로 뜸 인터넷에 검색해서 오류 해결가능
		exit(-1);
	}

	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // 컴퓨터에 구멍을 뚫는 것, 0써도 됨 / TCP쓰겠다 하는 것, PF_INET방식으로 통신할꺼임
	if (ListenSocket == INVALID_SOCKET)
	{
		cout << "INVALID_SOCKET : " << GetLastError() << endl;
		exit(-1);
	}

	// 주소 세팅
	SOCKADDR_IN ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	//ZeroMemory(&ListenSockAddr, sizeof(ListenSockAddr)); //윈도우에서만 쓰는 것
	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_addr.s_addr = INADDR_ANY; // 나한테 들어오는 IP다 받을께 서버 뭐쓸껀지 IP뭐 쓸껀지
	ListenSockAddr.sin_port = htons(40211);

	Result = bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr)); // 실제 램카드랑 가상으로 만든거랑 연결시키는 것 
	if (Result == SOCKET_ERROR)
	{
		cout << "can't bind : " << GetLastError() << endl;
		exit(-1);
	}

	Result = listen(ListenSocket, 5); // 누가 들어오면 알려줌 0써도 됨
	if (Result == SOCKET_ERROR)
	{
		cout << "can't bind : " << GetLastError() << endl;
		exit(-1);
	}


	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int ClientSocketAddrLength = sizeof(ClientSockAddr);
	SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSocketAddrLength); // 들어온친구 정보 받는 것
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "accept fail : " << GetLastError() << endl;
		exit(-1);
	}

	while (true)
	{
		int FirstNumber = rand() % 10000;
		int SecondNumber = rand() % 9999 + 1;
		int OperatorIndex = rand() % 5;
		char Operator[5] = { '+', '-', '*', '/', '%' };

		char Buffer[1024] = { 0, };
		sprintf_s(Buffer, 1024, "%d%c%d", FirstNumber, Operator[OperatorIndex], SecondNumber);
		//		sprintf(Buffer, "%d%c%d", FirstNumber, Operator[OperatorIndex], SecondNumber);

		int SentByte = send(ClientSocket, Buffer, (int)(strlen(Buffer) + 1), 0);
		if (SentByte < 0)
		{
			cout << "Error : " << GetLastError() << endl;
			continue;
		}
		else if (SentByte == 0)
		{
			cout << "Disconnected : " << GetLastError() << endl;
			continue;
		}
		else
		{
			cout << "Sent byte : " << SentByte << ", " << Buffer << endl;
		}

		char RecvBuffer[1024] = { 0, };
		int RecvByte = recv(ClientSocket, RecvBuffer, sizeof(RecvBuffer), 0); // 최대 크기 가져와야함
		if (RecvByte < 0)
		{
			cout << "Error : " << GetLastError() << endl;
			continue;
		}
		else if (RecvByte == 0)
		{
			cout << "Disconnected : " << GetLastError() << endl;
			continue;
		}
		else
		{
			cout << "Recv byte : " << RecvByte << ", " << RecvBuffer << endl;
		}
	}
	closesocket(ClientSocket); // 소켓 끊는 것
	closesocket(ListenSocket);

	WSACleanup();

	return 0;
}
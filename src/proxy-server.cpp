#include "proxy-server.hpp"

ProxyServer::~ProxyServer(){
	closesocket(sock);
	WSACleanup();
}

mysqlx::SessionSettings ProxyServer::setSessionSettings(const std::string host, unsigned port, 
								const std::string user, const char* pwd, const std::string db){
	mysqlx::SessionSettings settings(host, port, user, pwd, db);
	return settings;
}

int ProxyServer::run(){
	// WSAStartup для инициализации WS2_32.dll
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // Реквест версии Winsock 2.2
	if (iResult != 0) {
		std::cerr << "WSAStartup failed: " << iResult << std::endl;
		return 1;
	}

	// Создать сокет
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cerr << "Error when creating socket: " << strerror(errno) << std::endl;
		WSACleanup();
		return 1;
	}

	// Параметры сокета
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(SERVER_IP);
	address.sin_port = htons(SERVER_PORT);

	// Забиндить сокет
	iResult = bind(sock, (struct sockaddr*)&address, sizeof(address));
	if (iResult == -1) {
		std::cerr << "Error when binding socket: " << strerror(errno) << std::endl;
		closesocket(sock);
		return 1;
	}

	// Сокет слушает
	iResult = listen(sock, 1000);
	if (iResult == -1) {
		std::cerr << "Error when listening: " << strerror(errno) << std::endl;
		closesocket(sock);
		return 1;
	}
	std::cout << "Server set up and running..." << std::endl;

	// Логирование
	Logger::self().LoggerConnect();

	// Сокет принимает соединение
	while (true) {
		int clientSock = accept(sock, nullptr, nullptr);
		if (clientSock == -1) {
			std::cerr << "Error when accepting incoming connection: " << strerror(errno) << std::endl;
			closesocket(sock);
			return 1;
		}

		std::cout << "New connection on proxy-server" << std::endl;

		// Обработка подключения клиента в отдельном треде
		std::thread clientThread(&ProxyServer::handleClient, this, clientSock);
		clientThread.detach();
	}
	return 0;
}

// Отправка sql-query SQL серверу и вывод результата клиенту
void ProxyServer::sendQuery(int clientSock, SQLhandler& SQLhandle){
	bool empty_query = false;
	while(empty_query == false){
		char client_request[BUFFER_SIZE];
		memset(client_request, 0, sizeof(client_request));

		size_t bytesRead = recv(clientSock, client_request, sizeof(client_request) - 1, 0);
		if (bytesRead == -1) {
			std::cerr << "Error when reading data from client: " << strerror(errno) << std::endl;
			closesocket(clientSock);
			return;
		}
		if (((std::string)client_request).empty()){
			empty_query = true;
			break;
		}
		std::cout << "Client data recieved: " << client_request << std::endl;

		// Execute query
		SQLhandle.send_SQL_request(client_request);
		
		// Ответ на sql-query клиенту
		const char* response = client_request;
		size_t bytesSent = send(clientSock, response, strlen(response), 0);
		if (bytesSent == -1) {
			std::cerr << "Error when sending data to client: " << strerror(errno) << std::endl;
			closesocket(clientSock);
			return;
		}
		std::cout << "Data successfully sent to client." << std::endl;
	}
}

// Обработка сервером запроса клиента
void ProxyServer::handleClient(int clientSock) {
	std::lock_guard<std::mutex> lock(mx);

	char client_request[BUFFER_SIZE];
	memset(client_request, 0, sizeof(client_request));

	// Прочесть запрос клиента
	size_t bytesRead = recv(clientSock, client_request, sizeof(client_request) - 1, 0);
	if (bytesRead == -1) {
		std::cerr << "Error when reading data from client: " << strerror(errno) << std::endl;
		closesocket(clientSock);
		return;
	}
	std::cout << "Client credentials received" << std::endl;

	// Сплит переданной строки на user и pswd
	std::vector<std::string> credent;
	std::string temp;
	std::stringstream ss(client_request);
	while (std::getline(ss, temp, ' ')) {
		credent.push_back(temp);
	}

	// Задать настройки подключения
	mysqlx::SessionSettings settings = setSessionSettings(
		SQL_SERVER_IP, SQL_SERVER_PORT, credent[0], credent[1].c_str(), SQL_DATABASE);
	
	// Подключить к SQL Server
	SQLhandler SQLhandle(settings);

	// Коннект к SQL успешен
	const char* response = "Connected successfully to MySQL Server";
	size_t bytesSent = send(clientSock, response, strlen(response), 0);
	if (bytesSent == -1) {
		std::cerr << "Error when sending data to client: " << strerror(errno) << std::endl;
		closesocket(clientSock);
		return;
	}
	std::cout << "Data successfully sent to client." << std::endl;

	// Цикл для принятия запросов клиента
	sendQuery(clientSock, SQLhandle);


	closesocket(clientSock);
	SQLhandle.closeSession();
	std::cout << "Client disconnected" << std::endl;
}

// Дисконнект сокета
void ProxyServer::shutdown(){
	ProxyServer::~ProxyServer();
}
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "proxy.cpp"it 

int main() {
    int server, server_socket;

    struct sockaddr_in address; //структура описания api
    address.sin_family = AF_INET; //константа, обозначающая семейство адресов IPv4
    address.sin_addr.s_addr = INADDR_ANY; //константа в программировании сетевых сокетов (IPv4), обозначающая специальный IP-адрес 0.0.0.0. 
    address.sin_port = htons(8080); //htons(std::stoi(getenv("SERVER_PORT"))); // Порт 8080 //htons - преобразования числа в сетевой байт 

    int opt = 1; //буфер и размер в байта от типа данных 
    int addrlen = sizeof(address);
    
    // 1. Создание сокета
    server = socket(AF_INET, SOCK_STREAM, 0);//ipv4,Тип сокета TCP, Протокол по умолчанию
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));//сокет, уровень , привязка сокета к порту после освобождения

    // 2. Привязка сокета
    bind(server, (struct sockaddr *)&address, sizeof(address));  
    
    // 3. Ожидание подключений
    listen(server, 0);
    std::cout << "Server running on port" << std::endl;
    

    while(1) {

        server_socket = accept(server, (struct sockaddr *)&address, (socklen_t*)&addrlen); // переконнект для освобождения основлного соединения 
        //char buffer[1024] = {0};

        char* ip_str = inet_ntoa(address.sin_addr);
        unsigned short port = ntohs(address.sin_port);
        std::cout << "Connection from: " << ip_str << " Server " << std::to_string(server_socket) << " Port:"<< port <<std::endl;
     
        Proxy proxy(server_socket) ;
        proxy.init();
        proxy.loop();

        // Простой ответ HTTP
        //std::string json_response = "{\"status\": \"ok\", \"message\": \"Hello from C++\"}  ";
        //std::string http_header = 
          //  "HTTP/1.1 200 OK\n"
          //  "Content-Type: application/json\n"
          //  "Content-Length: " + std::to_string(json_response.length()) + "\n\n";
        
        //std::string full_response = http_header + json_response ;
        
        // 4. Отправка ответа
        //send(server_socket, full_response.c_str(), full_response.length(), 0);
        //close(server_socket);
        
    }
    return 0;
}

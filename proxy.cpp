#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <sstream>
#include <map>

class Proxy
{
private:
    int server,client =0 ;
public:
    Proxy(int server){
        //this->client = client;
        this->server = server;
        std::cout << "Init Class" << std::endl;


    };


std::map<std::string,std::string> parser (std::string data){
    std::vector<std::string> message = split(data,"\r\n\r\n");
    //std::cout << "Headers: "<< message[0] << "Body: " << message[1]<< std::endl;
    std::vector<std::string> headers = split(message[0],"\r\n");
    std::map<std::string,std::string> headersMap;

     for (const std::string& s : headers ){
        std::vector<std::string> head = split(s,": ");
        headersMap[head[0]]=head[1];
     }
     
     try{
        std::vector<std::string> temp = split(headersMap["Host"],":");
        headersMap["Host"] = temp[0];
        headersMap["Port"] = temp[1];
     }
     catch(const std::exception& e)
     {
        std::cerr << e.what() << '\n';
     }
     


     return headersMap;
     
    // for (const auto& pair : headersMap) {
      //  std::cout <<"key= " << pair.first << " value= " << pair.second << std::endl;
    //}
    //std::vector<std::string> headers = split(,": ");
    //for (const std::string& s : headers) {
     //   std::cout << s << " ";
    //}
    //std::cout << std::endl;
    //std::cout << data << std::endl;
}

void init (){
    std::cout << "Inition start";
    char buf[2048];
    int bytes_read;
    bytes_read = recv(this->server, buf, 2048, 0);
    std::map<std::string,std::string> headers = parser(buf);


    int client;
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    //addr.sin_port = htons(std::stoi(headers["Port"])); 
    addr.sin_port = htons(443); 
    //addr.sin_addr.s_addr = inet_addr(headers["Host"].c_str());
    addr.sin_addr.s_addr = inet_addr("google.com");
     
    client = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(client, (struct sockaddr *) &addr, sizeof(addr)) < 0 ){ 
        std::string response = "HTTP / 1.1 200 Connection established\nProxy-Agent: THE BB Proxy\n\n";
        send(this->server, response.c_str(), response.length(), 0);
        this->client = client;  
    } else {
        std::string response = "HTTP/1.1 403 Forbidden";
        send(this->server, response.c_str(), response.length(), 0); 
        close(server);
    }  
    std::cout << "Init Conections";
    std::cout << "Client Conect:"<< inet_ntoa(addr.sin_addr) << " Port:" << ntohs(addr.sin_port)<< std::endl;
    
}

void loop (){
    fd_set readfds;
    struct timeval tv;

    // Очистить набор и добавить сокет
    FD_ZERO(&readfds);
    FD_SET(this-> server, &readfds);
    FD_SET(this-> client, &readfds);

    // Установить время ожидания (5 секунд)
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    //Первый аргумент select должен быть равен максимальному номеру дескриптора + 1.
    int max_fd = this->client > this->server ? this->client  : this-> server;

    // Используем select()
    // select(макс_fd + 1, readfds, writefds, exceptfds, timeout)
    std::cout << "Init first loop";
    while (true)
    {
    
    int activity = select(max_fd + 1, &readfds, nullptr, nullptr, &tv);

    if (activity == -1) {
        perror("select()");
    } else if (activity == 0) {
        std::cout << "Таймаут: данных нет за 5 секунд." << std::endl;
    } else {
        int in, out = 0;
        FD_ISSET(this->client, &readfds) ? in = this->client, out = this->server : in = this->server, out = this->client;

        while (true){
            std::cout<< "Init inside loop";
        
        char buf[1024]; // Сообщение которое пришлёт клиент
        int bytes_read; // Размер сообщения в байтах

        if ((bytes_read = recv(in, buf, 1024, 0)) != 0) {
            send(out, buf, sizeof(buf), 0);
        }else{
            std::cout<< "Close inside loop";
            break;
        }
        }
    }
    }




    
}


void print(){
    std::cout <<"Server: "<< this->server<< " Client:  "<< this->client << std::endl;
}
std::vector<std::string> split(std::string str, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    
    // Находим позицию разделителя (например, "::" или "--")
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        tokens.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    // Добавляем оставшуюся часть строки
    tokens.push_back(str);
    
    return tokens;
}



};







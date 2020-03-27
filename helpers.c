#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "parson.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void compute_message(char *message, const char *line)
{
    strcat(message, line);
    strcat(message, "\r\n");
}

int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag)
{
    struct sockaddr_in serv_addr;
    int sockfd = socket(ip_type, socket_type, flag);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = ip_type;
    serv_addr.sin_port = htons(portno);
    inet_aton(host_ip, &serv_addr.sin_addr);
    if (sockfd < 0)
        error("ERROR opening socket");

    /* connect the socket */
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    return sockfd;
}

void close_connection(int sockfd)
{
    close(sockfd);
}

void send_to_server(int sockfd, char *message)
{
    int bytes, sent = 0;
    int total = strlen(message);
    do
    {
        bytes = write(sockfd, message + sent, total - sent);
        if (bytes < 0)
            error("ERROR writing message to socket");
        if (bytes == 0)
            break;
        sent += bytes;
    } while (sent < total);
}

char *receive_from_server(int sockfd)
{
    char *response = calloc(BUFLEN, sizeof(char));
    int total = BUFLEN;
    int received = 0;
    do
    {
        int bytes = read(sockfd, response + received, total - received);
        if (bytes < 0)
            error("ERROR reading response from socket");
        if (bytes == 0)
        {
            break;
        }
        received += bytes;
    } while (received < total);

    if (received == total)
        error("ERROR storing complete response from socket");

    return response;
}

void get_cookies(char str[], char *s1, char *s2)
{
    char *cookie = strstr(str, "Set-Cookie");
    char *cook1 = strtok(cookie, ";");

    strcpy(s1, cook1 + strlen("Set-Cookie: "));

    cook1 = strtok(NULL, "\n");
    cook1 = strtok(NULL, "\n");
    char *cook2 = strtok(cook1, ";");

    strcpy(s2, cook2 + strlen("Set-Cookie: "));
}

char *get_login_credentials(char username[], char password[])
{
    char *buff = (char*) malloc (BUFLEN * sizeof (char));

    strcpy(buff, "username=");
    strcat(buff, username);
    strcat(buff, "&password=");
    strcat(buff, password);

    return buff;
}

// functie care elibereaza 3 zone de memorie
void free_mem(char *s1, char *s2, char *s3)
{
    if(s1 != NULL) free(s1);
    if(s2 != NULL) free(s2);
    if(s3 != NULL) free(s3);
}

// functie care intoarce anumite informatii din JSON-ul din raspuns
void get_json_values(char *response, char *url, char *method, char *type, char *p1, char *p2, char *p3, char *p4)
{
    char *json = strchr(response, '{');
    JSON_Value *value = json_parse_string(json);

    JSON_Object *object = json_value_get_object(value);
    char *url_p = (char*) json_object_get_string(object, "url");
    char *method_p = (char*) json_object_get_string(object, "method");
    char *type_p = (char*) json_object_get_string(object, "type");

    JSON_Object *object2 = json_object_get_object(object, "data");
    char *username_p = (char*) json_object_get_string(object2, "username");
    char *password_p = (char*) json_object_get_string(object2, "password");
    char *token_p = (char*) json_object_get_string(object2, "token");
    char *url2_p = (char*) json_object_get_string(object2, "url");

    JSON_Object *object3 = json_object_get_object(object2, "queryParams");
    char *id_p = (char*) json_object_get_string(object3, "id");
    char *q_p = (char*) json_object_get_string(object3, "q");
    char *appid_p = (char*) json_object_get_string(object3, "APPID");

    if(url_p != NULL) strcpy(url, url_p);
    if(method_p != NULL) strcpy(method, method_p);
    if(type_p != NULL) strcpy(type, type_p);
    if(username_p != NULL) strcpy(p1, username_p);
    if(password_p != NULL) strcpy(p2, password_p);
    if(token_p != NULL) strcpy(p3, token_p);
    if(id_p != NULL) strcpy(p4, id_p);
    if(url2_p != NULL) strcpy(p1, url2_p);
    if(q_p != NULL) strcpy(p2, q_p);
    if(appid_p != NULL) strcpy(p3, appid_p);

    free_mem(url_p, method_p, type_p);
    free_mem(username_p, password_p, token_p);
    free_mem(id_p, url2_p, q_p);
    free_mem(appid_p, NULL, NULL);
}

char *get_url_params(char s1[], char s2[])
{
    char *buff = (char*) malloc (BUFLEN * sizeof (char));

    strcpy(buff, s1);
    strcat(buff, s2);

    return buff;
}

char *get_ip(char* address, int *port)
{
    int ret;
    struct addrinfo hints, *result;
    char *buff = calloc(BUFLEN, sizeof(char));

    // set hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;

    // get addresses
    ret = getaddrinfo(address, "http", &hints, &result);

    if (ret < 0)
        error("Error at \'getaddrinfo\'");

    struct sockaddr_in* addr = (struct sockaddr_in*) result->ai_addr;
    inet_ntop(AF_INET, &addr->sin_addr, buff, BUFLEN);
    *port = ntohs(addr->sin_port);
    
    freeaddrinfo(result);

    return buff;
}
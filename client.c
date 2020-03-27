#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;

    // _____________________________Task1___________________________________________

    // deschiderea conexiunii pe serverul si portul specificat
    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0); 
    message = compute_get_request("185.118.200.35", "/task1/start", NULL, NULL, NULL, NULL);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd);

    printf("task1 done\n");
    // _____________________________Task2___________________________________________

    char str[BUFLEN];
    char cookie1[BUFLEN];
    char cookie2[BUFLEN];
    strcpy(str, response);
    get_cookies(str, cookie1, cookie2); // preluarea cookie-urilor din raspuns

    char *url = calloc (BUFLEN, sizeof (char));
    char *method = calloc (BUFLEN, sizeof (char));
    char *type = calloc (BUFLEN, sizeof (char));
    char *username = calloc (BUFLEN, sizeof (char));
    char *password = calloc (BUFLEN, sizeof (char));

    get_json_values(response, url, method, type, username, password, NULL, NULL); // parsare json      
    char *login = get_login_credentials(username, password);

    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
    message = compute_post_request("185.118.200.35", url, type, cookie1, cookie2, login, NULL);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd);

    // free memory
    free_mem(url, method, type);
    free_mem(username, password, login);

    printf("task2 done\n");
    // _____________________________Task3___________________________________________

    strcpy(str, response);
    strcpy(cookie1, "");
    strcpy(cookie2, "");
    get_cookies(str, cookie1, cookie2);

    url = calloc (BUFLEN, sizeof (char));
    method = calloc (BUFLEN, sizeof (char));
    char *token = calloc (BUFLEN, sizeof (char));
    char *id = calloc (BUFLEN, sizeof (char));

    get_json_values(response, url, method, NULL, NULL, NULL, token, id);  
    char *url_params = get_url_params("raspuns1=Omul&raspuns2=Numele&id=", id);

    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("185.118.200.35", url, url_params, cookie1, cookie2, token);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd);

    free_mem(url, method, url_params);
    free_mem(id, NULL, NULL);

    printf("task3 done\n");
    // _____________________________Task4___________________________________________

    strcpy(str, response);
    strcpy(cookie1, "");
    strcpy(cookie2, "");
    get_cookies(str, cookie1, cookie2);

    url = calloc (BUFLEN, sizeof (char));
    method = calloc (BUFLEN, sizeof (char));

    get_json_values(response, url, method, NULL, NULL, NULL, NULL, NULL);  

    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("185.118.200.35", url, NULL, cookie1, cookie2, token);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd);

    free_mem(url, method, NULL);

    printf("task4 done\n");
    // _____________________________Task5___________________________________________

    strcpy(str, response);
    strcpy(cookie1, "");
    strcpy(cookie2, "");
    get_cookies(str, cookie1, cookie2);

    url = calloc (BUFLEN, sizeof (char));
    method = calloc (BUFLEN, sizeof (char));
    type = calloc (BUFLEN, sizeof (char));
    url_params = calloc (BUFLEN, sizeof (char));
    char *url_tmp = calloc (BUFLEN, sizeof (char));
    char *s1 = calloc (BUFLEN, sizeof (char));
    char *s2 = calloc (BUFLEN, sizeof (char));

    get_json_values(response, url, method, type, url_tmp, s1, s2, NULL);

    // prelucrare parametri url
    strcat(s1, "&APPID=");
    char tmp[BUFLEN];
    strcpy(tmp, "q=");
    strcat(tmp, s1);
    strcpy(s1, tmp);
    url_params = get_url_params(s1, s2);

    // prelucrare nume adresa DNS
    char address[BUFLEN];
    strcpy(address, url_tmp);
    strtok(address, "/");

    // prelucrare port si ip al adresei
    int port = 0;
    char *ip = get_ip(address, &port);

    // prelucrare url pentru get_request
    char url2[BUFLEN];
    strcpy(url2, url_tmp + strlen(address));

    sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request(ip, url2, url_params, cookie1, cookie2, token);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd);

    // preluare json pentru a fi trimis
    char *json = strchr(response, '{');

    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
    message = compute_post_request("185.118.200.35", url, type, cookie1, cookie2, json, token);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd);

    printf("task5 done\n");

    // prelucrare mesaj final
    char *newline;
    char *last_line = response;
    while ((newline = strchr(last_line, '\n')))
        last_line = newline + 1;

    printf("%s\n", last_line);

    free_mem(url, method, type);
    free_mem(url_params, url_tmp, s1);
    free_mem(s2, ip, token);
    free_mem(message, response, NULL);

    return 0;
}
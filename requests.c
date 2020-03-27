#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"

char *compute_get_request(char *host, char *url, char *url_params, char *cook1, char *cook2, char *token)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Se scriu numele metodei, calea, parametri din url (daca exista) si tipul protocolului
    if (url_params != NULL)
    {
        sprintf(line, "GET %s?%s HTTP/1.1", url, url_params);
    }
    else
    {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
    compute_message(message, line);

    // Se adauga host-ul
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    
    // Se adauga headere si/sau cookies, respectand forma protocolului
    if(token != NULL)
    {
        sprintf(line, "Authorization: Basic %s", token);
        compute_message(message, line);
    }
    
    if(cook1 != NULL && cook2 != NULL)
    {
        sprintf(line, "Cookie: %s; %s", cook1, cook2);
        compute_message(message, line);
    }

    // Adaugarea liniei de final
    strcat(message, "\r\n");
    free(line);

    return message;
}

char *compute_post_request(char *host, char *url, char *type, char *cook1, char *cook2, char *form_data, char *token)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Se scriu numele metodei, calea si tipul protocolului
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);

    // Se adauga host-ul
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // Se adauga headere si/sau cookies, respectand forma protocolului
    sprintf(line, "Content-Type: %s", type);
    compute_message(message, line);
    sprintf(line, "Content-Length: %ld", strlen(form_data));
    compute_message(message, line);

    if(token != NULL)
    {
        sprintf(line, "Authorization: Basic %s", token);
        compute_message(message, line);
    }

    sprintf(line, "Cookie: %s; %s", cook1, cook2);
    compute_message(message, line);
    
    // Se adauga linia de final de antent
    strcat(message, "\r\n");

    // Se adauga data
    compute_message(message, form_data);
    free(line);

    return message;
}
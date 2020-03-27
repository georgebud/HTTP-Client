#ifndef _HELPERS_
#define _HELPERS_

#define BUFLEN 4096
#define LINELEN 1000
#define IP_SERVER "185.118.200.37"
#define PORT_SERVER 8081

void error(const char *msg);
void compute_message(char *message, const char *line);
int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag);
void close_connection(int sockfd);
void send_to_server(int sockfd, char *message);
char *receive_from_server(int sockfd);
void get_cookies(char str[], char *s1, char *s2);
char *get_login_credentials(char username[], char password[]);
void free_mem(char *s1, char *s2, char *s3);
void get_json_values(char *response, char *url, char *method, char *type, char *username, char *password, char *token, char *id);
char *get_url_params(char s1[], char s2[]);
char *get_ip(char* address, int *port);

#endif

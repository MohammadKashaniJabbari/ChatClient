#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "winsock2.h"
#include "string.h"
#include "cJSON.c"

int openSocket (void);
void accountMenu (void);
void reg (void);
void login (void);
void mainMenu (char * authToken);
void createChannel (char * authToken);
void joinChannel (char * authToken);
void logout (char * authToken);
void chatMenu (char * authToken);
void message (char * authToken);
void refresh (char * authToken);
void members (char * authToken);
void leave (char * authToken);
void req (char *request, int size);
void rmvqout (char * str);

int main()
{
    accountMenu();
    return 0;
}

int openSocket (void)
{
    int client_socket;
	struct sockaddr_in servaddr;
	WORD wVersionRequested;
    WSADATA wsaData;
    int err;
	// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return -1;
    }
	// Create and verify socket
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		printf("Socket creation failed...\n");
		return -1;
	}
	// Assign IP and port
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(12345);
    // Connect the client socket to server socket
	if (connect(client_socket, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Connection to the server failed...\n");
		return -1;
	}
    return client_socket;
}

void accountMenu(void)
{
    printf ("Account menu:\n1: Register\n2: Log in\n");
    int option;
    scanf ("%d", &option);
    if (option == 1)
        reg ();
    else if (option == 2)
        login ();
    else
    {
        puts ("Please enter a valid number");
        accountMenu();
    }
    return;
}
void reg (void)
{
    char user[200], pass[65];
    printf ("Register:\nEnter username\n");
    scanf ("%s", user);
    printf ("Enter password\n");
    scanf ("%s", pass);
    char request [300] = "register ";
    strcat (request, user);
    strcat (request, ", ");
    strcat (request, pass);
    strcat (request, "\n");
    req (request, 300);
    cJSON *item = cJSON_Parse (request);
    cJSON *type, *content;
    type = cJSON_GetObjectItem (item, "type");
    content = cJSON_GetObjectItem (item, "content");
    char *compare = cJSON_Print(type);
    int answer = strcmp (compare, "\"Successful\"");
    if (answer == 0)
        login ();
    else
    {
        char * output = cJSON_Print(output);
        rmvqout(output);
        printf ("%s\n", output);
        reg ();
    }
    return;
}

void login (void)
{
    char user[200], pass[65];
    printf ("Log in:\nEnter your username\n");
    scanf ("%s", user);
    printf ("Enter your password\n");
    scanf ("%s", pass);
    char request[300] = "login ";
    strcat (request, user);
    strcat (request, ", ");
    strcat (request, pass);
    strcat (request, "\n");
    req (request, 300);
    cJSON *item = cJSON_Parse (request);
    cJSON *type, *content;
    type = cJSON_GetObjectItem (item, "type");
    content = cJSON_GetObjectItem (item, "content");
    char *compare = cJSON_Print(type);
    int answer = strcmp (compare, "\"AuthToken\"");
    if (answer == 0)
    {
        char * authToken = cJSON_Print (content);
        rmvqout (authToken);
        mainMenu (authToken);
    }
    else
    {
        char * output = cJSON_Print(content);
        rmvqout(output);
        printf ("%s\n", output);
        login ();
    }
    return;
}

void mainMenu (char * authToken)
{
    printf ("Main menu:\n1: Create channel\n2: Join channel\n3: Log out\n");
    int option;
    scanf ("%d", &option);
    if (option == 1)
        createChannel (authToken);
    else if (option == 2)
        joinChannel (authToken);
    else if (option == 3)
        logout (authToken);
    else
    {
        puts ("Please enter a valid number");
        mainMenu (authToken);
    }
    return;
}

void logout (char * authToken)
{
    char request[50] = "logout ";
    strcat (request, authToken);
    strcat (request, "\n");
    req (request, 50);
    puts ("You logged out successfully");
    return;
}

void createChannel (char * authToken)
{
    char request[160] = "create channel ";
    char channelName[100];
    puts ("Please enter your channel name");
    scanf ("%s", channelName);
    strcat (request, channelName);
    strcat (request, ", ");
    strcat (request, authToken);
    strcat (request, "\n");
    req (request, 160);
    cJSON *item = cJSON_Parse (request);
    cJSON *type, *content;
    type = cJSON_GetObjectItem (item, "type");
    content = cJSON_GetObjectItem (item, "content");
    char *compare = cJSON_Print(type);
    int answer = strcmp (compare, "\"Successful\"");
    if (answer == 0)
        chatMenu (authToken);
    else
    {
        char * output = cJSON_Print(content);
        rmvqout(output);
        printf ("%s\n", output);
        mainMenu (authToken);
    }
    return;
}
void joinChannel (char * authToken)
{
    char request[160] = "join channel ";
    char channelName[100];
    puts ("Please enter name of channel");
    scanf ("%s", channelName);
    strcat (request, channelName);
    strcat (request, ", ");
    strcat (request, authToken);
    strcat (request, "\n");
    req (request, 160);
    cJSON *item = cJSON_Parse (request);
    cJSON *type, *content;
    type = cJSON_GetObjectItem (item, "type");
    content = cJSON_GetObjectItem (item, "content");
    char *compare = cJSON_Print(type);
    int answer = strcmp (compare, "\"Successful\"");
    if (answer == 0)
        chatMenu (authToken);
    else
    {
        char * output = cJSON_Print(content);
        rmvqout(output);
        printf ("%s\n", output);
        mainMenu (authToken);
    }
    return;
}

void chatMenu (char * authToken)
{
    printf ("Chat menu:\n1: Send messsage\n2: Refresh\n3: Channel members\n4: Leave channel\n");
    int option;
    scanf ("%d", &option);
    if (option == 1)
        message (authToken);
    else if (option == 2)
        refresh (authToken);
    else if (option == 3)
        members (authToken);
    else if (option == 4)
        leave (authToken);
    else
    {
        puts ("Please enter a valid number");
        chatMenu (authToken);
    }
    return;
}

void message (char * authToken)
{
    char request[300] = "send ";
    puts ("Please enter your message");
    char payam[250] = {0}, buffer;
    scanf("%c%[^\n]s", &buffer, payam);
    strcat (request, payam);
    strcat (request, ", ");
    strcat (request, authToken);
    strcat (request, "\n");
    req (request, 300);
    chatMenu (authToken);
    return;
}

void refresh (char * authToken)
{
    char request[10000] = "refresh ";
    puts ("Unseen messages are:");
    strcat (request, authToken);
    strcat (request, "\n");
    req (request, 10000);
    cJSON *item = cJSON_Parse (request);
    cJSON *type;
    type = cJSON_GetObjectItem (item, "type");
    char *compare = cJSON_Print(type);
    int answer = strcmp (compare, "\"List\"");
    cJSON *content = cJSON_GetObjectItem(item, "content");
    for (int i = 0; i < cJSON_GetArraySize(content); i++)
    {
        cJSON *server = cJSON_GetArrayItem (content, i);
        cJSON *sender = cJSON_GetObjectItem (server, "sender");
        cJSON *payam = cJSON_GetObjectItem (server, "content");
        char * output = cJSON_Print(sender);
        rmvqout(output);
        printf ("%s: ", output);
        output = cJSON_Print(content);
        rmvqout(output);
        printf ("%s\n", output);
    }
    chatMenu(authToken);
    return;
}

void members (char * authToken)
{
    char request[10000] = "channel members ";
    puts ("Channel members are:");
    strcat (request, authToken);
    strcat (request, "\n");
    req (request, 10000);
    cJSON *item = cJSON_Parse (request);
    cJSON *type;
    type = cJSON_GetObjectItem (item, "type");
    char *compare = cJSON_Print(type);
    int answer = strcmp (compare, "\"List\"");
    cJSON *content = cJSON_GetObjectItem(item, "content");
    for (int i = 0; i < cJSON_GetArraySize(content); i++)
    {
        cJSON *server = cJSON_GetArrayItem (content, i);
        char * output = cJSON_Print(server);
        rmvqout(output);
        printf ("%s\n", output);
    }
    chatMenu(authToken);
    return;
}

void leave (char * authToken)
{
    char request[300] = "leave ";
    puts ("You left the channel successfully");
    strcat (request, authToken);
    strcat (request, "\n");
    int client_socket = openSocket();
    if (client_socket == -1)
        return;
    send (client_socket, request, sizeof(request), 0);
    shutdown (client_socket, 1);
    memset (request, 0, sizeof(request));
    recv (client_socket, request, sizeof(request), 0);
    shutdown (client_socket, 0);
    closesocket (client_socket);
    mainMenu (authToken);
    return;
}

void req (char *request, int size)
{
    int client_socket = openSocket();
    if (client_socket == -1)
        return;
    send(client_socket, request, size, 0);
    shutdown(client_socket, 1);
    memset(request, 0, size);
    recv(client_socket, request, size, 0);
    shutdown (client_socket, 0);
    closesocket (client_socket);
    return;
}

void rmvqout (char * str)
{
    int i;
    for (i = 1; str[i] != '\"'; i++)
        str [i - 1] = str [i];
    str [i - 1] = '\0';
    return;
}

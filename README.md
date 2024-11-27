# Basic HTTP Server in C 

## Overview
This project demonstrates how to build a basic HTTP server in C that handles GET, POST, PUT requests. It aims to teach the fundamentals of HTTP and server-side programming. ## Features - Handles multiple simultaneous connections using `fork()`. - Supports basic HTTP methods: GET, POST, PUT. - Serves static files from a specified directory. - Simple request parsing and response generation. 

## Project Structure
Project Structure

```
Web-Server
├── src
│   ├── main.c
│   ├── server.c
│   ├── request_handler.c
│   └── response.c
├── include
│   ├── server.h
│   ├── request_handler.h
│   └── response.h
├── web
│   └── index.html
├── license
├── Readme.md
└── Makefile
```

- **src/**: Contains the source code files.
  - `main.c`: The entry point of the application.
  - `server.c`: Manages socket connections, binding, listening, and accepting requests.
  - `request_handler.c`: Parses HTTP requests and handles different methods.
  - `response.c`: Generates and sends HTTP responses.
- **include/**: Contains header files for the project.
  - `server.h`: Header for server functions.
  - `request_handler.h`: Header for request handling functions.
  - `response.h`: Header for response functions.
- **web/**: Contains static files served by the server.
  - `index.html`: Example HTML file.
- **license**: License information for the project.
- **Readme.md**: This readme file.
- **Makefile**: Script to build the project.

## Building the Project
To build the project, run:
```sh
make
```

## Running the Server
To run the server, execute:
```sh
./server
```

The server will start on port 1337 and serve files from the web/ directory.

## Example Requests
```http
GET Request
GET /index.html HTTP/1.0
Host: example.com
```
## Response:

```http
HTTP/1.0 200 OK
Content-Type: text/html; charset=UTF-8
Content-Length: 39

<html><body>Hello, World!</body></html>
```

## POST Request

```http
POST /submit HTTP/1.0
Host: example.com
Content-Length: 13

name=example
```
## Response:

```http
HTTP/1.0 200 OK
Content-Type: text/html; charset=UTF-8
Content-Length: 53

<html><body>Received POST data: name=example</body></html>
```

## PUT Request

```http
PUT /update HTTP/1.0
Host: example.com
Content-Length: 13

name=example
```

## Response:

```http
HTTP/1.0 200 OK
Content-Type: text/html; charset=UTF-8
Content-Length: 53

<html><body>Received PUT data: name=example</body></html>
```


Adding Concurrency
The server uses fork() to handle multiple simultaneous client connections, making it capable of serving multiple requests at the same time.

License
[Your License Information Here](./LICENSE)
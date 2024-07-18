# 项目目录结构

```plaintext
my_web_server/
├── src/
│   ├── main.cpp
│   ├── server/
│   │   └── Server.cpp
│   ├── http/
│   │   ├── HttpRequest.cpp
│   │   └── HttpResponse.cpp
│   ├── utils/
│   │   ├── Logger.cpp
│   │   └── Config.cpp
│   └── handlers/
│       └── RequestHandler.cpp
├── include/
│   ├── server/
│   │   └── Server.h
│   ├── http/
│   │   ├── HttpRequest.h
│   │   └── HttpResponse.h
│   ├── utils/
│   │   ├── Logger.h
│   │   └── Config.h
│   ├── threadpool/
│   │   └── ThreadPool.h
│   └── handlers/
│       └── RequestHandler.h
├── tests/
│   ├── test_http_request.cpp
│   ├── test_http_response.cpp
│   ├── test_logger.cpp
│   └── test_server.cpp
├── .gitignore
├── CMakeLists.txt
└── server.config

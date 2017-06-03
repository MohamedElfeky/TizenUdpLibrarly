
# Tizen 라이브러리

> **사용 순서:**
>1. 어플리케이션 시작할때 라이브러리 초기화(서버 아이피 및 포트 설정)
>2. 클라이언트를 중계서버와 연결하고 통신할 상대방을 찾습니다.
>3. 상대방을 찾으면 라이브러리에서 제공되는 API를 통해 통신을 합니다

---------
#### 사용자 인증 방법
현재 이 라이브러리에서 지원하는 사용자 인증 방법은 2가지로 아래와 같습니다.

> **인증 방법:**
> - 특정 token과 ID값을 양쪽 클라이언트에서 인증
> - 서버에서 한 클라이언트의 인증 토큰 생성 후 해당 토큰으로 인증 -- 미지원 

#### 적용 방법

해당 라이브러리가 지원하는 함수는 다음과 같습니다.
```cpp
extern int tul_init(char * server_ip,int port);

extern int tul_bind_port(int port);

extern int tul_send(char * message,int message_len);

extern int tul_bind_port(int port);

extern int tul_connect(char * token,char * id);

extern int tul_start_listen(Ecore_Thread_Notify_Cb callback, void * data);

extern int tul_stop_listen();

extern int tul_get_time_id(char ** id);
```
각 함수들에 대한 설명

- tul_init 
```cpp
// Send 함수
String token = "사용자 인증 토큰";//인증 과정에서 받아온 토큰
int myCallbackFunction(){//해당 함수가 끝난 후 실행할 콜백 함수
......
};
sendAsync(token,"보낼 메시지",myCallbackFunction);//메시지 보내기
```

- tul_init
```cpp
// Send 함수
String token = "사용자 인증 토큰";//인증 과정에서 받아온 토큰
int myCallbackFunction(){//해당 함수가 끝난 후 실행할 콜백 함수
......
};
sendAsync(token,"보낼 메시지",myCallbackFunction);//메시지 보내기
```

- tul_bind_port
```cpp
// Send 함수
String token = "사용자 인증 토큰";//인증 과정에서 받아온 토큰
int myCallbackFunction(){//해당 함수가 끝난 후 실행할 콜백 함수
......
};
sendAsync(token,"보낼 메시지",myCallbackFunction);//메시지 보내기
```

- tul_send
```cpp
// Send 함수
String token = "사용자 인증 토큰";//인증 과정에서 받아온 토큰
int myCallbackFunction(){//해당 함수가 끝난 후 실행할 콜백 함수
......
};
sendAsync(token,"보낼 메시지",myCallbackFunction);//메시지 보내기
```

- tul_bind_port
```cpp
// Send 함수
String token = "사용자 인증 토큰";//인증 과정에서 받아온 토큰
int myCallbackFunction(){//해당 함수가 끝난 후 실행할 콜백 함수
......
};
sendAsync(token,"보낼 메시지",myCallbackFunction);//메시지 보내기
```

- tul_connect
```cpp
// Send 함수
String token = "사용자 인증 토큰";//인증 과정에서 받아온 토큰
int myCallbackFunction(){//해당 함수가 끝난 후 실행할 콜백 함수
......
};
sendAsync(token,"보낼 메시지",myCallbackFunction);//메시지 보내기
```

- tul_start_listen
```cpp
// Send 함수
String token = "사용자 인증 토큰";//인증 과정에서 받아온 토큰
int myCallbackFunction(){//해당 함수가 끝난 후 실행할 콜백 함수
......
};
sendAsync(token,"보낼 메시지",myCallbackFunction);//메시지 보내기
```

- tul_stop_listen
```cpp
// Send 함수
String token = "사용자 인증 토큰";//인증 과정에서 받아온 토큰
int myCallbackFunction(){//해당 함수가 끝난 후 실행할 콜백 함수
......
};
sendAsync(token,"보낼 메시지",myCallbackFunction);//메시지 보내기
```

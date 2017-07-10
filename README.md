# mongoose 

## 相关知识

- 数据结构

``` c

    struct mg_mgr;           // 持有所有活动的连接的事件管理器
    struct mg_connection ;   // 用于连接的套接字状态的描述
    struct mbuf ;            // 用于接收和发送数据缓存的描述
    
    
  　收发缓冲区 
        每个连接都有自己的收发缓冲区struct mbuf，当Mongoose 接收到数据时数据被追加到接收缓冲区并触发一个MG_EV_RECV 事件。
        当需要发送数据时，只需要使用 mg_send()或者mg_printf()将数据追加到发送缓冲区(struct mg_connection::send_mbuf)，
        当数据发送成功(writes data to the socket)，Mongoose 将数据从发送缓冲区删除并发送一个MG_EV_SEND事件。
        当连接关闭时，触发MG_EV_CLOSE事件。以下为mbuf的定义：
        
        /* Memory buffer descriptor */
        struct mbuf {
          char *buf;   /* Buffer pointer */
          size_t len;  /* Data length. Data is located between offset 0 and len. */
          size_t size; /* Buffer size allocated by realloc(1). Must be >= len */
        };
    
```

- 工作流程

``` c

    1.声明与初始化事件管理器
        struct mg_mgr mgr;
        mg_mgr_init(&mgr, NULL);
        
    2.创建一个连接.例如对于服务器应用而言,创建监听连接
         struct mg_connection *c = mg_bind(&mgr, "80", ev_handler_function);
         mg_set_protocol_http_websocket(c);
         
    3.调用mg_mgr_poll()函数进行事件的循环检测
         for (;;) {
           mg_mgr_poll(&mgr, 1000);
         }
         
     mg_mgr_poll iterates over all sockets, accepts new connections, sends and receives data,
      closes connections and calls event handler functions for the respective events.
        

        
    
```
 [参考资料](http://blog.h5min.cn/weixin_36381867/article/details/61618326)
 
 ## 事件及事件处理函数 
 
``` c
 
     Mongoose 为连接、读写、关闭等都定义了事件，每个连接都有与其关联的事件处理函数——该函数由用户自身实现，该函数的原型如下：
     
     static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
       switch (ev) {
         /* Event handler code that defines behavior of the connection */
         ...
       }
     }
     
```

 - 典型的事件序列如下：
 
 ``` c
     对于客户端：MG_EV_CONNECT -> (MG_EV_RECV, MG_EV_SEND, MG_EV_POLL …) -> MG_EV_CLOSE
     对于服务端： MG_EV_ACCEPT -> (MG_EV_RECV, MG_EV_SEND, MG_EV_POLL …) -> MG_EV_CLOSE
      
```
 
- Mongoose的核心事件：

 ``` c
  
     MG_EV_ACCEPT：当accept到一个新的连接请求时触发该事件，void *ev_data is union socket_address of the remote peer.
     MG_EV_CONNECT：当使用mg_connect() 创建连接套接字时触发该事件，
                    ev_data 为int *success，success为0表示成功，否则为失败的errno
     MG_EV_RECV：接收到新的数据，void *ev_data is int *num_received_bytes. 是接收到的字节数，接收到应该使用recv_mbuf来获取数据，
                 但是需要由用户删除已接收的数据——比如mbuf_remove()。
                使用mg_send()发送数据。Mongoose 使用realloc来扩展接收缓冲区，
     MG_EV_SEND：Mongoose 已经将(int *)ev_data 的数据写到了远端并将数据从send_mbuf中删除。
                mg_send()并不发送数据，只是将数据追加到缓冲区，正真的IO操作由mg_mgr_poll()完成。
     MG_EV_POLL：该事件被发送给所有的已连接套接字，它可以被用来作任何持续性的事务，
                 比如检查某个连接是否已经超时或者关闭、过期，或者用来发送心跳消息。
     MG_EV_TIMER：当mg_set_timer() 被调用时被用来发送给指定的connection
 
 ```

- 连接标记位 mg_connection->flags

 ``` c
 
    每个连接都有自己的标记位，比如当创建一个UDP连接时，Mongoose 将为这个连接的标记为设置为MG_F_UDP。
    
    以下标记为用户设定： 
    - MG_F_SEND_AND_CLOSE：告诉Mongoose 数据已经全部存放到了发送缓冲区，当Mongoose 将数据发送完毕时，主动关闭连接。 
    - MG_F_CLOSE_IMMEDIATELY ：告诉Mongoose 立马关闭连接，一般在出错的时候设置 
    - MG_F_USER_1, MG_F_USER_2, MG_F_USER_3, MG_F_USER_4：用户自定义，用来存放应用的指定状态
    
    以下标记由Mongoose 设定：
    
    MG_F_SSL_HANDSHAKE_DONE SSL：只有在SSL连接中才会设置，当SSL的握手完成时设定
    MG_F_CONNECTING：在调用mg_connect() 后但是连接还没有完成时设置
    MG_F_LISTENING：为所有监听套接字设置
    MG_F_UDP：如果是UDP协议则设置
    MG_F_IS_WEBSOCKET：如果是网络套接字则设置
    MG_F_WEBSOCKET_NO_DEFRAG：由用户希望关闭自动的websocket框架碎片整理时设置
    
  ```
  
- struct mg_connection::user_data 是可以让用户自己保存属于自己的信息

- struct mg_mgr mgr:

``` c

     Mongoose manager is single threaded,它的内部不会通过mutex来其数据安全,
    因此除了mg_broadcast（）函数之外，所有处理特定事件管理器的函数都应该从同一个线程调用
    
```
    
## 接口函数

- 定时器接口

``` c

    double mg_set_timer(struct mg_connection *c, double timestamp);
    
    描述:
        设置一个连接的定时器,当达到条件时,触发ev_handler(struct mg_connection *c, int ev, void *ev_data)的回调函数,
        其中(ev == MG_EV_TIMER)
        
    参数:
        c:对应的特定的连接
        timestamp:定时器触发的时间戳
        
    返回值:
        上一次timestamp的设置值
        
    static void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
        switch (ev) {
    
            case MG_EV_TIMER: {
                double now = *(double *) ev_data;  //该定时器触发时的时间
                double next = mg_set_timer(c, 0) + 2;
                printf("timer event, current time: %.2lf\n", now);
                printf("last mg_set_timer value: %.2lf\n", next - 2);
                mg_set_timer(c, next);  // Send us timer event again after 0.5 seconds
                break;
            }
        }
    }
    
    其中　*(double *) ev_data的值 代表MG_EV_TIMER发生时的时间戳
    
    mg_set_timer(c, 0) 作用是取得其返回值表示上一次timestamp的设置值,且不进行定时器的触发.
    
    mg_set_timer的设置值(时间戳)即使已经过了现在的时间戳,也会触发回调函数.
    
```
　　　　   
- mg_connect_opt 参数接口

``` c

    作为客户端连接到远端的服务器
    
    static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
      int connect_status;
    
      switch (ev) {
        case MG_EV_CONNECT:
          connect_status = * (int *) ev_data;
          if (connect_status == 0) {
            // Success
          } else  {
            // Error
            printf("connect() error: %s\n", strerror(connect_status));
          }
          break;
      
      }
    }
    
    
      mg_connect(mgr, "my_site.com:80", ev_handler);
    
```

- mg_next() 

``` c

    struct mg_connection *mg_next(struct mg_mgr *, struct mg_connection *);
    
    描述:
        循环所有活动的连接
        
    返回值:
        Returns the next connection from the list of active connections　或者 NULL
        
        
     for (c = mg_next(srv, NULL); c != NULL; c = mg_next(srv, c)) {
       // Do something with connection `c`
     }
    
```

- mg_socketpair() 

``` c

    int mg_socketpair(sock_t[2], int sock_type);
    
    描述:
        Creates a socket pair.
        
    参数:
        sock_type :SOCK_STREAM 适用于TCP
                   SOCK_DGRAM 适用于UDP
     
    返回值:
        1: success
        0:fail
    
```

- 创建线程

``` c

    void *mg_start_thread(void *(*thread_func)(void *), void *thread_func_param);
    
    描述:
        跟系统pthread_create()一样,该线程定义为detached属性
        
    参数:
        thread_func:注册的函数名
        thread_func_param:要给函数传入的参数
    
    返回值:
        线程ID:(void *) thread_id
    
```

- mg_add_sock()

``` c

    
    
```

- mg_connect() 客户端连接

``` c

    struct mg_connection *mg_connect(struct mg_mgr *mgr, const char *address,
                                     mg_event_handler_t handler);
                                     
    描述:
        作为客户端调用mg_connect函数,与服务器建立连接.
        
    返回值:
        与服务器建立的连接.
    
```
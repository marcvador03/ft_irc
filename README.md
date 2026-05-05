# ft_irc : IRC Server Implementation

A **custom IRC (Internet Relay Chat) server** written in **C++98**, designed as a project for [School 42](https://www.42.fr/).
This implementation adheres to the [RFC 1459](https://tools.ietf.org/html/rfc1459) and [RFC 2810-2813](https://tools.ietf.org/html/rfc2810) standards, supporting core IRC features like channels, user authentication, and basic commands.
Developed in collaboration with Milosz Pietrzak.
---

## **Features**

-  **User Authentication**: Nickname, username, and password handling.  
- **Channel Management**: Create, join, leave, and manage channels (`#channel`).  
- **IRC Commands**: Supports `NICK`, `USER`, `JOIN`, `PRIVMSG`, `NOTICE`, `PART`, `QUIT`, `PING`, `PONG`, `MODE`, and more.  
- **Message Broadcasting**: Private messages and channel-wide messages.  
- **Server-to-Server Communication**: Basic support for networked IRC servers (if applicable).  
- **Error Handling**: Compliant with IRC error replies (e.g., `ERR_NICKNAMEINUSE`, `ERR_NOSUCHNICK`).  
- **Modular Design**: Clean separation of concerns (e.g., `Server`, `Client`, `Channel` classes).

---

## 🛠 **Technologies & Requirements**

- **Language**: C++98
- **Compiler**: `g++` or `clang++` (tested with `-std=c++98`).
- **Dependencies**:
  - POSIX sockets (`<sys/socket.h>`, `<netinet/in.h>`, etc.).
  - Standard C++ libraries (`<vector>`, `<map>`, `<string>`, etc.).
- **OS**: Linux (tested on Debian/Ubuntu).

---

## **Getting Started**

### 1. **Clone the Repository**

```bash
git clone https://github.com/marcvador03/ft_irc.git
cd ft_irc
```

### 2. **Compile the Server**

```bash
make
```

- **Optional**: Use `make clean` to remove object files, or `make fclean` for a full clean.

### 3. **Run the Server**

```bash
./ircserv <port> <password>
```

- **Arguments**:
  - `<port>`: Port to listen on (e.g., `6667`).
  - `<password>`: Server password (optional, but recommended for security).

### 4. **Connect with an IRC Client**

Use an IRC client like [HexChat](https://hexchat.github.io/), [Irssi](https://irssi.org/), or `nc` (netcat) to connect:

```bash
nc localhost <port>
```

- **Example**:
  ```
  PASS yourpassword
  NICK yournick
  USER yourusername 0 * :Your Name
  JOIN #test
  ```

---

## **Project Structure**

```
ft_irc/
├── src/               # Source files (Server, Client, Channel, etc.)
│   ├── Server.cpp     # Main server logic
│   ├── Client.cpp     # Client handling
│   ├── Channel.cpp    # Channel management
│   └── ...
├── inc/               # Header files
│   ├── Server.hpp
│   ├── Client.hpp
│   └── ...
├── Makefile           # Build configuration
├── irc_config         # Configuration file (if used)
└── README.md          # This file
```

---

## **Configuration**

- **Port**: Default is `6667` (standard IRC port).
- **Password**: Set during startup (e.g., `./ircserv 6667 mypassword`).
- **Max Clients**: Adjustable via `MAX_CLIENTS` in `Server.hpp`.
- **Channel Limits**: Configured in `Channel.hpp` (e.g., `MAX_CHANNELS`, `MAX_NICK_LENGTH`).

---

## **Supported IRC Commands**


| Command       | Description                     | Example                    |
| ------------- | ------------------------------- | -------------------------- |
| `NICK`        | Set/change nickname.            | `NICK newnick`             |
| `USER`        | Register user details.          | `USER user 0 * :Real Name` |
| `JOIN`        | Join a channel.                 | `JOIN #channel`            |
| `PRIVMSG`     | Send a private/channel message. | `PRIVMSG #channel :Hello!` |
| `PART`        | Leave a channel.                | `PART #channel`            |
| `QUIT`        | Disconnect from the server.     | `QUIT :Bye!`               |
| `MODE`        | Set channel/user modes.         | `MODE #channel +i`         |
| `PING`/`PONG` | Keepalive mechanism.            | `PING :server1`            |


---

## **Known Issues & Limitations**

-  **No SSL/TLS**: Communication is unencrypted (for simplicity).
-  **Single-Threaded**: Uses `select()` for I/O multiplexing (no multi-threading).
-  **Limited RFC Compliance**: Some edge cases may not be fully handled.

---

## **Resources**

- [RFC 1459 (IRC Protocol)](https://tools.ietf.org/html/rfc1459)
- [RFC 2810-2813 (IRCv3)](https://tools.ietf.org/html/rfc2810)
- [42 IRC Project Subject](https://github.com/42School/norminette) (if available)
- [IRCv3 Modern IRC](https://ircv3.net/)

---

## **License**

This project is open-source and available under the [MIT License](LICENSE).

---

## **Acknowledgments**

- Big thanks to my project companyon Milosz (https://github.com/mpietrza/mpietrza) !
- Thanks to [École 42](https://www.42.fr/) for the project guidelines.
- We tested AI to generate some functions, and we did not like it... Eventually it helped to generate this README :)

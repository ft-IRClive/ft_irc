# ft_irc

## 📡 Internet Relay Chat Server (IRC)

Proyecto del common core de 42 Madrid llamado `ft_irc`. Es una implementación en **C++** de un servidor **IRC** siguiendo el protocolo definido en **RFC 1459**.
El servidor permite la conexión de múltiples clientes simultáneamente, la gestión de canales y usuarios, y la ejecución de los comandos IRC más comunes.

![Logo 42 Madrid](42-Madrid.jpeg)

---

## 📚 Índice

* [Características](#-características)
* [Demostración rápida](#-demostración-rápida)
* [Compilación](#-compilación)
* [Ejecución](#-ejecución)
* [Conexión al servidor](#-conexión-al-servidor)
* [Comandos implementados](#-comandos-implementados)
* [Arquitectura del servidor](#-arquitectura-del-servidor)
* [Gestión de conexiones](#-gestión-de-conexiones)
* [Manejo de errores](#-manejo-de-errores)

---

## ✨ Características

* Servidor **IRC concurrente** (múltiples clientes).
* Uso de **sockets TCP**.
* I/O multiplexado con **poll()**.
* Gestión de:

  * Clientes
  * Canales
  * Operadores
* Autenticación mediante `PASS`, `NICK` y `USER`.
* Soporte para clientes reales como **HexChat** o **nc**.
* Mensajes y errores conforme al protocolo IRC.

---

## 💻 Demostración rápida

![Demo ft_irc](video_irc.gif)


## 🔨 Compilación

```bash
make
```

Esto generará el ejecutable:

```bash
ircserv
```

Para limpiar archivos objeto:

```bash
make clean
make fclean
```

---

## ▶️ Ejecución

```bash
./ircserv <puerto> <password>
```

Ejemplo:

```bash
./ircserv 6667 password
```

### 📌 Notas

* El puerto debe estar entre **1024 y 65535**.
* El servidor escucha conexiones TCP entrantes en ese puerto.

---

## 🔌 Conexión al servidor

### Con HexChat

* Dirección: `127.0.0.1`
* Puerto: `6667`
* Password: la indicada al lanzar el servidor

### Con netcat (nc)

```bash
nc -C 127.0.0.1 6667
```

Y luego:

```text
PASS password
NICK usuario
USER usuario
```

---

## 🧾 Comandos implementados

### Autenticación

* `PASS`
* `NICK`
* `USER`
* `CAP`

### Canales

* `JOIN`
* `PART`
* `TOPIC`
* `MODE`
* `INVITE`
* `KICK`
* `QUIT`

### Usuarios

* `PRIVMSG`
* `WHO`

---

## 🏗 Arquitectura del servidor

El proyecto está dividido en varias clases principales:

* **Server**

  * Maneja sockets, poll y el loop principal.
* **Client**

  * Representa un usuario conectado.
* **Channel**

  * Representa un canal IRC.

Cada comando se gestiona mediante una función handler específica, seleccionada a partir del comando recibido.

---

## 🔄 Gestión de conexiones

* El servidor utiliza **poll()** para:

  * Aceptar nuevas conexiones.
  * Leer mensajes de clientes existentes.
* Cada cliente está asociado a un **file descriptor**.
* Al desconectarse un cliente:

  * Se elimina del servidor.
  * Se elimina de todos los canales.
  * Se liberan recursos correctamente.

---

## ❌ Manejo de errores

El servidor envía respuestas de error estándar IRC, como:

* `ERR_NOTREGISTERED`
* `ERR_NOSUCHCHANNEL`
* `ERR_NOSUCHNICK`
* `ERR_CHANOPRIVSNEEDED`
* `ERR_NEEDMOREPARAMS`

Esto garantiza compatibilidad con clientes IRC reales.

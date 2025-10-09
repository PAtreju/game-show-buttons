# Game Show Buttons - Server

This is a Node.js server using TypeScript and Express, with WebSocket support and a UDP listener.

## Features

- **HTTP Server**: Express server running on port 3000 (default)
- **WebSocket Server**: Real-time bidirectional communication on the same port
- **UDP Listener**: Responds to UDP messages on port 5555

## Quick Start

1. Install dependencies

   ```bash
   npm install
   ```

2. Build

   ```bash
   npm run build
   ```

3. Start

   ```bash
   npm start
   ```

For development with automatic restart:

```bash
npm run dev
```

## Usage

### HTTP Server

Access the HTTP server at: `http://localhost:3000`

### WebSocket

Connect to the WebSocket server at: `ws://localhost:3000`

The WebSocket server:

- Sends a welcome message when a client connects
- Echoes messages back to the sender
- Broadcasts messages to all other connected clients

### UDP Listener

The UDP listener is bound to port 5555 and responds with "Hellow there!" to incoming messages.

Test with:

```bash
echo "Hello" | nc -u -w1 localhost 5555
```

import express from "express";
import { createServer } from "http";
import { createUdpServer } from "./udp";
import { createWebSocketServer } from "./websocket";
import { devicesRouter } from "./devices";
import teamsRouter from "./teams";

const app = express();
const PORT = process.env.PORT ? parseInt(process.env.PORT) : 3000;

app.use(express.json());
app.use("/devices", devicesRouter);
app.use("/teams", teamsRouter);
app.get("/", (req, res) => {
  res.send("Hello from Express + UDP + WebSocket server");
});

// Create HTTP server
const httpServer = createServer(app);

// Start HTTP server
httpServer.listen(PORT, () => {
  console.log(`HTTP server listening on http://localhost:${PORT}`);
  console.log(`WebSocket server available at ws://localhost:${PORT}`);
});

// Start WebSocket server
createWebSocketServer(httpServer);

// Start UDP server
createUdpServer();

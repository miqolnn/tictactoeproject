const net = require("net");
const WebSocket = require("ws");

const TCP_SERVER_HOST = "127.0.0.1";
const TCP_SERVER_PORT = 8888;

const wss = new WebSocket.Server({ port: 8080 });
console.log("WebSocket server running on ws://localhost:8080");

wss.on("connection", (ws) => {
    console.log("Web client connected");

    const socket = new net.Socket();
    socket.connect(TCP_SERVER_PORT, TCP_SERVER_HOST, () => {
        console.log("Connected to TCP server");
    });

    socket.on("data", (data) => {
	console.log(data.toString());
        ws.send(data.toString());
    });

    socket.on("error", (err) => {
        console.error("TCP error:", err.message);
        ws.close();
    });

    ws.on("message", (msg) => {
    	console.log(msg.toString());
	socket.write(msg.toString());
    });

    ws.on("close", () => {
        console.log("Web client disconnected");
        socket.end();
    });
});


var socketHelp = function () {
    var self = this;

    var scheme = document.location.protocol == "https:" ? "wss" : "ws";
    var port = document.location.port ? (":" + document.location.port) : "";
    self.connectionUrl = scheme + "://" + document.location.hostname + port + "/ws";
    self.onMessage = function (data) { };
    self.closeConnection = function () {
        self.socket.close(1000, "Closing from client");
    }

    self.sendData = function (data) {
        if (self.socket.readyState == WebSocket.OPEN) {
            self.socket.send(data);
            console.log("send:", data);
        } else {
            console.log("error-sedning data", self.socket.readyState);
        }
    }

    self.connect = function () {

        self.socket = new WebSocket(self.connectionUrl);
        self.socket.onopen = function (event) {
            console.log("Socket Connected: ", self.connectionUrl);
        };
        self.socket.onclose = function (event) {
            console.log("Socket Disconnected: ", self.connectionUrl);
        };
        //socket.onerror = updateState;
        self.socket.onmessage = function (event) {
            console.log("rec: ", event.data);
            self.onMessage(event.data);
        };
    };
};
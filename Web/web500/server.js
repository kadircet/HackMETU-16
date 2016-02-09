var ws = require("ws").Server;
var htmlspecialchars = require("htmlspecialchars");
var wss = new ws({port:12345});
var clients = [];
var flag = "HOLY MOTHERFUCKING FLAG";

var pairs = [];

function id_in_clients(id) {
	for(var i=0;i<clients.length;i++) {
		if(clients[i]["conn_id"] == id) {
			return true;
		}
	}
	return false;
}

function print_everything() {
	console.log("PAIRS");
	for(var i=0;i<pairs.length;i++) {
		console.log(pairs[i]["first"] + " " + pairs[i]["second"]);
	}
	console.log("CLIENTS");
	for(var i=0;i<clients.length;i++) {
		console.log(clients[i]["conn_id"]);
	}
}

function generate_id() {
	var text = ""
	var pool = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	for(var i=0;i<32;i++)
		text += pool.charAt(Math.floor(Math.random() * pool.length));

	while(id_in_clients(text)) {
		for(var i=0;i<32;i++)
			text += pool.charAt(Math.floor(Math.random() * pool.length));
	}
	return text;
}

function is_paired(pair_id) {
	for(var i=0;i<pairs.length;i++) {
		if(pairs[i]["first"] == pair_id || pairs[i]["second"] == pair_id) {
			return true;
		}
	}
	return false;
}

function make_pair(f,s) {
	pairs.push({"first" : f, "second" : s});
}

function get_pair_sock(conn_id) {
	var pair_id = null;
	for(var i=0;i<pairs.length;i++) {
		if(pairs[i]["first"] == conn_id) {
			pair_id = pairs[i]["second"];
			break;
		}
		if(pairs[i]["second"] == conn_id) {
			pair_id = pairs[i]["first"];
			break;
		}
	}
	if(pair_id != null) {
		for(var i=0;i<clients.length;i++) {
			if(clients[i]["conn_id"] == pair_id)
				return clients[i]["sock"];
		}
	}
	return null;
}

function remove_pair(conn_id) {
	var pair_id = null;
	for(var i=0;i<pairs.length;i++) {
		if(pairs[i]["first"] == conn_id) {
			pair_id = pairs[i]["second"];
			pairs.splice(i,1);
			break;
		}
		if(pairs[i]["second"] == conn_id) {
			pair_id = pairs[i]["first"];
			pairs.splice(i,1);
			break;
		}
	}
	if(pair_id != null) {
		for(var i=0;i<clients.length;i++) {
			if(clients[i]["conn_id"] == pair_id) {
				clients[i]["sock"].close();
				clients.splice(i,1);
				break;
			}
		}
	}
}

function get_conn_id(sock) {
	for(var i=0;i<clients.length;i++) {
		if(clients[i]["sock"] == sock)
			return clients[i]["conn_id"];
	}
	return null;
}

function remove_everything(sock) {
	var conn_id = null;
	for(var i=0;i<clients.length;i++) {
		if(clients[i]["sock"] == sock) {
			conn_id = clients[i]["conn_id"];
			clients.splice(i,1);
			break;
		}
	}
	if(conn_id != null) {
		var pair_sock = get_pair_sock(conn_id);
		if(pair_sock != null) {
			pair_sock.send(JSON.stringify({"status" : "error", "message" : "pair_removed"}));
			for(var i=0;i<pairs.length;i++) {
				if(pairs[i]["first"] == conn_id || pairs[i]["second"] == conn_id) {
					pairs.splice(i,1);
					break;
				}
			}
		}
	}
}

wss.on('connection', function connection(sock) {
	var conn_id = generate_id();
	clients.push({"conn_id" : conn_id, "sock" : sock});
	sock.send(JSON.stringify({"status" : "success" , "action" : "transmit_conn_id" , "conn_id" : conn_id}));
	console.log("New Connection: " + conn_id);
	sock.on('message', function(data) {
		console.log(data);
		data = data.toString('utf-8');
		try {
			if(data.toLowerCase().indexOf("require") != -1 || data.toLowerCase().indexOf("dirname") != -1 || data.toLowerCase().indexOf("filename") != -1 || data.toLowerCase().indexOf("buffer") != -1 || data.toLowerCase().indexOf("class") != -1 || data.toLowerCase().indexOf("interval") != -1 || data.toLowerCase().indexOf("timeout") != -1 || data.toLowerCase().indexOf("console") != -1 || data.toLowerCase().indexOf("exports") != -1 || data.toLowerCase().indexOf("global") != -1 || data.toLowerCase().indexOf("module") != -1 || data.toLowerCase().indexOf("process") != -1 || data.toLowerCase().indexOf("while") != -1 || data.toLowerCase().indexOf("for") != -1)
				throw new EvalError();
			data = eval("var a = " + data + ";a");
			var action = data['action'];
			if(action == 'send_message') {
				var message = data['message'];
				var conn_id = get_conn_id(sock);
				if(!is_paired(conn_id)) {
					sock.send(JSON.stringify({"status" : "error" , "message" : "no_pair"}));
					return;
				}
				var pair_sock = get_pair_sock(conn_id);
				if(pair_sock == null) {
					sock.send(JSON.stringify({"status" : "error" , "message" : "no_pair"}));
					return;
				} else {
					try {
						pair_sock.send(JSON.stringify({"status" : "success", "action" : "message_received", "message": htmlspecialchars(message)}));
						sock.send(JSON.stringify({"status" : "success" , "action" : "message_sent", "message" : htmlspecialchars(message)}));
					} catch(e) {
						remove_pair(conn_id);
						sock.send(JSON.stringify({"status" : "error" , "message" : "pair_removed"}));
					}
				}
			} else if(action == 'pair_with') {
				var pair_id = data['pair_id'];
				var conn_id = get_conn_id(sock);
				if(pair_id == conn_id) {
					sock.send(JSON.stringify({"status" : "error", "message" : "pair_with_own"}));
				} else if(is_paired(pair_id) || is_paired(conn_id)) {
					sock.send(JSON.stringify({"status" : "error", "message" : "already_paired"}));
				} else if(!id_in_clients(pair_id) || !id_in_clients(conn_id)) {
					sock.send(JSON.stringify({"status" : "error", "message" : "no_pair"}));
				} else {
					make_pair(pair_id,conn_id);
					sock.send(JSON.stringify({"status" : "success", "action": "pair_success", "pair_id": pair_id}));
					var pair_sock = get_pair_sock(conn_id);
					pair_sock.send(JSON.stringify({"status" : "success", "action": "pair_success", "pair_id": conn_id}));
				}
			}
		} catch(e) {
			console.log(e)
			sock.send(JSON.stringify({"status" : "error", "message" : "format_error"}));
		}
	});
	sock.on("close", function(data) {
		console.log("Connection closed: " + get_conn_id(sock));
		remove_everything(sock);
		print_everything();
	});
});

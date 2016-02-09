function d() {
	var conn_id = null;
	var pair_id = null;


	function go_pair_screen() {
		$("#messages").html('');
		$("#char_pair_id").html('');
		$("#mess").fadeOut(function(){
			$("#conn").fadeIn();
		});
	}

	function send_error(msg) {
		$("#error_div").html(msg).fadeIn().delay(1000).fadeOut();
	}


	var ws = new WebSocket("ws://localhost:12345");
	ws.onopen = function() {

	};

	ws.onmessage = function (evt) {
		var msg = JSON.parse(evt.data);
		if(msg["status"] == "success") {
			if(msg["action"] == "transmit_conn_id") {
				conn_id = msg["conn_id"];
				$("#conn_id").html(conn_id);
			} else if(msg["action"] == "pair_success") {
				pair_id = msg["pair_id"];
				jQuery("#chat_pair_id").html(pair_id);
				$("#conn").fadeOut(function() {
					$("#mess").fadeIn();
				});
			} else if(msg["action"] == "message_received") {
				var message = msg["message"];
				var div = jQuery("<div></div>");
				div.html(message);
				div.addClass("received_message");
				$("#messages").append(div);
			} else if(msg["action"] == "message_sent") {
				var message = msg["message"];
				var div = jQuery("<div></div>");
				div.html(message);
				div.addClass("sent_message");
				$("#messages").append(div);
			}
		} else if(msg["status"] == "error") {
			if(msg["message"] == "no_pair") {
				go_pair_screen();
				send_error("not a valid pair");
			} else if(msg["message"] == "pair_removed") {
				go_pair_screen();
				send_error("your pair is disconnected.");
			} else if(msg["message"] == "pair_with_own") {
				go_pair_screen();
				send_error("you cannot pair with yourself");
			} else if(msg["message"] == "already_paired") {
				go_pair_screen();
				send_error("already paired");
			} else if(msg["message"] == "format_error") {
				send_error("format error");
			}
		}
	};

	ws.onclose = function() {
		go_pair_screen();
		send_error("connection closed unexpectedly");
		$("#conn_id").html('');
	};

	$(document).ready(function(){
		$("#connect_with_pair").click(function(){
			var pair_id = $("#pair_id").val();
			var data = {
				'action' : 'pair_with',
				'pair_id' : pair_id
			};
			ws.send(JSON.stringify(data));
		});

		$("#send_message").click(function(){
			var message = $("#message_body").val();
			var data = {
				'action' : 'send_message',
				'message' : message
			};
			ws.send(JSON.stringify(data));
		});
	});
}

d();


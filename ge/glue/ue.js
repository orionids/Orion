// vim: ts=4 sw=4 noet :


function
ue_get_elem(e) {
	return typeof(e) === "string" ? document.getElementById(e) : e;
}


function
ueDetectBrowser()
{
	if (this.browser !== undefined) return this.browser;
	var re = new RegExp( navigator.appName == 'Microsoft Internet Explorer' ?
		"MSIE ([0-9]{1,}[\.0-9]{0,})" : "Trident/.*rv:([0-9]{1,}[\.0-9]{0,})" );
	return this.browser = re && re.exec(navigator.userAgent) != null ?
		parseInt( RegExp.$1 ) : -1;
}

if (ueDetectBrowser() > 0) {
	String.prototype.repeat = function(n, z) {
		var s = "";
		while (n-- > 0)
			s += this;
		return s;
	}
	String.prototype.padStart = function(n, s) {
		var diff = n - this.length;
		return diff > 0 ? s.repeat(diff, s) + this : this;
	}

	function URLSearchParams(s) {
		var i = s.indexOf('?');
		if (i >= 0) s = s.substring(i + 1)
		s = s.split('&');

		for (i = 0; i < s.length; i++) {
			var p = s[i];
			var j = p.indexOf('=');
			if (j > 0) this[p.substring(0, j)] = p.substring(j + 1);
		}

	}

	URLSearchParams.prototype.get = function(name) {
		return this[name];
	}

	function XMLHttpRequest() {
		return new ActiveXObject("Microsoft.XMLHTTP")
	}

	var JSON = {
		parse: function(s) {
			eval("var r=" + s + ";"); // XXX security
			return r;
		}
	};
	
	var crypto = {
		randomUUID: function() { // XXX temp
			function _rand() {
				return (Math.random() * 1e9 | 0).toString(16).padStart(8, '0');
			}
			var uuid = _rand();
			for (var i = 0; i < 2; i++) {
				var r = _rand();
				uuid += "-" + r.substring(0, 4) + "-" + r.substring(4);
			}
			return uuid + _rand();
		}
	};
}


function
ueSetStorage(name, s)
{
	if (document.location.host && typeof(Storage) !== "undefined") {
		if (name === undefined)
			localStorage.clear();
		else if (s === undefined) localStorage.removeItem(name);
		else localStorage.setItem(name,s);
	} else {
		if (name === undefined) {
			//XXX
		} else if (s === undefined) {
			document.cookie = name +
				'=;Path=/;expires=Thu, 01 Jan 1970 00:00:01 GMT;';
		} else {
			var path = 0;
			var expiry = 0;
			s = name + "=" + s;
			if (!path) s += ";path=/";
			if (!expiry) {
				var expire = new Date();
				expire.setFullYear(expire.getFullYear() + 200);
				s += ";expires=" + expire.toGMTString();
			}
			document.cookie = s;
		}
	}
}

function
ueGetStorage(name)
{
	if (document.location.host && typeof(Storage) !== "undefined") {
		return localStorage.getItem(name);
	} else {
		var ca = document.cookie.split(";");
		for (var i = 0; i < ca.length; i++) {
			var c = ca[i];
			var j = c.indexOf(name + "=");
			if (j >= 0) return c.substring(j + name.length + 1);
		}
	}
	return null;
}


function
ueCompatibleStorage(path, name, callback)
{
	var s = ueGetStorage(name);
	if (s == null) {
		name = "ueStorage_" + name;
		if (typeof ueCompatibleStorageLoaded === "undefined") {
			ueLoadScript(path, function() {
				ueCompatibleStorageLoaded = 1;
				callback(window[name]);
			});
			return;
		}
		s = window[name];
	}
	callback ( s );
}


function
ueGetComputedProperty(e, p)
{
	return window.getComputedStyle(ue_get_elem(e))[p];
}

function
ueTerminal(parent, command, option)
{
	// IE6 doesn't allow child position of insertBefore is undefined
	var placeholder, font, fontSize, border, borderColor, bgcolor;
	var childPosition = null;

	switch (parent) {
		case undefined:
		parent = document.body;
		childPosition = document.body.firstChild;
		border = "solid 1px";
		borderColor = "black";
		bgcolor = "lightgrey"; // lightgray doesn't work for IE
		window.onerror = function(e, url, ln) {
			console.log(e, url, ln);
		};
		break;
		case null: parent = document.body; break;
		default: parent = ue_get_elem(parent);
		if (!parent) return console;
	}

	if (option) {
		placeholder = option.placeholder;
		font = option.font;
		fontSize = option.fontSize;
		if (option.border) border = option.border;
		if (option.borderColor) borderColor = option.borderColor;
	}

	var compat = ueDetectBrowser() > 0? {
		empty: true,
		inputSpace: 3,
		cursor: function(input, pos) {
			var range = input.createTextRange();
			range.move('character', 0);
			range.select();
		},
		input: function(input, line) {
			function _edit() {
				if (compat.empty) {
					input.style.color = "black";
					input.value = "";
					compat.empty = false;
				}
			}
			input.style.paddingLeft = 0;
			input.style.width = "100%";
			input.style.fontSize = "100%";

			var line = _command_line(
				terminalContainer = document.createElement("tbody"));
			// if <!DOCTYPE html> and meta http-equiv was given, size of
			// input font and default font are mismatched

			input.style.fontFamily = font?
				font : document.documentElement.currentStyle.fontFamily;
			terminal.appendChild(terminalContainer);

			input.attachEvent("onkeyup", _keyup);
			input.attachEvent("onkeydown", _edit);
			input.attachEvent("onpaste", _edit);
			input.attachEvent("onclick", function(e) {
				if (compat.empty) compat.cursor(input, 0);
			});

			var focus = input.focus;
			input.focus = function() {
				compat.empty = true;
				setTimeout(function() {
					try {
						if (compat.empty) {
							input.style.color = "gray"; // grey doesn't work
							if (paceholder = input.getAttribute("placeholder"))
								input.value = placeholder; 
							compat.cursor(input, 0);
						} else {
							document.body.focus();
							focus();
						}
					} catch (e) {
					}
				}, 10);
			}
			return line;
		},
		line: function(r, c) {
			var t = _column(r, "100%", c.style.paddingTop);

			var fs = c.style.fontSize
			if (fs.length <= 0) fs = 16;
			else fs = parseFloat(fs);
			c.style.lineHeight = t.style.lineHeight = parseInt(
				fs + 12 * fs / 81 + 4) + "px";

			c.style.paddingRight = compat.inputSpace + "px";
			return t;
		},
		clear: function() {
			parent.removeChild(element);
			regen();
		}
	} : {
		inputSpace: 0,
		inputWidth: "calc(100% - " + Math.ceil(document.createElement(
			"canvas").getContext("2d").measureText("&gt; ").width) + "px)",
		input: function(input) {
			input.style.width = compat.inputSpace > 0?
				"100%" : compat.inputWidth;
			input.style.padding = 0;
			input.style.font = "inherit";
			input.addEventListener("keyup", _keyup);
			terminalContainer = terminal;
			return _command_line(terminalContainer);
		},
		line: function(r, c) {
			if (compat.inputSpace > 0) {
				c.style.paddingRight = compat.inputSpace + "px";
				return _column(r, compat.inputWidth, c.style.paddingTop);
			}
			return c;
		},
		clear: function() {
			terminal.replaceChildren(terminalContainer.lastChild);
		}
	}

	function _column(row, width, padding) {
		var col = document.createElement("td");
		col.style.width = width;
		col.style.border = 0;
		if (font) col.style.fontFamily = font;
		if (fontSize) col.style.fontSize = fontSize;
		col.style.paddingTop = padding;
		row.appendChild(col);
		return col;
	}

	function _line(padding) {
		var r = document.createElement("tr");
		var c = _column(
			r, compat.inputSpace > 0? "0%" : "100%", padding + "px");
		return {"row": r, "content": c, "text": compat.line(r, c)};
	}

	function _command_line(container) {
		var line = _line(10);
		// tailing blank is ignored for multi-column mode but paddingRight 3px
		// in _line function will take the place of it
		line.content.innerHTML = "&gt; ";
		container.appendChild(line.row);
		return line;
	}

	var to;
	function callback(state, r) {
		if (output === undefined) {
			output = _line(0);
			output.content.colSpan = 2;
			//var content = document.createElement("span");
			//output.content.appendChild(content);
			//output.content = content;
			terminalContainer.insertBefore(output.row, lastLine);
		}
		if (state == -1) {
			output.content.innerHTML = r;
		} else {
			if (to) {
				clearTimeout(to);
				to = null;
			}
			if (r) output.content.innerHTML += r;
			if (state == 0) {
				lastLine.style.visibility = "visible";
				input.focus();
				output = undefined;
			}
		}
		parent.scrollTop = parent.scrollHeight;
		parent.scrollLeft = 0;
		document.documentElement.scrollTop = parent.scrollHeight;
		document.documentElement.scrollLeft = 0;
	}

	function _run(param) {
		var line = _command_line(terminalContainer);

		lastLine.style.visibility = "hidden";
		input.value = "";
		line.text.innerHTML += param;
		terminalContainer.insertBefore(line.row, lastLine);
		to = undefined;
		if (command) {
			if (command("execute", param, callback)) {
				(function progress(i,interval) {
					if (to !== null) {
						callback(-1, "<b>" + ".".repeat(i) + "</b>");
						if (++i > 3) i = 1;
						// IE compatible setTimeout with args
						to = setTimeout((
							function(i, interval) {
								return function () {
									progress(i, interval);
								}
							})(i, interval), interval);
					}
				})(1, 1000);
			}
		} else {
			try {
				eval(param);
				callback(0);
			} catch(e) {
				callback(0);
				callback(1, e.message);
			}
		}
	}

	function _keyup(e) {
		if (input.value == "")
			input.focus();
		else if (e.keyCode == 13)
			_run(input.value);
	}

	var element, terminal, terminalContainer, input, output, lastLine;
	(function regen() {
		element = document.createElement("div");
		terminal = document.createElement("table");
		terminal.style.width = "100%";
		terminal.style.border = border === undefined? 0 : border;
		if (borderColor) terminal.style.borderColor = borderColor;
		if (bgcolor) terminal.style.backgroundColor = bgcolor;
		terminal.style.whiteSpace = "nowrap"; // redundant for multi-column mode
		terminal.setAttribute("cellspacing", "0");
		terminal.setAttribute("cellpadding", "0");

		input = document.createElement("input");
 		// A form element should have and id or name attribute
		input.setAttribute("id", "ueTerminalInput");
		input.setAttribute("autofocus", true);
		if (placeholder) input.setAttribute("placeholder", placeholder);
		input.style.margin = 0;
		input.style.border = 0;
		input.style.outline = 0;
		input.style.backgroundColor = terminal.style.backgroundColor;
		var line = compat.input(input);

		input.focus();
		line.text.appendChild(input);
		element.appendChild(terminal);
		lastLine = line.row;
		parent.insertBefore(element, childPosition);
	})();

	return {
		placeholder: function(s) {
			input.setAttribute("placeholder", s === undefined? "" : s);
		},
		run: function(s) {
			setTimeout(function() {
				_run(s);
			});
		},
		clear: function() {
			compat.clear();
			output = true; // don't append output
			callback(0)
		},
		log: function() {
			var a, s = "", l = arguments.length;
			if (l > 0) for (var i = 0; a = arguments[i++], i < l;) s += a + " ";
			else a = "";
			callback(1, ueTerminalString(s + a + "\n"));
		}
	}
}

function
ueTerminalString(s)
{
	function color(name) {
		currentColor = name;
		return '<span style="color:' + name + '">'
	}
	var currentColor = undefined;
	var re1 = / {2,}|\t|\n|\r\n|\n\r|\r|\\&|<|>|\033\[[;0-9]*m|\033\[0m/;
	var re2 = /|(https|http)[^\033\r\n\t '"]*/;
	return s.replace(
		new RegExp(re1.source + re2.source, "g"),
		function(match) {
			if (match.charAt(0) == ' ') return "&nbsp;".repeat(match.length);
			switch(match) {
				case '\t': return "&emsp;";
				case '\n': return "<br>";
				case '&': return "&amp;";
				case '<': return "&lt;";
				case '>': return "&gt;";
				case '\033[91m':
				return color("red");
				case '\033[92m':
				return color("green");
				case '\033[93m':
				return color("yellow");
				case '\033[94m':
				return color("blue");
				case '\033[95m':
				return color("magenta");
				case '\033[96m':
				return color("cyan");
				case '\033[0m':
				currentColor = undefined;
				return '</span>'
			}
			return (currentColor? "<a style=color:" + currentColor : "<a") +
				" target=_blank href=\"" + match + "\">" + match + "</a>";
		});
}

// some utility functions

var http = "http";
var https = "https";
function httpreq(protocol, o, callback)
{
	var xhttp = new XMLHttpRequest();
	var host = o.host;
	var port = o.port;
	var path = o.path
	var timeout = o.timeout;
	xhttp.open(o.method, protocol + "://" + (host? host : "localhost") +
		(port? ":" + port + path : path));
	var headers = o.headers;
	if (headers) for (var h in headers)
		xhttp.setRequestHeader(h, headers[h]);
	if (timeout) xhttp.timeout = timeout;

	var data, body;
	xhttp.send((data = o.data)? data : 
		(body = o.body)? JSON.stringify(body) : undefined);

	xhttp.onerror =function(e){
		console.log(e, xhttp.readyState);
		xhttp.abort();
	};

	xhttp.ontimeout = function(e) {
		callback(e);
		xhttp.abort();
	}

	xhttp.onreadystatechange = function() {
		if (xhttp.readyState == 4) {
			if (xhttp.status >= 400) {
				callback({
					statusCode: 400
				});
			} else {
				var r = xhttp.responseText;
				var res = {
					statusCode: xhttp.status
				};
				if (o.json) {
					try {
						r = JSON.parse(r);
					} catch (e) {
						f (e, null, res);
						return;
					}
				}
	callback( null, r, res);
			}
		} else if (o.progress && xhttp.readyState === 2) {
			callback(null, null);
		}
	};
}

var longPollContext = {
}

function LongPoll(url) {
	function error() {
		if (ctx.onerror) ctx.onerror(new Event(-1));
	}
	var i = url.indexOf("://");
	if (i > 0) {
		var protocol;
		switch (url.substring(0, i)) {
			case "ws": protocol = http; break;
			case "wss": protocol = https; break;
			default: error(); return;
		}
		var clientId = crypto.randomUUID();
		var host = url.substring(i + 3);
		this.host = host;
		this.protocol = protocol;
		this.clientId = null;
		this.serverId = null;
		this.onmessage = null;
		this.opened = false;
		var ctx = this;
		httpreq(protocol, {
			method: "POST",
			host: host,
			path: "/@socket",
			headers: {
				"Content-type": "application/json"
			},
			body: {
				clientId: clientId
			},
			json: true
		}, function(err, data) {
			ctx.protocol = protocol;
			ctx.host = host;
			ctx.clientId = clientId;
			longPollContext[ctx.serverId = data.serverId] = ctx;

			(function longPoll() {
				httpreq(protocol, {
					method: "GET",
					host: host,
					path: "/@socket?serverId=" + ctx.serverId +
						"&clientId=" + ctx.clientId,
					progress: true,
					timeout: 10000
				}, function(err, data, res) {
					if (err) {
						if (err.statusCode >= 400)
							console.log("err");
						else {
							console.log("LONGPOLL:", err);
							longPoll();
						}
					} else if (data === null) {
						if (!ctx.opened) {
							ctx.opened = true;
							// TODO: buffering is needed!!!! if disconnected!!
							if (ctx.onopen) ctx.onopen();
						}
					} else if (ctx.onmessage) {
						ctx.onmessage({
							data: data
						});
						longPoll();
					}
				});
			})();
		});
	} else {
		error();
	}
}

LongPoll.prototype.send = function(p) {
	httpreq(this.protocol, {
		method: "PUT",
		host: this.host,
		path: "/@socket?serverId=" + this.serverId +
			"&clientId=" + this.clientId,
		data: p
	}, function(err, data, res) {
		// XXX
	})
}

if (typeof WebSocket === "undefined")
	var WebSocket = LongPoll;
else
	var WebSocket = LongPoll;

// EOF

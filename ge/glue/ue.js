// vim: ts=4 sw=4 noet :

function ueParameter() {
	var p = location.search.substring(1);
	if ( !p ) return null;
	p = p.split('&');
	var param = {};
	for ( var i = 0; i < p.length; i++ ) {
		var v = p[i].split('=');
		if ( v.length > 1 ) param[v[0]] = v[1];
	}
	return param;
}

function
ueSetValue(id,value) {
	document.getElementById(id).value=value;
}

function
ueGetValue(id) {
	return document.getElementById(id).value;
}

function uePutContent( a, e ) {
	ue_get_elem(e).value = a.text;
}

function
ue_get_elem(e) {
	return typeof(e) === "string" ? document.getElementById(e) : e;
}

function
ueAppendOption(list,text,index) {
	var op = new Option();
	op.text = text;
	list = ue_get_elem(list);
	if ( list ) list.options.add(op,index < 0 ? 0 : index);
}

function
ueGetOption(list,index) {
	list = ue_get_elem(list);
	if( list != null && list.options.length>0 ) {
		if( index<0 ) index = list.selectedIndex;
		return list.options[index].text;
	}
	return "";
}

function
ueGetOptionIndex(list) {
	list = ue_get_elem(list);
	if ( list != null ) return list.selectedIndex;
	return -1;
}

function
ueGetOptionCount(id) {
	var list=document.getElementById(id);
	if( list!=null ) return list.options.length;
	return -1;
}

function
ueClearOption(id,from,n) {
	var list = document.getElementById(id);
	if ( list ) {
		var i;
		if ( typeof(from) === "undefined" ) from = 0;
		if ( typeof(n) === "undefined" ) n = list.options.length;
		i = from + n;
		if ( i > list.options.length ) i = list.options.length;
		while ( --i >= from ) list.remove(i);
	}
}

function
ueRealToHexString(value) {
	var l = 0, h=0;
	switch ( value ) {
		case Number.POSITIVE_INFINITY: h = 0x7FF00000; break;
		case Number.NEGATIVE_INFINITY: h = 0xFFF00000; break;
		case +0.0: h = 0x40000000; break;
		case -0.0: h = 0xC0000000; break;
		default:
		if ( isNaN(value) ) {
			h = 0x7FF80000;
		}
		else {
			if (value <= -0.0) {
				h = 0x80000000;
				value = -value;
			}
			else {
				h = 0;
			}

			var e = Math.floor( Math.log(value) / Math.log(2) );
			var m = Math.floor( value / Math.pow(2, e) * Math.pow(2, 52) );

			l = m & 0xFFFFFFFF;
			if( l<0 ) l = (0xFFFFFFFF+l+1)
			m /= Math.pow(2, 32);

			e += 1023;
			if ( e >= 0x7FF ) { // infinite
				e = 0x7FF;
				m = 0;
			} else if ( e < 0 ) {
				e = 0;
			}

			h = h | (e << 20) | (m & ~(-1 << 20));
			if( h<0 ) h = (0xFFFFFFFF+h+1)
		}
	}

	return h.toString(16) + l.toString(16);
};

function
ueCommand( cmd )
{
	var iframe = document.createElement("iframe");
	iframe.setAttribute( "src", cmd.replace( /#/g, "%23" ) );
	document.documentElement.appendChild(iframe);
	iframe.parentNode.removeChild(iframe);
	iframe = null;
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

	function URLSearchParams(s) {
		var i = s.indexOf('?');
		if (i >= 0) s = s.substring(i + 1)
		s = s.split('&');

		for (i = 0; i < s.length; i++ ) {
			var p = s[i];
			var j = p.indexOf('=');
			if (j > 0) this[p.substring(0, j)] = p.substring(j + 1);
		}

		this.get = function(name) {
			return this[name];
		}
	}

	function XMLHttpRequest() {
		return new ActiveXObject("Microsoft.XMLHTTP")
	}

	var JSON = {
		parse: function(s) {
			eval("var r=" + s + ";"); // XXX security
			return r;
		}
	}
}

function
ue_load_script( url, callback )
{
	var head = document.getElementsByTagName('head')[0];
	var script = document.createElement('script');
	script.type = 'text/javascript';

	// old IE cannot load script synchorously if it resides in remote site
	// so do synchronous GET
/*	if (document.location.host) { // remote
		var req = new XMLHttpRequest();
		req.open("GET", url, false); // 'false': synchronous.
		req.send(null);
		if ( req.readyState != 4 || req.status != 200 ) {
			if ( callback ) callback ( -1 );
			return;
		}

		script.text = req.responseText;
	}
	else*/ {
		if ( callback ) {
			script.onreadystatechange = function() {
				if (this.readyState == 'complete' )
					callback(0);
			};
			script.onload = function() { callback(0); };
			script.onerror = function() { callback(-1); };
		}
		script.src = url;
	}
	head.appendChild(script);
}


function
ueLoadScript( url, callback )
{
	/* setting script.src to non-existing file causes
	  crash for ie6, and script error for some ie8
	  so file existence should be tested */
	var b = ueDetectBrowser();
	if ( callback && 0<= b && b <= 8 ) {
		/* if .js file is used for src attr of iframe, default action
		  is download so it's preferable not to use .js as file extension,
		  to load a script which can be missing */
		var f = document.createElement("iframe");
		f.style.display = "none";
		f.attachEvent("onload",function() {
			if ( typeof f.contentWindow["document"] === "object" ) {
				ue_load_script( url, callback );
			}
			else {
				callback( -1 );
			}
			f.parentNode.removeChild(f);
		}, false );
		f.src = url;
		document.getElementsByTagName('head')[0].appendChild(f);
	} else {
		ue_load_script( url, callback );
	}
}

function
ueSetStorage(name,s)
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
			if ( !path ) s += ";path=/";
			if ( !expiry ) {
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
	if ( document.location.host && typeof(Storage) !== "undefined") {
		return localStorage.getItem(name);
	} else {
		var ca = document.cookie.split(";");
		for ( var i = 0; i < ca.length; i++ ) {
			var c = ca[i];
			var j = c.indexOf(name + "=");
			if ( j >= 0 ) return c.substring( j + name.length + 1 );
		}
	}
	return null;
}

function
ueCompatibleStorage(path,name,callback)
{
	var s = ueGetStorage(name);
	if ( s == null ) {
		name = "ueStorage_" + name;
		if ( typeof ueCompatibleStorageLoaded === "undefined" ) {
			ueLoadScript(path,function() {
				ueCompatibleStorageLoaded = 1;
				callback( window[name] );
			} );
			return;
		}
		s = window[name];
	}
	callback ( s );
}

function
ueInjectString(id,s)
{
	var e = document.getElementById(id);
	if (e) e.innerHTML = s;
}

function
ueGetClass(e)
{
	return e.classList ?
		e.classList : e.className.split(" ");
}

function
ueAddClass(cl,s)
{
	if ( cl.add ) cl.add(s);
	else cl.push(s);
}

function
ueCheckClass(cl,s)
{
	return cl.contains ? cl.contains(s) :
		cl[s] ? true : false;
}

function
ueToggleClass(cl,s)
{
//	var cl = ueGetClass(e);
	if ( cl.toggle ) {
		cl.toggle(s);
	} else if ( cl.add ) {
alert('!!' );
	} else {
		cl[s] ? cl.remove(s) :
cl.push(s);
// ueAddClass(cl,s);
	}
}

function
ueSetClass(e,cl)
{
	if ( !e.classList )
		e.className = cl.join(" ");
}

function ue_popup_callback(contents,show) {
	var e = document.getElementById(contents);
	e.style.display = e.style.display == show ? "none" : show;
}

function
ueMakePopup(item,contents,show,color,minwidth,shadow)
{
	var i, n = 0, len = 1;
	for (; n < len; n++ ) {
		if ( item instanceof Array ) {
			len = item.length;
			i = item[n];
			
		} else {
			i = item;
		}

		if ( contents instanceof Array ) {
			if ( len > contents.length ) len = contents.length;
			c = contents[n];
		} else {
			c = contents;
		}

		if ( c == null ) {
			c = i + "_contents";
			i += "_item";
		}
		var e = document.getElementById(c);
		e.setAttribute( "style", "display:none;position:absolute;background-color:"
			+ color + ";min-width:" + minwidth +
	    ";box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);z-index:1000" );
		e = document.getElementById(i);
		e.setAttribute( "onclick", "ue_popup_callback('" + c + "','" + show +  "')" );
	}
}

function
ueClosePopup(contents)
{
	var e = document.getElementById(contents);
	e.style.display = "none";
}

function
ueGetComputedProperty(e,p) {
	return parseFloat(window.getComputedStyle(ue_get_elem(e))[p]);
}

function
ueTerminal(parent, option, command)
{
	var placeholder, font, fontSize;
	if (option) {
		placeholder = option.placeholder;
		font = option.font;
		fontSize = option.fontSize;
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
							input.style.color = "gray";
							input.value = input.getAttribute("placeholder");
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
		terminal.style.border = 0;
		terminal.style.whiteSpace = "nowrap"; // redundant for multi-column mode
		terminal.setAttribute("cellspacing", "0");
		terminal.setAttribute("cellpadding", "0");

		input = document.createElement("input");
 		// A form element should have and id or name attribute
		input.setAttribute("id", "ueTerminalInput");
		input.setAttribute("autofocus", true);
		input.setAttribute("placeholder", placeholder);
		input.style.margin = 0;
		input.style.border = 0;
		input.style.outline = 0;
		input.style.backgroundColor = parent.style.backgroundColor;
		var line = compat.input(input);

		input.focus();
		line.text.appendChild(input);
		element.appendChild(terminal);
		lastLine = line.row;
		parent.appendChild(element);
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
		}
	}
}

function
ueTerminalString(s)
{
	function color(name) {
		return '<span style="color:' + name + '">'
	}
	return s.replace(
		/ {2,}|\t|\n|\r\n|\n\r|\r|\\&|<|>|\033\[[;0-9]*m|\033\[0m/g,
		function(match) {
			if (match.charAt(0) == ' ') return "&nbsp;".repeat(match.length);
			switch(match) {
				case '\t': return "&emsp;";
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
				case '\033[0m': return '</span>'
			}
			return "<br>"
		});
}

/* EOF */

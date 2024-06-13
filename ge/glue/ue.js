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

if (ueDetectBrowser() > 0)
	String.prototype.repeat = function(n, z) {
		var s = "";
		while (n-- > 0)
			s += this;
		return s;
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
	if ( document.location.host && typeof(Storage) !== "undefined") {
		localStorage.setItem(name,s);
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

function
ueGetStorage(name)
{
	if ( document.location.host && typeof(Storage) !== "undefined") {
		localStorage.getItem(name);
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
ueRemoveStorage(name)
{
	if ( document.location.host && typeof(Storage) !== "undefined") {
		localStorage.removeItem(name);
	} else {
		document.cookie = name + '=;Path=/;expires=Thu, 01 Jan 1970 00:00:01 GMT;';
	}
}

function
ueInjectString(id,s)
{
	var e = document.getElementById(id);
	if ( e ) e.innerHTML = s;
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
ueConsole(parent, option, command)
{
	var placeholder, contentTag, font, fontSize, initialCommand;
	if (option) {
		placeholder = option.placeholder;
		contentTag = option.contentTag;
		font = option.font;
		fontSize = option.fontSize;
		initialCommand = option.initialCommand;
	}
	if (!contentTag) contentTag = "span";
	function _line(padding) {
		var r = document.createElement("tr");
		var c = document.createElement("td");
		var t = document.createElement("td");
		c.style.width = "0%";
		t.style.width = "100%";
		c.style.border = 0;
		t.style.border = 0;
		if (font) c.style.fontFamily = t.style.fontFamily = font;
		if (fontSize) c.style.fontSize = t.style.fontSize = fontSize;
		if (padding === undefined) padding = 0;
		c.style.paddingTop = padding + "px";
		c.style.paddingRight = "3px";
		t.style.paddingTop = padding + "px";
		r.appendChild(c);
		r.appendChild(t);
		return {"row": r, "content": c, "text": t};
	}

	function _command_line(container) {
		line = _line(10);
		line.content.innerHTML = "&gt;";
		container.appendChild(line.row);
		return line;
	}

	function _input() {
		var input = document.createElement("input");
		input.setAttribute("id", "ueConsoleInput"); // A form element should have and id or name attribute
		input.setAttribute("autofocus", true);
		input.setAttribute("placeholder", placeholder);
		input.style.margin = 0;
		if (ueDetectBrowser() > 0) {
			input.style.paddingLeft = 0;
			input.style.fontSize = "100%";
			input.style.fontFamily = font;
		} else {
			input.style.padding = 0;
			input.style.font = "inherit";
		}
		input.style.border = 0;
		input.style.outline = 0;
		input.style.width = "100%";
		input.style.backgroundColor = parent.style.backgroundColor;
		return input;
	}

	function _run(param) {
			var line = _command_line(inputContainer);

			lastLine.style.visibility = "hidden";
			input.value = "";
			if (ueDetectBrowser() == 6) {
				var fs = line.content.style.fontSize
				if (fs.length <= 0) fs = 16;
				fs = parseFloat(fs);
				line.content.style.lineHeight =
				line.text.style.lineHeight =
				parseInt(fs + 12 * fs / 81 + 4) + "px";
			}
			line.text.innerHTML += param;
			inputContainer.insertBefore(line.row, lastLine);
			var to;
			function callback(state, r) {
				if (output === undefined) {
					output = _line();
					output.content.colSpan = 2;
					var content = document.createElement(contentTag);
					output.content.appendChild(content);
					output.content = content;
					inputContainer.insertBefore(output.row, lastLine);
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
						_focus(input);
						output = undefined;
					}
				}
				parent.scrollTop = parent.scrollHeight;
				parent.scrollLeft = 0;
			}
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
			_focus(input);
		else if (e.keyCode == 13)
			_run(input.value);
	}

	function _focus(elem) {
		empty = true;
		if (ueDetectBrowser() > 0) {
			setTimeout(function() {
				try {
					if (empty) {
						elem.style.color = "lightgrey";
						elem.value = placeholder;
						_cursor(input,0);
					} else {
						document.body.focus();
						elem.focus();
					}
				} catch (e) {
				}
			}, 10);
		} else {
			elem.focus();
		}
	}

	var element = document.createElement("div");

	var inputLine = document.createElement("table");
	inputLine.style.width = "100%";
	inputLine.style.border = 0;
	inputLine.setAttribute("cellspacing", "0");
	inputLine.setAttribute("cellpadding", "0");

	var input = _input();
	var empty = true;
	var inputContainer;
	if (ueDetectBrowser() > 0) {
		function _edit() {
			if (empty) {
				input.style.color = "black";
				input.value = "";
				empty = false;
			}
		}
		function _cursor(input, pos) {
			var range = input.createTextRange();
			range.move('character', 0);
			range.select();
		}
		inputContainer = document.createElement("tbody");
		inputLine.appendChild(inputContainer);
		input.attachEvent("onkeyup", _keyup);
		input.attachEvent("onkeydown", _edit);
		input.attachEvent("onpaste", _edit);
		input.attachEvent("onclick", function(e) {
			if (empty) _cursor(input, 0);
		});
	} else {
		inputContainer = inputLine;
		input.addEventListener("keyup", _keyup);
	}

	var line = _command_line(inputContainer);
	_focus(input);

	var output;
	line.text.appendChild(input);
	element.appendChild(inputLine);
	var lastLine = line.row;
	parent.appendChild(element);
	if (initialCommand) _run(initialCommand);
}

function
ueConsoleString(s)
{
	function color(name) {
		return '<span style="color:' + name + '">'
	}
	return s.replace(
	/\n|\r\n|\n\r|\r|\t| |&|<|>|\033\[[;0-9]*m/g,
	function(match) {
		var r = (this.pattern === undefined?
		this.pattern = {
			"&": "&amp;",
			"<": "&lt;",
			">": "&gt;",
			"\033[91m": color("red"),
			"\033[92m": color("green"),
			"\033[93m": color("orange"),
			"\033[95m": color("magenta"),
			"\033[96m": color("cyan"),
			"\033[0m": "</span>",
			"\n": "<br>",
			"\r\n": "<br>",
			"\n\r": "<br>",
			"\r": "<br>",
			" ": "&nbsp;",
			"\t": "<span style=white-space:pre>\t</span>"
		} : this.pattern)[match];
		return r? r : match;
	})
}

/* EOF */


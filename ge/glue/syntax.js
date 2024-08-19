// vim: ts=4 sw=4 noet 

var syntaxHTML = {
	tag: "<span style=\"color:",
	classTag: "<span class=\"",
	done: "\">",
	end: "</span>",
	color: function (name) {
		if (name === "default") {
			//return window.getComputedStyle(document.body).color;
			return ueGetComputedProperty(document.body, "color");
		}
		return {
			"key": "red",
			"number": "darkorange",
			"string": "green",
			"boolean": "blue",
			"null": "magenta",
			"array": "cyan"
		}[name]
	},
	stack: null
};

function
syntaxContext(u, t, c)
{
	var tag;
	if (t === undefined) t = exports.target;
	if (typeof c ==="string") {
		tag = t[c + "Tag"];
		c = t[c + "Color"];
	} else {
		tag = t.tag;
		if (c === undefined) c = t.color;
	}
	return {
		target: t,
		tag: tag,
		color: c,
		unknown: u,
		stack: t.stack? new t.stack() : null
	};
}

function
syntaxBegin(ctx, color)
{
	var c = ctx.color;
	if (c) {
		c = c(color);
		if (c === undefined) {
			if (ctx.unknown) return undefined;
		} else {
			color = c;
		}
	}
	var begin = ctx.tag + color + ctx.target.done;
	return ctx.stack? ctx.stack.push(begin) : begin;
}

function
syntaxEnd(ctx)
{
	return ctx.stack? ctx.stack.pop(ctx.target.end) : ctx.target.end;
}

function
syntaxString(ctx, color, s)
{
	return syntaxBegin(ctx, color) + s + syntaxEnd(ctx);
}

function
syntaxHighlight(s, ctx) {
	if (exports.off) return s;
	if (ctx === undefined) ctx = syntaxContext(true);
	var depth = 0;
	var re = RegExp("{|}|\\[|\\]|((\"(?:[^\"\\\\\\n]|\\\\.|\\\\\\n.)*\"|'(?:[^'\\\\\\n]|\\\\.|\\\\\\n.)*')(\\s*:)?|[A-Za-z0-9]*:|\\b(true|false|null)\\b|-?\\d+(?:\\.\\d*)?(?:[eE][+\\-]?\\d+)?)", "g")
	return s.replace(re,
	function (match) {
		var color;
		switch (match) {
			case '{':
			color = "object";
			depth ++;
			break;
			case '}':
			color = "object";
			if (depth > 0) depth --;
			break;
			case '[':
			case ']':
			color = "array";
			break;
			default:
			if (depth <= 0) break;
			if (/:$/.test(match)) {
				color = "key";
				break;
			}
			var m = match.charAt(0);
			if (m === '"' || m === '\'' &&
				match.charAt(match.length - 1) !== m) {
				color = "string";
			} else {
				color = /^['"]/.test(match)? "string" :
					/true|false/.test(match)? "boolean" :
					/null/.test(match)? "null" : "number";
			}
		}
		var begin = syntaxBegin(ctx, color);
		return begin? begin + match + syntaxEnd(ctx) : match;
	});
}

if (typeof exports === 'undefined') {
	var exports = {
		target: syntaxHTML,
	};
} else {
	exports.html = syntaxHTML;
	exports.context = syntaxContext;
	exports.begin = syntaxBegin;
	exports.end = syntaxEnd;
	exports.string = syntaxString;
	exports.highlight = syntaxHighlight;
}

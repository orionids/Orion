// vim: ts=4 sw=4 noet :

function
syntaxHighlight(s, r, c) {
	function getColor(name) { return c? c[name] : name; }
	var depth = 0;
	if (r === undefined)
		r = {
			"color": "<span class=\"",
			"done": "\">",
			"end": "</span>"
		};

	var re = RegExp("{|}|\\[|\\]|((\"(?:[^\"\\\\\\n]|\\\\.|\\\\\\n.)*\"|'(?:[^'\\\\\\n]|\\\\.|\\\\\\n.)*')(\\s*:)?|[A-Za-z0-9]*:|\\b(true|false|null)\\b|-?\\d+(?:\\.\\d*)?(?:[eE][+\\-]?\\d+)?)", "g")
	return s.replace(re,
	function (match) {
		var color;
		switch (match) {
			case '{':
			color = getColor("object");
			depth ++;
			break;
			case '}':
			color = getColor("object");
			if (depth > 0) depth --;
			break;
			case '[':
			case ']':
			color = getColor("array");
			break;
			default:
			if (depth <= 0) break;
			if (/:$/.test(match)) {
				color = getColor("key");
				break;
			}
			var m = match.charAt(0);
			if (m === '"' || m === '\'' &&
				match.charAt(match.length - 1) !== m) {
				color = getColor("string");
			} else {
					color = getColor(
						/^['"]/.test(match)? "string" :
						/true|false/.test(match)? "boolean" :
						/null/.test(match)? "null" :
						"number");
			}
		}
		return color?
			r.color + color + r.done + match + r.end : match;
	});
}

if (typeof exports !== 'undefined')
	exports.highlight = syntaxHighlight;

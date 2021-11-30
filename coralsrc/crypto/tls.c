/* Coral library for low level data structure implementation
 Copyright (C) 1996-2021 AdaptiveFlow

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include <coralsrc/def.h>
#include <coral/inline.h>
#include <coral/crypto.h>



unsigned __API
tlsRecord
	A_B((buf, record, major, minor, size))
	A_1		_unsigned8 __FAR* buf
	A_N		unsigned record
	A_N		unsigned major
	A_N		unsigned minor
	A_N		unsigned size
	A_E
{
	buf[0] = (_unsigned8)record;
	buf[1] = (_unsigned8)major;
	size -= 5;
	buf[2] = (_unsigned8)minor;
	buf[3] = OP_HIGH16(size);
	buf[4] = OP_LOW16(size);
	return size;
}



unsigned __API
tlsHandshakeRecord
	A_B((buf, major, minor, size, type))
	A_1		_unsigned8 __FAR* buf
	A_N		unsigned major
	A_N		unsigned minor
	A_N		unsigned size
	A_N		unsigned type
	A_E
{
	size = tlsRecord(
		buf, TLS_HANDSHAKE_RECORD, major, minor, size);
	buf[5] = (_unsigned8)type;
	buf[6] = 0;
	size -= 4;
	buf[7] = OP_HIGH16(size);
	buf[8] = OP_LOW16(size);
	return size;
}



unsigned __API
tlsHelloExtensionOffset
	A_B((hello, type))
	A_1		struct _TLSHello __FAR* hello
	A_N		unsigned type
	A_E
{
	unsigned offset =
		(TLS_HELLO_FIXED_SIZE + 5) + hello->session;
	return type == TLS_CLIENT_HELLO?
		offset + hello->cipher + hello->compression : offset;
}



void __API
tlsHello
	A_B((hello, buf, major, minor, size, type))
	A_1		struct _TLSHello __FAR* hello
	A_N		_unsigned8 __FAR* buf
	A_N		unsigned major
	A_N		unsigned minor
	A_N		unsigned size
	A_N		unsigned type
	A_E
{
	unsigned offset;
	major += TLS_MAJOR_VERSION_DIFF;
	minor += TLS_MINOR_VERSION_DIFF;
	size = tlsHandshakeRecord(
		buf, major, minor, size, type);
	buf[9] = (_unsigned8)major;
	buf[10] = (_unsigned8)minor;

	offset = TLS_HELLO_FIXED_SIZE + (
		buf[43] = (_unsigned8)hello->session);
	hello->session = TLS_HELLO_FIXED_SIZE;

	if (type == TLS_CLIENT_HELLO) {
		buf[2] = 1; /* version 1.0 in record header */
		buf += offset;
		size =  hello->cipher;
		buf[0] = OP_HIGH16(size);
		buf[1] = OP_LOW16(size);
		hello->cipher = offset + 2;
		offset += size + 3;
		buf += size + 2;

		size = hello->compression;
		hello->compression = offset;
		buf[0] = (_unsigned8)size;
		buf += size + 1;
	} else {
		hello->cipher = offset;
		hello->compression = offset + 2;
		buf += offset + 3;
	}

	size = hello->extension;
	buf[0] = OP_HIGH16(size);
	buf[1] = OP_LOW16(size);
}



unsigned __API
tlsHelloExtension
	A_B((p, code, size))
	A_1		_unsigned8 __FAR* p
	A_N		int code
	A_N		unsigned size
	A_E
{
	OP_SETCONST16(p, 0, code);
	OP_SETCONST16(p, 2, size);
	return TLS_EXTENSION_OFFSET + size;
}



unsigned __API
tlsHelloListExtension
	A_B((p, code, size))
	A_1		_unsigned8 __FAR* p
	A_N		int code
	A_N		unsigned size
	A_E
{
	OP_SETCONST16(p, 4, size);
	return tlsHelloExtension(p, code, size + 2);
}


int __API
tlsParseRecord
	A_B((p, major, minor, size))
	A_1		_unsigned8 __FAR* p
	A_N		unsigned major
	A_N		unsigned minor
	A_N		unsigned __FAR* size
	A_E
{
	unsigned in = *size;
	if (in >= TLS_RECORD_PAYLOAD_OFFSET + 1) {
		unsigned total = OP_COMBINE16(p[3], p[4]) + 5;
		if (in >= total) {
			int result;
			*size = total;
			if (major + 2 != p[1] || minor + 1 != p[2])
				return FAIL_SIGNATURE;
			return p[0];
		}
	}
	*size = 0;
	return 0;
}



int __API
tlsParseAlertRecord
	A_B((p, in))
	A_1		const _unsigned8 __FAR* p
	A_N		unsigned in
	A_E
{
	if (in < TLS_ALERT_RECORD_SIZE) return FAIL_UNDERFLOW;
	return p[5] == 2? TLS_FATAL_ALERT : TLS_WARNING_ALERT;
}



_min32 __API
tlsParseSequence
	A_B((p, offset, size))
	A_1		const _unsigned8 __FAR* p
	A_N		unsigned __FAR* offset
	A_N		unsigned size
	A_E
{
	unsigned l, o = *offset + 2;
	if (size >= o) {
		p += o;
		l = p[-1];
		if (l == 0x82) {
			if (size < (o += 2)) return FAIL_UNDERFLOW;
			l = OP_COMBINE16(p[0], p[1]);
		}
		if (size >= o + l) {
			*offset = o;
			return l;
		}
	}
	return FAIL_UNDERFLOW;
}



int __API
tlsParseHelloExtension
	A_B((len, p, offset, size))
	A_1		unsigned __FAR* len
	A_N		const _unsigned8 __FAR* p
	A_N		unsigned __FAR* offset
	A_N		unsigned size
	A_E
{
	unsigned l, o = *offset;
	if (memField(&l, p, o + 2, size, 2) == 0) {
		*offset += l + 4;
		*len = l;
		p += o;
		return OP_COMBINE16(p[0], p[1]);
	}
	return FAIL_UNDERFLOW;
}



int __API
tlsParseHello
	A_B((handshake, hello, p, major, minor, size, type))
	A_1		struct _TLSHandshakeContext __FAR* handshake
	A_N		struct _TLSHello __FAR* hello
	A_N		const _unsigned8 __FAR* p
	A_N		unsigned major
	A_N		unsigned minor
	A_N		unsigned size
	A_N		unsigned type
	A_E
{
	if (size > TLS_HELLO_FIXED_SIZE) {
		unsigned l, keyx;
		unsigned offset;

		if (p[TLS_RECORD_PAYLOAD_OFFSET] != type)
			return 0;
		offset = TLS_HELLO_FIXED_SIZE +
			p[TLS_HELLO_FIXED_SIZE - 1];
		hello->session = TLS_HELLO_FIXED_SIZE;
		if (type == TLS_SERVER_HELLO) {
			if (size <= offset + 3) return FAIL_UNDERFLOW;
			hello->cipher = offset;
			hello->compression = offset + 2;
			l = 3;
			keyx = TLS_SERVER_KEY_EXCHANGE_OFFSET;
		} else {
			if (memField(&l, p, offset, size, 2) < 0)
				return FAIL_UNDERFLOW;
			hello->cipher = offset += 2;
			if (memField(&l, p, offset += l, size, 1) < 0)
				return FAIL_UNDERFLOW;
			hello->compression = offset += 1;
			keyx = TLS_CLIENT_KEY_EXCHANGE_OFFSET;
		}
		offset += l + 2;
		hello->extension = offset;
		while (offset < size) {
			unsigned o = offset;
			const _unsigned8 __FAR* s = p + offset;
			int code = tlsParseHelloExtension(
				&l, p, &offset, size);
			if (code < 0) return code;
			switch (code) {
				case TLS_KEY_SHARE:
				/* consider key exange method XXX */
				memField(&l, p, o + keyx + 2, size, 2);
				if (TLS_KEY_MAX_LENGTH < l)
					return FAIL_OVERFLOW;
				handshake->keysize = l;
				memcpy(handshake->key, s + keyx + 4, l);
			}
		}
		return 1;
	}
	return FAIL_UNDERFLOW;
}

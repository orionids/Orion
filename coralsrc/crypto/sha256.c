
/* Coral library for low level data structure implementation
  Copyright (C) 1996-2019 AdaptiveFlow

  This library is dual-licensed: you can redistribute it and/or modify
 it under the terms of the GNU General Public License version 3 as 
 published by the Free Software Foundation. For the terms of this 
 license, see license/gpl-3.txt or <http://www.gnu.org/licenses/>.

  You are free to use this library under the terms of the GNU General
 Public License, but WITHOUT ANY WARRANTY; without even the implied 
 warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU General Public License for more details.

  Alternatively, you can use this library under any other
 OSS licences ( like BSD ) or your own commercial license,
 as set out in LICENSE.txt. */

#include <coral/inline.h>
#include <coral/integer.h>
#include <coral/crypto/sha256.h>


#define SHA256_SIG(w, r1, r2, s) (\
	OP_ROTATE_RIGHT(w, r1, 32 - r1) ^ \
	OP_ROTATE_RIGHT(w, r2, 32 - r2) ^ \
	((w) >> s))
#define SHA256_SUM(h, r1, r2, r3) (\
	OP_ROTATE_RIGHT(h, r1, 32 - r1) ^ \
	OP_ROTATE_RIGHT(h, r2, 32 - r2) ^ \
	OP_ROTATE_RIGHT(h, r3, 32 - r3))

static const _unsigned32 sha256_k[] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static void
sha256_compute
	A_B((hash, chunk))
	A_1		_unsigned32 __FAR* hash
	A_N		const _unsigned8 __FAR* chunk
	A_E
{
	unsigned i;
	_unsigned32 w[SHA256_CHUNK], a, b, c, d, e, f, g, h;

	/* XXX for big endian, memcpy is possible */
	for (i = 0; i < 16; i++) {
		w[i] = OP_COMBINE32(
			OP_COMBINE16(chunk[0], chunk[1]),
			OP_COMBINE16(chunk[2], chunk[3]));
		chunk += 4;
	}
	for (; i < SHA256_CHUNK; i++) {
		_unsigned32 w0 = w[i - 15], w1 = w[i - 2];
		w[i] = w[i - 16] + SHA256_SIG(w0, 7, 18, 3) +
			w[i - 7] + SHA256_SIG(w1, 17, 19, 10);
	}

	/* compression */
	a = hash[0];
	b = hash[1];
	c = hash[2];
	d = hash[3];
	e = hash[4];
	f = hash[5];
	g = hash[6];
	h = hash[7];

	for (i = 0; i < SHA256_CHUNK; i++) {
		unsigned t1 = h + SHA256_SUM(e, 6, 11, 25) + 
			((e & f) ^ (~e & g)) + sha256_k[i] + w[i],
			t2 = SHA256_SUM(a, 2, 13, 22) +
				((a & b) ^ (a & c) ^ (b & c));
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	hash[0] += a;
	hash[1] += b;
	hash[2] += c;
	hash[3] += d;
	hash[4] += e;
	hash[5] += f;
	hash[6] += g;
	hash[7] += h;
}

void
sha256Initialize
	A_B((hash))
	A_1		_unsigned32 __FAR* hash
	A_E
{
	hash[0] = 0x6a09e667;
	hash[1] = 0xbb67ae85;
	hash[2] = 0x3c6ef372;
	hash[3] = 0xa54ff53a;
	hash[4] = 0x510e527f;
	hash[5] = 0x9b05688c;
	hash[6] = 0x1f83d9ab;
	hash[7] = 0x5be0cd19;
}

unsigned __API
sha256Update
	A_B((hash, chunk, offset, input, size))
	A_1		_unsigned32 __FAR* hash
	A_N		_unsigned8 __FAR* chunk
	A_N		unsigned offset
	A_N		_unsigned8 __FAR* input
	A_N		unsigned size
	A_E
{
	unsigned remained;
	while ((remained = SHA256_CHUNK - offset) <= size) {
		if (offset > 0) {
			memcpy(chunk + offset, input, remained);
			sha256_compute(hash, chunk);
		} else {
			sha256_compute(hash, input);
		}
		input += remained;
		size -= remained;
		offset = 0;
	}
	memcpy(chunk + offset, input, size);
	return offset + size;
}

void __API
sha256Finalize
	A_B((hash, chunk, offset, size))
	A_1		_unsigned32 __FAR* hash
	A_N		_unsigned8 __FAR* chunk
	A_N		unsigned offset
	A_N		_unsigned64 size
	A_E
{
	chunk[offset++] = 0x80;
	if (offset < 56) {
		memset(chunk + offset, 0, 56 - offset);
	} else {
		memset(chunk + offset, 0, SHA256_CHUNK - offset);
		sha256_compute(hash, chunk);
		memset(chunk, 0, 56);
	}
	x_ushls(size, 3);
	chunk[56] = (_unsigned8)x_ulow(x_ushr(size, 56));
	chunk[57] = (_unsigned8)x_ulow(x_ushr(size, 48));
	chunk[58] = (_unsigned8)x_ulow(x_ushr(size, 40));
	chunk[59] = (_unsigned8)x_ulow(x_ushr(size, 32));
	chunk[60] = (_unsigned8)x_ulow(x_ushr(size, 24));
	chunk[61] = (_unsigned8)x_ulow(x_ushr(size, 16));
	chunk[62] = (_unsigned8)x_ulow(x_ushr(size, 8));
	chunk[63] = (_unsigned8)x_ulow(size);
	sha256_compute(hash, chunk);
}


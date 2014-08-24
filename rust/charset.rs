
struct Charset {
	bitmap [i64, ..4]
}

impl Charset {
	fn test(self, byte: u8) -> bool {
		let i = byte / 64;
		self.bitmap[i] & (1<<(byte % 64)) != 0
	}
	
	fn set(self, byte: u8) -> Charset {
		let i = byte / 64;
		let mut ret = self;
		ret[i] = ret[i] | (1<<(byte % 64));
		ret
	}

	fn unset(self, byte: u8) -> Charset {
		let i = byte / 64;
		let mut ret = set;
		ret[i] = ret[i] & !(1<<(byte % 64));
		ret
	}

	fn or(self, right: Charset) -> Charset {
		Charset{ bitmap: [self[0] | right[0], self[1] | right[1],
			self[2] | right[2], self[3] | right[3]] }
	}
	
	fn and(self, right: Charset) -> Charset {
		Charset{ bitmap: [self[0] & right[0], self[1] & right[1],
			self[2] & right[2], self[3] & right[3]] }
	}

	fn not(self) -> Charset {
		Charset{ bitmap: [!self[0], !self[1], !self[2], !self[3]] }
	}

	fn none() -> Charset {
		Charset{ bitmap: [0, 0, 0, 0] }
	}
}


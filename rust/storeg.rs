
enum StoregState {
	OR(Regex),
	AND(Regex),
	KLEENE(Regex),
	END_OR(Regex),
	END_AND(Regex),
	CHAR(Regex)
};

//this would normally be done recursively but Rust doesn't handle recursion
fn storeg(s &str, r Result<Regex>) -> Result<Regex> {
	let (fc, nx) s.slice_shift_char();
	let reg = match r {
		Ok(regex)	=> { regex }
		Err(message)	=> { return Err(message); }
	};
	let ch = match fc {
		Some(c)	=> c
		None ()	=> {return Ok(reg);}
	};
	match ch {
	'['	=> stoclass(nx, reg)
	'.'	=> storeg(nx, concat_of(reg, ANY_CHARACTER))
	'%'	=> stostdclass(nx, reg)
	'*'	=> storeg(nx, kleene_of(reg))
	'+'	=> storeg(nx, concat_of(kleene_of(reg), kleene_p(reg)))
	'?'	=> storeg(nx, maybe_of(reg))
	'!'	=> storeg(nx, strict_of(reg))
	'|'	=> stoor(nx, reg)
	'&'	=> stoand(nx, reg)
	'('	=> regconcat(reg, stoscope(nx, reg))
	')'	=> regendops(nx, reg)
	'^'	=> match reg {
		EMPTY_STRING	=> sbeginfile(nx)
		_		=> storeg(nx, concat_of(reg, chartoset('^')))
		}
	'$'	=> match nx.len() {
		0	=> sendfile(reg)
		_	=> storeg(nx, concat_of(reg, chartoset('$')))
		}
	'{'	=> {
			let (nstr, reps) = splitn(nx, '}', 1);
			storeg(nstr, nrep_from(reg, _read_ints(reps)))
		}
	'\\'	=> {
			let (nstr, nc) = nx.slice_shift_char();
			storeg(nx, concat_of(reg, chartoset(nc)))
		}
	'/'	=> Err("Slashes are reserved for compatibility.")
	_	=> storeg(nx, concat_of(reg, chartoset(ch)))
	}
}

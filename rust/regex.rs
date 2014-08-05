
enum Regex {
	Capture(Box<Regex>, &str),
	Compl(Box<Regex>),
	Or(Box<Reglist>),
	And(Box<Reglist>),
	Concat(Box<Reglist>),
	Kleene(Box<Regex>),
	Chars(Box<Reglist>)
}

struct Reglist {
	this Regex,
	next Box<Reglist>,
	prev Box<Reglist>
}

impl Regex {
	fn find_scope(pt Regex) -> Regex {
		if self as int < pt as int {
			match self {
			Capture(r)	=> r.find_scope(pt),
			Compl(r)	=> r.find_scope(pt),
			Or(l)		=> l.prev.this.find_scope(pt),
			And(l)		=> l.prev.this.find_scope(pt),
			Concat(l)	=> l.prev.this.find_scope(pt),
			_		=> { fail!("Wat?!") }
			}
		} else {
			self
		}
	}
}

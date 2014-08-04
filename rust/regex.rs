enum Regex {
	Compl(Regex),
	Kleene(Regex),
	Or(&[Regex]),
	And(&[Regex]),
	Concat(&[Regex]),
	Chars(&[Charset])
}

impl Regex {
	fn find_scope(pt Regex) -> Regex {
		//FIXME
	}
}

enum Regex {
	Compl(Regex),
	Kleene(Regex),
	Or(&[Regex]),
	And(&[Regex]),
	Concat(&[Regex]),
	Chars(&[Charset])
}

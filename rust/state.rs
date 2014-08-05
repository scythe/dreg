
struct Regstate {
	capture_symbol &str,
	regex Regex,
	transitions Statelist
}

enum Statelist {
	Edge(Charset, Regex, Box<Statelist>),
	Empty
}


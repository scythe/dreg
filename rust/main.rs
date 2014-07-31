
extern crate getopts;
use getopts::{
		 optopt
		,optflag
		,getopts
		,usage
		,short_usage
		,OptGroup};
use std::os;
use std::str;
use std::io::{
		 File
		,stdio
		,BufferedReader};

fn print_usage() {} //FIXME

fn print_help() {} //FIXME

fn main() {
	let args: Vec<String> = os::args;

	let program = args[0].clone() + " regex ";

	let opts = [
		 optflag("h", "help", "display this help menu")
		,optopt("s", "string", "string to match against", "string")
		,optopt("f", "input", "file to match/search against"
			+"[defaults to stdin]", "input file")
		,optflag("g", "grep", "search for a match")
		,optopt("o", "output", "file in which to store captures"
			+"[defaults to stdout]", "output file")
	];
	let matches = match getopts(args.tail(), opts) {
		Ok(m) => { m }
		Err(f) => { fail!(f.to_string()) }
	};
	if matches.opt_present("h") {
		usage(program.as_slice(), opts);
		return;
	};
	let mut input = if matches.opt_present("f") {
		let filename = match matches.opt_str("f") {
			Some(s) => { s }
			None => { //TODO: make me a macro!
				short_usage(program.as_slice(), opts);
				fail!("Error: no input file provided, but expected.")
			}
		};
		let path = Path::new(filename);
		let file = File::open(&path);
		BufferedReader::new(file)
	} else if matches.opt_present("s") {
		let string = match matches.opt_str("s") {
			Some(s) => { s }
			None => {
				short_usage(program.as_slice(), opts);
				fail!("Error: no input string provided, but expected.")
			}
		};
		fail!("Error: string matching not implemented.")
	} else {
		stdio::stdin()
	};
	let mut output = if matches.opt_present("o") {
		//FIXME
		fail!("Error: output files not implemented.")
	} else {
		stdio::stdout()
	};
	let regex = storeg(match matches.free[0] {
		Some(s) => { s }
		None => {
			short_usage(program.as_slice(), opts);
			fail!("Error: no regex provided, but required.")
		}
	});
	if matches.opt_present("g") {
		fail!("Error: searching not implemented!")
	} else {
		fail!("Error: nothing is implemented, lol.")
	}
}

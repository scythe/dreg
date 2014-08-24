# dreg - regular expression derivative-based pattern matcher

dreg is currently in the very early stage of development. The aim is simply to make
the smallest efficient regex matcher possible. Code is approximately documented in header files.

dreg is currently undergoing a rewrite in Rust. The reason for this is that I feel like learning Rust and don't have
the time to devote to many side projects, so they get condensed together.

# whatever happened to this repo?

Development of `dreg` was suspended when I lost the paper it was based on. That was many moons ago, but I
should probably finish it anyway, seeing as there isn't much left to do.

# syntax

`dreg` syntax is ever-so-slightly distinct from traditional regexen. Most symbols retain their ordinary meanings,
there are no backreferences, and the strictness operator ! is introduced. Additionally, dreg targets UTF-8, including
within character classes, e.g. [aégiß], but all matching is performed on byte arrays.

pat\* - matches 0 or more repetitions of pat

pat+ - matches 1 or more repetitions of pat

pat? - matches 0 or 1 repetitions of pat

pat{m} - matches at least m repetitions of pat

pat{m,n} - matches at least m and no more than n repetitions of pat

pat\*!, pat+!, pat{m}!, pat{m,n}! -- matches repetitions of pat "strictly", such that all repetitions must match the first character-for-character.
For example [abc]+! matches "aaa" and "cccc" but not "abacb".

pat1pat2 - matches pat1, then pat2

pat1|pat2 - matches pat1 or pat2, beginning from the same position

pat1&pat2 - matches pat1 and pat2, beginning from the same position

(^pat) - matches anything that does not match pat, possibly including the empty string.

(?!pat) - treats pat as exempt from any enclosing strictness, so that ((?![abc])[abc])+! matches "cccc", "abcb", "caba", etc, but not "abca".

/ - reserved character, so that dreg maintains some drop-in potential for e.g. domain-specific languages, should it get that far

The strictness operators are largely an algorithmic curiosity at this point, they represent my best attempt at replacing the usefulness of backreferences without actually implementing backreferences. Whether they are actually feasible to implement is yet to be determined. Some of the power of backreferences remains; in particular, the primality-testing regex /(11+)\1+/ becomes /(1{2}){2}!/

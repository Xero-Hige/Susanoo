/* This is the Porter stemming algorithm, coded up as thread-safe ANSI C
 by the author.

 It may be be regarded as cononical, in that it follows the algorithm
 presented in

 Porter, 1980, An algorithm for suffix stripping, Program, Vol. 14,
 no. 3, pp 130-137,

 only differing from it at the points maked --DEPARTURE-- below.

 See also http://www.tartarus.org/~martin/PorterStemmer

 The algorithm as described in the paper could be exactly replicated
 by adjusting the points of DEPARTURE, but this is barely necessary,
 because (a) the points of DEPARTURE are definitely improvements, and
 (b) no encoding of the Porter stemmer I have seen is anything like
 as exact as this version, even with the points of DEPARTURE!

 You can compile it on Unix with 'gcc -O3 -o stem stem.c' after which
 'stem' takes a list of inputs and sends the stemmed equivalent to
 stdout.

 The algorithm as encoded here is particularly fast.

 Release 1: the basic non-thread safe version

 Release 2: this thread-safe version

 Release 3: 11 Apr 2013, fixes the bug noted by Matt Patenaude (see the
 basic version for details)
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct stemmer stemmer_t;

void stemword(struct stemmer * z, char* s, size_t len);

stemmer_t * create_stemmer();

void free_stemmer(stemmer_t * z);

#ifdef __cplusplus
} /* extern "C" */
#endif

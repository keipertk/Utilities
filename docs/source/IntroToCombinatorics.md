Introduction To Combinatorics
=============================

The purpose of this page is to gather some notes and musings regarding
combinatorics that hopefully helps other people out as they peruse the functions
and classes in UtililitesEx.  This page also contains details pertaining to the 
algorithms behind our implementations.

Contents
--------

- [Combinations From Permutations](#combinations-from-permutations)
- [Indexing Permutations Without Repetition](#indexing-permutations-without-repetition)  
- [Indexing Permutations With Repetition](#indexing-permutations-with-repetition)

Combinations From Permutations
------------------------------

The STL provides routines `prev_permutation` and `next_permutation` that can 
generate each unique permutation of an \f$n\f$ element sequence, \f$S\f$ with a 
complexity \f$\mathcal{O}(n)\f$ (which is the same complexity as doing it 
with \f$n\f$ nested loops).  It does not however provide the corresponding 
generators for combinations. Any combination, \f$C\f$, of \f$S\f$ can be 
represented by a sequence \f$P\equiv\left[ x_1,x_2,\ldots,x_N\right]\f$ where 
\f$x_i=1\f$ (true) if the \f$i\f$-th element of \f$S\f$ is in \f$C\f$ and 
\f$x_i=0\f$ (false) otherwise.  Hence, we can generate all possible 
combinations of \f$n\f$ objects, taken \f$k\f$ at a time, by iterating over all 
unique permutations of \f$k\f$ 1s and \f$n-k\f$ 0s.  The resulting algorithm 
will then scale the same as the STL permutation routines. 

1. Fill a vector, \f$P\f$ with \f$k\f$ 1s followed by \f$n-k\f$ 0s.
2. First combination is the first \f$k\f$ elements
   - Time: 0 (it's the input)
3. Compute next permutation of \f$P\f$
   - Time: \f$\mathcal{O}(n)\f$
4. Fill combination container based off \f$P\f$
   - Time: \f$\mathcal{O}(n)\f$
5. Repeat 3 and 4, \f${n\choose k}-2\f$ more times
   - Time: \f$\mathcal{O}(n^k)\f$   

Now let us consider combinations with repetition.  To that end, assume we want 
all combinations with repetitions of an \f$n\f$ element sequence \f$S\f$, 
such that we take \f$k\f$ elements at a time.  As with our analysis of 
combinations without repeats, for a given combination with repetition, \f$C\f$,
we can again define an \f$n\f$ element sequence \f$P\equiv\left[ x_1,x_2,
\ldots,x_N\right]\f$ where \f$x_i\f$ is now the number of times the 
\f$i\f$-th element of \f$S\f$ appears in \f$C\f$ (technically a 
generalization of above).  Of course this again implies the sum of the 
\f$x_i\f$s is equal to \f$k\f$; however, now they are not restricted to 0s 
and 1s.  Hence its not enough to take all permutations of \f$P\f$, like it 
was before.  This is easily seen by setting \f$x_i=k\f$ and taking all 
permutations.  The resulting set is the \f$N\f$ combinations that can be 
formed by repeating the same element \f$k\f$ times.  This neglects say the 
combination where the first element appears \f$k-1\f$ times and the second 
element appears \f$k\f$ times.  In other words, we need to "unclump" the 
\f$x_i\f$s and then take permutations.  To this end we can instead think of 
\f$P\f$ as being \f$x_1\f$ 1s, a separator, \f$x_2\f$ 1s, a separator, ..., 
and then \f$x_n\f$ 1s. Accounting for the separators this gives us a 
permutation of \f$k\f$ 1s and \f$n-1\f$ separators.  The set of all possible 
unique permutations of \f$k\f$ 1s and \f$n-1\f$ separators (say 0s) then can be
used to generate the set of combinations with repetition.

1. Fill a vector, \f$P\f$ with \f$k\f$ 1s and \f$n-1\f$ 0s
   - Time \f$\mathcal{O}(n+k)\f$
2. First combination is first element \f$k\f$ times
   - Time \f$\mathcal{O}(k)\f$
3. Next permutation of \f$P\f$
   - Time \f$\mathcal{O}(n+k)\f$
4. Count 1s and form combinations
   - Time \f$\mathcal{O}(n+k)\f$ 
5. Repeat 3 and 4, \f${n+k-1\choose k}-2\f$ more times
   - Time: \f$\mathcal{O}(n^k+k^k)\f$           

Indexing Permutations Without Repetition
----------------------------------------

Given a sequence of length \f$N\f$ there are \f$N!\f$ permutations.  Given a 
means of ordering the permutations (here assumed to be lexicographical order), 
the goal is to be able to generate the \f$i^\text{th}\f$ permutation on 
demand (*i.e.* without permuting \f$i\f$-1 times).  To this end let us denote 
the elements of our input sequence as the numbers 0 to \f$N\f$-1.  We can 
immediately write down all permutations:
```
0,1,2,3,...,(N-2),(N-1)
0,1,2,3,...,(N-1),(N-2)
...
(N-1),(N-2),...,3,2,1,0
```
Remember these are sequences and not numbers. We need a unique means of mapping 
each of these sequences to a single unique number.  It should be clear that
simply removing the commas is not sufficient (for example the sequences
0,1,2,3,...11,12,13,... and 0,12,3,...11,1,2,13... would both be identical).
Progress can be made by making the \f$i^\text{th}\f$ digit representative of
which element was chosen.  To this end we aim to devise a number system in which
the \f$i^\text{th}\f$ digit from the left tells us which element was the 
\f$i^\text{th}\f$ chosen element of the permutation.  Mapping of a number in
this number system to decimal can then be accomplished by the usual 
conversions (the value in decimal, \f$D\f$, is given by 
\f[
D=\sum_{i=0}^N x_ib_i,
\f]
where \f$x_i\f$ is the \f$i^\text{th}\f$ digit and \f$b_i\f$ is the value of the
\f$i^\text{th}\f$ place). 

To establish our number system we start from the left.  There are \f$N\f$ 
choices for the value of the zeroth digit.  Hence we can encode its value as 
a single number in base \f$N\f$.  Once we've picked the zeroth digit we have 
\f$N\f$-1 choices for the first digit and can thus encode its value in base 
\f$N\f$-1 (*N.B.* this means value of the \f$i^\text{th}\f$ digit is always 
determined with respect to elements remaining in the sequence and that the 
elements are renumbered after each choice, but not reordered).  This leads to
the general observation that the \f$i^\text{th}\f$ digit of the number can
be expressed in base \f$N-i\f$.  Now counting from the right this means there 
is only 1 choice for the zeroth digit (0), two choices for first digit (0, 1), 
three choices for the second digit (0, 1, 2) and so on.  Consequentially, 
counting in our number system we get:
```
0000
0010
0100
0110
0200
0210
1000
1010
1100
1110
1200
1210
2000
...
```
What we see is that in order to increment the \f$i^\text{th}\f$ digit requires
\f$i!\f$ increments.  Hence the \f$i^\text{th}\f$ digit is in the \f$i!\f$ 
place (by analogy to the ones, tens, hundreds, *etc.* places).  This is the 
definition of the factorial number system (FNS).

The FNS makes it straightforward to index permutations of length \f$N\f$ when 
all values in the sequence are unique.  One converts the permutation to its FNS
value, then maps that value to decimal.  To convert to the FNS one takes the 
decimal number and divides off 1; the remainder is the zeroth digit (from 
the right).  Next, divide 2 off the integer quotient of the previous division;
the remainder is the first digit.  Then, divide 3 off the integer quotient
of the previous division, the remainder is the second digit and so on and so 
forth.  Hence the \f$i^\text{th}\f$ digit is the remainder obtained by dividing 
the integer quotient of the (\f$i\f$-1)\f$^\text{th}\f$ step by \f$i\f$+1.

Indexing Permutations With Repetition
-------------------------------------

The internet is all too quick to tell you how to index permutations without 
repeats, but information on how to do it when there are repeats is not so 
forthcoming.  This section devises such a scheme.  As with indexing permutations
with all unique elements we again assume we wish to generate the permutations in
lexicographical order.

Given a sequence of length \f$N\f$, where there are \f$M\f$ unique elements, 
the \f$i^\text{th}\f$ one appearing \f$m_i\f$ times, there are 
\f[ 
{{N} \choose {m_0, m_1, \ldots, m_M}} = \frac{N!}{\prod_{i=1}^Mm_i!}
\f]
unique permutations.  Again our purpose is to devise a number system in which
the \f$i^\text{th}\f$ digit tells us which of the \f$N\f$ elements was the 
\f$i^\text{th}\f$ element chosen from the sequence.

The procedure for establishing the value of the number in this number system is 
the same as for the case where we had no repeats aside from the fact that we, by
convention, always pull the first occurrence of a repeated element from the 
sequence.  The result is a number  in the FNS; however, not all values less than
\f$N!\f$ will appear when iterating over permutations (when multiple values 
in the FNS map to the same permutation we only obtain the smallest value).  What 
does differ from the case with no repeats is the value of the places, more over
the values of the places depends on the permutation.

To derive the values of the places let us assume our permutation is 
\f$\lbrace P\rbrace\f$.  This sequence is of length \f$N\f$, with the 
\f$i^\text{th}\f$ element being denoted \f$P_i\f$.  Starting from \f$P_0\f$ we 
can assume that by analogy with the no-repeat case it would have taken a 
number of permutations equal to the number of permutations of the sequence 
\f$\lbrace P_1,\ldots, P_N\rbrace\f$ to increment this place; however, if 
\f$P_0\f$ appears more than once in \f$\lbrace P\rbrace\f$ we actually will get 
here quicker. This is perhaps clearer with an 
example.  Consider the permutation \f$\lbrace 3,4,4\rbrace\f$, there are three 
unique permutations \f$\lbrace 3,4,4\rbrace\f$, \f$\lbrace 4,3,4 \rbrace\f$, and
\f$\lbrace 4,4,3\rbrace\f$. Respectively these map to \f$000_!\f$, \f$100_!\f$, 
and \f$110_!\f$ (the subscript "!" reminds us these are values in the FNS).  If 
we look at the second two values we conclude that the first digit will change 
after \f$\frac{2!}{1!1!}=2\f$ iterations.  However, if instead we consider the 
first value we (correctly) conclude that it will change after 
\f$\frac{2!}{2!}=1\f$ iterations.  So where'd we go wrong in deducing the place 
value of the first digit in the second two numbers?  The \f$2!\f$ ultimately 
assumed we'd also have to iterate over the value \f$\lbrace 0,1,0\rbrace\f$; 
however mapping this to a permutation shows that this is just 
\f$\lbrace 3,4,4\rbrace\f$, a permutation we've already counted.  Ultimately, 
what we really want is the number of permutations of length \f$N\f$ that start 
with \f$P_0\f$, which is just:
\f[
\frac{{{N} \choose {m_0, m_1, \ldots, m_M}}}{N}=
\frac{(N-1)!}{\prod_{i=1}^M m_i!}.
\f]
]

The final trick to this scenario is going from decimal back to FNS.  For this
we'll need the original sequence in lexicographical order, we'll call it 
\f$\lbrace S\rbrace\f$, let us assume that the number in decimal we are 
attempting to convert from is given by \f$D\f$. Furthermore let the number of 
permutations that start with \f$S_i\f$ be \f$P(S_i)\f$. Then we can compute the
FNS value by:

1. Set \f$P=0\f$
2. Loop over \f$S_i\in \lbrace S\rbrace\f$
   3. Compute \f$P(S_i)\f$
   4. Set \f$P+=P(S_i)\f$
   5. \f$D \stackrel{?}{\lt} P\f$
      - Yes: The current digit is \f$i\f$
        - Remove \f$S_i\f$ from \f$S\f$ and return to step 1
      - No: next iteration 

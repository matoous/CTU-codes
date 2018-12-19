# Autocorrect (PJC semestral homework.)

Autocorrect is command line utility for word correction. It can operate in interactive or command line mode.
Autocorrect compares words for correction with dictionary of words using Levenshtein distance and selects
the best matching words.

## Implementation

One thread implementation simply takes the word and compares it to all other words, returning words
with smallest (and same) Levenshtein distance from the provided word.

Multithreaded implementation works on vector of words using OpenMP and does the single thread process in parallel on
all the words.

## Benchmark

I added benchmark tool picobench and implemented tests for the parallel version.
Parallel version performance depends on the hardware. On my pc with i5-8600k (6 core) it was about 4 times faster
as seen in the table below:

===============================================================================
   Name (baseline is *)   |   Dim   |  Total ms |  ns/op  |Baseline| Ops/second
===============================================================================
         serial_correct * |       1 |  2062.333 |206233e4 |      - |        0.5
         parallel_correct |       1 |   543.164 |543163e3 |  0.263 |        1.8
         serial_correct * |       2 |  4151.893 |207594e4 |      - |        0.5
         parallel_correct |       2 |  1093.581 |546790e3 |  0.263 |        1.8
         serial_correct * |       4 |  8216.480 |205412e4 |      - |        0.5
         parallel_correct |       4 |  2217.357 |554339e3 |  0.270 |        1.8
===============================================================================


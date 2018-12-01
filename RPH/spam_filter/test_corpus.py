#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""Test the Corpus class."""

import os
import shutil
import unittest
import random
from datetime import datetime
from test_readClassificationFromFile import (
    random_filename, 
    random_string,
    replaced_open)

from corpus import Corpus

SPECIAL_FILENAME = '!special.txt'
CORPUS_DIR = 'testing_corpus_delete_me'
N_EMAILS = 20
LCCHARS = 'abcdefghijklmnopqrstuvwxyz'
FNAMECHARS = LCCHARS+'0123456789'
FCONTENTSCHARS = FNAMECHARS + ' \nříšžžčýůňúěďáéó'
# Dates from 2002-05-30 to 2010-08-16 will be used
DATETIME_ORDINAL_RANGE = (731000, 734000)


class TestCorpus(unittest.TestCase):

    def setUp(self):
        """Prepare classification file for the test."""
        self.expected = create_corpus_dictionary(N_EMAILS)
        create_corpus_dir_from_dictionary(self.expected)
    
    def tearDown(self):
        delete_corpus_directory()
        
    def test_corpusContainsOnlyEmails(self):
        """Test reading the corpus with email messages only."""
        corpus = Corpus(CORPUS_DIR)
        # Exercise the SUT
        observed = {}
        with replaced_open():
            for fname, contents in corpus.emails():
                observed[fname] = contents
        # Verify the results
        self.assertEqual(len(self.expected), len(observed),
                         'The emails() method did not generate all the corpus files.')
        self.assertEqual(self.expected, observed,
                         'The read file contents are not equal to the expected contents.')
        
    def test_corpusContainsAlsoSpecialFiles(self):
        """Test reading the corpus with special files."""
        # Add a special file into the corpus dir
        save_file_to_corpus_dir(
            fname=SPECIAL_FILENAME, contents='fake', dirname=CORPUS_DIR)     
        corpus = Corpus(CORPUS_DIR)
        # Exercise the SUT
        observed = {}
        with replaced_open():
            for fname, contents in corpus.emails():
                observed[fname] = contents
        # Verify the results
        self.assertEqual(len(self.expected), len(observed),
                         'The emails() method did not generate all the corpus files.')
        self.assertEqual(self.expected, observed,
                         'The read file contents are not equal to the expected contents.')


def random_email_address(namelength=5, domain2length=7, domain1length=3):
    return random_string(namelength, LCCHARS) + \
           '@' + \
           random_string(domain2length, LCCHARS) + \
           '.' + \
           random_string(domain1length, LCCHARS)


def random_date(dtrange=DATETIME_ORDINAL_RANGE):
    d = datetime.fromordinal(random.randint(dtrange[0], dtrange[1]))
    return d.strftime('%a, %d %b %Y %H:%M:%S +0200')


def create_corpus_dictionary(nitems=N_EMAILS):
    """Create a random dictionary of email file names and their contents."""
    d = {}
    for i in range(nitems):
        filename = random_filename()
        contents = create_fake_email_contents()
        d[filename] = contents
    return d
    

# The following structure contains a triple for each header:
# (header string, probability of generating this header, function used to generate the contents)
GENERATED_EMAIL_HEADERS = [
    ('Date: ', 1.0, random_date),
    ('From: ', 1.0, random_email_address),
    ('To: ', 0.5, random_email_address),
    ('Subject: ', 0.5, lambda: random_string(30, chars=FNAMECHARS+' '))
    ]

def create_fake_email_contents(n_headers=5):
    """Create email contents similar to what is requested by email RFC."""
    contents = []
    # Create required headers
    for header, prob, generator in GENERATED_EMAIL_HEADERS:
        if random.random() < prob:
            contents.append(header + generator())
    # Create additional fake headers
    for i in range(n_headers):
        contents.append(random_string(20, chars=FNAMECHARS) + ': ' + random_string(30, FNAMECHARS))
    # Add empty line separating headers and body
    contents.append('')
    # Add body
    contents.append(random_string(200, chars=FCONTENTSCHARS))
    return '\n'.join(contents)


def create_corpus_dir_from_dictionary(d, dirname=CORPUS_DIR):
    """Save the dictionary to a directory."""
    os.makedirs(dirname, exist_ok=True)
    for fname, contents in d.items():
        save_file_to_corpus_dir(fname, contents, dirname)
    

def save_file_to_corpus_dir(fname, contents, dirname=CORPUS_DIR):
    """Save the contents to the file into the dirname directory."""
    fpath = os.path.join(dirname, fname)
    with open(fpath, 'wt', encoding='utf-8') as f:
        f.write(contents)
        

def delete_corpus_directory(dirname=CORPUS_DIR):
    """Delete the directory with testing corpus."""
    shutil.rmtree(dirname, ignore_errors=True)


if __name__ == "__main__":
    unittest.main()

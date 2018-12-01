#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""Test the quality_score function."""

import unittest

from quality import quality_score

class QualityScoreTest(unittest.TestCase):

    def test_qualityScore_return1_ifConfmatContainsTpOnly(self):
        # Prepare fixture
        cm_dict = {'tp': 100, 'tn': 0, 'fp': 0, 'fn': 0}
        # Exercise the SUT
        q = quality_score(**cm_dict)
        # Assertions
        self.assertEqual(q, 1.0)

    def test_qualityScore_return1_ifConfmatContainsTnOnly(self):
        # Prepare fixture
        cm_dict = {'tp': 0, 'tn': 100, 'fp': 0, 'fn': 0}
        # Exercise the SUT
        q = quality_score(**cm_dict)
        # Assertions
        self.assertEqual(q, 1.0)

    def test_qualityScore_return1_ifCconfmatContainsTpOrTnOnly(self):
        # Prepare fixture
        cm_dict = {'tp': 100, 'tn': 100, 'fp': 0, 'fn': 0}
        # Exercise the SUT
        q = quality_score(**cm_dict)
        # Assertions
        self.assertEqual(q, 1.0)
        
    def test_qualityScore_return0_ifConfmatContainsFpOnly(self):
        # Prepare fixture
        cm_dict = {'tp': 0, 'tn': 0, 'fp': 100, 'fn': 0}
        # Exercise the SUT
        q = quality_score(**cm_dict)
        # Assertions
        self.assertEqual(q, 0.0)

    def test_qualityScore_return0_ifConfmatContainsFnOnly(self):
        # Prepare fixture
        cm_dict = {'tp': 0, 'tn': 0, 'fp': 0, 'fn': 100}
        # Exercise the SUT
        q = quality_score(**cm_dict)
        # Assertions
        self.assertEqual(q, 0.0)

    def test_qualityScore_return0_ifConfmatContainsFpOrFnOnly(self):
        # Prepare fixture
        cm_dict = {'tp': 0, 'tn': 0, 'fp': 100, 'fn': 100}
        # Exercise the SUT
        q = quality_score(**cm_dict)
        # Assertions
        self.assertEqual(q, 0.0)

    def test_qualityScore_whenConfmatHasAllCountersEqual(self):
        """
        Here we assume the quality function in the form:
        q = (tp + tn) / (tp + tn + 10*fp + fn)
        """
        # Prepare fixture
        cm_dict = {'tp': 1, 'tn': 1, 'fp': 1, 'fn': 1}
        # Exercise the SUT
        q = quality_score(**cm_dict)
        # Assertions
        self.assertEqual(q, 2/13)
    
if __name__=='__main__':
    unittest.main(exit=False)
class BinaryConfusionMatrix:
    """Used for testing quality of given filters"""

    def __init__(self, pos_tag, neg_tag):
        self.pos_tag = pos_tag  # positive tag
        self.neg_tag = neg_tag  # negative tag
        self.tp = self.tn = self.fp = self.fn = 0  # binary confusion matrix variables set to 0

    def as_dict(self):
        """
        Return binary confusion matrix as dictionary
        :return: dictionary in for tp, tn, fp, fn
        """
        return {'tp': self.tp, 'tn': self.tn, 'fp': self.fp, 'fn': self.fn}

    def update(self, truth, prediction):
        """
        Update binary confusion matrix
        :param truth: truth
        :param prediction: predicted value
        """
        if not (truth == self.pos_tag or truth == self.neg_tag) or not (prediction == self.pos_tag or prediction == self.neg_tag):
            raise ValueError  # if truth does not match negative or positive tag
        if truth == prediction:  # if true value was predicted
            if truth == self.pos_tag:
                self.tp += 1
            elif truth == self.neg_tag:
                self.tn += 1
        else:  # if mistake was made
            if truth == self.pos_tag:
                self.fp += 1
            elif truth == self.neg_tag:
                self.fn += 1

    def compute_from_dicts(self, truth_dict, pred_dict):
        """
        Computes binary confusion matrix from two given dictionaries
        :param truth_dict: dictionary with truth
        :param pred_dict: dictionary with prediction
        """
        for key, value in truth_dict.items():
            if key in pred_dict:  # if this key is also in prediction dictionary
                self.update(value, pred_dict[key])  # update binary confusion matrix

    def quality_score(self):
        """
        Score of filter from its binary confusion matrix
        :return: quality in float number
        """
        total = self.tp + self.tn + self.fp + self.fn
        return (self.tp + self.tn) / total

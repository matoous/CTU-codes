import utils
import os
import confmat


def compute_quality_for_corpus(corpus_dir):
    """
    Computes quality for given filter based on its prediction for given data
    :param corpus_dir: directory with emails and prediction
    :return:
    """
    truth_dict = utils.read_classification_from_file(os.path.join(corpus_dir, '!truth.txt'))  # read truth
    pred_dict = utils.read_classification_from_file(os.path.join(corpus_dir, '!prediction.txt'))  # read prediction
    bcm = confmat.BinaryConfusionMatrix("SPAM", "OK")  # create bin confusion matrix where SPAM is true and HAM is neg
    bcm.compute_from_dicts(truth_dict, pred_dict)  # compute from dictionaries
    dic = bcm.as_dict()
    return quality_score(dic['tp'], dic['tn'], dic['fp'], dic['fn'])  # get spam filter score


def quality_score(tp, tn, fp, fn):
    """
    Score of filter from its binary confusion matrix
    :return: quality in float number
    """
    total = tp + tn + 10*fp + fn
    return (tp + tn) / total

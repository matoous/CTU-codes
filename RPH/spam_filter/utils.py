import os
import corpus
import utils


def compute_quality_for_corpus(corpus_dir):
    """
    Computes quality for given filter based on its prediction for given data
    :param corpus_dir: directory with emails and prediction
    :return:
    """
    truth_dict = utils.read_classification_from_file(os.path.join(corpus_dir, '!truth.txt'))  # read truth
    pred_dict = utils.read_classification_from_file(os.path.join(corpus_dir, '!prediction.txt'))  # read prediction
    bcm = corpus.BinaryConfusionMatrix("SPAM","OK")  # create binary confusion matrix where SPAM is true and HAM is neg
    bcm.compute_from_dicts(truth_dict, pred_dict)  # compute from dictionaries
    nums = bcm.as_dict()  # get tp, tn, fp and fn values from binary confusion matrix
    return corpus.quality_score(nums['tp'],nums['tn'], nums['fp'], nums['fn'])  # get spam filter score


def read_classification_from_file(dir):
    """
    Reads emails classification from file
    :param dir: directory with emails and truth file
    :return: dictionary with file names (emails) and truth about them
    """
    dic = {}  # dictionary preparation
    with open(os.path.join(dir,'!truth.txt'),'r') as in_stream:
        for line in in_stream:
            key, value = line.rstrip().split(' ')
            dic[key] = value
    return dic


def write_classification_to_file(dir, dic):
    """
    Saves given prediction dictionary to file in given directory
    :param dir: directory where the prediction should be saved
    :param dic: dictionary with email classification prediction
    """
    with open(os.path.join(dir,"!prediction.txt"),'w',encoding="utf-8") as f:
        for key in dic:
            f.write(key + " " + dic[key] + "\n")


def clean_up(dir):
    """
    Cleans !prediction file from directory
    :param dir: directory to clear
    """
    for (dirpath, dirname, files) in os.walk(dir):
        for name in files:
            if name == '!prediction.txt':
                os.remove(os.path.join(dir,'!prediction.txt'))
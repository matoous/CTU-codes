from collections import Counter
from html.parser import HTMLParser

import utils
from confmat import BinaryConfusionMatrix
from corpus import Corpus

email_keys = ['X-Sender', 'From', 'Content-Type', 'Subject', 'Sender']  # keys I will be looking at, there are more,
# but I like these
stopwords = ["i", "a", "about", "above", "above", "across", "after", "afterwards", "again", "against", "all", "almost",
             "alone", "along", "already", "also", "although", "always", "am", "among", "amongst", "amoungst", "amount",
             "an", "and", "another", "any", "anyhow", "anyone", "anything", "anyway", "anywhere", "are", "around", "as",
             "at", "back", "be", "became", "because", "become", "becomes", "becoming", "been", "before", "beforehand",
             "behind", "being", "below", "beside", "besides", "between", "beyond", "bill", "both", "bottom", "but",
             "by", "call", "can", "cannot", "cant", "co", "con", "could", "couldnt", "cry", "de", "describe", "detail",
             "do", "done", "down", "due", "during", "each", "eg", "eight", "either", "eleven", "else", "elsewhere",
             "empty", "enough", "etc", "even", "ever", "every", "everyone", "everything", "everywhere", "except", "few",
             "fifteen", "fify", "fill", "find", "fire", "first", "five", "for", "former", "formerly", "forty", "found",
             "four", "from", "front", "full", "further", "get", "give", "go", "had", "has", "hasnt", "have", "he",
             "hence", "her", "here", "hereafter", "hereby", "herein", "hereupon", "hers", "herself", "him", "himself",
             "his", "how", "however", "hundred", "ie", "if", "in", "inc", "indeed", "interest", "into", "is", "it",
             "its", "itself", "keep", "last", "latter", "latterly", "least", "less", "ltd", "made", "many", "may", "me",
             "meanwhile", "might", "mill", "mine", "more", "moreover", "most", "mostly", "move", "much", "must", "my",
             "myself", "name", "namely", "neither", "never", "nevertheless", "next", "nine", "no", "nobody", "none",
             "noone", "nor", "not", "nothing", "now", "nowhere", "of", "off", "often", "on", "once", "one", "only",
             "onto", "or", "other", "others", "otherwise", "our", "ours", "ourselves", "out", "over", "own", "part",
             "per", "perhaps", "please", "put", "rather", "re", "same", "see", "seem", "seemed", "seeming", "seems",
             "serious", "several", "she", "should", "show", "side", "since", "sincere", "six", "sixty", "so", "some",
             "somehow", "someone", "something", "sometime", "sometimes", "somewhere", "still", "such", "system", "take",
             "ten", "than", "that", "the", "their", "them", "themselves", "then", "thence", "there", "thereafter",
             "thereby", "therefore", "therein", "thereupon", "these", "they", "thickv", "thin", "third", "this",
             "those", "though", "three", "through", "throughout", "thru", "thus", "to", "together", "too", "top",
             "toward", "towards", "twelve", "twenty", "two", "un", "under", "until", "up", "upon", "us", "very", "via",
             "was", "we", "well", "were", "what", "whatever", "when", "whence", "whenever", "where", "whereafter",
             "whereas", "whereby", "wherein", "whereupon", "wherever", "whether", "which", "while", "whither", "who",
             "whoever", "whole", "whom", "whose", "why", "will", "with", "within", "without", "would", "yet", "you",
             "your", "yours", "yourself", "yourselves", "the"]  # english stopwords
chars = [">", "-", "*", "|", "--", "&", "'", '"', "(", ")"]  # chars I do not want to count

do_not_count = stopwords + chars  # all stopwords/chars I do not want to count in classification


class MLStripper(HTMLParser):
    """This will be used to clean string from html tags"""
    def __init__(self):
        self.reset()
        self.strict = False
        self.convert_charrefs = True
        self.fed = []

    def handle_data(self, d):
        self.fed.append(d)

    def get_data(self):
        return ''.join(self.fed)


class MyFilter:
    """SPAM filter by Matous Dzivjak"""

    def __init__(self):
        """
        Instance of MyFilter
        :rtype: object
        """
        self.SPAM_words = {}  # save dictionaries with SPAM words here
        self.HAM_words = {}  # save dictionaries with HAM words here
        self.classification = {}  # save classification for words here
        self.predictions = {}  # save file HAM or SPAM predictions here
        self.SPAM_percentage = 0  # SPAM percentage in learning corpus
        self.truth = {}  # truth about learning emails
        self.username = "dzivjmat"

    def classify_part(self, train_corpus, part):
        """
        Classify part of header
        :param train_corpus: training corpus with mails
        :param part: which part of header are we analyzing now
        """
        self.SPAM_words[part] = Counter()  # prepare counter for SPAM words in given part
        self.HAM_words[part] = Counter()  # prepare counter for HAM words in given part
        for fname, email in train_corpus.emails():  # for each email
            if part in email:  # if email has this part in header
                text = self.preprocess(email[part])  # prepare text
                if self.truth[fname] == 'SPAM':  # if it is a SPAM
                    for word in text:
                        if word not in do_not_count:  # clear stop words and chars
                            self.SPAM_words[part][word] += 1  # plus one for this word
                else:  # if it is a HAM
                    for word in text:
                        if word not in do_not_count:  # clear stop words and chars
                            self.HAM_words[part][word] += 1  # plus one for this HAM word
        self.classification[part] = {}  # prepare dictionary for classification words fot this part of header
        for s in self.SPAM_words[part]:  # for each SPAM word of this part
            if self.HAM_words[part].get(s):  # if it is also in HAM
                if self.SPAM_words[part][s] + self.HAM_words[part][s] > 2:
                    self.classification[part][s] = self.SPAM_words[part][s] / (
                    self.SPAM_words[part][s] + self.HAM_words[part][s])  # save percentage of emails with this word that are SPAM
            else:  # if word occurs only in SPAM
                if self.SPAM_words[part][s] > 2:
                    self.classification[part][s] = 0.99
        for s in self.HAM_words[part]:
            if not self.classification[part].get(s):  # for each word that is only in HAM
                if self.HAM_words[part][s] > 2:
                    self.classification[part][s] = 0.01

    def strip_tags(self, html):
        """
        Cleans html tags from the text
        :param html: html string with tags
        :return: string without html tahs
        """
        s = MLStripper()  # god bless you!
        s.feed(html)
        return s.get_data()

    def preprocess(self, sentence):
        """
        Preprocesses data for further processing
        :param sentence: string
        :return: list of words
        """
        for char in chars:  # replace all chars, I do not want them in my words
            sentence.replace(char," ")
        sentence = self.strip_tags(sentence)  # get rid of html tags
        words = sentence.strip().lower().split()  # strip end and front white spaces, lower all, split to words
        return words  # return list of words

    def classify_payload(self, train_corpus):
        """
        Classify email payload
        :param train_corpus: corpus with truth
        """
        self.SPAM_words['payload'] = Counter()  # set counter for SPAM
        self.HAM_words['payload'] = Counter()  # set counter for HAM
        for fname, email in train_corpus.emails():  # for each email in train corpus
            text = ""
            if email.is_multipart():  # if payload is multipart
                for part in email.walk():
                    ctype = part.get_content_type()
                    cdispo = str(part.get('Content-Disposition'))
                    if ctype == 'text/plain' and 'attachment' not in cdispo:
                        text += part.get_payload()
            else:  # payload is just one part
                text += email.get_payload()
            words = self.preprocess(text)  # preprocess
            if self.truth[fname] == 'SPAM':  # is SPAM
                for word in words:
                    if word not in do_not_count:  # clean stop words and chars
                        self.SPAM_words['payload'][word] += 1  # add one
            else:
                for word in words:
                    if word not in do_not_count:  # clean stop words and chars
                        self.HAM_words['payload'][word] += 1
        self.classification['payload'] = {}  # prepare dictionary with classification for words
        for s in self.SPAM_words['payload']:  # for each SPAM word
            if self.HAM_words['payload'].get(s):  # if the word is also in HAM
                self.classification['payload'][s] = self.SPAM_words['payload'][s] / (
                self.SPAM_words['payload'][s] + self.HAM_words['payload'][s])  # percentage of SPAM emails with given word
            else:  # if word is only in SPAM
                self.classification['payload'][s] = 1
        for s in self.HAM_words['payload']:  # if the word is only in HAM
            if not self.classification['payload'].get(s):  # just to be sure
                self.classification['payload'][s] = 0

    def train(self, train_corpus_dir):
        """
        Train the corpus on given emails dataset
        :param train_corpus_dir:
        """
        self.truth = utils.read_classification_from_file(train_corpus_dir)  # load truth
        train_corpus = Corpus(train_corpus_dir)
        self.get_SPAM_percentage(train_corpus)  # not in use now
        for val in email_keys:      # get values for parts of email header
            self.classify_part(train_corpus, val.lower())
        self.classify_payload(train_corpus)  # get values for email payload/body

    def clean_values(self):
        """
        NOT IN USE NOW
        Cleans the stuff up, so the filter works better
        Specifically get rid off mid values - something that appears both in SPAM and HAM
        """
        to_delete = []  # add things to delete here, or the for cycle wont work, since classification len would change
        for dic in self.classification:  # for each dictionary in classification
            for key in self.classification[dic]:  # for each word in dictionary
                value = self.classification[dic][key]
                if 0.4 < value < 0.6:  # if value is in middle
                    to_delete.append([dic, key])
        for dic, key in to_delete:  # delete given values
            del self.classification[dic][key]

    def get_stat(self, key, msg):
        """
        Gets value from 0 to 1, 0 for HAM 1 for SPAM for given part of header of given email
        :param key: header part
        :param msg: email
        :return: value from 0 to 1
        """
        words = self.preprocess(msg[key])  # get value for some part of header of email
        counter = 0  # count found words
        value = 0  # sum value
        for word in words:
            if word in self.classification[key]:
                value += self.classification[key][word]
                counter += 1
        if counter == 0:
            return 0.5
        return value / counter  # magic number

    def get_SPAM_percentage(self, train_corpus):
        """
        Gets SPAM percentage in training corpus, not in use now, but might use it one day
        :param train_corpus: training corpus with mails and truth
        """
        total = 0
        SPAM = 0
        for name, body in train_corpus.emails():
            total += 1
            if self.truth[name] == 'SPAM':
                SPAM += 1
        self.SPAM_percentage = SPAM / total

    def get_stat_payload(self, msg):
        """
        Gets the status of payload
        value closer to 1 means the payload is SPAM and otherwise
        :param msg: email payload
        :return: value from 0 to 1, 0 for HAM 1 for SPAM
        """
        huge_string = ""
        if msg.is_multipart():  # if payload is multipart
            for part in msg.walk():
                ctype = part.get_content_type()
                cdispo = str(part.get('Content-Disposition'))
                if ctype == 'text/plain' and 'attachment' not in cdispo:
                    huge_string += part.get_payload()
        else:  # payload has one part
            huge_string += msg.get_payload()
        words = self.preprocess(huge_string)
        counter = 0  # counts words
        value = 0  # sums value
        for word in words:  # for each word in emails payload
            if word in self.classification['payload']:  # this should happen always, but just to be sure
                value += self.classification['payload'][word]
                counter += 1
        if counter == 0:  # if body was empty or something strange happened
            return 0.5
        return value / counter  # the magic

    def test(self, test_corpus_dir):
        """
        Tests given emails for being SPAM or HAM
        :param test_corpus_dir: directory with emails
        """
        test_corpus = Corpus(test_corpus_dir)  # instance of corpus for walking emails
        self.load_from_memory()  # load memory, in case no training was made, if training was made it makes it better
        for name, msg in test_corpus.emails():  # for name of file and the email
            values = []
            for a in msg.keys():  # for all email header parts
                a = a.lower()
                if self.classification.get(a):
                    values.append(self.get_stat(a, msg))  # get status of this part, values closer to 1 mean SPAM
            values.append(self.get_stat_payload(msg))  # get status of body/payload
            final_stat = sum(values) / (len(values))  # get final value
            if final_stat > 0.5:  # if value is closer to being SPAM
                self.predictions[name] = 'SPAM'
            else:  # if value is close to HAM
                self.predictions[name] = 'OK'
        utils.write_classification_to_file(test_corpus_dir, self.predictions)  # safe created classification
        self.save_to_memory()  # save the memory in case something new was learned on training data

    def load_from_memory(self):
        """
        Loads SPAM filter memory from file
        """
        with open("memory.txt","r+",encoding="utf-8") as f:  # memory directory and file
            for line in f.readlines():
                part, key, value = line.split()
                if self.classification.get(part):  # if already has this header part or body in memory
                    if self.classification[part].get(key):  # if already has this word in memory
                        self.classification[part][key] = (float(value) + self.classification[part][key]) / 2
                    else:
                        self.classification[part][key] = float(value)
                else:  # if only in memory but not in training corpus
                    self.classification[part] = {}
                    self.classification[part][key] = float(value)

    def save_to_memory(self):
        """
        Saves classification to memory
        If the word is already in memory, saves average
        """
        with open("memory.txt","r+",encoding="utf-8") as f:  # folder and file with memory
            for line in f.readlines():  # load saved data
                part, key, value = line.split()
                if self.classification[part].get(key):  # if same word was in training directory
                    self.classification[part][key] = (float(value) + self.classification[part][key]) / 2
                else:
                    self.classification[part][key] = float(value)
            for part in self.classification:  # save all (wow, is so huge.......)
                for key in self.classification[part]:
                    if self.classification[part][key]:
                        f.write(part + " " + key + " " + str(self.classification[part][key]) + "\n")


def convert(file,out):
    """
    I used this to convert the truth file for emails I found on internet, where the SPAM or HAM was before file name
    :param file:
    :param out:
    """
    dic = {}
    with open(file,'r', encoding="utf-8") as f:
        for line in f.readlines():
            key, val = line.split()
            dic[val] = key
    with open(out,'w',encoding="utf-8") as f:
        for key in dic:
            f.write(key + " " + dic[key] + "\n")


if __name__ == "__main__":
    # used for testing and debugging
    filter = MyFilter()
    filter.test("SPAM-data/2/")
    confusion_matrix = BinaryConfusionMatrix('SPAM', 'OK')
    confusion_matrix.compute_from_dicts(utils.read_classification_from_file("SPAM-data/2/"), filter.predictions)
    print("Quality: %.2f%%" % (confusion_matrix.quality_score() * 100))
    utils.clean_up("SPAM-data/2/")  # clean !truth

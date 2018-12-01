import os
import email


class Corpus:
    """Corpus will have saved dictionary of filenames and truth about them"""
    def __init__(self, directory, no_encoding=False):
        self.no_encoding = no_encoding  # used for importing this huge chunk of emails I found on internet
        if not os.path.isdir(directory):  # used for debugging
            print("Path is not OK")
        self.directory = directory  # save dictionary

    def emails(self):
        """
        method used for walking emails and their classification as SPAM or HAM
        """
        for (dirpath, dirname, files) in os.walk(self.directory):
            for name in files:  # for each file in directory
                if not name[0] == '!':  # if file doesnt begin with ! - these are files with truth or prediction
                    if not self.no_encoding:  # used normally
                        with open(os.path.join(dirpath, name), 'r', encoding="utf-8") as opened:
                            yield [name, email.message_from_file(opened)]  # yield file name and truth about it
                    else:  # used for that huge learning chunk
                        with open(os.path.join(dirpath, name), 'r') as opened:
                            yield [name, email.message_from_file(opened)]  # yield file name and truth about it

            

from nltk.tokenize import TweetTokenizer

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        # initialize nltk's tokenizer
        # (see: http://www.nltk.org/api/nltk.tokenize.html)
        self.tokenizer = TweetTokenizer()

        # initialize positives/negatives
        self.positives = set()
        self.negatives = set()
        
        # load positives
        with open(positives) as fpos:
            for word in fpos:
                word = word.strip()
                if self.validate(word):
                    self.positives.add(word)

        # load negatives
        with open(negatives) as fneg:
            for word in fneg:
                word = word.strip()
                if self.validate(word):
                    self.negatives.add(word)

        # done initializing

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        # tokenize input text (tokens will = list of str)
        tokens = self.tokenizer.tokenize(text)
        
        # set a running-total for score
        score = 0
        
        # update total for all tokens
        for token in tokens:
            token = token.lower()
            if token in self.positives:
                score += 1
            elif token in self.negatives:
                score -= 1

        return score

    def validate(self, word):
        """Validate words (as file input)"""
        return len(word) > 0 and not word.startswith(';')
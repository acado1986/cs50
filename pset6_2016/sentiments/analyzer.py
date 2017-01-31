import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        # open files and create a set attribute
        with open(positives, 'r') as file:
            self.positives = {line.strip(' \t\n\r') for line in file if not line.startswith(';')}
            self.positives.discard('')
        with open(negatives, 'r') as file:
            self.negatives = {line.strip(' \t\n\r') for line in file if not line.startswith(';')}
            self.negatives.discard('')

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        # tokenize text
        tokens = nltk.wordpunct_tokenize(text)
        
        # return result
        pos = len(self.positives.intersection(tokens))
        neg = len(self.negatives.intersection(tokens))
        return pos - neg
        

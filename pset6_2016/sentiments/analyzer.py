import nltk
from collections import Counter

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
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        
        # return result
        pos = sum(Counter(word for word in tokens if word.lower() in self.positives).values())
        neg = sum(Counter(word for word in tokens if word.lower() in self.negatives).values())
        return pos - neg
        

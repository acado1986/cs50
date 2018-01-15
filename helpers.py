import csv
import urllib.request

from flask import redirect, render_template, request, session, url_for
from functools import wraps
from html.parser import HTMLParser

def apology(top="", bottom=""):
    """Renders message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
            ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=escape(top), bottom=escape(bottom))
def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/0.11/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect(url_for("login", next=request.url))
        return f(*args, **kwargs)
    return decorated_function


def lookup(symbol, data_size="full",  function="TIME_SERIES_INTRADAY", interval=1):
    """Look up quote for symbol. 
    Optional arguments: data_size will return the original data from Alpha Vantage if supplied with 
    keyword 'full'
                        examples of keywords for the other two arguments can be found at Alpha Vantage Documentation.
    """

    # reject symbol if it starts with caret
    if symbol.startswith("^"):
        return None

    # reject symbol if it contains comma
    if "," in symbol:
        return None

    # query Alpha Vantage for quote instead
    # https://www.alphavantage.co/documentation/
    try:

        # GET CSV
        url = "https://www.alphavantage.co/query?apikey=NAJXWIA8D6VN6A3K&datatype=csv&function={function}&interval={interval}min&symbol={symbol}".format(function=function, interval=interval, symbol=symbol)
        webpage = urllib.request.urlopen(url)

        # parse CSV
        datareader = csv.reader(webpage.read().decode("utf-8").splitlines())

        if data_size != "full":
            return list(datareader)
        else:
            # ignore first row
            next(datareader)

            # parse second row
            row = next(datareader)

            # ensure stock exists
            try:
                price = float(row[4])
            except:
                return None

            # return stock's name (as a str), price (as a float), and (uppercased) symbol (as a str)
            return {
                "name": symbol.upper(), # for backward compatibility with Yahoo
                "price": price,
                "symbol": symbol.upper()
            }

    except:
        return None

def usd(value):
    """Formats value as USD."""
    return "${:,.2f}".format(value)

class ScrapeTableYF(HTMLParser):
    """
    It save's the data in a list of lists. The first list contains the
    header of the table  and the rest are data lists.
    The index of each header element corresponds to the index of each
    data element in the data lists.
    """
    def __init__(self):
        # html.parser requirement to initialize instace attributes
        HTMLParser.__init__(self)
        self.idx = 0
        self.dataset = list()
        self.isrow = False

    def handle_starttag(self, tag, attrs):
        # for each table row founded start new list
        if tag == 'tr':
            self.dataset.append(list())
            self.isrow = True

    def handle_data(self, data):
        # append data, exclude the '+' sign because Yahoo encloses it
        # in comments tags and it is not joined with the rest of the
        # data in 'td' tags, the '-' sign is no problem
        if self.isrow and data != '+':
            self.dataset[self.idx].append(data)

    def handle_endtag(self, tag):
        # end of a table row end the current list ,prepare next list
        if tag == 'tr':
            self.idx += 1
            self.isrow = False

    def get_data(self):
        # return data as a list of dictionaries
        d = [{key:value for key, value in zip(self.dataset[0], self.dataset[x])} for x in range(1, len(self.dataset))]
        return d

def scrape_yahoo(page):
    """
    Scrape https://finance.yahoo.com/{pages} for table that contains
    financial data. ex: {most-active}, {trending-tickers} etc.
    """
    # open yahoo finance page
    url = 'https://finance.yahoo.com/{}'.format(page)
    webpage = urllib.request.urlopen(url)
    content = str(webpage.read())

    # scrape for a table in the page
    parser = ScrapeTableYF()
    parser.feed(content)

    return parser.get_data()

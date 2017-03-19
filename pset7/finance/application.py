from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    return apology("TODO")

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":

        # save required data for buying
        shares = request.form.get("shares", type=int)
        stock = lookup(request.form.get("symbol"))
        user_cash = db.execute("SELECT cash FROM users WHERE id=:user_id", user_id=session["user_id"])[0]["cash"]

        # check if input values are valid
        if shares == None or shares  <= 0:
            return apology("shares amount is wrong", "input a positve number")
        if stock == None:
            return apology("stock symbol doesn't exits", "try again")

        # check if the user can afford to buy
        if user_cash < shares * stock["price"]:
           return apology("You can afford to buy", "Try buying less")

        # buy shares and update user's database
        try:
            # update transactions table with current transactions data
            rows1 = db.execute("INSERT INTO transactions (trans_type, id, cash_before, cash_current ,company, symbol, price, shares) VALUES (1, :user_id, :cash_before, :cash_current, :company, :symbol, :price, :shares)", user_id=session["user_id"], cash_before=user_cash, cash_current=user_cash - shares * stock["price"], company=stock["name"], symbol=stock["symbol"], price=stock["price"], shares=shares)

            # update the users table cash amount
            rows2 = db.execute("UPDATE users SET cash=(select cash_current from transactions where timestamp=(select max(timestamp) from transactions where id=:user_id)) WHERE id=:user_id", user_id=session["user_id"])

            # check for constraints
            if rows1 == None or rows2 == None:
                return apology("Error database not updated", "Database constraints violated")

        # in case of errors
        except RuntimeError:
            return apology("RuntimeError updating database", "Try again")

        return redirect(url_for("buy"))

    # if user routed via GET method
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    return apology("TODO")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

# forget any user_id
    session.clear()

# if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

# else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

# forget any user_id
    session.clear()

# redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # if a symbol is searched via POST method
    if request.method == "POST":

        # lookup quote
        quote = lookup(request.form.get("symbol"))

        # show result
        return render_template("quoted.html", name=quote["name"], price=quote["price"], symbol=quote["symbol"])
    # render form to search a symbol
    elif request.method == "GET":
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # ensure password is identical
        elif request.form.get("password") != request.form.get("retyped_password"):
            return apology("passwords don't match")

        # insert user in database
        # insert into database
        try:
            rows = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=request.form.get("username"), hash=pwd_context.encrypt(request.form.get("password")))
            if rows == None:
                return apology("username already exists")

        except RuntimeError:
            return apology("RuntimeError database")

        # remember which user has logged in
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        if rows != 1:
            session["user_id"] = rows[0]["id"]

        # make the first transaction
        # 0 = deposit, 1 = buy, 2 = sell
        rows = db.execute("INSERT INTO transactions (trans_type, id, cash_before, cash_current) VALUES (0, :user_id, 0, (SELECT cash FROM users WHERE id=:user_id))", user_id=session["user_id"])

        # redirect to login page after registration
        return redirect(url_for("index"))

    # if user reached route via GET
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    return apology("TODO")

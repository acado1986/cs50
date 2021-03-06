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

    # retrieve data from transactions Sqlite table
    try:
        rows = db.execute("SELECT symbol, sum(shares) AS shares FROM transactions WHERE users_id=:user_id GROUP BY symbol HAVING symbol!=''",user_id=session["user_id"])

        # check for empty list, no transactions
        if len(rows) < 1:
            return apology("No transactions founded", "Buy shares")

    # catch errors
    except RuntimeError:
        return apology("RuntimeError database", "Try again")

    # append new dict objectes to rows (keys: company, Price, Total)
    for row in rows:
        row.update({"company":lookup(row["symbol"])["name"], "price":lookup(row["symbol"])["price"]})
        row.update({"market_value":row["price"] * row["shares"]})

    # sum the value of total shares owned by the user
    total_market_value = sum(row["market_value"] for row in rows)

    # retrieve user curent cash flow
    try:
        user_cash=db.execute("SELECT cash FROM users WHERE id=:user_id", user_id=session["user_id"])

        # check for errors
        if len(user_cash) != 1:
            return apology("Error retrieving data", "Contact site administrator")

    # catch error on database
    except RuntimeError:
        return apology("RuntimeError database", "Try again")

    # sum total assets
    total_assets = total_market_value + user_cash[0]["cash"]

    # render page
    return render_template("index.html", rows=rows, total_market_value=str(total_market_value), user_cash=str(round(user_cash[0]["cash"], 2)), total_assets=str(round(total_assets, 2)))

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
        if shares == None or shares <= 0:
            return apology("shares amount is wrong", "input a positve number")
        if stock == None:
            return apology("stock symbol doesn't exits", "try again")

        # check if the user can afford to buy
        if user_cash < shares * stock["price"]:
           return apology("You can afford to buy", "Buy cheaper or deposit more cash")

        # buy shares and update user's database
        try:
            # update transactions Sqlite table with current transactions data
            # transactions table has 3 different operations type: buy, sell, deposit
            rows = db.execute("INSERT INTO transactions (users_id, type, symbol, price_deposit, shares) VALUES (:users_id,'buy', :symbol, :price_deposit, :shares)", users_id=session["user_id"],symbol=stock["symbol"], price_deposit=stock["price"], shares=shares)

            # check for constraints
            if rows == None:
                return apology("Error database not updated", "Database constraints violated")

        # in case of errors
        except RuntimeError:
            return apology("RuntimeError updating database", "Try again")

        # return to homepage
        flash("Bought!")
        return redirect(url_for("index"))

    # if user routed via GET method
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    # retrieve data from "transaction" Sqlite table
    try:
        rows = db.execute("SELECT timestamp, type, symbol, price_deposit, shares FROM transactions WHERE users_id=:user_id ORDER BY timestamp DESC", user_id=session["user_id"])

        # check for null data
        if len(rows) < 1:
            return apology("There are no transactions")
    # catch errors
    except RuntimeError:
        return apology("RuntimeError database")

    # append to the returned dict a new key: company and total
    for row in rows:
        # check for cash deposit transaction
        if row["type"] != "deposit":
            row.update({"company":lookup(row["symbol"])["name"]})
            row.update({"total":-(row["price_deposit"] * row["shares"])})

    # render page
    return render_template("history.html", rows=rows)

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
        try:
            rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

            # ensure username exists and password is correct
            if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
                return apology("invalid username and/or password")

        # check for reading errors
        except RuntimeError:
            return apology("Error quering database")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]
        session["username"] = rows[0]["username"]

        # redirect user to home page
        flash("You were successfully logged in")
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

        # redirect to login page after registration
        return redirect(url_for("index"))

    # if user reached route via GET
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":

        # save required data for selling
        shares = request.form.get("shares", type=int)
        stock = lookup(request.form.get("symbol"))

        # check if input values are valid
        if shares == None or shares <= 0:
            return apology("shares amount is wrong", "input a positve number")
        if stock == None:
            return apology("stock symbol doesn't exits", "try again")

        # number of shares to sell not higher than total owned
        try:
            owned_shares = db.execute("SELECT sum(shares) AS shares FROM transactions WHERE users_id=:users_id AND symbol=:symbol", users_id=session["user_id"],symbol=stock["symbol"])[0]["shares"]

            if owned_shares <= 0 or shares > owned_shares:
                return apology("You dont't own enough shares of this company")
            else:
                # update database with selled shares
                try:
                    # transactions table has 3 different operations type: buy, sell, deposit
                    # add shares as negative integer for a easy query on total shares
                    rows = db.execute("INSERT INTO transactions (users_id, type, symbol, price_deposit, shares) VALUES (:users_id,'sell', :symbol, :price_deposit, :shares)", users_id=session["user_id"],symbol=stock["symbol"], price_deposit=stock["price"], shares=-shares)

                    # check for constraints
                    if rows == None:
                        return apology("Error database not updated", "Database constraints violated")

                # in case of errors
                except RuntimeError:
                    return apology("RuntimeError updating database", "Try again")

        # in case of error
        except RuntimeError:
            return apology("RuntimeError quering database", "Try again")

        # return to homepage
        flash("Sold!")
        return redirect(url_for("index"))

    # if user routed via GET method
    else:
        return render_template("sell.html")

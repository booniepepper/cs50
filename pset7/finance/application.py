from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

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
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    """index - home page that shows details for a user"""

    user_id = session["user_id"]
    
    transactions = db.execute("SELECT symbol, SUM(shares) as shares FROM transactions WHERE user_id=:user_id GROUP BY symbol;", user_id=user_id)
    user = db.execute("SELECT username, cash FROM users WHERE id=:user_id;", user_id=user_id)
    
    # ensure username exists
    if len(user) != 1:
        return apology("invalid username, please log in")
    
    user = user[0]
    user["cash"] = usd(user["cash"])
    
    # remove any transactions results that have 0 shares
    transactions = list(filter(lambda t: t["shares"] > 0, transactions))
    
    # get current prices, set total value
    for transaction in transactions:
        transaction["price"] = lookup(transaction["symbol"])["price"]
        transaction["total"] = transaction["price"] * transaction["shares"]
        transaction["price"] = usd(transaction["price"])
        transaction["total"] = usd(transaction["total"])
        
    
    
    return render_template("index.html", user=user, transactions=transactions, trans_len=len(transactions))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    bought = False;
    transaction = {}
    
    # handle POST method
    if request.method == "POST":
        
        # ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide a stock symbol")
        
        symbol = request.form.get("symbol").upper()
        
        # ensure symbol was submitted
        if not request.form.get("shares") or int(request.form.get("shares")) < 1:
            return apology("must provide a nonzero, positive number of shares")
    
        shares = int(request.form.get("shares"))

        # get quote for symbol
        quote = lookup(symbol)
        if quote == None:
            return apology("no stock symbol found called {}".format(symbol))        
        
        # get cash and id for user
        user = db.execute("SELECT cash, id FROM users WHERE id = :id;", id=session["user_id"])

        # ensure username exists
        if len(user) != 1:
            return apology("invalid username, please log in")
        
        user = user[0]
            
        total_price = quote["price"] * shares
        
        if (total_price > user["cash"]):
            return apology("insufficient funds")
        
        trans_id = db.execute("INSERT INTO transactions(user_id, symbol, price, shares, total) VALUES (:user_id, :symbol, :price, :shares, :total);",
            user_id=user["id"], symbol=symbol, price=quote["price"], shares=shares, total=total_price)
        
        db.execute("UPDATE users SET cash=:cash WHERE id=:user_id;", cash=user["cash"]-total_price, user_id=user["id"])
        
        transaction = db.execute("SELECT * FROM transactions WHERE id=:trans_id;", trans_id=trans_id)

        # ensure transaction exists
        if len(transaction) != 1:
            return apology("transaction error")
        
        transaction = transaction[0]
        bought = True
        
        transaction["price"] = usd(transaction["price"])
        transaction["total"] = usd(transaction["total"])
    
    return render_template("buy.html", bought=bought, transaction=transaction)

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    
    trans_len = 0
    
    user = db.execute("SELECT username FROM users WHERE id=:user_id", user_id=session["user_id"])
    transactions = db.execute("SELECT * FROM transactions WHERE user_id=:user_id ORDER BY timestamp DESC;", user_id=session["user_id"])

    trans_len = len(transactions)
    
    if trans_len > 0:
        for transaction in transactions:
            (transaction["type"], transaction["color"], transaction["pre"]) = ("Purchase", "red", "-") if transaction["shares"] > 1 else ("Sale", "green", "+")
            transaction["shares"] = abs(transaction["shares"])
            transaction["price"] = usd(transaction["price"])
            transaction["total"] = usd(transaction["total"])

    return render_template("history.html", user=user, trans_len=trans_len, transactions=transactions)

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
    
    quoted = False
    quote = {}
    
    # handle POST method
    if request.method == "POST":
        
        # ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide a stock symbol")
        
        # get quote
        quote = lookup(request.form.get("symbol"))
        if quote == None:
            return apology("no stock symbol found called {}".format(request.form.get("symbol")))
            
        quote["price"] = usd(quote["price"])
        
        quoted=True
    
    return render_template("quote.html", quoted=quoted, quote=quote)

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    inserted = False
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password") or not request.form.get("password2"):
            return apology("must provide password")

        # ensure password was submitted
        elif request.form.get("password") != request.form.get("password2"):
            return apology("passwords didn't match")
        
        # ensure username doesn't exist in database yet
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        if len(rows) != 0:
            return apology("user with that name already exists")
        
        db.execute("INSERT INTO users(username, hash) values (:username, :hash);",
            username=request.form.get("username"), hash=pwd_context.hash(request.form.get("password")))
        
        inserted = True
        
        
    return render_template("register.html", inserted=inserted)

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    sold = False;
    transaction = {}
    
    # handle POST method
    if request.method == "POST":
        
        # ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide a stock symbol")
        
        symbol = request.form.get("symbol").upper()
        
        # ensure symbol was submitted
        if not request.form.get("shares") or int(request.form.get("shares")) < 1:
            return apology("must provide a nonzero, positive number of shares")
    
        shares = int(request.form.get("shares"))
        
        # check available shares to sell
        transactions = db.execute("SELECT symbol, SUM(shares) as shares FROM transactions WHERE user_id=:user_id GROUP BY symbol;", user_id=session["user_id"])
        # filter to shares for this symbol
        transactions = list(filter(lambda t: t["symbol"] == symbol, transactions))
        
        if len(transactions) != 1:
            apology("you have no transactions for that symbol")
        transaction = transactions[0]
        
        if transaction["shares"] < 1:
            apology("you have no shares to sell for that symbol")
        
        available_shares = transaction["shares"]
        
        if shares > available_shares:
            return apology("you don't have that many shares")

        # get quote for symbol
        quote = lookup(symbol)
        if quote == None:
            return apology("no stock symbol found called {}".format(symbol))        
        
        # get cash and id for user
        user = db.execute("SELECT cash, id FROM users WHERE id = :id;", id=session["user_id"])

        # ensure username exists
        if len(user) != 1:
            return apology("invalid username, please log in")
        
        user = user[0]
            
        total_price = quote["price"] * shares
        
        trans_id = db.execute("INSERT INTO transactions(user_id, symbol, price, shares, total) VALUES (:user_id, :symbol, :price, :shares, :total);",
            user_id=user["id"], symbol=symbol.upper(), price=quote["price"], shares=0-shares, total=total_price)
        
        db.execute("UPDATE users SET cash=:cash WHERE id=:user_id;", cash=user["cash"]+total_price, user_id=user["id"])
        
        transaction = db.execute("SELECT * FROM transactions WHERE id=:trans_id;", trans_id=trans_id)

        # ensure transaction exists
        if len(transaction) != 1:
            return apology("transaction error")
        
        transaction = transaction[0]
        transaction["price"] = usd(transaction["price"])
        transaction["total"] = usd(transaction["total"])
        sold = True
    
    return render_template("sell.html", sold=sold, transaction=transaction)

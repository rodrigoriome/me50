import os

from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, print_lines
from cs50 import SQL

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

db.execute("""CREATE TABLE IF NOT EXISTS history (
            id INTEGER AUTO_INCREMENT PRIMARY KEY,
            user_id INTEGER NOT NULL,
            symbol TEXT NOT NULL,
            action TEXT NOT NULL,
            value NUMERIC NOT NULL,
            shares INTEGER NOT NULL,
            date DATE NOT NULL DEFAULT CURRENT_TIMESTAMP
        )""")

db.execute("CREATE TABLE IF NOT EXISTS wallets (user_id INTEGER NOT NULL, symbol TEXT NOT NULL, shares INTEGER NOT NULL)")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    return apology("TODO")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("please provide a symbol")

        symbol_info = lookup(symbol)

        if not shares:
            return apology("please provide an amount of shares")

        if float(shares) * symbol_info["price"] > user["cash"]:
            return apology("you don't have enough money")

        newUserCash = user["cash"] - (float(shares) * symbol_info["price"])

        db.execute("INSERT INTO history (user_id, symbol, action, value, shares) VALUES (?, ?, ?, ?, ?)", user["id"], symbol, "BUY", symbol_info["price"], shares)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", newUserCash, user["id"])

        walletForCurrentPurchase = db.execute("SELECT * FROM wallets WHERE user_id = ? AND symbol = ?", user["id"], symbol)
        if walletForCurrentPurchase:
            db.execute("UPDATE wallets SET shares = ? WHERE user_id = ? AND symbol = ?", walletForCurrentPurchase[0]["shares"] + int(shares), user["id"], symbol)
        else:
            db.execute("INSERT INTO wallets (user_id, symbol, shares) VALUES (?, ?, ?)", user["id"], symbol, shares)

        flash(f"Successfully bought {shares} shares of {symbol}")
    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]
    history = db.execute("SELECT * FROM history WHERE user_id = ?", user["id"])

    return render_template("history.html", history = history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("please provide a symbol")
        symbol_lookup = lookup(symbol)
        if symbol_lookup == None:
            return apology("symbol don't exist", 404)
        return render_template("quoted.html", symbol = symbol_lookup)
    return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username:
            return apology("please provide a username", 403)

        if not password:
            return apology("please provide a password", 403)

        if not confirmation:
            return apology("please confirm your password", 403)

        if not password == confirmation:
            return apology("password confirmation doesn't match", 403)

        username_taken = db.execute("SELECT username FROM users WHERE username = ?", username)
        if username_taken:
            return apology("this username is not available", 403)

        pass_hash = generate_password_hash(request.form.get("password"))

        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, pass_hash)

        return redirect("/login")
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("please provide a symbol")

        symbol_info = lookup(symbol)

        if not shares:
            return apology("please provide an amount of shares")

        walletForTransaction = db.execute("SELECT * FROM wallets WHERE user_id = ? AND symbol = ?", user["id"], symbol)[0]

        if int(shares) > (walletForTransaction["shares"]):
            return apology("you don't have enough shares")

        newUserShares = (walletForTransaction["shares"]) - int(shares)
        newUserCash = user["cash"] + (float(shares) * symbol_info["price"])

        db.execute("INSERT INTO history (user_id, symbol, action, value, shares) VALUES (?, ?, ?, ?, ?)", user["id"], symbol, "SELL", symbol_info["price"], shares)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", newUserCash, user["id"])
        db.execute("UPDATE wallets SET shares = ? WHERE user_id = ?", newUserShares, user["id"])

        flash(f"Successfully sold {shares} shares of {symbol}")

    shares = db.execute("SELECT symbol FROM wallets WHERE user_id = ? AND shares > 0", user["id"])
    return render_template("sell.html", shares = shares)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)

import os
import re # used in validate_password function

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Query owned stocks for user
    stocks = db.execute("SELECT symbol, SUM(amount) as total_amount FROM transactions WHERE user_id = :user_id GROUP BY symbol HAVING SUM(amount) > 0", user_id = session["user_id"])
    # Build portfolio of user
    get_complete_portfolio(stocks)
    # Query cash for user
    cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id = session["user_id"])
    total_cash = cash[0]["cash"]
    # Calculate total worth of portfolio
    total_worth = total_cash;
    for asset in stocks:
        total_worth += asset["total_amount"] * asset["price"]
    # Render template
    return render_template("index.html", stocks=stocks, cash=total_cash, total_worth=total_worth)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # User reached /buy through GET method
    if request.method == "GET":
        return render_template("buy.html")
    # User reached /buy through POST method
    else:
        # Check validity symbol field
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("No symbol entered. Please try again.")
        elif not lookup(symbol):
            return apology("Symbol not found in stocks database. Please try again.")
        # Check validity shares field
        shares = request.form.get("shares")
        if not shares:
            return apology("No amount entered. Please try again.")
        elif not str.isdigit(shares) or int(shares) <= 0:
            return apology("Incorrect amount. Please enter a positive integer.")
        # If symbol and shares field are valid
        else:
            stock = lookup(symbol)
            amount = float(shares) * stock["price"]
            user = db.execute("SELECT * FROM users WHERE id=:user_id", user_id=session["user_id"])
            # Check if  user has enough cash
            if amount > user[0]["cash"]:
                return apology("Not enough cash available on your account. Please try again.")
            else:
                # Update transactions table
                db.execute("INSERT INTO transactions (user_id, symbol, price_per_share, amount) VALUES (:user_id, :symbol, :price_per_share, :amount)",
                    user_id=session["user_id"],
                    symbol=symbol,
                    price_per_share=stock["price"],
                    amount=shares)
                # Update users' cash
                db.execute("UPDATE users SET cash = cash - :amount WHERE id=:user_id", amount=amount, user_id=session["user_id"])

            # Show flash message to confirm buy
            flash("Bought shares!")
            # Redirect to portfolio
            return redirect(url_for("index"))


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Query all transactions for user, in descending order of creation
    history = db.execute("SELECT symbol, amount, price_per_share, time_created FROM transactions WHERE user_id = :user_id ORDER BY time_created DESC",
        user_id = session["user_id"])
    return render_template("history.html", history=history)


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
    return redirect(url_for("index"))


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # User reached /quote via GET method
    if request.method == 'GET':
        return render_template("quote.html")
    # User reached /quote via POST method
    else:
        if not request.form.get("symbol"):
            return apology("No symbol entered. Please try again.")
        elif lookup(request.form.get("symbol")) == None:
            return apology("Symbol not found in stocks database. Please try again.")
        else:
            stock = lookup(request.form.get("symbol"))
            return render_template("quoted.html", name=stock["name"], symbol=stock["symbol"], price=usd(stock["price"]))


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # User reached /quote via GET method
    if request.method == "GET":
        return render_template("register.html")
    # User reached /quote via POST method
    else:
        # Check if fields are filled in
        if not request.form.get("username"):
            return apology("You must enter a username. Please try again.")
        elif not request.form.get("password"):
            return apology("You must enter a password. Please try again.")
        elif not request.form.get("confirmation"):
            return apology("You must enter a password confirmation. Please try again.")
        # Check if username already exists
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        if len(rows) > 0:
            return apology("Username already used. Please try again with a different username.")
        # Check if password meets requirements
        if validate_password(request.form.get("password")) == -1:
            return apology("Password must consists of atleast 8 characters and contain atleast 1 number, 1 lowercase, 1 uppercase and 1 special character.")
        # Check if password and confirmation match
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("Your password and password confirmation don't match. Please try again.")
        # Insert user into database if all conditions are passed and save the returned primary key
        new_id = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
            username = request.form.get("username"),
            hash = generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8))
        # Save user_id in session
        session["user_id"] = new_id
        # Display flash message to confirm registration
        flash("Registered!")
        # Redirect to portfolio
        return redirect(url_for("index"))



@app.route("/changepassword", methods=["GET", "POST"])
@login_required
def change():
    """Allow user to change password"""
    # If user reached /changepassword via GET method
    if request.method == "GET":
        return render_template("changepassword.html")
    # If user reached /changepassword via POST method
    else:
        # Query database for user details
        rows = db.execute("SELECT * FROM users WHERE id = :user_id", user_id=session["user_id"])
        # Ensure username exists and password is correct
        if not check_password_hash(rows[0]["hash"], request.form.get("old-password")):
            return apology("Current password is incorrect. Please try again.")
        # Ensure new password meets the requirements
        if validate_password(request.form.get("new-password")) == -1:
            return apology("New password must consist of atleast 8 characters and contain atleast 1 number, 1 lowercase, 1 uppercase and 1 special character.")
        # Ensure confirmation matches the new password
        if request.form.get("new-password") != request.form.get("confirmation"):
            return apology("Your password and password confirmation don't match. Please try again.")
        # Change password if all conditions passed
        db.execute("UPDATE users SET hash = :hash WHERE id=:user_id",
            hash = generate_password_hash(request.form.get("new-password"), method='pbkdf2:sha256', salt_length=8),
            user_id = session["user_id"])
        # Flash confirmation message
        flash("Password changed!")
        # Redirect to portfolio
        return redirect(url_for("index"))


@app.route("/deleteaccount", methods=["GET", "POST"])
@login_required
def delete():
    """Delete a user's account"""
    # If user reached /deleteaccount via GET method
    if request.method == "GET":
        return render_template("deleteaccount.html")
    # If user reached /deleteaccount via POST method
    else:
        # If user pressed 'Yes' button
        if request.form["submit_button"] == "yes":
            # Sell all owned assets
            assets = db.execute("SELECT symbol, SUM(amount) as total_amount FROM transactions WHERE user_id=:user_id", user_id = session["user_id"])
            for row in assets:
                stock = lookup(row["symbol"])
                current_price = stock["price"]
                # Add sold asset to user's cash
                db.execute("UPDATE users SET cash = cash + :amount WHERE id=:user_id",
                amount = float(row["total_amount"]) * current_price,
                user_id=session["user_id"])
            # Delete user from users and transactions table
            db.execute("DELETE FROM users WHERE id=:user_id", user_id=session["user_id"])
            db.execute("DELETE FROM transactions WHERE user_id=:user_id", user_id=session["user_id"])
            # Forget session user_id
            session.clear()
            return redirect(url_for("index"))
        # If user pressed 'No' button
        else:
            return redirect(url_for("index"))


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # User reached /sell via GET method
    if request.method == "GET":
        # Query user's 'sellable' stock and exclude stock which has already been completely sold
        stocks = db.execute("SELECT symbol, amount FROM transactions WHERE user_id=:user_id GROUP BY symbol HAVING SUM(amount) > 0", user_id=session["user_id"])
        return render_template("sell.html", stocks=stocks)
    # User reached /sell via POST method
    else:
        # Check validity symbol field
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("No symbol selected. Please try again.")
        # Check validity shares field
        stocks = db.execute("SELECT SUM(amount) as total_amount FROM transactions WHERE user_id=:user_id AND symbol=:symbol", user_id=session["user_id"], symbol=symbol)
        shares = request.form.get("shares")
        if not shares:
            return apology("No amount entered. Please try again.")
        elif not str.isdigit(shares) or int(shares) <= 0:
            return apology("Incorrect amount. Please enter a positive integer.")
        # Check if user has enough shares of certain stock to sell
        elif int(shares) > int(stocks[0]["total_amount"]):
            return apology("Not that many shares in your portfolio. Please try again.")
        # If all conditions passed
        else:
            # Query current price of the stock
            stock = lookup(request.form.get("symbol"))
            # Log sell in transactions table
            db.execute("INSERT INTO transactions (user_id, symbol, price_per_share, amount) VALUES (:user_id, :symbol, :price_per_share, :amount)",
                user_id = session["user_id"],
                symbol = symbol,
                price_per_share = stock["price"],
                amount = int(shares) * -1)
            # Update users' cash
            db.execute("UPDATE users SET cash = cash + :amount WHERE id=:user_id",
                amount = float(shares) * stock["price"],
                user_id=session["user_id"])
            # Display flash message to confirm selling
            flash("Shares sold!")
            # Redirect to portfolio
            return redirect(url_for("index"))


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Helper function to retrieve entire portfolio
def get_complete_portfolio(stocks):
    for asset in stocks:
        temp = lookup(asset["symbol"])
        asset["name"] = temp["name"]
        asset["price"] = temp["price"]


# Helper function to validate password
def validate_password(password):
    if len(password) < 8:
        return -1
    elif re.search("[0-9]", password) is None:
        return -1
    elif re.search("[a-z]", password) is None:
        return -1
    elif re.search("[A-Z]", password) is None:
        return -1
    elif re.search("[!#$%&'()*+,-./:;<=>?@[\]^_`{|}~]", password) is None:
        return -1
    else:
        return 0


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)

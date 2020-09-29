import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
import datetime
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
    if request.method == 'GET':

        person_id = session["user_id"]
        r = db.execute("SELECT * FROM users WHERE id == (:person_id)", person_id=person_id)
        rows = db.execute("SELECT * FROM transactions WHERE person_id == (:person_id)", person_id=person_id)

        cash = round(r[0]['cash'], 2)
        x = 1
        suma = cash
        values = []
        mydict = {}
        title = ['Symbol', 'Name', 'Shares', 'Price', 'TOTAL']

        for z in rows:
            stock = z['stock']
            if stock not in values:
                values.append(stock)

        for i in values:
            stock = i
            amount = 0
            amt = db.execute("SELECT * FROM transactions WHERE person_id == (:person_id) AND stock == (:stock)", person_id=person_id, stock=stock)
            for a in amt:
                monto = a["amount"]
                amount = amount + monto
            share = lookup(stock)
            price = share["price"]
            name = share["name"]
            tots = amount*price
            suma = suma + tots
            mydict[x] = mydict.get(x, []) + [stock, name, amount, usd(price), usd(tots)]
            x += 1

        mydict[x] = mydict.get(x, []) + ['CASH','', '', '', usd(cash)]
        x += 1
        mydict[x] = mydict.get(x, []) + ['','', '', '', usd(suma)]

        return render_template("index.html", title=title, mydict=mydict)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == 'POST':
        if not request.form.get("symbol"):
            return apology("must provide Stock", 403)

        elif not request.form.get("amount"):
            return apology("must provide amount", 403)

        symbol = request.form.get("symbol")
        amount = int(request.form.get("amount"))

        share = lookup(symbol)

        if share == None:
            return apology("Share not found", 403)

        price = share["price"]
        stock = share["symbol"]



        if amount <= 0:
            return apology("Amount should be a positive number", 403)

        person_id = session["user_id"]
        rows = db.execute("SELECT * FROM users WHERE id == (:person_id)", person_id=person_id)
        cash = rows[0]['cash']
        today = datetime.datetime.now()
        now = today.strftime("%y-%m-%d %H:%M:%S")

        debit = cash - (price*amount)
        if cash >= (price*amount):
            db.execute("INSERT INTO transactions (person_id, stock, price, date, amount) VALUES (:person_id, :stock, :price, :date, :amount) ", person_id=person_id, stock=stock, price=price, date=now, amount=amount)
            db.execute("UPDATE users SET cash = (:cash) WHERE id == (:person_id)", cash = debit, person_id = person_id )
            return redirect("/")
        else:
            return apology("Failed to buy share", 403)

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    if request.method == 'GET':

        person_id = session["user_id"]
        r = db.execute("SELECT * FROM users WHERE id == (:person_id)", person_id=person_id)
        rows = db.execute("SELECT * FROM transactions WHERE person_id == (:person_id)", person_id=person_id)


        x = 1
        mydict = {}
        title = ['Symbol', 'Shares', 'Price', 'Bought', 'Price Now']

        for z in rows:
            stock = z['stock']
            amount = z["amount"]
            price = z["price"]
            date = z["date"]
            check = lookup(stock)
            price_n = check["price"]
            mydict[x] = mydict.get(x, []) + [stock, amount, usd(price), date, usd(price_n)]
            x += 1

        return render_template("history.html", title=title, mydict=mydict)


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


@app.route("/change_password", methods=["GET", "POST"])
def change_password():
    """Change Password"""
    if request.method == "POST":

        password=request.form.get("password")
        confirm_p=request.form.get("confirm_password")
        person_id = session["user_id"]

        if not request.form.get("password"):
            return apology("must provide password", 403)

        elif password != confirm_p:
            return apology("Must confirm password", 403)

        db.execute("UPDATE users SET hash = (:hashpass) WHERE id == (:person_id)", hashpass=generate_password_hash(password), person_id=person_id)

        return redirect("/")

    else:
        return render_template('change_password.html')


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")

    else:
        quote=request.form.get('quote')
        ans = lookup(quote)
        if ans == None:
            return apology("Share not found", 403)
        quoted = []
        txt_p = []
        for x in ans.values():
            quoted.append(x)
        txt = "A share of {} ({}) costs {}."
        txt_f = txt.format(quoted[0], quoted[2], usd(quoted[1]))
        txt_p.append(txt_f)
        return render_template("quoted.html", txt_f = txt_f )


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

        # insert to database
        username=request.form.get("username")
        password=request.form.get("password")
        confirm_p=request.form.get("confirm_password")

        if not request.form.get("username"):
            return apology("must provide username", 403)

        elif not request.form.get("password"):
            return apology("must provide password", 403)

        elif password != confirm_p:
            return apology("Must confirm password", 403)

        check_name = db.execute("SELECT * FROM users")

        names = []
        for x in check_name:
            n = x["username"]
            names.append(n)

        if username not in names:
            db.execute("INSERT INTO users (username, hash) VALUES (:username, :hashpass)",username=username, hashpass=generate_password_hash(password))
        else:
            return apology("Name in database please choose other user name", 403)

        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        return redirect("/")

    else:
        return render_template('register.html')


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("must provide Stock", 403)

        elif not request.form.get("amount"):
            return apology("must provide amount", 403)

        person_id = session["user_id"]
        symbol = request.form.get("symbol")
        amount = int(request.form.get("amount"))

        share = lookup(symbol)
        if share == None:
            return apology("Share not found", 403)

        if amount <= 0:
            return apology("Amount should be a positive number", 403)

        user = db.execute("SELECT * FROM users WHERE id == (:person_id)", person_id=person_id)
        cash = user[0]['cash']

        rows = db.execute("SELECT * FROM transactions WHERE person_id == (:person_id)", person_id=person_id)

        values = []
        temp_dict = {}

        # creating list of stocks in transaction of user
        for z in rows:
            stock = z['stock']
            if stock not in values:
                values.append(stock)

        if symbol.upper() not in values:
            txt = "Stock not found in owned shares ({})."
            return apology(txt.format(symbol), 402)

        # shares in each stock owned
        for i in values:
            stock = i
            n = 0
            amt = db.execute("SELECT * FROM transactions WHERE person_id == (:person_id) AND stock == (:stock)", person_id=person_id, stock=stock)
            for a in amt:
                monto = a["amount"]
                n = n + monto

        if amount > n:
            txt = "Amount to sell not owned (Shares owned : {})."
            return apology(txt.format(amount), 402)


        price = share["price"]
        stock = share["symbol"]
        today = datetime.datetime.now()
        now = today.strftime("%y-%m-%d %H:%M:%S")

        gains = cash + (price*amount)


        db.execute("INSERT INTO transactions (person_id, stock, price, date, amount) VALUES (:person_id, :stock, :price, :date, :amount) ", person_id=person_id, stock=stock, price=price, date=now, amount=-amount)
        db.execute("UPDATE users SET cash = (:cash) WHERE id == (:person_id)", cash = gains, person_id = person_id )
        return redirect("/")

    else:
        return render_template("sell.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)

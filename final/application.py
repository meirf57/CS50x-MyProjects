import os
import requests
from cs50 import SQL
from bs4 import BeautifulSoup
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from extra import apology, login_required, lookup, check_db, escape
from pprint import PrettyPrinter


pp = PrettyPrinter()

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

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

db = SQL("sqlite:///watched.db")


# when user logs in render homepage
@app.route("/")
@login_required
def layout():
    user_id = session["user_id"]
    rows = db.execute("SELECT username FROM users WHERE id = :user_id",user_id=user_id)
    name = rows[0].get('username')
    txt = "Welcome " + name.capitalize() +  "!"
    return render_template("layout.html", txt=txt)


# search movies
@app.route("/search", methods=["GET", "POST"])
@login_required
def search():
    # if method is get
    if request.method == "GET":
        return render_template("search.html")
    # if method is post
    else:
        # get movie data save global as "check"
        global check
        check = lookup(request.form.get("title"))
        if check == None:
            return apology("Title not found", 403)
        # make dict with movie data
        else:
            movie = {
            'name': check["name"],
            'run': check["year"] +" - "+ check["run"][-4:],
            'plot': check["plot"],
            'img': check["img"],
            'genre': check["genre"],
            'rating': check["rating"],
            'director': check['director']}
            # get other movie options from same director
            movies = []
            txt = 'SELECT title FROM movies JOIN ratings ON movies.id = ratings.movie_id WHERE id IN (SELECT movie_id FROM directors WHERE person_id IN (SELECT id FROM people WHERE name == "{}") ) ORDER BY rating DESC LIMIT 5'
            result = check_db("movies.db", txt.format(movie.get('director')))
            # save results in list
            for r in result:
                movies.append(r[0])
            # lookup movies and save data in list as dict
            director_movies = []
            for x in movies:
                movie_dict = {"img": "", "title":""}
                look = lookup(x)
                movie_dict["img"] = look['img']
                movie_dict["title"] = look['name']
                director_movies.append(movie_dict)

            # return and render data
            return render_template("movie_r.html", movie=movie, director_movies=director_movies)


# personalized list of movies
@app.route("/my_views", methods=["GET", "POST"])
@login_required
def my_views():
    # if method is get
    if request.method == "GET":
        user_id = session["user_id"]

        # get movie data of user
        row = db.execute("SELECT * FROM viewed WHERE user_id == (:user_id)", user_id=user_id)
        my_movies = []
        for r in row:
            dict_movies = {"title": "", "image": ""}
            dict_movies["image"] = r['poster']
            dict_movies["title"] = r['title']
            my_movies.append(dict_movies)

        # return data of all movies saved
        return render_template("my_views.html", my_movies=my_movies)
    # method is post
    else:
        # if no input
        if not request.form.get("title"):
            return apology("must provide Title", 403)

        # get user input
        user_id = session["user_id"]
        title = request.form.get("title")

        # get data from user
        rows = db.execute("SELECT * FROM viewed WHERE user_id == (:user_id)", user_id=user_id)

        values = []

        # make list of all movie titles
        for z in rows:
            t = z['title']
            if t not in values:
                values.append(t)

        # if input not in saved movie titles
        if title.title() not in values:
            txt = "Title not found in my views, input: ({})."
            return apology(txt.format(title), 402)
        # remove movie as requested
        else:
            db.execute("DELETE FROM viewed WHERE (title = :title) AND (user_id = :user_id) ", title=title.title(),user_id=user_id)
        return redirect("/")


# add movie to list
@app.route("/add", methods=["GET", "POST"])
def add():
    # if method is post get data from global "check"
    if request.method == "POST":
        user_id = session["user_id"]
        title = check["name"]
        poster = check["img"]
        genre = check["genre"]
        rows = db.execute("SELECT * FROM viewed WHERE user_id == (:user_id)", user_id=user_id)
        values = []
        # make temp list of current values in db
        for z in rows:
            seen = z['title']
            if seen not in values:
                values.append(seen)
        # if movie already exists in db return apology
        if title in values:
            return apology("Already in your watched list", 456)

        # save new movie
        db.execute("INSERT INTO viewed (title, poster, genre, user_id) VALUES (:title, :poster, :genre, :user_id) ", title=title,poster=poster,genre=genre,user_id=user_id)
        return render_template("my_views.html")


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


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
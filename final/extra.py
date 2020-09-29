import requests
import urllib.parse
import sqlite3

from flask import redirect, render_template, request, session
from functools import wraps
from pprint import PrettyPrinter


pp = PrettyPrinter()

def apology(message, code=400):
    """Render message as an apology to user."""
    return render_template("apology.html", top=code, bottom=message.upper()), code


def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/1.0/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def lookup(title):
    try:
        apiKey = "d3e99fda"
        params = {'t':"",'plot': "full"}
        params["t"] = title
        data_URL = 'http://www.omdbapi.com/?apikey='+apiKey
        response = requests.get(data_URL,params=params)
        pp.pprint(response)
    except requests.RequestException:
        return None
    # Parse response
    try:
        search = response.json()
        return {
            "name" : search["Title"],
            "year" : search["Released"],
            "plot" : search["Plot"],
            "img" : search["Poster"],
            "genre" : search["Genre"],
            "run" : search["Year"],
            "rating" : search["imdbRating"],
            "director" : search["Director"]
        }
    except (KeyError, TypeError, ValueError):
        return None


def check_db(db_file, query):
    connection = sqlite3.connect(db_file)
    cursor = connection.cursor()
    print(query)
    cursor.execute(query)
    results = cursor.fetchall()
    cursor.close()
    connection.close()
    return results


def escape(s):
    for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''"), (":", ".")]:
        s = s.replace(old, new)
    return s
import sqlite3

connection = sqlite3.connect('movies.db')
cursor = connection.cursor()
cursor.execute('SELECT title, year FROM movies WHERE title like "Harry Potter%" ORDER BY year')
results = cursor.fetchall()
for x in results:
    print(x)
cursor.close()
connection.close()

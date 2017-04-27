import sqlite3 as db

def create_admin_table(db_conn):
	cursor = db_conn.cursor()
	cursor.execute(
		"""CREATE TABLE IF NOT EXISTS admin (
			id INTEGER PRIMARY KEY, 
			username TEXT,
			password TEXT)""")
	db_conn.commit()
	db_conn.close()


def create_news_table(db_conn):
	cursor = db_conn.cursor()
	cursor.execute(
		"""CREATE TABLE IF NOT EXISTS news (
			news_id INTEGER PRIMARY KEY,
	 		header TEXT, 
	 		body TEXT,
	 		author TEXT)""")
	db_conn.commit()
	db_conn.close()


def register_admin(db_conn, username, password):
	cursor = db_conn.cursor()
	cursor.execute('INSERT INTO admin VALUES (NULL, ?, ?)', (username, password))
	db_conn.commit()
	db_conn.close()


def add_news(db_conn, header, body, author):
	cursor = db_conn.cursor()
	cursor.execute('INSERT INTO news VALUES (NULL, ?, ?, ?)', (header, body, author))
	db_conn.commit()
	db_conn.close()


def get_news(db_conn, id):
	cursor = db_conn.cursor()
	cursor.execute('SELECT * FROM news WHERE news_id = ' + id)
	news = cursor.fetchone()
	return news


def main():
	id = input("Enter news id: ")
	db_conn = db.connect('news.sqlite')
	print(get_news(db_conn, id))
	db_conn.close()

if __name__ == '__main__':
	main()

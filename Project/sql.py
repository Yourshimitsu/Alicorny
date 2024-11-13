import time

from database import Database
from parsing import parse

start_time = time.time()
db = Database(user='postgres', password='12346', host='localhost', port=5438, database='postgres')
items = parse()
ids = []
for item in items:
    id = db.add_item_to_db(item)
    ids.append(id)
    db.record_price(item, id)
db.close_connection()
end_time = time.time()
execution_time = end_time - start_time
print(f"Время выполнения: {execution_time:.4f} секунд")

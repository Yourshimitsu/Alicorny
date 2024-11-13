import pg8000

class Database:
    def __init__(self, user, password, host, port, database):
        self.connection = pg8000.connect(user=user, password=password, host=host, port=port, database=database)
        self.cursor = self.connection.cursor()

    def add_item_to_db(self, item: 'CSMarketItem'):
        try:
            self.cursor.execute("SELECT id FROM csmarket.names WHERE full_name = %s", (item.full_name,))
            existing_item = self.cursor.fetchone()
            if existing_item:
                item_id = existing_item[0]
            else:
                self.cursor.execute(
                    "INSERT INTO csmarket.names (full_name, gun, skin, quality, stat_track) VALUES (%s, %s, %s, %s, %s) RETURNING id",
                    (item.full_name, item.gun, item.skin, item.quality, bool(item.stat_track))
                )
                item_id = self.cursor.fetchone()[0]
                self.connection.commit()
            return item_id
        except Exception as e:
            print(f"An error occurred: {e}")
            return None
        
    def get_all_ids_by_name(self):
        try:
            self.cursor.execute("SELECT full_name, id FROM csmarket.names")
            all_items = self.cursor.fetchall()
            name_to_id_map = {row[0]: row[1] for row in all_items}
            return name_to_id_map
        except Exception as e:
            print(f"An error occurred: {e}")
            return None
        
    def get_latest_price(self, item_id: int):
        if item_id == None:
            return item_id
        try:
            self.cursor.execute(
                "SELECT price FROM csmarket.prices WHERE id = %s ORDER BY time DESC LIMIT 1",
                (item_id,)
            )
            latest_price = self.cursor.fetchone()
            if latest_price:
                return float(latest_price[0])
            else:
                return None
        except Exception as e:
            print(f"Произошла ошибка: {e}")
            return None
        
    def record_price(self, item: 'CSMarketItem', item_id: int):
        try:
            self.cursor.execute(
                "INSERT INTO csmarket.prices (id, price, volume) VALUES (%s, %s, %s)", 
                (item_id, item.price, item.volume)
            )
            self.connection.commit()
        except Exception as e:
            print(f"An error occurred while recording price: {e}")
            self.connection.rollback()


    def close_connection(self):
        self.cursor.close()
        self.connection.close()
        
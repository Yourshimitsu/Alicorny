import pg8000
from datetime import datetime, timedelta

import matplotlib.pyplot as plt

def get_item_full_name(connection, item_id):
    try:
        cursor = connection.cursor()
        cursor.execute("SELECT full_name FROM csmarket.names WHERE id = %s", (item_id,))
        result = cursor.fetchone()
        cursor.close()
        if result:
            return result[0]
        else:
            print("Предмет с таким id не найден.")
            return None
    except Exception as e:
        print(f"Произошла ошибка: {e}")
        connection.rollback()
        return None
    finally:
        connection.close()

def plot_price_history(item_id):
    connection = pg8000.connect(user='postgres', password='12346', host='localhost', port=5438, database='postgres')
    days = int(input("Введите количество последних дней для анализа: "))
    end_date = datetime.now()
    start_date = end_date - timedelta(days=days)
    try:
        cursor = connection.cursor()
        cursor.execute(
            "SELECT time, price, volume FROM csmarket.prices "
            "WHERE id = %s AND time >= %s "
            "ORDER BY time ASC",
            (item_id, start_date)
        )
        records = cursor.fetchall()
        cursor.close()
        if not records:
            print("Нет данных для указанного периода.")
            return

        dates = [record[0] for record in records] 
        prices = [record[1] for record in records]  
        volumes = [record[2] for record in records]  
        plt.figure(figsize=(10, 5))
        plt.plot(dates, prices, marker='o', color='b', linestyle='-')
        plt.title(f"История цены для {get_item_full_name(connection, item_id)} за последние {days} дней")
        plt.xlabel("Дата")
        plt.ylabel("Стоимость (руб)")
        plt.grid()
        plt.xticks(rotation=45)

        for date, price, volume in zip(dates, prices, volumes):
            plt.text(date, price, str(volume), fontsize=9, ha='right', color='purple')
        
        plt.tight_layout()
        plt.show()
    except Exception as e:
        print(f"Произошла ошибка: {e}")
        connection.rollback()
    finally:
        connection.close()

while True:
    print("Введите 1 если знаете точный id предмета")
    print("Введите 2 если хотите найти нужный id")
    print("Введите что угодно ещё, чтобы выйти")
    choice = input()
    match choice:
        case '1':
            id = int(input('Введите id: '))
            plot_price_history(id)
        case '2':
            pass
        case _:
            break
        
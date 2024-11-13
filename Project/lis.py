import re
import requests
import time

from database import Database


start_time = time.time()
db = Database(user='postgres', password='12346', host='localhost', port=5438, database='postgres')
name_to_id_map = db.get_all_ids_by_name()

class LisItem:
    def __init__(self, json_str):
        self.full_name = json_str['name']
        self.gun, self.skin, self.quality = self.parse_full_name(self.full_name)
        self.stat_track = 'StatTrak\u2122' in self.full_name
        if self.stat_track:
            self.gun = self.gun[10::]
        self.buy_price = json_str["price"]
        try: 
            self.sell_price = db.get_latest_price(name_to_id_map[self.full_name])
        except Exception as e:
            print(f"Ошибка при получении цены: {e}")
            self.sell_price = None

    def parse_full_name(self, name):
        if not "|" in name:
            return name, "", "Not Painted"
        pattern = r'\((.*?)\)'
        quality = re.findall(pattern, name)
        known_qualities = ["Factory New", "Minimal Wear", "Field-Tested", "Well-Worn", "Battle-Scarred"]
        if len(quality) > 0 and quality[0] in known_qualities:
            quality = quality[0]
            return name[0:name.find("|")-1], name[name.find("|")+2:name.find(quality)-2], quality
        elif len(quality) > 0:
            return name[0:name.find("|")-1], name[name.find("|")+2::], "Not Painted"
        return name[0:-1], "", "Not Painted"
    
    def __str__(self):
        return f"Пушка: {self.gun}, Скин: {self.skin}, Качество: {self.quality}, Цена: {self.buy_price}, {self.sell_price}, (Полное название: {self.full_name})"
    
def parse():
    url = "https://lis-skins.com/market_export_json/api_csgo_full.json"
    response = requests.get(url, headers= {
        "Authorization": "Bearer SECRET"
    })
    if response.status_code == 200:
        data = response.json()
        items = []
        for i in range(len(data['items'])):
            if i % 1000 == 0:
                print(100*i/len(data['items']))
            items.append(LisItem(data['items'][i]))
        #items = [Lis_item(item) for item in data['items']]
        return items
    else:
        print(f"Ошибка при скачивании данных: {response.status_code}")
        return ""
      
items = parse()
print(len(items))
for i in range(15):
    print(items[i])
db.close_connection()
end_time = time.time()
execution_time = end_time - start_time
print(f"Время выполнения: {execution_time:.4f} секунд")


known_qualities = ["Factory New", "Minimal Wear", "Field-Tested", "Well-Worn", "Battle-Scarred", "Not Painted"]
filters = {
    'gun': None,
    'skin': None,
    'qualities': [],
    'min_buy_price': None,
    'max_buy_price': None,
    'min_sell_price': None,
    'max_sell_price': None,
    'stat_track': None
}

while True:
    print("Выберите фильтры для поиска предметов:")
    print("1 - Фильтр по оружию (gun)")
    print("2 - Фильтр по скину (skin)")
    print("3 - Фильтр по качеству (quality)")
    print("4 - Фильтр по цене покупки (buy_price)")
    print("5 - Фильтр по цене продажи (sell_price)")
    print("6 - Фильтр по StatTrak™ (stat_track)")
    print("7 - Показать все подходящие предметы")
    print("8 - Очистить все фильтры")
    print("0 - Выход")
    command = int(input("Введите цифру для действия: "))
    if command == 1:
        gun_filter = input("Введите название оружия для фильтрации (или нажмите Enter для пропуска): ")
        filters['gun'] = gun_filter.lower() if gun_filter else None
    elif command == 2:
        skin_filter = input("Введите название скина для фильтрации (или нажмите Enter для пропуска): ")
        filters['skin'] = skin_filter.lower() if skin_filter else None
    elif command == 3:
        print("Существующие качества:")
        for idx, quality in enumerate(known_qualities, 1):
            print(f"{idx} - {quality}")
        selected_qualities = input("Введите номера качеств через запятую (например, 1, 3, 5) для фильтрации: ")
        selected_qualities = selected_qualities.split(',')
        filters['qualities'] = [known_qualities[int(i)-1] for i in selected_qualities if i.isdigit() and 1 <= int(i) <= len(known_qualities)]
    elif command == 4:
        try:
            min_buy_price = input("Введите минимальную цену покупки для фильтрации (или нажмите Enter для пропуска): ")
            max_buy_price = input("Введите максимальную цену покупки для фильтрации (или нажмите Enter для пропуска): ")
            filters['min_buy_price'] = float(min_buy_price) if min_buy_price else None
            filters['max_buy_price'] = float(max_buy_price) if max_buy_price else None
        except ValueError:
            print("Ошибка: введена некорректная цена.")
    elif command == 5:
        try:
            min_sell_price = input("Введите минимальную цену продажи для фильтрации (или нажмите Enter для пропуска): ")
            max_sell_price = input("Введите максимальную цену продажи для фильтрации (или нажмите Enter для пропуска): ")
            filters['min_sell_price'] = float(min_sell_price) if min_sell_price else None
            filters['max_sell_price'] = float(max_sell_price) if max_sell_price else None
        except ValueError:
            print("Ошибка: введена некорректная цена.")
    elif command == 6:
        print("Фильтр по StatTrak™:")
        print("1 - Только с StatTrak™")
        print("2 - Только без StatTrak™")
        print("3 - Не фильтровать")
        stat_track_choice = input("Введите номер для выбора фильтра: ")
        if stat_track_choice == '1':
            filters['stat_track'] = True
        elif stat_track_choice == '2':
            filters['stat_track'] = False
        else:
            filters['stat_track'] = None
    elif command == 7:
        filtered_items = []
        for item in items:
            if (filters['gun'] and filters['gun'] not in item.gun.lower()):
                continue
            if (filters['skin'] and filters['skin'] not in item.skin.lower()):
                continue
            if (filters['qualities'] and item.quality not in filters['qualities']):
                continue
            if (filters['min_buy_price'] and item.buy_price < filters['min_buy_price']):
                continue
            if (filters['max_buy_price'] and item.buy_price > filters['max_buy_price']):
                continue
            if (filters['min_sell_price'] and item.sell_price < filters['min_sell_price']):
                continue
            if (filters['max_sell_price'] and item.sell_price > filters['max_sell_price']):
                continue
            if (filters['stat_track'] is not None and item.stat_track != filters['stat_track']):
                continue
            filtered_items.append(item)
        filtered_items.sort(key=lambda x: (x.sell_price / x.buy_price if x.sell_price and x.buy_price else 0), reverse=True)
        filtered_items = filtered_items[::-1]
        print(f"Найдено {len(filtered_items)} предметов, соответствующих выбранным фильтрам:")
        for item in filtered_items:
            print(item)
    elif command == 8:
        print("Все фильтры очищены.")
        filters = {
            'gun': None,
            'skin': None,
            'qualities': [],
            'min_buy_price': None,
            'max_buy_price': None,
            'min_sell_price': None,
            'max_sell_price': None,
            'stat_track': None
        }
    elif command == 0:
        print("Выход...")
        break
    else:
        print("Неверная команда, попробуйте снова.")
import requests

from item import CSMarketItem

def parse():
    url = "https://market.csgo.com/api/v2/prices/USD.json"
    response = requests.get(url)
    if response.status_code == 200:
        data = response.json()
        items = [CSMarketItem(item) for item in data['items']]
        return items
    else:
        return f"Ошибка при скачивании данных: {response.status_code}"
    
import re

class CSMarketItem:
    def __init__(self, json_str: dict):
        self.full_name = json_str['market_hash_name']
        self.gun, self.skin, self.quality = self.parse_full_name(self.full_name)
        self.stat_track = 'StatTrak™' in self.gun
        if self.stat_track:
            self.gun = self.gun[10::]
        self.volume = json_str["volume"]
        self.price = json_str["price"]

    def parse_full_name(self, name: str):
        if "|" not in name:
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
        return f"Пушка: {self.gun}, Скин: {self.skin}, Качество: {self.quality}, Объем: {self.volume}, Цена: {self.price}, (Полное название: {self.full_name})"

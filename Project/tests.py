import unittest

from main import CSMarketItem

class TestItem(unittest.TestCase):
    def test_item_with_full_info(self):
        json_item = {
            "market_hash_name": "AK-47 | Aquamarine Revenge (Field-Tested)",
            "volume": 86,
            "price": 3578.27
        }
        item = CSMarketItem(json_item)
        
        self.assertEqual(item.gun, "AK-47", f"Ожидалось 'AK-47', но получено {item.gun}")
        self.assertEqual(item.skin, "Aquamarine Revenge", f"Ожидалось 'Aquamarine Revenge', но получено {item.skin}")
        self.assertEqual(item.quality, "Field-Tested", f"Ожидалось Field-Tested, но получено {item.quality}")
        self.assertEqual(item.volume, 86, f"Ожидалось 86, но получено {item.volume}")
        self.assertEqual(item.price, 3578.27, f"Ожидалось 3578.27, но получено {item.price}")

    def test_item_with_karambit_and_skin(self):
        json_item = {
            "market_hash_name": "★ Karambit | Autotronic (Minimal Wear)", 
            "volume": 20, 
            "price": 11883.86
        }
        item = CSMarketItem(json_item)
        
        self.assertEqual(item.gun, "★ Karambit", f"Ожидалось '★ Karambit', но получено {item.gun}")
        self.assertEqual(item.skin, "Autotronic", f"Ожидалось 'Autotronic', но получено {item.skin}")
        self.assertEqual(item.quality, "Minimal Wear", f"Ожидалось Minimal Wear, но получено {item.quality}")
        self.assertEqual(item.volume, 20, f"Ожидалось 20, но получено {item.volume}")
        self.assertEqual(item.price, 11883.86, f"Ожидалось 11883.86, но получено {item.price}")
        self.assertEqual(item.stat_track, False, f"Ожидалось False, но получено {item.stat_track}")

    def test_item_with_karambit_no_skin(self):
        json_item = {
            "market_hash_name": "★ Karambit", 
            "volume": 88, 
            "price": 1765.28
        }
        item = CSMarketItem(json_item)
        
        self.assertEqual(item.gun, "★ Karambit", f"Ожидалось '★ Karambit', но получено {item.gun}")
        self.assertEqual(item.skin, "", f"Ожидалось '', но получено {item.skin}")
        self.assertEqual(item.quality, "Not Painted", f"Ожидалось Not Painted, но получено {item.quality}")
        self.assertEqual(item.volume, 88, f"Ожидалось 88, но получено {item.volume}")
        self.assertEqual(item.price, 1765.28, f"Ожидалось 1765.28, но получено {item.price}")
        self.assertEqual(item.stat_track, False, f"Ожидалось False, но получено {item.stat_track}")

    def test_item_graffiti(self):
        json_item = {
            "market_hash_name": "Sealed Graffiti | Broken Heart (Violent Violet)",
            "volume": 15,
            "price": 5.28
        }
        item = CSMarketItem(json_item)
        
        self.assertEqual(item.gun, "Sealed Graffiti", f"Ожидалось 'Sealed Graffiti', но получено {item.gun}")
        self.assertEqual(item.skin, "Broken Heart (Violent Violet)", f"Ожидалось 'Broken Heart (Violent Violet)', но получено {item.skin}")
        self.assertEqual(item.quality, "Not Painted", f"Ожидалось Not Painted, но получено {item.quality}")
        self.assertEqual(item.volume, 15, f"Ожидалось 15, но получено {item.volume}")
        self.assertEqual(item.price, 5.28, f"Ожидалось 5.28, но получено {item.price}")
        self.assertEqual(item.stat_track, False, f"Ожидалось False, но получено {item.stat_track}")

    def test_item_stattrack(self):
        json_item = {
            "market_hash_name": "StatTrak™ P90 | Vent Rush (Battle-Scarred)",
            "volume": 35,
            "price": 155.28
        }
        item = CSMarketItem(json_item)
        
        self.assertEqual(item.gun, "P90", f"Ожидалось 'P90', но получено {item.gun}")
        self.assertEqual(item.skin, "Vent Rush", f"Ожидалось 'Vent Rush', но получено {item.skin}")
        self.assertEqual(item.quality, "Battle-Scarred", f"Ожидалось Battle-Scarred, но получено {item.quality}")
        self.assertEqual(item.volume, 35, f"Ожидалось 35, но получено {item.volume}")
        self.assertEqual(item.price, 155.28, f"Ожидалось 155.28, но получено {item.price}")
        self.assertEqual(item.stat_track, True, f"Ожидалось True, но получено {item.stat_track}")

if __name__ == "__main__":
    unittest.main()

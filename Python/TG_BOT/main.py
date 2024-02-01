import json

import requests
import telebot


def fixed(n):
    return int(100 * n) / 100


def print_rate(req):
    name = req["Name"]
    cur = req["Value"]
    prev = req["Previous"]
    trend = '▲'
    if cur < prev:
        trend = '▼'
    return f"{name} {fixed(cur)} {trend} {fixed(abs(cur - prev))}\n"


def check(card1, card2):
    value1 = 1000 * card1["Value"] / card1["Previous"]
    value2 = 1000 * card2["Value"] / card2["Previous"]
    return value1 <= value2


token = ""
request = requests.get("https://www.cbr-xml-daily.ru/daily_json.js").json()["Valute"]
request["HKD"]["Value"] /= 10  # Там у них ошибка - значение в 10 раз больше реального

filename = 'users.json'
with open(filename) as f:
    users = json.load(f)

def telegram_bot(token):
    bot = telebot.TeleBot(token)

    @bot.message_handler(commands=["start"])
    def rate(message):
        try:
            bot.send_message(
                message.chat.id,
                "Привет! Управление кнопками, напиши любое сообщение (не команду), чтобы данные сохранились."
            )
        except Exception as ex:
            print(ex)
            bot.send_message(
                message.chat.id,
                "Ooops... Ошибочка"
            )

    @bot.message_handler(commands=["rate"])
    def rate(message):
        try:
            if users.get(str(message.chat.id)) is None:
                users[str(message.chat.id)] = list()
            text = ""
            for tag in request:
                if tag in users[str(message.chat.id)]:
                    req = request[tag]
                    text += print_rate(req)
            bot.send_message(
                message.chat.id,
                text
            )
        except Exception as ex:
            print(ex)
            bot.send_message(
                message.chat.id,
                "Ooops... Ошибочка"
            )

    @bot.message_handler(commands=["addfav"])
    def addfav(message):
        try:
            if users.get(str(message.chat.id)) is None:
                users[str(message.chat.id)] = list()
            tag = message.text.split()[1]
            users[str(message.chat.id)].append(tag)
            bot.send_message(
                message.chat.id,
                "Успешно"
            )
        except Exception as ex:
            print(ex)
            bot.send_message(
                message.chat.id,
                "Ooops... Ошибочка"
            )

    @bot.message_handler(commands=["delfav"])
    def delfav(message):
        try:
            if users.get(str(message.chat.id)) is None:
                users[str(message.chat.id)] = list()
            tag = message.text.split()[1]
            users[str(message.chat.id)].remove(tag)
            bot.send_message(
                message.chat.id,
                "Успешно"
            )
        except Exception as ex:
            print(ex)
            bot.send_message(
                message.chat.id,
                "Ooops... Ошибочка"
            )

    @bot.message_handler(commands=["findtag"])
    def fintag(message):
        try:
            if users.get(str(message.chat.id)) is None:
                users[str(message.chat.id)] = list()
            try:
                substr = message.text.split()[1].lower()
            except:
                substr = ""
            text = ""
            for tag in request:
                name = request[tag]["Name"].lower()
                if substr in name:
                    text += f"{tag} - {name}\n"
            bot.send_message(
                message.chat.id,
                text
            )
        except Exception as ex:
            print(ex)
            bot.send_message(
                message.chat.id,
                "Ooops... Ошибочка"
            )

    @bot.message_handler(commands=["tagrate"])
    def tagrate(message):
        try:
            tag = message.text.split()[1]
            req = requests.get("https://www.cbr-xml-daily.ru/daily_json.js").json()["Valute"][tag]
            bot.send_message(
                message.chat.id,
                print_rate(req)
            )
        except Exception as ex:
            print(ex)
            bot.send_message(
                message.chat.id,
                "Ooops... Ошибочка"
            )

    @bot.message_handler(commands=["up24h"])
    def up24h(message):
        try:
            uptag = "USD"
            for tag in request:
                if check(request[uptag], request[tag]):
                    uptag = tag
            bot.send_message(
                message.chat.id,
                print_rate(request[uptag])
            )
            if request[uptag]["Value"] < request[uptag]["Previous"]:
                bot.send_message(
                    message.chat.id,
                    "Как рубль вырос за сегодня, загляденье!"
                )
        except Exception as ex:
            print(ex)
            bot.send_message(
                message.chat.id,
                "Ooops... Ошибочка"
            )

    @bot.message_handler(commands=["down24h"])
    def down24h(message):
        try:
            downtag = "USD"
            for tag in request:
                if not check(request[downtag], request[tag]):
                    downtag = tag
            bot.send_message(
                message.chat.id,
                print_rate(request[downtag])
            )
        except Exception as ex:
            print(ex)
            bot.send_message(
                message.chat.id,
                "Ooops... Ошибочка"
            )

    @bot.message_handler(content_types=["text"])
    def text(message):
        try:
            filename = 'users.json'
            with open(filename, 'w') as f:
                json.dump(users, f)
        except Exception as ex:
            print(ex)

    bot.polling()


if __name__ == '__main__':
    telegram_bot(token)

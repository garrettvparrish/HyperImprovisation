from nanpy import (ArduinoApi, SerialManager)
from nanpy import Arduino as A
from flask import Flask, render_template, request

red = 8
green = 11
A.pinMode(red, A.OUTPUT)
A.pinMode(green, A.OUTPUT)

connection = SerialManager()
a = ArduinoApi(connection=connection)
a.pinMode(13, a.OUTPUT)
a.digitalWrite(13, a.HIGH)


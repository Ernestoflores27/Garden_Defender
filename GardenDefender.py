import RPi.GPIO as GPIO
import time

shootPin = 18

GPIO.setmode(GPIO.BCM)
GPIO.setup(shootPin, GPIO.OUT)
GPIO.output(shootPin, GPIO.LOW)
time.sleep(1)
GPIO.output(shootPin, GPIO.HIGH)
GPIO.cleanup()

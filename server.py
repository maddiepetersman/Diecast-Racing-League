from flask import Flask, jsonify, render_template
app = Flask(__name__)

import serial
import time

def release():
    try:
        ser = serial.Serial('COM4', 9600, timeout=1) 
        print(f"Connected to {ser.name}")
        ser.write(b'RELEASE\n')
        print("Car released!") 
    except serial.SerialException as e:
        print(f"Error opening serial port: {e}")
        exit()
    time.sleep(2) 
    
    return "Gate opened"

@app.route('/ajax_trigger', methods=['POST'])
def ajax_trigger():
    result = release()
    return jsonify({"status": "success", "message": result})

@app.route('/')
def index():
  return render_template('index.html')

@app.route('/my-link/')
def my_link():
  print ('I got clicked!')

  return 'Click.'

if __name__ == '__main__':
  app.run(debug=True)
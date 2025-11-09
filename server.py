from flask import Flask, render_template, jsonify
from flask_socketio import SocketIO, emit
import serial
import threading
import time

app = Flask(__name__)
socketio = SocketIO(app)

# -----------------------------
# Serial setup with retry loop
# -----------------------------
def init_serial(port="COM4", baud=9600, timeout=1, retries=5):
    for i in range(retries):
        try:
            ser = serial.Serial(port, baud, timeout=timeout)
            print(f"Connected to {ser.port}")
            return ser
        except serial.SerialException as e:
            print(f"Attempt {i+1} failed: {e}")
            time.sleep(1)
    print("Failed to open COM port. Exiting.")
    exit()

ser = init_serial()

# -----------------------------
# Background thread for reading Arduino
# -----------------------------
def serial_listener():
    while True:
        try:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                if line:
                    print("Arduino:", line)
                    # Emit to browser via SocketIO
                    socketio.emit('serial_event', {'data': line})
        except serial.SerialException as e:
            print(f"Serial error: {e}")
            time.sleep(1)

threading.Thread(target=serial_listener, daemon=True).start()

# -----------------------------
# Release function
# -----------------------------
def release():
    try:
        ser.write(b'RELEASE\n')
        print("Car released!")
        return "Gate opened"
    except serial.SerialException as e:
        print(f"Error writing to serial port: {e}")
        return "Error releasing car"

# -----------------------------
# Flask routes
# -----------------------------
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/ajax_trigger', methods=['POST'])
def ajax_trigger():
    result = release()
    return jsonify({"status": "success", "message": result})

# -----------------------------
# Run app
# -----------------------------
if __name__ == '__main__':
    socketio.run(app, debug=True, use_reloader=False)

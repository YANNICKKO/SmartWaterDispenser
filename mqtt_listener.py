import paho.mqtt.client as mqtt

# MQTT settings
broker = "broker.mqtt-dashboard.com"
port = 1883
topic = "water-dispenser"

# Callback function when a message is received
def on_message(client, userdata, message):
    print(f"Received message on topic {message.topic}: {message.payload.decode()}")

# Callback function when the client connects to the broker
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker")
        client.subscribe(topic)
    else:
        print("Failed to connect, return code %d\n", rc)

# Initialize the MQTT client
client = mqtt.Client()

# Assign event callbacks
client.on_message = on_message
client.on_connect = on_connect

# Connect to the MQTT broker
client.connect(broker, port, 60)

# Start the MQTT client
client.loop_start()

try:
    while True:
        pass  # Keep the script running
except KeyboardInterrupt:
    print("Exiting...")
    client.loop_stop()
    client.disconnect()

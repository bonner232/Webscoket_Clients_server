import asyncio
import websockets

SERVER_URI = "ws://192.168.1.68:6789"

async def client():
    async with websockets.connect(SERVER_URI) as websocket:
        print("Connected to server.")

        # Listen to messages from the server
        async def listen():
            try:
                async for message in websocket:
                    print("[Server]:", message)
            except websockets.exceptions.ConnectionClosed:
                print("Disconnected from server.")

        # Start listening for server messages in the background
        asyncio.create_task(listen())

        # Send a "Hello" message to the server
        await websocket.send("Hello")

        # Wait a moment before closing (to receive the echo)
        await asyncio.sleep(1)  # Adjust this time as needed
        print("Exiting client...")

asyncio.run(client())
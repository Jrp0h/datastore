import socket
import time
import random


class Client:
    def __init__(self):
        self.HEADER = 32
        self.PORT = 1278
        self.FORMAT = 'ascii'
        self.SERVER = "127.0.0.1"
        self.ADDR = (self.SERVER, self.PORT)

    def connect(self):
        self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client.connect(self.ADDR)

    def send(self, message):
        message = message.encode(self.FORMAT)
        message_length = len(message)
        send_length = str(message_length).encode(self.FORMAT)
        send_length += b' ' * (self.HEADER - len(send_length))
        self.client.send(send_length)
        self.client.send(message)


def main():

    queries = [
        "TO user_auth CREATE user_id=\"5\", auth_token=\"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9\";",
        "DESTROY user_auth;",
        "DEFINE TABLE user_auth:300 WITH user_id, token MOD :POKE;",
        "SELECT 7;",
        "DESTROY user_auth;",
        "FROM user_auth DELETE WHERE toke=\"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9\";",
        "FROM dead_drop MOD :WITH_TTL, :ONE;",
        "DEFINE TABLE posts WITH post_id, description, title;",
        "DEFINE TABLE dead_drop:450 WITH message MOD :ONE_TOUCH;"
    ]

    c = Client()
    c.connect()

    while(True):
        c.send(queries[random.randint(0, len(queries) - 1)])
        time.sleep(4)
        #  pass


if __name__ == "__main__":
    main()
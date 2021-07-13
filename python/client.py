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

    def recv(self, size):
        return self.client.recv(size)


def get_random_query():
    queries = [
        # "SELECT",
        # "TO user_auth CREATE user_id=\"5\", auth_token=\"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9\";",
        # "DESTROY user_auth;",
        # "DEFINE TABLE user_auth:300 WITH user_id, token MOD :POKE;",
        # "WHICH;"
        # "DESTROY user_auth;",
        # "FROM user_auth WHERE token=\"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9\";",
        "FROM user_auth WHERE token=\"SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c\";",
        # "FROM dead_drop MOD :WITH_TTL, :ONE;",
        # "DEFINE TABLE posts WITH post_id, description, title;",
        # "DEFINE TABLE dead_drop:450 WITH message MOD :ONE_TOUCH;"
    ]

    return f"FROM user_auth WHERE user_id =\"{random.randint(0, 10)}\";"
    which = random.randint(0, len(queries) - 1)

    # if which == 0:
    #     return f"SELECT {random.randint(0, 24)};"

    return queries[which]


def main():

    c = Client()
    c.connect()

    c.send("DEFINE TABLE user_auth:300 WITH user_id, token;")

    for _ in range(50):
        c.send(f"TO user_auth CREATE user_id=\"{random.randint(0, 10)}\", token=\"{ random.randint(-20000000,000000000) }\";")
        pass

    # c.send("TO user_auth CREATE user_id=\"5\", token=\"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9\";")
    # c.send("TO user_auth CREATE user_id=\"4\", token=\"eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9\";")
    # c.send("TO user_auth CREATE user_id=\"7\", token=\"eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiYWRtaW4iOnRydWUsImlhdCI6MTUxNjIzOTAyMn0\";")

    loop = 0

    while(True):
        c.send(get_random_query())

        code = c.recv(2).decode('ascii')
        print(code)
        bufsize = int(c.recv(32).decode('ascii'))
        print(bufsize)

        msg = c.recv(bufsize).decode('ascii')

        print(f"{code=}")
        print(f"{bufsize=}")
        print(f"{msg}")

        if loop < 50:
            loop += 1
        else:
            time.sleep(4)
        #  pass


if __name__ == "__main__":
    main()

import time
import socket
import struct
import hashlib
import json


def make_message(cmd, payload):
    magic = "F9BEB4D9".decode("hex") # Main network
    command = cmd + (12 - len(cmd)) * "\00"
    length = struct.pack("I", len(payload))
    check = hashlib.sha256(hashlib.sha256(payload).digest()).digest()[:4]
    return magic + command + length + check + payload


def make_version_message():
    version = struct.pack("i", 70015)
    services = struct.pack("Q", 0)
    timestamp = struct.pack("q", time.time())
    addr_recv = struct.pack("Q", 0)
    addr_recv += struct.pack(">16s", "127.0.0.1")
    addr_recv += struct.pack(">H", 8333)
    addr_from = struct.pack("Q", 0)
    addr_from += struct.pack(">16s", "127.0.0.1")
    addr_from += struct.pack(">H", 8333)
    nonce = struct.pack("Q", 0) # not important, was random.getrandbits(64)
    user_agent_bytes = struct.pack("B", 0)
    height = struct.pack("i", 500000)
    payload = version + services + timestamp + addr_recv + addr_from + nonce + user_agent_bytes + height
    return payload


def get_height(version_message):
    block_height = struct.unpack('i', version_message[121:125])[0]
    return block_height


def save_message():
    message = make_message("version", make_version_message())
    open('version_message.txt', 'w').write(message.encode('hex'))

def save_message_bytes():
    message = make_message("version", make_version_message())
    open('version_message', 'wb').write(message)

if __name__ == "__main__":
    conf = json.load(open("config.json"))
    while True:
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect((conf["peer_ip"], 8333))
            message = open('version_message.txt', 'r').read().decode('hex')
            sock.send(message)
            version_message = sock.recv(126)
            block_height = get_height(version_message)
            if block_height < 550000:
                raise Exception
            print(block_height)
            break
        except Exception as err:
            sock.close()
            time.sleep(2.5)
            print(err)
        finally:
            sock.close()

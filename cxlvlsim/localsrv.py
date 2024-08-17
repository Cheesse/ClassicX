#!/bin/python3
# This script redirects standard files
# of the local server to a TCP port.

from io import SEEK_CUR
from socket import (
  AddressFamily, SocketKind, socket,
  timeout, IPPROTO_TCP, TCP_NODELAY
)
from subprocess import PIPE, Popen
from time import sleep

BINARY = 'out/x64/Debug/blox.exe'
SERIAL_PORT = 'COM9'
#HOST ='127.0.0.1'
HOST = '0.0.0.0'
PORT = 25566
ADDR = (HOST, PORT)

#p = Popen(
#  args=[BINARY],
  #bufsize=0,
  #stdin=PIPE,
  #stdout=PIPE
#)
#pr = open(SERIAL_PORT, 'rb', buffering=0)
#p = open(SERIAL_PORT, 'w+b', buffering=0)
p = None

sock = socket(
  family=AddressFamily.AF_INET,
  type=SocketKind.SOCK_STREAM
)
sock.bind((HOST, PORT))
sock.listen(0)
sock.settimeout(1)
print('Waiting for a connection...')

#def end(p: Popen):
def end(p):
  print('Local server closed.')
  #p.stdin.close()
  p.close()
  print('Closed stdin. Waiting for child...')
  #p.wait()
  print('Done!')
  exit()

def loop(p):
  p = open(SERIAL_PORT, 'w+b', buffering=0)
  try:
    (remo, remo_addr) = sock.accept()
    sock.setsockopt(IPPROTO_TCP, TCP_NODELAY, 1)
  except timeout:
    return

  remo.settimeout(None)
  print('Accepted, spawning child...')

  while True:
    inbuf = b''
    try:
      #print('read from tcp')
      inbuf = remo.recv(2048)
    except ConnectionResetError:
      inbuf = b''
    except ConnectionAbortedError:
      inbuf = b''

    if len(inbuf) == 0:
      print('Remote host closed.')
      break
    #p.stdin.write(inbuf)
    #p.stdin.flush()
    #print('writing to serial:', inbuf)
    #p.seek(0, SEEK_CUR)
    p.write(inbuf)
    # p.seek(0, SEEK_CUR)
    p.flush()
    # sleep(1/20)
    outbuf = bytearray(0)
    #p.close()
    #p = open(SERIAL_PORT, 'rb', buffering=0)
    #p.seek(0)
    try:
      while True:
        #sleep(0.025)
        oldlen = len(outbuf)
        #print('read from serial')
        outbuf += p.read(2048)#p.stdout.read(2048)
        if len(outbuf) == oldlen:
          # end(p)
          # Try again.
          continue
        #print('writing to tcp:', outbuf)
        if outbuf[-1] == 1:
          #remo.send(outbuf[:-1])
          remo.send(outbuf[:-1])
          break
        else:
          # Keep reading.
          pass
          #remo.send(outbuf)
      #p.close()
      #p = open(SERIAL_PORT, 'wb', buffering=0)
    except ConnectionResetError:
      p.close()
      break
    except ConnectionAbortedError:
      p.close()
      break

  remo.close()
  print('Waiting for a connection...')

try:
  while True:
    loop(p)
except KeyboardInterrupt as e:
  print('Stopped.')
  print(e)
  exit()
end()

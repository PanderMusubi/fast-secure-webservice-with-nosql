#!/usr/bin/env python
# -*- coding: utf-8 -*-

import base64, errno, httplib, urllib
from Crypto.Cipher import AES

def encrypt(data, cypher):
    mod = len(data) % 16
    if mod != 0:
        for i in range(16 - mod):
            data += ':'
    return cipher.encrypt(data)

def decrypt(data, cypher):# only for testing
    return cipher.decrypt(data).replace(':', '')

def encode(data):
    return base64.b64encode(data)

def decode(data):# only for testing
    return base64.b64decode(data)

def get_score(user_id, location, host, path, headers, conn):
    try:
        params = urllib.urlencode({'u': user_id, 'l': location})
        print 'http://' + host + path + '?' + params
        conn.request('POST', path, headers)
        response = conn.getresponse()
        print response.status, response.reason
        data = response.read()
    except:
        print 'Unable to upload'

user_id1 = '****'
location1 = 'NL-UT'

user_id2 = '++++'
location2 = 'BE-VLI'

print 'data:'
print 'user_id1', user_id1
print 'location1', location1
print 'user_id2', user_id2
print 'location2', location2

print 'encrypting data...'
key = b'Sixteen byte key'
cipher = AES.new(key, AES.MODE_ECB)# AES.MODE_ECB is the default
euser_id1 = encrypt(user_id1, cipher)
euser_id2 = encrypt(user_id2, cipher)

print 'encoding encrypted data...'
eeuser_id1 = encode(euser_id1)
eeuser_id2 = encode(euser_id2)

print 'encoded encrypted data:'
print 'user_id1', eeuser_id1
print 'user_id2', eeuser_id2

print 'decrypted decoded encoded encrypted data:'
print 'user_id1', decrypt(decode(eeuser_id1), cipher)
print 'user_id2', decrypt(decode(eeuser_id2), cipher)

print 'making HTTP POST requests...'
headers = {'Content-type': 'application/x-www-form-urlencoded', 'Accept': 'text/plain'}
path = '/blahblah/score'
host = 'hellebaard.nl:8081'
conn = httplib.HTTPConnection(host)

get_score(eeuser_id1, location1, host, path, headers, conn)
get_score(eeuser_id2, location2, host, path, headers, conn)

conn.close()

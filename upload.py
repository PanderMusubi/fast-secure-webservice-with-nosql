#!/usr/bin/env python
# -*- coding: utf-8 -*-

import base64, errno, httplib, urllib
from Crypto.Cipher import AES

def serialize(user_id, word_id, word_type, synset_id='', data=''):
    return '%s,%s,%s,%s,%s' %(user_id, word_id, synset_id, word_type, data)

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

def upload(data, host, path, headers, conn):
    try:
        params = urllib.urlencode({'d': data})
        print 'http://' + host + path + '?' + params
        conn.request('POST', path, headers)
        response = conn.getresponse()
        print response.status, response.reason
        data = response.read()
    except:
        print 'Unable to upload'

# type 1 (is singular with plural(s))
data1a = serialize(
    user_id = '****',
    word_id = '41012530',
    word_type = 1,
    synset_id = '37581',
    data = 'dogmata;dogma\'s')

data1b = serialize(
    user_id = '****',
    word_id = '41012531',
    word_type = 1,
    data = 'smörgåsbord')# testing Unicode UTF-8

# type 2 (is uncountable)
data2a = serialize(
    user_id = '****',
    word_id = '41012532',
    word_type = 2,
    synset_id = '37582')

data2b = serialize(
    user_id = '****',
    word_id = '41012533',
    word_type = 2)

# type 3 (is only plural)
data3a = serialize(
    user_id = '****',
    word_id = '41012534',
    word_type = 3,
    synset_id = '37583')

data3b = serialize(
    user_id = '****',
    word_id = '41012535',
    word_type = 3)

# type 4 (is plural for singular(s))
data4a = serialize(
    user_id = '****',
    word_id = '41012536',
    word_type = 4,
    synset_id = '37584',
    data = 'boek')

data4b = serialize(# e.g. offered word == 'krassen' which is not singular but plural of 'kras' (scratch) and 'krasse' (smart elderly person)
    user_id = '****',
    word_id = '41012537',
    word_type = 4,
    data = 'kras;krasse')

# type 5 (is a noun but combination of types)
data5a = serialize(
    user_id = '****',
    word_id = '41012538',
    word_type = 5,
    synset_id = '37585')

data5b = serialize(
    user_id = '****',
    word_id = '41012539',
    word_type = 5)

# type 6 (is not a noun)
data6a = serialize(
    user_id = '****',
    word_id = '41012540',
    word_type = 6,
    synset_id = '37586')

data6b = serialize(
    user_id = '****',
    word_id = '41012541',
    word_type = 6)

print 'data:'
print data1a
print data1b
print data2a
print data2b
print data3a
print data3b
print data4a
print data4b
print data5a
print data5b
print data6a
print data6b

print 'encrypting data...'
key = b'Sixteen byte key'
cipher = AES.new(key, AES.MODE_ECB)# AES.MODE_ECB is the default
edata1a = encrypt(data1a, cipher)
edata1b = encrypt(data1b, cipher)
edata2a = encrypt(data2a, cipher)
edata2b = encrypt(data2b, cipher)
edata3a = encrypt(data3a, cipher)
edata3b = encrypt(data3b, cipher)
edata4a = encrypt(data4a, cipher)
edata4b = encrypt(data4b, cipher)
edata5a = encrypt(data5a, cipher)
edata5b = encrypt(data5b, cipher)
edata6a = encrypt(data6a, cipher)
edata6b = encrypt(data6b, cipher)

print 'encoding encrypted data...'
eedata1a = encode(edata1a)
eedata1b = encode(edata1b)
eedata2a = encode(edata2a)
eedata2b = encode(edata2b)
eedata3a = encode(edata3a)
eedata3b = encode(edata3b)
eedata4a = encode(edata4a)
eedata4b = encode(edata4b)
eedata5a = encode(edata5a)
eedata5b = encode(edata5b)
eedata6a = encode(edata6a)
eedata6b = encode(edata6b)

print 'encoded encrypted data:'
print eedata1a
print eedata1b
print eedata2a
print eedata2b
print eedata3a
print eedata3b
print eedata4a
print eedata4b
print eedata5a
print eedata5b
print eedata6a
print eedata6b

print 'decrypted decoded encoded encrypted data:'
print decrypt(decode(eedata1a), cipher)
print decrypt(decode(eedata1b), cipher)
print decrypt(decode(eedata2a), cipher)
print decrypt(decode(eedata2b), cipher)
print decrypt(decode(eedata3a), cipher)
print decrypt(decode(eedata3b), cipher)
print decrypt(decode(eedata4a), cipher)
print decrypt(decode(eedata4b), cipher)
print decrypt(decode(eedata5a), cipher)
print decrypt(decode(eedata5b), cipher)
print decrypt(decode(eedata6a), cipher)
print decrypt(decode(eedata6b), cipher)

print 'making HTTP POST requests...'
headers = {'Content-type': 'application/x-www-form-urlencoded', 'Accept': 'text/plain'}
path = '/blahblah/plurals'
host = 'hellebaard.nl:8081'
conn = httplib.HTTPConnection(host)

upload(eedata1a, host, path, headers, conn)
upload(eedata1b, host, path, headers, conn)
upload(eedata2a, host, path, headers, conn)
upload(eedata2b, host, path, headers, conn)
upload(eedata3a, host, path, headers, conn)
upload(eedata3b, host, path, headers, conn)
upload(eedata4a, host, path, headers, conn)
upload(eedata4b, host, path, headers, conn)
upload(eedata5a, host, path, headers, conn)
upload(eedata5b, host, path, headers, conn)
upload(eedata6a, host, path, headers, conn)
upload(eedata6b, host, path, headers, conn)

conn.close()

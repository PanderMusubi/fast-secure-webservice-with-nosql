fast-secure-webservice-with-nosql
=================================

Fast and secure webservice written in C with MongoDB NoSQL database

Prerequisites
=============
sudo apt-get install mongodb-clients mongodb-server colorgcc gcc
sudo ln -sf /usr/bin/colorgcc /etc/alternatives/cc
cd /tmp
wget https://github.com/algernon/libmongo-client/archive/master.zip
unzip master.zip
cd libmongo-client-master
autoreconf -i
./configure
make
sudo make (install in /usr/local)

Client
======
Clients to test are Python scripts

./upload.py encrypts and Base64 encodes a payload and sends it in a POST request to webservice plurals.

./get_score.py encrypts and Base64 encodes user_id and and sends it with location in a POST request to webservice score.

Server
======
The server consists of several parts.

Part 1: Receiving

A thread in C code is running in TODO webserver. All it does is:
* process only POSTs with parameter called d with a value
* that value of d is URL-decoded and stored in field encrypted_encoded in MongoDB database blahblah.plurals
* the value is unique in the database, duplicates are ignored
Example call is http://hellebaard.nl:8081/blahblah/plurals?d=Lj55GxX%2Beq3KcrOszba793CUA7TLIkx5xx5RqgPWD0w%3D

Part 2: Decoding, decrypting and calculating score

A cron job runs daily a Python script that:
* decodes and decrypts the value of field encrypted_encoded in MongoDB database blahblah.plurals
* stores comma separated values in fields user_id, word_id, synset_id, word_type and data.
* updates user statistics in MongoDB database blahblah.users
See data model below for examples.

Part 3: Providing scores

A thread in C code is running in the same webserver. All it does is:
* receive user chosen location in parameter called l, when this parameter is present
* this parameter is in form of string containing ISO 3166-2 code and is stored
* scores for user are being returned

Data model
==========
MongoDB database has by default a unique ObjectID or _id from which also the insert time can be derived. The following extra fields are defined:

Namespace blahblah.plurals

"Field name"
  "Mongo BSON type in C"
  "Notes or examples"

_id (ObjectId)
  BSON_TYPE_OID
  this contains timestamp from moment of insert

encoded_encrypted_data
  BSON_TYPE_STRING
  e.g. Lj55GxX+eq3KcrOszba793CUA7TLIkx5xx5RqgPWD0w=
  
user_id
  BSON_TYPE_STRING
  e.g. ****

word_id
  BSON_TYPE_INT64
  e.g. 41012531
  
synset_id
  BSON_TYPE_INT64
  e.g. 0
  
word_type
  BSON_TYPE_INT32
  e.g. 1
  
data
  BSON_TYPE_STRING
  e.g. smörgåsbord
  
  
Namespace blahblah.users

"Field name"
  "Mongo BSON type in C"
  "Notes or examples"

_id (ObjectId)
  BSON_TYPE_OID
  this contains timestamp from moment of insert

user_id
  BSON_TYPE_STRING
  e.g. ***
  
location
  BSON_TYPE_STRING
  e.g. NL-UT
  
submitted_plurals
  BSON_TYPE_INT32
  e.g. 1
  
country_rating
  BSON_TYPE_INT32
  e.g. 1
    
region_ration
  BSON_TYPE_INT32
  e.g. 1
  
Database can be viewed by running mongodump. Contents can be viewed by running mongoexport -d blahblah -c plurals. Real-time statistics can be monitored by running mongostats.

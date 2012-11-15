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

./upload.py encrypts and Base64 encodes a payload and sends it in a POST request to webservice plurals. This call will be made again after x hours when reply is not successful.

./get_score.py encrypts and Base64 encodes user_id and and sends it with location in a POST request to webservice score. This call may be made only once every 24 hours, client will govern this.

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
Example call is http://hellebaard.nl:8081/blahblah/score?u=sikfr42CDJ8DpSThurbzsw%3D%3D&l=NL-UT and the reply is JSON with score.

{
    "plural_submits": 100,
    "plural_position_overall": 20,
    "plural_position_country": 10,
    "plural_position_region": 1,
    "for_location": "NL-UT"
}

When another location is submitted then is known in database, that location will be used next time when score is retrieved. Information message in client when location is changed that it will take 24 hours later before new score is reported for the new location.

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
  e.g. NULL or ****

word_id
  BSON_TYPE_INT64
  e.g. NULL or 41012531
  
synset_id
  BSON_TYPE_INT64
  e.g. NULL or 37581
  
word_type
  BSON_TYPE_INT32
  e.g. NULL or 1
  
data
  BSON_TYPE_STRING
  e.g. NULL or smörgåsbord
  
  
Namespace blahblah.users

"Field name"
  "Mongo BSON type in C"
  "Notes or examples"

_id (ObjectId)
  BSON_TYPE_OID
  this contains timestamp from moment of insert

encrypted_user_id
  BSON_TYPE_STRING
  e.g. sikfr42CDJ8DpSThurbzsw%3D%3D
  
user_id
  BSON_TYPE_STRING
  e.g. NULL or ***
  
location
  BSON_TYPE_STRING
  e.g. NULL or NL-UT
  
plurals_submitted
  BSON_TYPE_INT32
  e.g. NULL or 100
  
plurals_position_overall
  BSON_TYPE_INT32
  e.g. NULL or 20
    
plurals_position_country
  BSON_TYPE_INT32
  e.g. NULL or 10
    
plurals_position_region
  BSON_TYPE_INT32
  e.g. NULL or 1
  
Database can be viewed by running mongodump. Contents can be viewed by running mongoexport -d blahblah -c plurals or ./dump-db.sh. Real-time statistics can be monitored by running mongostats.

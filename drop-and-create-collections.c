#include <mongo.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

	static void
drop_collection (mongo_sync_connection *conn, char *db, char *coll)
{
	if (mongo_sync_cmd_exists(conn, db, coll)) {
		if (!mongo_sync_cmd_drop(conn, db, coll)) {
			fprintf (stderr, "Error dropping collection %s.%s: %s\n", db, coll, strerror(errno));
			exit(1);
		}
	}
}

	static void
create_collection (mongo_sync_connection *conn, char *db, char *coll)
{
	if (!mongo_sync_cmd_exists(conn, db, coll)) {
		if (!mongo_sync_cmd_create(conn, db, coll, MONGO_COLLECTION_DEFAULTS)) {
			fprintf (stderr, "Error creating collection %s.%s: %s\n", db, coll, strerror(errno));
			exit(1);
		}
	}
}

	static void
create_index (mongo_sync_connection *conn, char *db, char *coll, char *field)
{
	int len = strlen(db)+strlen(coll)+2;
	char dbcoll[len];
	strcat(dbcoll, db);
	strcat(dbcoll, ".");
	strcat(dbcoll, coll);

	bson *index;
	index = bson_build (BSON_TYPE_INT64, field, 1,
			BSON_TYPE_NONE);
	bson_finish (index);

	if (!mongo_sync_cmd_index_create(conn, dbcoll, index, MONGO_INDEX_UNIQUE | MONGO_INDEX_DROP_DUPS | MONGO_INDEX_BACKGROUND | MONGO_INDEX_SPARSE)) {
		fprintf (stderr, "Error creating on collection %s index %s: %s\n", dbcoll, field, strerror(errno));
		exit (1);
	}
	bson_free (index);
}

	int
main (void)
{
	mongo_sync_connection *conn;

	conn = mongo_sync_connect ("localhost", 27017, FALSE);
	if (!conn)
	{
		fprintf (stderr, "Connection failed: %s\n", strerror (errno));
		return 1;
	}
	if (!mongo_sync_conn_set_auto_reconnect (conn, TRUE))
	{
		fprintf (stderr, "Enabling reconnect failed: %s\n", strerror (errno));
		return 1;
	}

	char* db = "blahblah";
	drop_collection(conn, db, "scores");
	drop_collection(conn, db, "plurals");
	create_collection(conn, db, "scores");
	create_collection(conn, db, "plurals");
	create_index(conn, db, "scores", "encrypted_data");
	create_index(conn, db, "plurals", "encrypted_user_id");

	mongo_sync_disconnect (conn);

	return 0;
}

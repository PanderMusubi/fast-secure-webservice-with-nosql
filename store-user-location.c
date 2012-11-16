#include <mongo.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

	static void
delete_old (mongo_sync_connection *conn)
{
	bson *b;
	b = bson_build
		(BSON_TYPE_STRING, "encrypted_user_id", "UUFSOhFPVNOMlZxF29DKTw==", -1,
		 BSON_TYPE_NONE);
	bson_finish (b);

	//TODO How to delete all records?

	if (!mongo_sync_cmd_delete(conn, "blahblah.score", 0, b)) {
		fprintf (stderr, "Error deleting: %s\n", strerror(errno));
		exit (1);

		bson_free (b);
	}

	bson *b2;
	b2 = bson_build
		(BSON_TYPE_STRING, "encrypted_user_id", "sikfr42CDJ8DpSThurbzsw%3D%3D", -1,
		 BSON_TYPE_NONE);
	bson_finish (b2);

	//TODO How to delete all records?

	if (!mongo_sync_cmd_delete(conn, "blahblah.score", 0, b2)) {
		fprintf (stderr, "Error deleting: %s\n", strerror(errno));
		exit (1);

		bson_free (b2);
	}

}

	static void
drop_and_create (mongo_sync_connection *conn)
{
	if (mongo_sync_cmd_exists(conn, "blahblah", "score")) {
//	    printf("collection exists and is being dropped\n");
    	if (!mongo_sync_cmd_drop(conn, "blahblah", "score")) {
        	fprintf (stderr, "Error dropping collection: %s\n", strerror(errno));
        	exit(1);
    	}
	}
	if (!mongo_sync_cmd_exists(conn, "blahblah", "score")) {
//	    printf("collection does not exist and is being created\n");
    	if (!mongo_sync_cmd_create(conn, "blahblah", "score", MONGO_COLLECTION_DEFAULTS)) {
        	fprintf (stderr, "Error creating collection: %s\n", strerror(errno));
        	exit(1);
        }
	}
}

//FIXME Error creating index: Operation not supported
	static void
create_index (mongo_sync_connection *conn)
{
	bson *indexes;

	indexes = bson_build (BSON_TYPE_INT64, "encrypted_user_id", 1,
	BSON_TYPE_NONE);
	bson_finish (indexes);

	if (!mongo_sync_cmd_index_create(conn, "blahblah.score", indexes, MONGO_INDEX_UNIQUE | MONGO_INDEX_DROP_DUPS | MONGO_INDEX_BACKGROUND | MONGO_INDEX_SPARSE)) {
    	fprintf (stderr, "Error creating index: %s\n", strerror(errno));
       	exit (1);
    }
	bson_free (indexes);
}

	static void
do_inserts (mongo_sync_connection *conn)
{
	bson *user1;
	user1 = bson_build
		(BSON_TYPE_STRING, "encrypted_user_id", "sikfr42CDJ8DpSThurbzsw%3D%3D", -1,
		 BSON_TYPE_STRING, "user_id", "", -1,
		 BSON_TYPE_STRING, "location", "", -1,
		 BSON_TYPE_INT32, "plurals_submitted", 0,
		 BSON_TYPE_INT32, "plurals_position_overall", 0,
		 BSON_TYPE_INT32, "plurals_position_country", 0,
		 BSON_TYPE_INT32, "plurals_position_region", 0,
		 BSON_TYPE_NONE);
	bson_finish (user1);
	if (!mongo_sync_cmd_insert (conn, "blahblah.score", user1, NULL))
	{
		fprintf (stderr, "Error inserting document 1: %s\n", strerror(errno));
		exit (1);
	}
	bson_free (user1);

	bson *user2;
	user2 = bson_build
		(BSON_TYPE_STRING, "encrypted_user_id", "UUFSOhFPVNOMlZxF29DKTw==", -1,
		 BSON_TYPE_STRING, "user_id", "****", -1,
		 BSON_TYPE_STRING, "location", "BE-VLI", -1,
		 BSON_TYPE_INT32, "plurals_submitted", 100,
		 BSON_TYPE_INT32, "plurals_position_overall", 20,
		 BSON_TYPE_INT32, "plurals_position_country", 10,
		 BSON_TYPE_INT32, "plurals_position_region", 1,
		 BSON_TYPE_NONE);
	bson_finish (user2);
	if (!mongo_sync_cmd_insert (conn, "blahblah.score", user2, NULL))
	{
		fprintf (stderr, "Error inserting document 2: %s\n", strerror(errno));
		exit (1);
	}
	bson_free (user2);

	bson *user3;
	user3 = bson_build
		(BSON_TYPE_STRING, "encrypted_user_id", "UUFSOhFPVNOMlZxF29DKTw==", -1,
		 BSON_TYPE_STRING, "user_id", "****", -1,
		 BSON_TYPE_STRING, "location", "BE-VLI", -1,
		 BSON_TYPE_INT32, "plurals_submitted", 100,
		 BSON_TYPE_INT32, "plurals_position_overall", 20,
		 BSON_TYPE_INT32, "plurals_position_country", 10,
		 BSON_TYPE_INT32, "plurals_position_region", 1,
		 BSON_TYPE_NONE);
	bson_finish (user2);
	if (!mongo_sync_cmd_insert (conn, "blahblah.score", user3, NULL))
	{
		fprintf (stderr, "Error inserting document 2: %s\n", strerror(errno));
		exit (1);
	}
	bson_free (user3);
}

	static void
do_query (mongo_sync_connection *conn)
{
	mongo_sync_cursor *c;
	bson *query;
	gchar *error = NULL;

	query = bson_build
		(BSON_TYPE_STRING, "user_id", "****", -1,
		 BSON_TYPE_NONE);
	bson_finish (query);

	c = mongo_sync_cursor_new (conn, "blahblah.score",
			mongo_sync_cmd_query (conn, "blahblah.score", 0,
				0, 10, query, NULL));
	if (!c)
	{
		fprintf (stderr, "Error creating the query cursor: %s\n",
				strerror (errno));
		exit (1);
	}
	bson_free (query);

	while (mongo_sync_cursor_next (c))
	{
		bson *b = mongo_sync_cursor_get_data (c);
		bson_cursor *bc;
		const gchar *loc;

		if (!b)
		{
			int e = errno;

			mongo_sync_cmd_get_last_error (conn, "blahblah.score", &error);
			fprintf (stderr, "Error retrieving cursor data: %s\n",
					(error) ? error : strerror (e));
			exit (1);
		}

		bc = bson_find (b, "location");
		bson_cursor_get_string (bc, &loc);
		printf ("\rLocation: %s\n", loc);

		bson_cursor_free (bc);
		bson_free (b);
	}
	printf ("\n");

	mongo_sync_cursor_free (c);
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

	delete_old (conn);
    drop_and_create(conn);
	create_index (conn);
	do_inserts (conn);
	do_query (conn);

	mongo_sync_disconnect (conn);

	return 0;
}

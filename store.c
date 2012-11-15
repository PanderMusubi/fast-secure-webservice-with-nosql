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
		(BSON_TYPE_TIMESTAMP, "processed", 1294860709000,
		 BSON_TYPE_NONE);
	bson_finish (b);

//TODO How to delete all records?

	if (!mongo_sync_cmd_delete(conn, "blahblah.plurals", 0, b)) {
		fprintf (stderr, "Error creating index: %s\n", strerror(errno));
		exit (1);

		bson_free (b);
	}
}

//FIXME Error creating index: Operation not supported
/*	static void
create_index (mongo_sync_connection *conn)
{
	bson *indexes;

	indexes = bson_build (BSON_TYPE_STRING,    "encoded_encrypted_data", "", -1,
			BSON_TYPE_NONE);
	bson_finish (indexes);

	if (!mongo_sync_cmd_index_create(conn, "blahblah.plurals", indexes, MONGO_INDEX_UNIQUE | MONGO_INDEX_DROP_DUPS | MONGO_INDEX_BACKGROUND | MONGO_INDEX_SPARSE)) {
		fprintf (stderr, "Error creating index: %s\n", strerror(errno));
		exit (1);

		bson_free (indexes);
	}
}*/

	static void
do_inserts (mongo_sync_connection *conn)
{
	bson *data1a;
	data1a = bson_build
		(BSON_TYPE_TIMESTAMP, "processed", NULL,
		 BSON_TYPE_STRING,    "encoded_encrypted_data", "SGVsbG8gV29ybGQgCg==", -1,
		 BSON_TYPE_STRING,    "user_id", NULL, -1,
		 BSON_TYPE_INT64,     "word_id", NULL,
		 BSON_TYPE_INT64,     "synset_id", NULL,
		 BSON_TYPE_INT32,     "word_type", NULL,
		 BSON_TYPE_STRING,    "data", NULL, -1,
		 BSON_TYPE_NONE);
    // Missing values or NULL values are simply not provided.
	bson_finish (data1a);

	if (!mongo_sync_cmd_insert (conn, "blahblah.plurals", data1a, NULL))
	{
		fprintf (stderr, "Error inserting document: %s\n", strerror(errno));
		exit (1);
	}
	bson_free (data1a);

	bson *data1b;
	data1b = bson_build
		(BSON_TYPE_TIMESTAMP, "processed", 1294860709000,
		 BSON_TYPE_STRING,    "encoded_encrypted_data", "SGVsbG8gV29ybGQgCg==", -1,
		 BSON_TYPE_STRING,    "user_id", "****", -1,
		 BSON_TYPE_INT64,     "word_id", 41012531,
		 BSON_TYPE_INT64,     "synset_id", NULL,
		 BSON_TYPE_INT32,     "word_type", 1,
		 BSON_TYPE_STRING,    "data", "smörgåsbord", -1,
		 BSON_TYPE_NONE);
	bson_finish (data1b);

	if (!mongo_sync_cmd_insert (conn, "blahblah.plurals", data1b, NULL))
	{
		fprintf (stderr, "Error inserting document: %s\n", strerror(errno));
		exit (1);
	}
	bson_free (data1b);
}

	static void
do_query (mongo_sync_connection *conn)
{
	mongo_sync_cursor *c;
	bson *query;
	gchar *error = NULL;

	query = bson_build
		(BSON_TYPE_TIMESTAMP, "processed", 1294860709000,
		 BSON_TYPE_NONE);
	bson_finish (query);

	c = mongo_sync_cursor_new (conn, "blahblah.plurals",
			mongo_sync_cmd_query (conn, "blahblah.plurals", 0,
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
		gint32 w_t;
		gint64 w_i;

		if (!b)
		{
			int e = errno;

			mongo_sync_cmd_get_last_error (conn, "blahblah.plurals", &error);
			fprintf (stderr, "Error retrieving cursor data: %s\n",
					(error) ? error : strerror (e));
			exit (1);
		}

		bc = bson_find (b, "word_type");
		bson_cursor_get_int32 (bc, &w_t);
		printf ("\rWord type: %d\n", w_t);

		bc = bson_find (b, "word_id");
		bson_cursor_get_int64 (bc, &w_i);
		printf ("\rWord id: %d\n", (int)w_i);

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
	do_inserts (conn);
//	create_index (conn);
	do_query (conn);

	mongo_sync_disconnect (conn);

	return 0;
}

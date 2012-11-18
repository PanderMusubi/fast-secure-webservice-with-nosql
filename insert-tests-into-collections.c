#include <mongo.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

	static void
do_inserts (mongo_sync_connection *conn, char *db)
{
	char *coll = "scores";
	int len = strlen(db)+strlen(coll)+4;
	char dbcoll1[len];
	strcat(dbcoll1, db);
	strcat(dbcoll1, ".");
	strcat(dbcoll1, coll);

	bson *user;
	user = bson_build
		(BSON_TYPE_STRING, "encrypted_user_id", "sikfr42CDJ8DpSThurbzsw%3D%3D", -1,
		 BSON_TYPE_STRING, "user_id", "", -1,
		 BSON_TYPE_STRING, "location", "", -1,
		 BSON_TYPE_INT32, "plurals_submitted", 0,
		 BSON_TYPE_INT32, "plurals_position_overall", 0,
		 BSON_TYPE_INT32, "plurals_position_country", 0,
		 BSON_TYPE_INT32, "plurals_position_region", 0,
		 BSON_TYPE_NONE);
	bson_finish (user);
	if (!mongo_sync_cmd_insert (conn, dbcoll1, user, NULL))
	{
		fprintf (stderr, "Error inserting into collection %s: %s\n", dbcoll1, strerror(errno));
		exit (1);
	}
	bson_free (user);

printf("1");
	user = bson_build
		(BSON_TYPE_STRING, "encrypted_user_id", "UUFSOhFPVNOMlZxF29DKTw==", -1,
		 BSON_TYPE_STRING, "user_id", "****", -1,
		 BSON_TYPE_STRING, "location", "BE-VLI", -1,
		 BSON_TYPE_INT32, "plurals_submitted", 100,
		 BSON_TYPE_INT32, "plurals_position_overall", 20,
		 BSON_TYPE_INT32, "plurals_position_country", 10,
		 BSON_TYPE_INT32, "plurals_position_region", 1,
		 BSON_TYPE_NONE);
	bson_finish (user);
	if (!mongo_sync_cmd_insert (conn, dbcoll1, user, NULL))
	{
		fprintf (stderr, "Error inserting into collection %s: %s\n", dbcoll1, strerror(errno));
		exit (1);
	}
printf("1");
	// testing uniqueness via index on encrypted_user_id
	if (!mongo_sync_cmd_insert (conn, dbcoll1, user, NULL))
	{
		fprintf (stderr, "Error inserting into collection %s: %s\n", dbcoll1, strerror(errno));
		exit (1);
	}
	bson_free (user);

	coll = "plurals";
	len = strlen(db)+strlen(coll)+4;
	char dbcoll2[len];
	strcat(dbcoll2, db);
	strcat(dbcoll2, ".");
	strcat(dbcoll2, coll);
}

	int
main (void)
{
	mongo_sync_pool_connection *conn;
    mongo_sync_pool *pool;
    pool = mongo_sync_pool_new ("localhost", 27017, 6, 0);
    conn = mongo_sync_pool_pick (pool, TRUE);
 	
	do_inserts ((mongo_sync_connection*) conn, "blahblah");

    if (!mongo_sync_pool_return (pool, conn))
    {
		fprintf (stderr, "Returning connection to pool failed: %s\n", strerror (errno));
		return 1;
    }
 	mongo_sync_pool_free (pool);

	return 0;
}

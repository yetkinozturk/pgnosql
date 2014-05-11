CREATE EXTENSION hstore;
CREATE EXTENSION pgcrypto;
CREATE EXTENSION pg_trgm;
CREATE EXTENSION plv8;


CREATE TABLE kvstore (
	  id serial PRIMARY KEY,
	  key varchar(128) UNIQUE,
	  key_hash bytea UNIQUE,
	  value text,
	  entry_date TIMESTAMP,
	  expire_date TIMESTAMP
);


CREATE UNIQUE INDEX kvstore_eq_key ON kvstore USING btree (key);
CREATE UNIQUE INDEX kvstore_hashedkey ON kvstore USING btree (key_hash);
CREATE INDEX kvstore_likekey ON kvstore USING GIST (key gist_trgm_ops);

CREATE OR REPLACE FUNCTION DROP_ALL_LIKE(IN _schema TEXT, IN _parttionbase TEXT) 
RETURNS void 
LANGUAGE plpgsql
AS
$$
DECLARE
    row     record;
BEGIN
    FOR row IN 
        SELECT
            table_schema,
            table_name
        FROM
            information_schema.tables
        WHERE
            table_type = 'BASE TABLE'
        AND
            table_schema = _schema
        AND
            table_name ILIKE (_parttionbase || '%')
    LOOP
        EXECUTE 'DROP TABLE ' || quote_ident(row.table_schema) || '.' || quote_ident(row.table_name);
        RAISE INFO 'Dropped table: %', quote_ident(row.table_schema) || '.' || quote_ident(row.table_name);
    END LOOP;
    TRUNCATE TABLE kvstore;
END;
$$;
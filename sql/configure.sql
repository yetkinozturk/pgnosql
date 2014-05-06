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


CREATE TABLE user_tables (
  id serial PRIMARY KEY,
  table_name varchar(128) UNIQUE,
  table_type varchar(128)
);

CREATE UNIQUE INDEX user_tables_eq_table_name ON user_tables USING btree (table_name);
CREATE INDEX user_tables_liketable_name ON user_tables USING GIST (table_name gist_trgm_ops);


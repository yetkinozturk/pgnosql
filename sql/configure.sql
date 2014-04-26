CREATE EXTENSION hstore;
CREATE EXTENSION pgcrypto;
CREATE EXTENSION pg_trgm;


CREATE TABLE kvstore (
	  id serial PRIMARY KEY,
	  key varchar(128) UNIQUE,
	  key_hash bytea UNIQUE,
	  value text,
	  entry_date DATE,
	  expire_date DATE
);


CREATE UNIQUE INDEX kvstore_eq_key ON kvstore USING btree (key);
CREATE UNIQUE INDEX kvstore_hashedkey ON kvstore USING btree (key_hash);
CREATE INDEX kvstore_likekey ON kvstore USING GIST (key gist_trgm_ops);


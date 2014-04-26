pgnosql : NoSQL Frontend for Postgresql (WARNING: UNDER HEAVY DEVELOPMENT)
=======

Instructions
============
- Install dependencies :)

- Start Postgresql

For Debian based Linux distros:

sudo /etc/init.d/postgresql start

- Configure pgnosql server with config/pgnosql.cfg

- Configure sql/setup.sql (make your password same with config/pgnosql.cfg)

- psql -U <superusername> -f sql/setup.sql

- psql -U pgnosql pgnosql -f sql/configure.sql

- make


Dependencies
============

- Postgresql >= 9.3

For Debian based Linux distros:

apt-get install postgresql-9.3


see instructions here if it is not in your repo:
http://www.postgresql.org/download/linux/debian/


- Other Dependencies:

For Debian based Linux distros:

sudo apt-get install autoconf automake libpqxx-3.1 libpqxx-3.1-dev liblog4cxx10 liblog4cxx10-dev libgmpxx4ldbl libgmp3-dev libapr1 libaprutil1 libtool libconfig++-dev postgresql-contrib-9.3 libboost-all-dev






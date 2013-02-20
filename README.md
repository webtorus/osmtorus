osmtorus
========

### short description
osmtorus, daemon software that returns json representative a list of points of the shortest path between 2 points


### explanation of possible commands

###### generating application from source
make

###### deleting temporary files generated at compile
make clean

###### installation server as a daemon
make install

###### uninstall daemonuse case
make uninstall

###### specify that the daemon must be launched at startup
make startup

###### specify that the daemon should be started removing applications on startup
make startdown

###### start the server
sudo service osmtorus start

###### stop the server
sudo service osmtorus stop


### use case : 12 steps

###### step 01 : import the project
git clone https://github.com/webtorus/osmtorus.git

###### step 02 : go to the directory osmtorus
cd osmtorus

###### step 03 : compilation of sources
make

###### step 04 : installation server as a daemon
make install

###### step 05 : get file montpellier.osm


###### step 06 : rename the file montpellier.osm in map.osm
mv montpellier.osm map.osm

###### step 07 : place the file map.osm in directory /etc/osmtorus/
cp map.osm /etc/osmtorus/

###### step 08 : start the server
sudo service osmtorus start

###### step 09 : start a http request to the server
bin/osmtorus_client "http://localhost:7777/route?source=43.622252,3.858335&target=43.604878,3.880791&type=foot"

###### step 10 : stop the server
sudo service osmtorus stop

###### step 11 : remove the daemon osmtorus
make uninstall

###### step 12 : delete files generated during compilation
make clean

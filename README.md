# uvstudy
Starting the server tour
I need to build the server myself. 

All protocols are revisited. 
Some enum IDs are re-encoded. 

# Here is how it goes.
Ubuntu(14.04 LTS) or some components are hard to find.

*Install the boost library. 
*Clone the libuv and make it. (github.com/libuv/libuv)
*Install the protobuf-compiler and libprotobuf-dev
   sudo apt-get install -y protobuf-compiler
	 apt-cache search protobuf
   sudo apt-get install -y libprotobuf-dev

* mkdir out(my output files are written here.)
Here is the plan: integrate this module into our test app.


/*
http://www.ibm.com/developerworks/cloud/library/cl-bluemix-node-redis-app/index.html

also taken from

http://howtonode.org/node-redis-fun

and from 

https://gist.github.com/ixzo/4750663
*/

/*
to view data in redis using redis-cli

show all keys for snippet
keys snippet:94:cpu*

get data for snippet
get snippet:94:cpu:4

flush the database
flushdb
*/

//initalize express
var express = require("express");
var fs = require('fs');
var http = require('http');
var path = require('path');

var app = express();
app.set('port', 8080);
app.use(express.static(path.join(__dirname, 'public')));
app.use(express.json());

var redis = require('redis');

//index for all values being pushed
var nextId = 1;

//create http server
http.createServer(app).listen(app.get('port'), function(){
  console.log('Express server listening on port ' + app.get('port'));
});

//setup access to redis server
var redisService = true;
var credentials =  {"host":"127.0.0.1", "port":6379};

var db = redis.createClient(credentials.port, credentials.hostname);
db.on("error", function(err) {
  console.error('There was an error with the redis client ' + err);
});


// Serve up our static resources 
app.get('/', function(req, res) {
  fs.readFile('./public/index.html', function(err, data) {
    res.end(data);
  });
});
 

// create API endpoint 
app.post('/api', function(req, res) {
  message = req.body;
  
  //var date = new Date();
  console.log('Received data round: ' + nextId + ' >> ', Date() );
  console.log(' ' );

  //loop through datasets and push to redis
  for(var i = 0; i < message.length; i++) {
      //publish data to REDIS
      db.set( 'snippet:'+nextId+':'+message[i].plugin+':'+i    , JSON.stringify( message[i] ));

      //log core plugin name to console
      //console.log(message[i].plugin);
   }

  //increment counter
  nextId += 1; 
 
  res.end();
});








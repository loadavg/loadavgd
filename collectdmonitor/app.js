/*

http://www.ibm.com/developerworks/cloud/library/cl-bluemix-node-redis-app/index.html

also taken from

http://howtonode.org/node-redis-fun

and from 

https://gist.github.com/ixzo/4750663
*/

/*
 auth_to_expressjs_with_curl.sh
https://gist.github.com/ryankirkman/892018

*/
 
//initalize express
var express = require("express");
var fs = require('fs');
var http = require('http');
var path = require('path');

var queryString = require('querystring');
var url = require('url');

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

  /*
   * hacked this in for authentication from collectd
   * but getting no auth headers in data sent over from write_http plugin 
   * https://github.com/collectd/collectd/blob/master/src/write_http.c
   * need these to validate posts ?
   * HELP: IRC #collectd channel
   */

  var header=req.headers['authorization']||'',        // get the header
      token=header.split(/\s+/).pop()||'',            // and the encoded auth token
      auth=new Buffer(token, 'base64').toString(),    // convert from base64
      parts=auth.split(/:/),                          // split on colon
      username=parts[0],
      password=parts[1];

  //res.writeHead(200,{'Content-Type':'text/plain'});
  //res.end('username is "'+username+'" and password is "'+password+'"');
  //console.log('username is "'+username+'" and password is "'+password+'"');

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








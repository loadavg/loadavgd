/*
API
 */

/*
https://erichonorez.wordpress.com/2013/02/10/how-create-a-rest-api-with-node-js-and-express/
*/

/*
testing

curl -i -X POST http://localhost:8080/api --data '{"description":"blabla"}' -H "Content-Type: application/json"
*/

/*
needed for collectd http_write to get past selinux

setsebool -P collectd_tcp_network_connect 1
*/

function CollectdMonitor() {
    //this.tasks = [];
    this.nextId = 1;
}

CollectdMonitor.prototype.save = function () {
    
        this.nextId++;
		console.log('Received data round >> ', this.nextId );

}

var express = require('express');
var app = express();

var CollectdMonitor = new CollectdMonitor();


app.configure(function () {
    // used to parse JSON object given in the body request
    app.use(express.bodyParser());
});



/**
 * HTTP POST /tasks/
 * Body Param: the JSON task you want to create
 * Returns: 200 HTTP code
 */
app.post('/api', function (request, response) {
    
    var task = request.body;

    //response.write(JSON.stringify(task));
    //response.end();

	var date = new Date();
	
	console.log('Received data >> ', date );
	
	var jsonobj = JSON.stringify(task, null ,2);
	//var jsonobj = JSON.stringify(task);
	//console.log(jsonobj[0]);
	
	//console.log(task);

	for(var i = 0; i < task.length; i++) {
	    	console.log(task[i].plugin);

	 }

	CollectdMonitor.save();
	console.log(' ' );

});



 
//app.listen(8080); //to port on which the express server listen


var server = app.listen(8080, function () {

  var host = server.address().address
  var port = server.address().port

  console.log('collectd collector app listening at http://%s:%s', host, port)

})


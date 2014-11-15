//index.js

var express = require('express'),
	app = express(),
	MongoClient = require('mongodb').MongoClient,
	colors = require('colors'),
	http = require('http'),
	port = 8888;

MongoClient.connect('mongodb://localhost:27017/clockdata', function(err, db) {
	if (!err) {
		// users = db.collection('users')
		data = db.collection('data')
		console.log('connected to database'.green)
	} else {
		console.log('can not connected to db'.red)
	}
})

app.use(express.static(__dirname + '/public'))

app.get('/input', function(req,res){
	console.log(">> /INPUT query from URL: ".cyan + JSON.stringify(req.query))
	insertData(req.query,function(error, data){
		if (!error) {
			console.log("insertData complete: \n".green+JSON.stringify(data))
      		res.send(data)
		}else{
			console.log("error on insertData: ".red + error)
      		res.send("error insertData: \n" + error)
		}
	})
})

app.get('/latest',function(req,res){
	latestData(function(error,data){
		if (!error) {
			res.send(data)
		}else{
			res.send(error)
		}
	})
})

http.createServer(app).listen(port, function() {
	console.log(('Server running on port' + port).yellow)
})

function latestData(callback){
	data.find().sort({_id:-1}).limit(1).toArray(function(error, data){
    	if (!error) {
      		console.log(data)
      		callback(null,data)
    	}else{
    		callback(error,null)
    	}
  	})
}

function insertData(query,callback){
	if (query.name != null) {
		var dataObject={name:query.name, time: new Date().getTime()}
		data.insert(dataObject, function(error, object){
			if (!error) {
				callback(null, object)
			}else{
				console.log('inser error')
				callback(error,null)
			}
		})

	}else{
		console.log('please insert with username');
	}
}
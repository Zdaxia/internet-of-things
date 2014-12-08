//index.js

var express = require('express'),
	app = express(),
	MongoClient = require('mongodb').MongoClient,
	colors = require('colors'),
	http = require('http'),
	port = 8888,
	WunderNodeClient = require("wundernode"),
	URL = require('url'),
	net = require('net')

var apikey = "YOUR_KEY"

var debug = false

var sunRiseHour,
	sunRiseMin, 
	sunSetHour,
	sunSetMin,
	dateInUsing = 0,
	sleepHours = 8

var netsocketConnections = []

// Create Client
var wunder = new WunderNodeClient(apikey, debug,  10, 'minute')


app.use(express.static(__dirname + '/public'))


var server = http.createServer(app).listen(port, function(){
  console.log(('Server running on port' + port).yellow)
})


app.get('/', function(req, res) {
        res.end('Hello from wundernode!')
})

var io = require('socket.io')(server)
io.on('connection', function(socket){
	socket.on('sunset', function(){
		console.log('hey it is night now')
	})

	socket.on('blink', function(){
		console.log('hey go to bed now')
		for(var i=0; i<netsocketConnections.length; i++){
	      			//emit to all of our netsocket (arduino) connections!
	      			netsocketConnections[i].write('2');
    	}

	})

	socket.on('sunrise', function(){	
		console.log('hey get up')
	})
})


var EventEmitter = require("events").EventEmitter
var netSocketEmitter = new EventEmitter()
var netsocketServer = net.createServer( function (netsocket){

  netsocketConnections.push(netsocket) //stick into our global array
  console.log("new netsocket server connection made".green)
  
  netsocket.on('end', function(){
    netsocketConnections = [] //empty the array, everyone will get added back on their next connection
    console.log("netsocket disconnected".red)
  })
})

netsocketServer.listen(5000, function(){
  console.log("netsocket server listening on port 5000".cyan)
})

var GetSunData = function(){
	wunder.astronomy('New%20York,NY', function(err, obj) {
            if (err){
                    console.log('errors: ' + err)
                    res.end("Error processing query string:" + 'New%20York,NY')
            }
            console.log('Successfully get sun data from weather Api'.green)
            
            var data = JSON.parse(obj)
            sunRiseHour = data.sun_phase.sunrise.hour
			sunRiseMin = data.sun_phase.sunrise.minute
			sunSetHour = data.sun_phase.sunset.hour
			sunSetMin = data.sun_phase.sunset.minute

			console.log(('sunrise- '+sunRiseHour+':'+sunRiseMin).green)
			console.log(('sunset- '+sunSetHour+':'+sunSetMin).green)
            
    })
}


var setIntervalInSever = function(){
	setInterval(function(){
		var date = new Date(),
			currentHour = date.getHours(),
			currentMin = date.getMinutes(),
			currentDate = date.getDate()

			if (dateInUsing != currentDate) {
				// which means that it's a new day, then we update the sun data
				GetSunData()
				dateInUsing = currentDate
			}

			if (currentHour == sunRiseHour &&  currentMin == sunRiseMin) {
				//make the alarm work
				for(var i=0; i<netsocketConnections.length; i++){
	      			//emit to all of our netsocket (arduino) connections!
	      			netsocketConnections[i].write('1');
    			}
			}

			if (currentHour == sunSetHour &&  currentMin == sunSetMin) {
				// light up
				for(var i=0; i<netsocketConnections.length; i++){
	      			//emit to all of our netsocket (arduino) connections!
	      			netsocketConnections[i].write('2');
	    		}
			}

			if (currentHour == (sunRiseHour-8+12) &&  currentMin == sunRiseMin) {
				// blink
				for(var i=0; i<netsocketConnections.length; i++){
	      			//emit to all of our netsocket (arduino) connections!
	      			netsocketConnections[i].write('1');
    			}
			}

			// console.log((currentDate+':'+currentHour+':'+currentMin).red)
			
	},1000)
}


var inputGetupData = function(){
	var getupTime = new Date().getTime()
	//put this timestamp to db
}

var init = function(){
	setIntervalInSever()
}

init()





/*
 *Mongdb I'm not sure to use it or not
*/

// MongoClient.connect('mongodb://localhost:27017/clockdata', function(err, db) {
// 	if (!err) {
// 		// users = db.collection('users')
// 		data = db.collection('data')
// 		console.log('connected to database'.green)
// 	} else {
// 		console.log('can not connected to db'.red)
// 	}
// })



// app.get('/input', function(req,res){
// 	console.log(">> /INPUT query from URL: ".cyan + JSON.stringify(req.query))
// 	insertData(req.query,function(error, data){
// 		if (!error) {
// 			console.log("insertData complete: \n".green+JSON.stringify(data))
//       		res.send(data)
// 		}else{
// 			console.log("error on insertData: ".red + error)
//       		res.send("error insertData: \n" + error)
// 		}
// 	})
// })

// app.get('/latest',function(req,res){
// 	latestData(function(error,data){
// 		if (!error) {
// 			res.send(data)
// 		}else{
// 			res.send(error)
// 		}
// 	})
// })



// function latestData(callback){
// 	data.find().sort({_id:-1}).limit(1).toArray(function(error, data){
//     	if (!error) {
//       		console.log(data)
//       		callback(null,data)
//     	}else{
//     		callback(error,null)
//     	}
//   	})
// }

// function insertData(query,callback){
// 	if (query.name != null) {
// 		var dataObject={name:query.name, time: new Date().getTime()}
// 		data.insert(dataObject, function(error, object){
// 			if (!error) {
// 				callback(null, object)
// 			}else{
// 				console.log('inser error')
// 				callback(error,null)
// 			}
// 		})

// 	}else{
// 		console.log('please insert with username')
// 	}
// }
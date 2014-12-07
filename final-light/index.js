//index.js

var express = require('express'),
	app = express(),
	MongoClient = require('mongodb').MongoClient,
	colors = require('colors'),
	http = require('http'),
	port = 8888,
	WunderNodeClient = require("wundernode"),
	URL = require('url')

var apikey = "dd72459b64310aa3"

var debug = false

var sunRiseHour,
	sunRiseMin,
	sunSetHour,
	sunSetMin,
	dateInUsing=0

// Create Client
var wunder = new WunderNodeClient(apikey, debug,  10, 'minute')


app.use(express.static(__dirname + '/public'))

http.createServer(app).listen(port, function() {
	console.log(('Server running on port' + port).yellow)
})

app.get('/', function(req, res) {
        res.end('Hello from wundernode!')
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

			if (dateInUsing!=currentDate) {
				// which means that it's a new day, then we update the sun data
				GetSunData()
				dateInUsing=currentDate
			}

			// console.log((currentDate+':'+currentHour+':'+currentMin).red)
	},1000)
}

var init = function(){

	setIntervalInSever()
	// GetSunData()
	
	


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
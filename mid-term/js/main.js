(function() {

	xively.setKey("[YOUR-KEY]");

	var feedID = [YOUR-ID], // Feed ID  
		datastreamID1 = "meal"; // Datastream ID  
	var fishIhadToday = 0;
	// selector = "#myelement"; // Your element on the page  

	xively.datastream.get(feedID, datastreamID1, function(datastream) {

		//first round for current data

		console.log('----current_value ' + datastream["current_value"]);
		fishIhadToday = datastream["current_value"];
		$('#fishNumber').text(fishIhadToday);
		$(".fishContiner").empty();
		for (var i = 0; i < fishIhadToday; i++) {
			$('.fishContiner').append('<div class="fish"></div>');
		};

		// Getting realtime!   
		// The function associated with the subscribe method will be executed   
		// every time there is an update to the datastream  
		xively.datastream.subscribe(feedID, datastreamID1, function(event, datastream_updated) {
			// Display the current value from the updated datastream  
			// $(selector).html(datastream_updated["current_value"]);
			console.log(datastream_updated["current_value"]);
			fishIhadToday = datastream_updated["current_value"];
			$('#fishNumber').text(fishIhadToday);
			$(".fishContiner").empty();
			for (var i = 0; i < fishIhadToday; i++) {
				$('.fishContiner').append('<div class="fish"></div>');
			};
		});

	});


}());
// global data imported from the server
var server_data = server;

// create a database with the symbols
function currentPrice(responses, symbol ) {
	// iterate responses
	for (var i in  responses) {
		// save the random key name of the first object that contains the last "close price" property to access the indez later
		var first_obj = Object.keys(responses[i]["Time Series (1min)"])[0];

		// save the current price of the shares
		server_data.find(function (e) { return e.symbol === symbol[i];})["current_price"] = Number(responses[i]["Time Series (1min)"][first_obj]["4. close"]);	
	}
}

// a javascript implementation of the lookup function in python helpers.py
var lookup = function(symbol) { 
	// Return a new promise.
	return new Promise(function(resolve, reject) {
		// Do the usual XHR stuff
		var url = "https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=" + symbol + "&interval=1min&apikey=NAJXWIA8D6VN6A3K";
		var req = new XMLHttpRequest();
		req.responseType = "json";
		req.open('GET', url, true);
		req.onload = function() {
			// This is called even on 404 etc
			// so check the status
			if (req.status == 200) {
				// Resolve the promise with the response text
				resolve(req.response);
			}
			else {
				// Otherwise reject with the status text
				// which will hopefully be a meaningful error
				reject(Error(req.statusText));
			}
		};
		// this throws an error so i comment it out
		// Handle network errors
//		req.onerror = function() {
//			reject(Error("Network Error"));
//		};

		// Make the request
		req.send();
	});
}

// array of lookup's function for every symbol in the user databases
var symbols = server_data.map(function(a) { return a.symbol;});
var promises = [];
for (let i in symbols) {
      promises.push(lookup(symbols[i]));
}

// chain promises to get data, update prices and only then render the charts
Promise.all(promises).then(function(responses) { currentPrice(responses, symbols); return server_data;}).then(function (server_data) {drawPieChart("pie", server_data); drawBarChart("bar", server_data);}).catch(reason => {console.log(reason)});

// https://stackoverflow.com/questions/23095637/how-do-you-do-a-random-rbg-in-javascript
function random_rgba(num, transparency){
	var colors = [];
	var o = Math.round, r = Math.random, s = 255;
	for (var idx = 0; idx < num; idx++) {
		colors[idx]= 'rgba(' + o(r()*s) + ',' + o(r()*s) + ',' + o(r()*s) + ',' + transparency + ')';
	}
	return colors;
}

// draw charts functions
function drawPieChart(id, obj) {
	var ctx = document.getElementById(id);
	var labels =  obj.map(function(e) {return e.symbol; });
	var data = obj.map(function(e) {return e.current_price * e.shares; });
	var myPieChart = new Chart(ctx,{
		type: 'pie',
		data: data = {
			labels: labels,
			datasets: [{
				data: data,
				backgroundColor: random_rgba(data.length, 0.2)
				}]
			}
	});
}

function drawBarChart (id, obj) {
	var ctx = document.getElementById(id);
	var labels =  obj.map(function(e) {return e.symbol; });
	var stock_prices = {
		    label: "Current Stock Price",
		    data: obj.map(function(e) {return e.current_price; }),
		    backgroundColor: '#1CAC78'
	};
	var stock_quantity = {
		    label: "# of Shares Owned",
		    data: obj.map(function(e) {return e.shares; }),
		    backgroundColor: '#FFA700'
	};
	var myChart = new Chart(ctx, {
	    type: 'bar',
	    data: {
		labels: labels,
		datasets: [stock_quantity, stock_prices] 
	    },
	    options: {
		responsive: true,
		maintainAspectRatio: true,
		scales: {
		    yAxes: [{
			ticks: {
			    beginAtZero:true
			}
		    }]
		}
	    }
	});
}

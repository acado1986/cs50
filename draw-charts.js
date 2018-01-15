// https://stackoverflow.com/questions/23095637/how-do-you-do-a-random-rbg-in-javascript
function random_rgba(num, transparency){
	var colors = [];
	var o = Math.round, r = Math.random, s = 255;
	for (var idx = 0; idx < num; idx++) {
		colors[idx]= 'rgba(' + o(r()*s) + ',' + o(r()*s) + ',' + o(r()*s) + ',' + transparency + ')';
	}
	return colors;
}

// Other way to randomize colors
//https://stackoverflow.com/questions/19269545/how-to-get-n-no-elements-randomly-from-an-array
function getRandom(arr, n) {
    var result = new Array(n),
        len = arr.length,
        taken = new Array(len);
    if (n > len)
        throw new RangeError("getRandom: more elements taken than available");
    while (n--) {
        var x = Math.floor(Math.random() * len);
        result[n] = arr[x in taken ? taken[x] : x];
        taken[x] = --len;
    }
    return result;
}
var colors = ["#e6194b", "#3cb44b", "#ffe119", "#0082c8", "#f58231", "#911eb4", "#46f0f0", "#f032e6", "#d2f53c", "#fabebe", "#008080", "#e6beff", "#aa6e28", "#fffac8", "#800000", "#aaffc3", "#808000", "#ffd8b1", "#000080"]

function drawPieChart(id, obj) {
	var ctx = document.getElementById(id);
	ctx.style.backgroundColor = 'rgba(255,255,255,255)';
	var labels =  obj.map(function(e) {return e.symbol; });
	var data = obj.map(function(e) {return e.price * e.shares; });
	var myPieChart = new Chart(ctx,{
		type: 'pie',
		data: data = {
			labels: labels,
			datasets: [{
				data: data,
				backgroundColor: getRandom(colors, data.length),
				}]
			}
	});
}

function drawBarChart (id, obj) {
	var ctx = document.getElementById(id);
	ctx.style.backgroundColor = 'rgba(255,255,255,255)';
	var labels =  obj.map(function(e) {return e.symbol; });
	var current_prices = {
		    label: "Current Stock Price",
		    data: obj.map(function(e) {return e.price; }),
		    backgroundColor: '#1CAC78',
	};
	var acquired_prices = {
		    label: "Acquired Stock Price",
		    data: obj.map(function(e) {return e.price_deposit; }),
		    backgroundColor: '#FFA700',
	};
	var myChart = new Chart(ctx, {
	    type: 'bar',
	    data: {
		labels: labels,
		datasets: [current_prices, acquired_prices]
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
 

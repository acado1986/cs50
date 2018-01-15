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
 
/* Table pagination */
/*		=================================
**		==== Simple Table Controller ====
**		=================================
**
**
**			With Pure JavaScript .. 
**	 
**
**		No Libraries or Frameworks needed!
**
**
**			fb.com/bastony
**	
**	https://codepen.io/bastony/pen/eBvOGv?editors=0010
*/

/* Modify for this page, no headers check condition, all pagination is done in <tbody> */

// get the table element
var $table = document.getElementsByClassName("transactions")[0].children[1],
// number of rows per page
$n = 20,
// number of rows of the table
$rowCount = $table.rows.length,
// an array to hold each row
$tr = [],
// loop counters, to start count from rows[1] (2nd row) if the first row has a head tag
$i,$ii,$j = 0;
// count the number of pages
var $pageCount = Math.ceil($rowCount / $n);
// if we had one page only, then we have nothing to do ..
if ($pageCount > 1) {
	// assign each row outHTML (tag name & innerHTML) to the array
	for ($i = $j,$ii = 0; $i < $rowCount; $i++, $ii++)
		$tr[$ii] = $table.rows[$i].outerHTML;
	// create a div block to hold the buttons
	$table.insertAdjacentHTML("afterend","<div id='buttons'></div");
	// the first sort, default page is the first one
	sort(1);
}

// ($p) is the selected page number. it will be generated when a user clicks a button
function sort($p) {
	/* create ($rows) a variable to hold the group of rows
	** to be displayed on the selected page,
	** ($s) the start point .. the first row in each page, Do The Math
	*/
	var $rows = "",$s = (($n * $p)-$n);
	for ($i = $s; $i < ($s+$n) && $i < $tr.length; $i++)
		$rows += $tr[$i];
	
	// now the table has a processed group of rows ..
	$table.innerHTML = $rows;
	// create the pagination buttons
	document.getElementById("buttons").innerHTML = pageButtons($pageCount,$p);
	// CSS Stuff
	document.getElementById("id"+$p).setAttribute("class","active");
}


// ($pCount) : number of pages,($cur) : current page, the selected one ..
function pageButtons($pCount,$cur) {
	/* this variables will disable the "Prev" button on 1st page
	   and "next" button on the last one */
	var	$prevDis = ($cur == 1)?"disabled":"",
		$nextDis = ($cur == $pCount)?"disabled":"",
		/* this ($buttons) will hold every single button needed
		** it will creates each button and sets the onclick attribute
		** to the "sort" function with a special ($p) number..
		*/
		$buttons = "<input type='button' value='&lt;&lt; Prev' onclick='sort("+($cur - 1)+")' "+$prevDis+">";
	for ($i=1; $i<=$pCount;$i++)
		$buttons += "<input type='button' id='id"+$i+"'value='"+$i+"' onclick='sort("+$i+")'>";
	$buttons += "<input type='button' value='Next &gt;&gt;' onclick='sort("+($cur + 1)+")' "+$nextDis+">";
	return $buttons;
}

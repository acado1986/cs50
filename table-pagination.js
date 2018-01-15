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
var $table = document.getElementsByClassName("pagination")[0].children[1],
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
	$table.parentNode.insertAdjacentHTML("afterend","<div id='buttons'></div");
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

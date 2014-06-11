var sampleStep = 13;
var name = "charP"
var cornerCount = 0;

function init() {
	updateSampleStep(sampleStep);
	updateName(name);
}

function updateSampleStep(newsampleStep) {
	sampleStep = parseInt(newsampleStep);
	var sampleStepDisplayed = sampleStep;
	document.getElementById('sampleStep').innerHTML = "sample step: " + sampleStepDisplayed;
	document.getElementById('sampleSlider').value = sampleStep;
	processInput();
}

function updateName(newName) {
	name = newName;
	document.getElementById('name').value = name;
	processInput();
}

function processInput() 
{
	document.getElementById('path').setAttribute('d', document.getElementById('pathInput').value);

	var points = pointsSampledFromPath(document.getElementById('path'), sampleStep);

	clearSvg(document.getElementById('sampled'));
	drawSamples(points);
	printSamples(points)
}


function selectText(elem) {
	var range = document.createRange();
	range.selectNode(elem);
	var selection = window.getSelection();
	selection.removeAllRanges();
	selection.addRange(range);
}

function findAngle(p0,p1,c) {
    var p0c = Math.sqrt(Math.pow(c.x-p0.x,2)+
                        Math.pow(c.y-p0.y,2)); // p0->c (b)   
    var p1c = Math.sqrt(Math.pow(c.x-p1.x,2)+
                        Math.pow(c.y-p1.y,2)); // p1->c (a)
    var p0p1 = Math.sqrt(Math.pow(p1.x-p0.x,2)+
                         Math.pow(p1.y-p0.y,2)); // p0->p1 (c)
    return Math.acos((p1c*p1c+p0c*p0c-p0p1*p0p1)/(2*p1c*p0c));
}

function isCorner(lastPoint, middlePoint, firstPoint) {
	return (Math.PI - findAngle(lastPoint, firstPoint, middlePoint)) > 0.5;
}

function pointsSampledFromPath(path, step) {
	cornerCount = 0;
	var points = [];
	var len = path.getTotalLength();
	var lastPoint;
	var lastLastPoint;
	var count = 0;
	var lastWasCorner = false;
	for (var i=0; i<len; i+=0.5) { //step through path in small steps
		var point = path.getPointAtLength(i);
		//at corners, always push multiple points
		if (count > 2 && !lastWasCorner && isCorner(point, lastPoint, lastLastPoint)) {
			cornerCount++;
			points.push(lastLastPoint);
			points.push(lastPoint);
			points.push(point);
			lastWasCorner = true;
		} else {
			lastWasCorner = false;
			if(count % step == 0) //at non-corners push only on sample step
				points.push(point);
		}
		count++;
		lastLastPoint = lastPoint;
		lastPoint = point;
	}
	//always push last point
	points.push(path.getPointAtLength(len));
	return points;
}

function clearSvg(svg) {
	while (svg.firstChild) {
	    svg.removeChild(svg.firstChild);
	}
}

function drawCircle(parent, position) {
	var circle = parent.ownerDocument.createElementNS("http://www.w3.org/2000/svg", "circle");

	circle.setAttribute("r", 1);
	circle.setAttribute("fill", "#00A030");
	circle.setAttribute("cx", position.x);
	circle.setAttribute("cy", position.y);

	parent.appendChild(circle);
}

function drawSamples(points) {
	var svg = document.getElementById('sampled');

	for (var i=0; i < points.length; i++) {
		drawCircle(svg, points[i]);
	}
}

function printSamples(points) {
	document.getElementById('samplesCount').innerHTML = "resulting in " + points.length + " points";
	if(cornerCount == 1)
		document.getElementById('cornerCount').innerHTML = cornerCount + " relevant corner detected";
	else
		document.getElementById('cornerCount').innerHTML = cornerCount + " relevant corners detected";

	var output = "&#9;&#9;&#9;&#9;{";

	var leftMostX = 10000;
	var rightMostX = -10000;
	for (var i=0; i<points.length; i++) {
		if(points[i].x > rightMostX)
			rightMostX = points[i].x;
		if(points[i].x < leftMostX)
			leftMostX = points[i].x;
		if(i > 0) {
			output += ", ";
			if(i % 3 == 0)
				output +="<br/>";
		}
		output += "Point(" + Math.round(points[i].x * 100) + ", " + Math.round(points[i].y * 100) + ")";		
	}
	output += "};<br />&#9;addToPointLibrary(\"" + name + "\", " + name;
	output += ", (sizeof(" + name + ")/sizeof(*" + name + ")), " + Math.round(leftMostX * 100) + ", " + Math.round(rightMostX * 100) + "); //" + points.length + " points"

	document.getElementById('output').innerHTML = output;
	selectText(document.getElementById('output'));
}

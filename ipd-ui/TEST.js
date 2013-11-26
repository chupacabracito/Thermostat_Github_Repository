// JavaScript Document


var graph = {}  //"JSON" key value store type of variable.

 graph.scale = {
    x: d3.scale.linear()
      .domain([0,60])
      .range([200,500]),  //translates from values to pixels

    y: d3.scale.linear()
      .domain([40,90])
      .range([100,500])  //translates from values to pixels
	  
    

  };



  // Setup axes
  graph.axis = {
    x: d3.svg.axis()
      .scale(graph.scale.x),
    y: d3.svg.axis()
      .scale(graph.scale.y)
  }


  graph.axis.x.orient("bottom");
  graph.axis.y.orient("left");




graph.svgGroup = {};

graph.drawGraph = function() {
graph.svgGroup.main = d3.select("#container").append("svg");


    // Draw Axes
    graph.svgGroup.xAxis = graph.svgGroup.main.append("g");

    graph.svgGroup.xAxis.call(graph.axis.x);

    graph.svgGroup.yAxis = graph.svgGroup.main.append("g")
      .call(graph.axis.y)
	  .attr("transform", "translate(200,10)");
	  
    graph.yAxisLabel = graph.svgGroup.yAxis.append("text")
      .attr("dy","1em")
      .attr("transform","rotate(-90)")
      .style("text-anchor","end")
      .text("Number of Students");
}
  
  
  //call function telling browser it's done rendering: "OnDOMupload" or "FinishedRendering" or something.  Function(graph.drawGraph);
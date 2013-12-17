// GET HOME PAGE
var fs = require('fs');                 // filesystem package -> test this

exports.index = function(req, res) {
    res.writeHead(200, {'Content-Type': 'text/html'});
    var contents = fs.readFileSync("./index.html", "UTF-8");
    res.end(contents);
    //res.render('index'); //<- not using JADE templates for this
}

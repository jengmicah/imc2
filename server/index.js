const express = require('express')
const bodyParser = require('body-parser')
const port = 80;

var registry_router = require('./routes/registry');

const app = express()

app.use(bodyParser.json())
app.use(bodyParser.urlencoded({extended: false}))

// set default content-type
app.use(function (req, res, next) {
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  res.header("Content-Type",'application/json');
  next();
});

app.use(registry_router);

app.listen(port, function () {
  console.log('Listening on port ' + port + '!')
})

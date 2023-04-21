const express = require('express');
const app = express();
const Point = require('./Point');
const Robot = require('./Robot');

var method = "turn";
var value = 180;
var time = 0;


const console = require('console');
const bodyParser = require('body-parser');
app.use(express.static('public'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

app.get('/api', (req, res) => {
  res.send(
    {
        method: method,
        time: time,
        value: value,
    }
    );
});

app.post('/submit', (req, res) => {
    console.debug(req.body.value);
    value = req.body.value;
    method = req.body.method;
    time = req.body.time;
    res.send('Thanks for submitting the form!');
  });

app.post('/coordinates', (req, res) => {
  
  const coordinates_text = req.body.coordinates;
  const coordinates = JSON.parse(coordinates_text)
  const points = []

  coordinates.forEach(c => {
    points.push(new Point(c));
  });
  const robbo = new Robot();
  const instructions = robbo.generateInstructions(points);
  res.send(JSON.stringify(instructions));
});

app.listen(process.env.PORT || 3000, () => {
  console.log('Server listening on port 3000');
});
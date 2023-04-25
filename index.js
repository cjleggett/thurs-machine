const express = require('express');
const app = express();
const Robot = require('./Robot');

var method = "turn";
var value = 180;
var time = 0;

var current_gyro;

var current_instructions = {
  instructions: [],
  id: -1
};

function getRandomInt(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1)) + min;
}


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

app.get('/api_coordinates', (req, res) => {
  res.send(current_instructions);
});

app.get('/api_gyro', (req, res) => {
  res.send(current_gyro);
});

app.get('/draw_circle', (req, res) => {
  let diameter = 1000;
  res.send(current_gyro);
});

app.post('/submit', (req, res) => {
    console.debug(req.body.value);
    value = req.body.value;
    method = req.body.method;
    time = req.body.time;
    res.send('Thanks for submitting the form!');
  });



app.post('/gyro', (req, res) => {
  let euler_x = req.body.euler_x * 180 / Math.PI;
  let euler_y = req.body.euler_y * 180 / Math.PI;
  let euler_z = req.body.euler_z * 180 / Math.PI;
  current_gyro = {
    x: euler_x,
    y: euler_y,
    z: euler_z,
  }
  res.send('Thanks, Geoff!');
});

app.post('/coordinates', (req, res) => {
  
  const coordinates_text = req.body.coordinates;
  const coordinates = JSON.parse(coordinates_text)
  const robbo = new Robot();
  const instructions = robbo.generateInstructions(coordinates);
  const t = getRandomInt(0, 10000);
  const output = {
    instructions: instructions,
    id: t
  }
  current_instructions = output;
  res.send("Successfully Updated!");
});

app.listen(process.env.PORT || 3000, () => {
  console.log('Server listening on port 3000');
});
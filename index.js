const express = require('express');
const app = express();
const Robot = require('./Robot');

var method = "turn";
var value = 180;
var time = 0;

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
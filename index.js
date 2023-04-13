const express = require('express');
const app = express();

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

app.listen(process.env.PORT || 3000, () => {
  console.log('Server listening on port 3000');
});
const express = require('express');
const bodyParser = require('body-parser');
const routes = require('./routes');

const app = express()

app.use(bodyParser.urlencoded({
    extended: true
}));

app.use(bodyParser.json());

// routes
app.use('/', routes);

const port = 3000
app.listen(port, () => console.log(`Server started on port ${port}!`))
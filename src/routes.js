const express = require('express');
const data = require('./data');

const router = express.Router();

router.get('/', data.getData)
router.post('/data', data.receiveData)

module.exports = router;
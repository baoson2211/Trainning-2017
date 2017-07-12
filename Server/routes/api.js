/* require modules */
var express = require('express');
var router = express.Router();
var fs = require('fs');
var path = require('path');
var DataDetails = require('../models/data');
var config = require('../config.json');


/* GET /api/data/
 */
router.get('/data', (req, res) => {
  DataDetails.find((err, data) => {
    if (err) {
      return console.log(err);
    }
    var DataArr = [];

    data.forEach(function (data) {
      var dataObject = {
        "name" : "",
        "data" : ""
      };
      dataObject.name = data.name;
      dataObject.data = data.data;

      DataArr.push(dataObject);
    });

    var reply = { "Devlist": DataArr};

    res.jsonp(reply);
    res.end();
  });
});

/* GET /api/data/
 */
router.get('/data/:name', (req, res) => {
  DataDetails.findOne({'name' : req.params.name}, (err, data) => {
    if (err) {
      return console.log(err);
    }

    var dataObject = {
      "data" : ""
    };

    try {
      dataObject.data = data.data + '°C';
    } catch (error) {
      dataObject.data = 'Not Available';
      console.log(error);
    }

    res.jsonp(dataObject);
    res.end();
  });
});


/* POST /api/data/
 */
router.post('/data/:name', (req, res) => {
  // console.log("Receive POST request!");
  // console.log('param1:', req.body.param1,'- type:', typeof req.body.param1);
  // console.log('param2:', req.body.param2,'- type:', typeof req.body.param2);
  DataDetails.findOne({'name' : req.params.name}, (err, data) => {
    if (err) {
      return console.log(err);
    }
    // already exists
    if (data) {
      console.log('name already exists');
      return res.end('Name already exists');
    }

    var newData      = new DataDetails();
    newData.name     = req.params.name;
    newData.data     = req.body.data ? req.body.param1 : '0';
    newData.save((err) => {
      if (err) {
        console.log('Error in Saving data: ' + err);
      }
      console.log('Create Device succesful');

      res.end("OK! Create Device");
    });
  });
});

/* PUT /api/data/
 */
router.put('/data/:name', (req, res) => {
  // console.log("Receive PUT request!");
  // console.log('param1:', req.body.param1,'- type:', typeof req.body.param1);
  // console.log('param2:', req.body.param2,'- type:', typeof req.body.param2);
  DataDetails.findOne({'name' : req.params.name}, (err, data) => {
    if (err) {
      return console.log(err);
    }

    data.data     = req.body.data ? req.body.data : data.data;

    data.save((err) => {
      if (err) {
        console.log('Error in Saving data: ' + err);
      }

      res.end("OK! Update succesfully");
      console.log('temp:', req.body.data,'- type:', typeof req.body.data);
    });
  });
});

module.exports = router;

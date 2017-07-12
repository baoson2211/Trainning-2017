// connect to mongoDB, DB named example4, document named UserDetail
var mongoose = require('mongoose');
var config   = require('../config.json');

var DataSchema = mongoose.Schema;

var DataDetail = new DataSchema({
	name: String,
	data: String
}, {collection: config.datas_collection});

var DataDetails = mongoose.model(config.datas_collection,DataDetail);

module.exports = DataDetails;

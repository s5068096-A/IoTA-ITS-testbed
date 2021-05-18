var mqtt = require('mqtt');
var client = mqtt.connect('mqtt://localhost')

client.on('connect', function () {
  client.subscribe('car_mqtt');


  console.log('Client has subscribed successfully');
  });

var readings;

client.on('message', function (topic, message){
  var readings = message.toString();
  console.log(readings); //if toString is not given, the message comes as buffer


});

const Iota = require('@iota/core');
const Converter = require('@iota/converter');

// Connect to a node
const iota = Iota.composeAPI({
//  provider: 'https://nodes.devnet.iota.org:443'
  provider: 'https://nodes.comnet.thetangle.org'

});

const depth = 3;
//MWM 9 for legacy devnet
const minimumWeightMagnitude = 10;

// Define a seed and an address.
// These do not need to belong to anyone or have IOTA tokens.
// They must only contain a mamximum of 81 trytes
// or 90 trytes with a valid checksum
const address =
  'HEQLOWORLDHELLOWORLDHELLOWORLDHELLOWORLDHELLOWORLDHELLOWORLDHELLOWORLDHELLOWOR99D';
const seed =
  'PUEOTSEITFEVEWCWBTSIZM9NKRGJEIMXTULBACGFRQK9IMGICLBKW9TTEVSDQMGWKBXPVCBMMCXWMNPDX';

// Define a message to send.
// This message must include only ASCII characters.
const message = JSON.stringify({"message": (readings)});

// Convert the message to trytes
const messageInTrytes = Converter.asciiToTrytes(message);

// Define a zero-value transaction object
// that sends the message to the address
const transfers = [
  {
    value: 0,
    address: address,
    message: messageInTrytes
  }
];


// Create a bundle from the `transfers` array
// and send the transaction to the node
iota
  .prepareTransfers(seed, transfers)
  .then(trytes => {
    return iota.sendTrytes(trytes, depth, minimumWeightMagnitude);
  })
  .then(bundle => {
    console.log(bundle[0].hash);
  })
  .catch(err => {
    console.error(err)
  });


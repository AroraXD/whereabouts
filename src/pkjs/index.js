var Pusher = require('./pusher');

function fetch() {
  //var req = new XMLHttpRequest();
  /*
  // build the GET request
  req.open('GET', 'http://dev.markitondemand.com/Api/Quote/json?symbol=' + symbol, true);
  req.onload = function(e) {
    if (req.readyState == 4) {
      // 200 - HTTP OK
      if(req.status == 200) {
        console.log(req.responseText);
        
        var response = JSON.parse(req.responseText);
        if (response['Message']) {
          // the markitondemand API sends a response with a Message
          // field when the symbol is not found
          Pebble.sendAppMessage({ 'QuoteKeyPrice': 'Not Found' });
        }
        if (response['Data']) {
          // data found, look for LastPrice
          var price = response['Data']['LastPrice'];
          console.log('Price is ' + price);

          Pebble.sendAppMessage({
            'YourKey': true
          }, function(e) {
            console.log('sent');
          }, function() {
            console.log('failed');
          });
        }
      } else {
        console.log('Request returned error code ' + req.status.toString());
      }
    }
  };
  req.send(null);
  */
}

// Set callback for the app ready event
Pebble.addEventListener('ready', function(e) {
  console.log('connect!');
  console.log(e.type);
  fetch();
  

  Pebble.sendAppMessage({
    'YourKey': true
  }, function(e) {
    console.log('sent');
  }, function() {
    console.log('failed');
  });
  
  
pusherTest();
  
});


function pusherTest() {
  // Enable pusher logging - don't include this in production
    Pusher.logToConsole = true;

    var pusher = new Pusher('f0c2cfa5ebd364b6521b', {
      cluster: 'eu',
      encrypted: true
    });

    var channel = pusher.subscribe('test_channel');
    channel.bind('my_event', function(data) {
      console.log(data.message);
    });
}

// Set callback for appmessage events
Pebble.addEventListener('appmessage', function(e) {
  console.log('message');
  //Receive from watch
  fetch();
});

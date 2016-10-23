function fetch() {
  var req = new XMLHttpRequest();
  // build the GET request
  req.open('GET', "http://545f9eab.ngrok.io/messages?for=4916092192757", true);
  req.onload = function(e) {
    if (req.readyState == 4) {
      // 200 - HTTP OK
      if(req.status == 200) {
        console.log(req.responseText);
        
        var response = JSON.parse(req.responseText);
        response.forEach(function(item) {
          if (item['message']) {
            Pebble.sendAppMessage({ 'location': item['message'], 'from', item['from'] });
          }
        })
      } else {
        console.log('Request returned error code ' + req.status.toString());
      }
    }
  };
  req.send(null);
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
});


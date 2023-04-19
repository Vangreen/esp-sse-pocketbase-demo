## Simple demo to connect to [PocketBase realtime api](https://pocketbase.io/docs/api-realtime/). 

#### Requirements
* esp32 base board (I used [wemos s2 mini](https://www.wemos.cc/en/latest/s2/s2_mini.html) )
* Imported ArduinoJson liblary to project
* Pocketbase with grant everyone access to collection we're going to listen to 

#### How poscketbase realtime api works 
That's basically SSE (Server-sent events) technology. 

To listen for events you need to Send GET request to realtime endpint, when connection has been established you need to set subscription with POST request. After that you are able to listen for events

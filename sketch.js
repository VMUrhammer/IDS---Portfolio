// We create a varible to our MQTT-connection
let mqttClient;

// The ID of the nearest DMI station closest to RUC, located at Kastrup Lufthavn.
const STATION_ID = "06180";
const DATA = "https://opendataapi.dmi.dk/v2/metObs/collections/observation/items";
// Topic sends data from our P5.js, and our ESP32 recivies it
const topic = "esp32IDS/DMI";

// Fuction sets our connection up with the MQTT
function setupMQTT() {
  // Creating a unique ID for the client/ computer 
  let randomId = "p5js_client_" + Math.random().toString(36).substr(2, 5);
  //adress of our broker/ server
  let broker = "wss://public.cloud.shiftr.io"; 

  
  // Connecting our broker using our own ID and password
  mqttClient = mqtt.connect(broker, {
    clientId: randomId,
    username: "public",
    password: "public",
  });

 
  // If it connects successfully run OnConnect
  mqttClient.on("connect", onConnect);
  // If the connection is failing, show a error in the console
  mqttClient.on("error", (err) => console.log("MQTT Error: ", err));
}


function setup() {
  createCanvas(400, 200);
  setupMQTT();
}
// This function runs once the connection is successful
function onConnect() {
  console.log("Connected to MQTT Broker");
  // Initial fetch every 10 seconds
  setInterval(getDataDMI, 10000); 
}

// Function fetches the data from DMI
async function getDataDMI() {
  try {
    // API fetches the latest temperature and humitidy from the DMI-station
    const tRes = await fetch(`${DATA}?stationId=${STATION_ID}&parameterId=temp_dry&sortorder=observed,DESC&limit=1`);
    const hRes = await fetch(`${DATA}?stationId=${STATION_ID}&parameterId=humidity&sortorder=observed,DESC&limit=1`);
    
    // Convert our data to a JSON format
    const tempData = await tRes.json();
    const humidData = await hRes.json();
      
    // Fetch values from DMI data
    const temp  = tempData.features[0].properties.value;
    const humid = humidData.features[0].properties.value;

    // If were still connected, we transform the data to a JSON string 
    // And transfere the data out on our channel/ Topic
    if (mqttClient && mqttClient.connected) {
      mqttClient.publish(topic, temp +","+ humid );
    
    }
    //if it cathes a fail, it prints error in our console
  } catch (error) {
    console.error("API Error:", error);
  }
}

function draw(){
  background("BLUE");
  // Visual feedback for your p5 canvas
  textAlign(CENTER, CENTER);
  text("DMI Data is fetching", width/2, height/2);
  textSize(20);
}
<?php
// Receive data from ESP8266
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $sensorValue = $_POST['sensor_value'];
    
    // Process or store the received data as needed
    // For example, you can save it to a database, log it, etc.

    // Send a response back to the ESP8266 if needed
    echo "Data received successfully.";
} else {
    // Invalid request method
    http_response_code(405);
    echo "Invalid request method.";
}
?>

<?php
// Check if data is received through the GET method
if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    // Retrieve data from the query string using $_GET
    $words = isset($_GET['words']) ? $_GET['words'] : '';

    // Process the data (you can perform any server-side logic here)
    echo "Received data via GET: " . $words;
} else {
    // Return an error response for non-GET requests
    header('HTTP/1.1 405 Method Not Allowed');
    echo "Method Not Allowed";
}
?>

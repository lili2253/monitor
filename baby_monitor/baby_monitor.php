<!DOCTYPE HTML>
<html>
  <head>
    <title>ESP8266 WITH MYSQL DATABASE</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css">
    <style>
      html {font-family: Arial; display: inline-block; text-align: center;}
      p {font-size: 1.2rem;}
      h4 {font-size: 0.8rem;}
      body {margin: 0;}
      .topnav {overflow: hidden;background:linear-gradient(to bottom right, #70c1b3, #247b9f);color:white; }
      .content {padding: 5px; }
      .card {background-color: white; box-shadow: 0px 0px 10px 1px rgba(140,140,140,.5); border: 1px solid #0c6980; border-radius: 15px;}
      .card.header {background-color: #0c6980; color: white; border-bottom-right-radius: 0px; border-bottom-left-radius: 0px; border-top-right-radius: 12px; border-top-left-radius: 12px;}
      .cards {max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));}
      .reading {font-size: 1.3rem;}
      .packet {color: #bebebe;}
      .temperatureColor {color: #fd7e14;}
      .humidityColor {color: #1b78e2;}
      .statusreadColor {color: #702963; font-size:12px;}
      .FanColor {color: #183153;}
      
      /* ----------------------------------- Toggle Switch */
      .switch {
        position: relative;
        display: inline-block;
        width: 50px;
        height: 24px;
      }

      .switch input {display:none;}

      .sliderTS {
        position: absolute;
        cursor: pointer;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #D3D3D3;
        -webkit-transition: .4s;
        transition: .4s;
        border-radius: 34px;
      }

      .sliderTS:before {
        position: absolute;
        content: "";
        height: 16px;
        width: 16px;
        left: 4px;
        bottom: 4px;
        background-color: #f7f7f7;
        -webkit-transition: .4s;
        transition: .4s;
        border-radius: 50%;
      }

      input:checked + .sliderTS {
        background-color: #00878F;
      }

      input:focus + .sliderTS {
        box-shadow: 0 0 1px #2196F3;
      }

      input:checked + .sliderTS:before {
        -webkit-transform: translateX(26px);
        -ms-transform: translateX(26px);
        transform: translateX(26px);
      }

      .sliderTS:after {
        content:'OFF';
        color: white;
        display: block;
        position: absolute;
        transform: translate(-50%,-50%);
        top: 50%;
        left: 70%;
        font-size: 10px;
        font-family: Verdana, sans-serif;
      }

      input:checked + .sliderTS:after {  
        left: 25%;
        content:'ON';
      }

      input:disabled + .sliderTS {  
        opacity: 0.3;
        cursor: not-allowed;
        pointer-events: none;
      }
     /* Gradient color */
.card.header {
    background: linear-gradient(to right, #70c1b3, #247b9f); /* Mixed gradient color for header */
}


.card {
    background-color:white;
    box-shadow: 0px 0px 20px rgba(0, 0, 0, 0.1); /* Adjusted shadow size for 3D effect */
    border: 1px solid #ccc; /* Softened the border color */
    border-radius: 15px;
    overflow: hidden; /* Ensure the overflow doesn't affect the shadow */
    transition: transform 0.3s ease; /* Smooth transition for hover effect */
}

.card:hover {
    transform: translateY(-5px); /* Lift the card slightly on hover */
}
.esp_color{
  color:white;
}
      /* ----------------------------------- */
    </style>
  </head>
  <body>
   
  <body>
    <div class="topnav">
      <h3>ESP8266 WITH MYSQL DATABASE</h3>
    </div>
    
    <br>
    
    <!-- __ DISPLAYS MONITORING AND CONTROLLING ____________________________________________________________________________________________ -->
    <div class="content">
      <div class="cards">
        
        <!-- == MONITORING ======================================================================================== -->
        <div class="card">
          <div class="card header">
            <h3 style="font-size: 1rem;">MONITORING</h3>
          </div>
          
          <!-- Displays the humidity and temperature values received from ESP32. *** -->
          <h4 class="temperatureColor"><i class="fas fa-thermometer-half"></i> TEMPERATURE</h4>
          <p class="temperatureColor"><span class="reading"><span id="ESP8266_01_Temp"></span> &deg;C</span></p>
          <h4 class="humidityColor"><i class="fas fa-tint"></i> HUMIDITY</h4>
          <p class="humidityColor"><span class="reading"><span id="ESP8266_01_Humd"></span> &percnt;</span></p>
          <!-- *********************************************************************** -->
          
          <p class="statusreadColor"><span>Status Read Sensor DHT11 : </span><span id="ESP8266_01_Status_Read_DHT11"></span></p>
        </div>
        <!-- ======================================================================================================= -->
        
        <!-- == CONTROLLING ======================================================================================== -->
        <div class="card">
          <div class="card header">
            <h3 style="font-size: 1rem;">CONTROLLING</h3>
          </div>
          
          <!-- Buttons for controlling the LEDs on Slave 2. ************************** -->
          <h4 class="LEDColor"><i class="fas fa-fan fan-icon"></i> Fan</h4>
          <label class="switch">
            <input type="checkbox" id="ESP8266_01_Fan" onclick="GetTogBtnLEDState('ESP8266_01_Fan')">
            <div class="sliderTS"></div>
          </label>
        
        </div>  
        <!-- ======================================================================================================= -->
        
      </div>
    </div>
    
    <br>
    
    <!-- ___________________________________________________________________________________________________________________________________ -->
    
    <script>
    //------------------------------------------------------------
    document.getElementById("ESP8266_01_Temp").innerHTML = "NN"; 
    document.getElementById("ESP8266_01_Humd").innerHTML = "NN";
    document.getElementById("ESP8266_01_Status_Read_DHT11").innerHTML = "NN";
   
    //------------------------------------------------------------
    
    Get_Data("esp8266_01");
    
    setInterval(myTimer, 5000);
    
    //------------------------------------------------------------
    function myTimer() {
        Get_Data("esp8266_01");
    }
    //------------------------------------------------------------
    
    //------------------------------------------------------------
    function Get_Data(id) {
        var xmlhttp;
        if (window.XMLHttpRequest) {
            // code for modern browsers
            xmlhttp = new XMLHttpRequest();
        } else {
            // code for old IE browsers
            xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
        }
        xmlhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                const myObj = JSON.parse(this.responseText);
                if (myObj.id == "esp32_01") {
                    document.getElementById("ESP8266_01_Temp").innerHTML = myObj.temperature;
                    document.getElementById("ESP8266_01_Humd").innerHTML = myObj.humidity;
                    document.getElementById("ESP8266_01_Status_Read_DHT11").innerHTML = myObj.status_read_sensor_dht11;
                    
                    if (myObj.Fan == "ON") {
                        document.getElementById("ESP8266_01_Fan").checked = true;
                    } else if (myObj.Fan == "OFF") {
                        document.getElementById("ESP8266_01_Fan").checked = false;
                    }
                }  
            }
        };
        xmlhttp.open("POST","getdata.php",true);
        xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
        xmlhttp.send("id="+id);
    }
    //------------------------------------------------------------
    
    //------------------------------------------------------------
    function GetTogBtnLEDState(togbtnid) {
        if (togbtnid == "ESP8266_01_Fan") {
            var togbtnchecked = document.getElementById(togbtnid).checked;
            var togbtncheckedsend = "";
            if (togbtnchecked == true) togbtncheckedsend = "ON";
            if (togbtnchecked == false) togbtncheckedsend = "OFF";
            Update_Fan("esp8266_01", "fan", togbtncheckedsend);
        }
    }
    //------------------------------------------------------------
    
    //------------------------------------------------------------
    function Update_Fan(id, Fnum, Fstate) {
        var xmlhttp;
        if (window.XMLHttpRequest) {
            // code for modern browsers
            xmlhttp = new XMLHttpRequest();
        } else {
            // code for old IE browsers
            xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
        }
        xmlhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                // you can handle response if needed
            }
        }
        xmlhttp.open("POST","updateFan.php",true);
        xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
        xmlhttp.send("id="+id+"&Fnum="+Fnum+"&Fstate="+Fstate);
    }
    //------------------------------------------------------------
</script>

  </body>
</html>
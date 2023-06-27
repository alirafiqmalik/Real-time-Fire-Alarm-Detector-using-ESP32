const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
body {
  background-color: black;
  color: white;
}

.card {
  max-width: 400px;
  min-height: 250px;
  background: #0c2e8a;
  padding: 30px;
  box-sizing: border-box;
  margin: 20px;
  box-shadow: 0px 2px 18px -4px rgba(0, 0, 0, 0.75);
}

h4 {
  color: white;
}

h1 {
  color: c;
  display: inline-block;
}

#MSG {
  color: white;
  background-color: #02b875;
  padding: 10px;
  border-radius: 5px;
}
</style>
<body>
  <div class="card">
    <h4>ALARM DETECTION</h4>
    
    <p id="MSG">No Alarm DETECTED</p>
    
  </div>
  <script>
    setInterval(function() {
      // Call a function repetatively with 2 Second interval
      getData();
    }, 100); //2000mSeconds update rate

    function getData() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("MSG").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "MSG", true);
      xhttp.send();
    }
  </script>
</body>
</html>

)=====";
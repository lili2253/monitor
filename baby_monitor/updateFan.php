<?php
  require 'database.php';
  
  //---------------------------------------- Condition to check that POST value is not empty.
  if (!empty($_POST)) {
    //........................................ keep track post values
    $id = $_POST['id'];
    $lednum = $_POST['Fnum'];
    $ledstate = $_POST['Fstate'];
    //........................................ 
    
    //........................................ 
    $pdo = Database::connect();
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    $sql = "UPDATE tem_fan_update SET " . $Fnum . " = ? WHERE id = ?";
    $q = $pdo->prepare($sql);
    $q->execute(array($Fstate,$id));
    Database::disconnect();
    //........................................ 
  }
  //---------------------------------------- 
?>
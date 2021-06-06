<?php
    header("Access-Control-Allow-Origin: *");
	
	//Database info
    $host = "------"; 
    $userName = "------";
    $password = "------";
    $dbName = "------";
	
    $conn = mysqli_connect($host, $userName, $password,$dbName );

    function getDistance($latitude1, $longitude1, $latitude2, $longitude2) {
        $earth_radius = 6371000;
        $dLat = deg2rad($latitude2 - $latitude1);
        $dLon = deg2rad($longitude2 - $longitude1);
        $a = sin($dLat/2) * sin($dLat/2) + cos(deg2rad($latitude1)) * cos(deg2rad($latitude2)) * sin($dLon/2) * sin($dLon/2);
        $c = 2 * asin(sqrt($a));
        $d = $earth_radius * $c;
        return $d; 
    }
    
    function resetData($conn,$arr){
        $sql = "TRUNCATE TABLE coords";
        $q = mysqli_query($conn, $sql) or die(mysqli_error($conn));
        $sql = "UPDATE `startEnd` SET `data` = '". $arr[1].",".$arr[2].",0,0', `distance` = '0', `startTime` = '0', `endTime` = '0'";
        $q = mysqli_query($conn, $sql) or die(mysqli_error($conn));
        if($q) {echo "S";}
    }


?>
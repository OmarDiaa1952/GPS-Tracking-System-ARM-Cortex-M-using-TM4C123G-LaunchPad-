<?php
include('conn.php');

    //Tiva commands
    if(isset($_POST['command'])){
        $cmd = $_POST['command'];
        switch($cmd[1]){ //@_,[...data]
            case 'I':   //@I,latitude,longitude
                $arr = explode(",",$cmd);
                resetData($conn,$arr);
                break;

            case 'G': //@G,
                $sql = "SELECT * FROM `startEnd` LIMIT 1";
                $q = mysqli_query($conn, $sql);
                $data = "";
                while($r = mysqli_fetch_array($q)){$data = $r['data'];}
                if(substr($data,-2) == ",0"){echo "F";} 
                else{ 
                    $arr = explode(",",$data); 
                    echo "\$S,".$arr[0].",".$arr[1].",".$arr[2].",".$arr[3].";\n";
                }

                break;
            
            case 'S': //@S,latitude,longitude
                $arr = explode(",",$cmd);
                $time = time();
                $data = "'".$arr[1]."','".$arr[2]."','".$time."'";
                $sql = "INSERT INTO `coords` (`lat`,`log`,`time`) VALUE (".$data.")";
                $q = mysqli_query($conn, $sql) or die(mysqli_error($conn));
                if($q){echo "S1\n";}
                $sql = "UPDATE `startEnd` SET `startTime` = '".$time."'";
                $q = mysqli_query($conn, $sql) or die(mysqli_error($conn));
                if($q){echo "S2";}
                break;

            case 'C'://@C,latitude,longitude
                $arr = explode(",",$cmd);
                $time = time();
                $data = "'".$arr[1]."','".$arr[2]."','".$time."'";
                
                $sql = "SELECT * FROM `coords` ORDER BY id DESC LIMIT 1";
                $q = mysqli_query($conn, $sql) or die(mysqli_error($conn));
                $lastLat = 0;
                $lastLog = 0;
                while($r = mysqli_fetch_array($q)){
                    $lastLat = $r['lat'];
                    $lastLog = $r['log'];
                }
                $distance = getDistance($arr[1], $arr[2], $lastLat, $lastLog);

                $sql = "INSERT INTO `coords` (`lat`,`log`,`time`) VALUE (".$data.")";
                $q = mysqli_query($conn, $sql) or die(mysqli_error($conn));

                $sql = "UPDATE `startEnd` SET distance = distance + ".$distance."";
                $q = mysqli_query($conn, $sql) or die(mysqli_error($conn));

                if($q){echo "S";}
                break;

            case 'E'://@E,latitude,longitude
                $arr = explode(",",$cmd);
                $time = time();
                $data = "'".$arr[1]."','".$arr[2]."','".$time."'";

                $sql = "SELECT * FROM `coords` ORDER BY id DESC LIMIT 1";
                $q = mysqli_query($conn, $sql) or die(mysqli_error($conn));
                $lastLat = 0;
                $lastLog = 0;
                while($r = mysqli_fetch_array($q)){
                    $lastLat = $r['lat'];
                    $lastLog = $r['log'];
                }
                $distance = getDistance($arr[1], $arr[2], $lastLat, $lastLog);

                $sql = "INSERT INTO `coords` (`lat`,`log`,`time`) VALUE (".$data.")";
                $q = mysqli_query($conn, $sql) or die(mysqli_error($conn));

                $sql = "UPDATE `startEnd` SET `endTime` = '".$time."' , distance = distance + ".$distance."";
                $q = mysqli_query($conn, $sql) or die(mysqli_error($conn));
                if($q){echo "S";}
                break;
        }

    }
    
    if(isset($_POST['update'])){//Server-To-Server
        $data = $_POST['latStart'].",".$_POST['logStart'].",".$_POST['latEnd'].",".$_POST['logEnd'];
        $sql = "UPDATE `startEnd` SET `data` = '".$data."'";
        $q = mysqli_query($conn, $sql) or die(mysqli_error($conn));
        if($q){header("Location:index.php");}
    }
    
    if(isset($_GET['startEnd'])){
        $sql = "SELECT * FROM `startEnd` LIMIT 1";
        $q = mysqli_query($conn, $sql) or die(mysqli_error($conn));
        $data = "s";
        while($r = mysqli_fetch_array($q)){$data = $r['data'];}
        echo $data;
    }

    if(isset($_GET['reading'])){
        $sql = "SELECT * FROM `startEnd` LIMIT 1";
        $q = mysqli_query($conn, $sql) or die(mysqli_error($conn));
        $data = "";
        while($r = mysqli_fetch_array($q)){
            $data = $r['distance'].",".$r['startTime'].",".$r['endTime'];
        }
        $sql = "SELECT * FROM `coords` ORDER BY id DESC LIMIT 2";
        $q = mysqli_query($conn, $sql) or die(mysqli_error($conn));
        $coords = [];
        while($r = mysqli_fetch_array($q)){
            array_push($coords,$r['lat'],$r['log']);
            $data .= ",".$r['lat'].",".$r['log'].",".$r['time'];
        }
        if(count($coords) == 4){
            $distance = getDistance(floatval($coords[0]), floatval($coords[1]), floatval($coords[2]), floatval($coords[3]));
        }else{
            $distance = 0;
        }
        $data .= ",".$distance;
        echo $data;
    }

    if(isset($_GET['forceReset'])){
        resetData($conn,[0,0,0]);

    }



?>
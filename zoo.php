<?php
    $zoo = new Zoo([
        'database_type' => 'mysql',
        'database_name' => 'mysql',
        'server' => '192.168.40.41',
        'port' => '3306',
        'username' => 'root',
        'password' => 'root',
        'charset' => 'utf8',
        // 'option' => ['a']
    ]);

    // debug_zval_dump($zoo->pdo);
    // $zoo->select('user', ['host', 'user', 'password']);
    // return;

    // var_dump($zoo->select('user', ['host', 'user', 'password']));
    // return;

    // $rs = $zoo->select('user');
    
    var_dump($zoo->select('user'));
    
    // $rs = $zoo->query("select * from user");

    // while($row = $rs->fetch()){
        // print_r($row);
    // }

    // $dsn = "mysql:host=192.168.40.41;dbname=mysql";
    // $db = new PDO($dsn, 'root', 'root');
    
    // var_dump($zoo->pdo);

    //  var_dump($zoo->query("select * from user"));
?>

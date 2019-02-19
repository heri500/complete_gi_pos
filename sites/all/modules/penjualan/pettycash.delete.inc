<?php

function delete_petty_cash(){
    if (isset($_POST['idpc'])){
        db_query('DELETE FROM user_petty_cash WHERE id=%d',$_POST['idpc']);
        $ReturnData = new stdClass();
        $ReturnData->message = 'Petty cash berhasil di hapus...!!!';
    }
    $Ret = array();
    $Ret[] = $ReturnData;
    print json_encode($Ret);
    exit();
}
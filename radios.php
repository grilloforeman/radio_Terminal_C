<?php
header('Content-Type: application/json; charset=utf-8');

$radios = [
    [
        "descricao" => "Clássicos do rock nacional e internacional",
        "nome_radio" => "Rádio Rock Brasil",
        "estipo" => "Rock",
        "regiao" => "SP, São Paulo",
        "url" => "https://s2.audiostream.hu/bdpstrock_192k"
    ],
    [
        "descricao" => "HBR1.COM DREAM FACTORY",
        "nome_radio" => "DREAM FACTORY",
        "estipo" => "TECH Music",
        "regiao" => "GW, Munique",
        "url" => "http://ubuntu.hbr1.com:19800/ambient.ogg"
    ],
    [
        "descricao" => "HBR1.COM DREAM TRANCE",
        "nome_radio" => "HBR1 TRANCE",
        "estipo" => "TECH Music",
        "regiao" => "GW, Munique",
        "url" => "http://ubuntu.hbr1.com:19800/trance.ogg"
    ],
    [
        "descricao" => "HBR1.COM DREAM TRonic",
        "nome_radio" => "HBR1 TRONIC",
        "estipo" => "TECH Music",
        "regiao" => "GW, Munique",
        "url" => "http://ubuntu.hbr1.com:19800/trONIC.ogg"
    ],
    [
        "descricao" => "RADIO PUNK FM BRASIL",
        "nome_radio" => "Rádio PUNK FM",
        "estipo" => "Punk",
        "regiao" => "SP, São Paulo",
        "url" => "https://radio.punkfm.com.br/listen/punkfmbrasil/punkfmbrasil.mp3?_=7343110462787&1764440598658"
    ],
];

echo json_encode($radios, JSON_PRETTY_PRINT | JSON_UNESCAPED_UNICODE);
?>

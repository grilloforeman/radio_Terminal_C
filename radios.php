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
        "url" => "https://radio.punkfm.com.br/listen/punkfmbrasil/punkfmbrasil.mp3?_=7343110462787&1764440598658
"
    ],
    [
        "descricao" => "Rock alternativo e indie",
        "nome_radio" => "Rádio Indie Rock",
        "estipo" => "Rock",
        "regiao" => "RS, Porto Alegre",
        "url" => "http://stream.radioindierock.com/live"
    ],
    [
        "descricao" => "Pop atual e sucessos internacionais",
        "nome_radio" => "Rádio Pop Brasil",
        "estipo" => "Pop",
        "regiao" => "RJ, Rio de Janeiro",
        "url" => "http://stream.radiopopbrasil.com/live"
    ],
    [
        "descricao" => "Rock clássico anos 70",
        "nome_radio" => "Rádio Classic Rock",
        "estipo" => "Rock",
        "regiao" => "SP, Campinas",
        "url" => "http://stream.radioclassicrock.com/live"
    ],
    [
        "descricao" => "Pop Rock nacional",
        "nome_radio" => "Rádio BR Pop Rock",
        "estipo" => "Pop Rock",
        "regiao" => "DF, Brasília",
        "url" => "http://stream.radiobrpoprock.com/live"
    ],
    [
        "descricao" => "Punk internacional e nacional",
        "nome_radio" => "Rádio Punk Station",
        "estipo" => "Punk",
        "regiao" => "PR, Curitiba",
        "url" => "http://stream.radiopunkstation.com/live"
    ],
    [
        "descricao" => "Rock pesado e metal",
        "nome_radio" => "Rádio Metal Rock",
        "estipo" => "Rock",
        "regiao" => "PE, Recife",
        "url" => "http://stream.radiometalrock.com/live"
    ],
    [
        "descricao" => "Pop internacional anos 2000",
        "nome_radio" => "Rádio Pop 2000",
        "estipo" => "Pop",
        "regiao" => "RJ, Rio de Janeiro",
        "url" => "http://stream.radiopop2000.com/live"
    ],
    [
        "descricao" => "Pop Rock internacional",
        "nome_radio" => "Rádio Pop Rock World",
        "estipo" => "Pop Rock",
        "regiao" => "SP, São Paulo",
        "url" => "http://stream.radiopoprockworld.com/live"
    ],
    [
        "descricao" => "Punk hardcore",
        "nome_radio" => "Rádio Hardcore Punk",
        "estipo" => "Punk",
        "regiao" => "RS, Porto Alegre",
        "url" => "http://stream.radiohardcorepunk.com/live"
    ],
    [
        "descricao" => "Rock alternativo nacional",
        "nome_radio" => "Rádio Rock Alternativo",
        "estipo" => "Rock",
        "regiao" => "SP, São Paulo",
        "url" => "http://stream.radiorockalternativo.com/live"
    ],
    [
        "descricao" => "Pop internacional atual",
        "nome_radio" => "Rádio Pop Mix",
        "estipo" => "Pop",
        "regiao" => "RJ, Rio de Janeiro",
        "url" => "http://stream.radiopopmix.com/live"
    ],
    [
        "descricao" => "Pop Rock anos 90",
        "nome_radio" => "Rádio 90 Pop Rock",
        "estipo" => "Pop Rock",
        "regiao" => "SP, São Paulo",
        "url" => "http://stream.radio90poprock.com/live"
    ],
    [
        "descricao" => "Punk nacional underground",
        "nome_radio" => "Rádio Underground Punk",
        "estipo" => "Punk",
        "regiao" => "RJ, Rio de Janeiro",
        "url" => "http://stream.radioundergroundpunk.com/live"
    ],
    [
        "descricao" => "Rock internacional",
        "nome_radio" => "Rádio Rock World",
        "estipo" => "Rock",
        "regiao" => "SP, São Paulo",
        "url" => "http://stream.radiorockworld.com/live"
    ],
    [
        "descricao" => "Pop nacional",
        "nome_radio" => "Rádio Pop Nacional",
        "estipo" => "Pop",
        "regiao" => "MG, Belo Horizonte",
        "url" => "http://stream.radiopopnacional.com/live"
    ],
    [
        "descricao" => "Pop Rock internacional",
        "nome_radio" => "Rádio Pop Rock Internacional",
        "estipo" => "Pop Rock",
        "regiao" => "SP, São Paulo",
        "url" => "http://stream.radiopoprockinternacional.com/live"
    ],
    [
        "descricao" => "Punk internacional",
        "nome_radio" => "Rádio Punk Internacional",
        "estipo" => "Punk",
        "regiao" => "RJ, Rio de Janeiro",
        "url" => "http://stream.radiopunkinternacional.com/live"
    ],
    [
        "descricao" => "Rock alternativo",
        "nome_radio" => "Rádio Rock Indie",
        "estipo" => "Rock",
        "regiao" => "SP, São Paulo",
        "url" => "http://stream.radiorockindie.com/live"
    ],
    [
        "descricao" => "Pop internacional",
        "nome_radio" => "Rádio Pop Internacional",
        "estipo" => "Pop",
        "regiao" => "RJ, Rio de Janeiro",
        "url" => "http://stream.radiopopinternacional.com/live"
    ],
    [
        "descricao" => "Pop Rock nacional",
        "nome_radio" => "Rádio Pop Rock Brasil",
        "estipo" => "Pop Rock",
        "regiao" => "SP, São Paulo",
        "url" => "http://stream.radiopoprockbrasil.com/live"
    ],
    [
        "descricao" => "Punk rock",
        "nome_radio" => "Rádio Punk Rock",
        "estipo" => "Punk",
        "regiao" => "RS, Porto Alegre",
        "url" => "http://stream.radiopunkrock.com/live"
    ],
    [
        "descricao" => "Rock clássico",
        "nome_radio" => "Rádio Rock Clássico",
        "estipo" => "Rock",
        "regiao" => "SP, São Paulo",
        "url" => "http://stream.radiorockclassico.com/live"
    ],
    [
        "descricao" => "Pop atual",
        "nome_radio" => "Rádio Pop Atual",
        "estipo" => "Pop",
        "regiao" => "RJ, Rio de Janeiro",
        "url" => "http://stream.radiopopatual.com/live"
    ],
    [
        "descricao" => "Pop Rock atual",
        "nome_radio" => "Rádio Pop Rock Atual",
        "estipo" => "Pop Rock",
        "regiao" => "SP, São Paulo",
        "url" => "http://stream.radiopoprockatual.com/live"
    ],
    [
        "descricao" => "Punk alternativo",
        "nome_radio" => "Rádio Punk Alternativo",
        "estipo" => "Punk",
        "regiao" => "RJ, Rio de Janeiro",
        "url" => "http://stream.radiopunkalternativo.com/live"
    ]
];

echo json_encode($radios, JSON_PRETTY_PRINT | JSON_UNESCAPED_UNICODE);
?>

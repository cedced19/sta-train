const TrainsDef = document.querySelector("#Trains");
const NumberOfTrains = 6;
const Colors = ['#FF0000', '#008000', '	#000080', '#0000FF', '#800080', '	#00FFFF'];

let vitesse = [];
let vitesseDes = [];
let position = [];
let messages = '';

let x = [];
let time = 0;
let vec_reel = [[], [], [], [], [], []];
let vec_des = [[], [], [], [], [], []];

let plotFlag = false;
let LastPlot = null;

function zoomer(){
    var div = document.getElementById("Map");
   
    if(!div){
        div = document.getElementById("MapExt");
        div.removeAttribute('id');
        div.setAttribute('id', 'Map');
    }else{
        div.removeAttribute('id');
        div.setAttribute('id', 'MapExt');
    }
}

function setTrains()
{   
    TrainsSet = TrainsDef;
    TrainsSet.innerHTML = null;
    let valeurHTML = '';
    for(var i = 0; i < NumberOfTrains; i++)
    {
        valeurHTML += `
        <div class="trainSquare" id="${i+1}" onclick="plotGraph(this.id)">
                <div style="background: ${Colors[i]};" class="circle"></div>
                <h3>Train ${i+1}</h3>
                <p>Vitesse : ${vitesse[i]} </p>
                <p>Position : ${position[i]} </p>
        </div>
        `
    }

    TrainsSet.innerHTML = valeurHTML;

}

function PrintMessages(){
    var Table = document.getElementById("Table");
    let valeurHTML = '';
    valeurHTML += `${messages}`
    if(valeurHTML != Table.innerHTML){
        Table.innerHTML = valeurHTML;
    }
}

function reloadStylesheets() {
    var queryString = '?reload=' + new Date().getTime();
    $('link[rel="stylesheet"]').each(function () {
        this.href = this.href.replace(/\?.*|$/, queryString);
    });
}

function plotGraph(train_id){
    LastPlot = train_id;
    plotFlag = true;
    document.getElementById("Graph").style.display = 'block';

    if(!document.getElementById("Plot")){
        document.getElementById("Graph").innerHTML += `
            <div id="Plot"></div>
        `
        var Vitesse_Desiree = {
            x: x,
            y: vec_des[train_id-1],
            mode: 'lines',
            name: 'Vitesse Désirée'
        };

        var Vitesse_Reel = {
            x: x,
            y: vec_reel[train_id-1],
            mode: 'lines',
            name: 'Vitesse Réel'
        };

        var data_graph = [Vitesse_Desiree, Vitesse_Reel];

        var layout_graph = {
            title: `Vitesse: Désirée x Réel Train ${train_id}`
        };

        Plotly.newPlot('Plot', data_graph, layout_graph);

    }else{
        removeGraph();
        plotGraph(train_id);
        
    }
}

function removeGraph(){
    var id = document.getElementById("Plot");
    id.parentNode.removeChild(id);
    plotFlag = false;
    document.getElementById("Graph").style.display = 'none';
}

function calculPosition(valeur){
    /*
    B1 = 0 = 17608  : (397, 32)
    B2 = 1671       : (620, 32)
    B3 = 3569       : (807, 158)
    B4 = 5833       : (793, 465)
    B5 = 7450       : (610, 548)
    B6 = 9062       : (396, 548)
    B7 = 11196      : (112, 540)
    B8 = 13031      : (22, 314)
    B9 = 14659      : (30, 127)
    */

    var posX, posY;

    if (valeur <= 2210) {
        posY = 32;
        posX = 397 + valeur*284/2210;
    }
    else if (valeur > 2210 && valeur <= 3569){
        posX = 681 + ((valeur-2210)*126/1359);
        posY = 158 - Math.sqrt(15876 - Math.pow(posX - 681, 2));
    }
    else if(valeur > 3569 && valeur <= 5345){
        posX = 807;
        posY = 158 + (valeur - 3569)*237/1776;
    }
    else if(valeur > 5345 && valeur <= 7111){
        posX = 807 - ((valeur - 5345)*150/1766);
        posY = 395 + Math.sqrt(22500 - Math.pow(posX - 657, 2));
    }
    else if(valeur > 7111 && valeur <= 10835){
        posX = 657 - ((valeur-7111)*497/3724);
        posY = 548;
    }
    else if(valeur > 10835 && valeur <= 12459){
        posX = 160 - ((valeur - 10835)*138/1624);
        posY = 410 + Math.sqrt(19044 - Math.pow(posX - 160, 2));
    }
    else if(valeur > 12459 && valeur <= 14257){
        posX = 22;
        posY = 410 - ((valeur-12459)*240/1798);
    }
    else if(valeur > 14257 && valeur <= 15705){
        posX = 22 + ((valeur - 14257)*123/1448);
        posY = 170 - Math.sqrt(15129 - Math.pow(posX - 145, 2));
    }
    else if(valeur > 15705 && valeur <= 15967){
        posX = 145 + ((valeur - 15705)*35/262);
        posY = 47;
    }
    else if(valeur > 15967 && valeur <= 16394){
        posX = 180 + ((valeur - 15967)*55/427);
        posY = -0.2727*posX + 96.09;
    }
    else if(valeur > 16394 && valeur <= 17608){
        posX = 235 + ((valeur - 16394)*162/1214);
        posY = 32;
    }
    else {
        return null;
    }

    return [parseInt(posX), parseInt(posY)]
}

function UpdateMap(){
    document.querySelectorAll(".posMap").forEach(e => e.remove());
    var mapdiv = document.getElementById("svgDiv");

    for (var i = 0; i < NumberOfTrains; i++){
        if(position[i] != '--'){
            var coorde = calculPosition(parseInt(position[i]));
            if (coorde != null){
                mapdiv.innerHTML += `<circle class="posMap" cx="${coorde[0]}" cy="${coorde[1]}" r="8" fill="${Colors[i]}"/>`
            }
        }
    }

}

window.setInterval(function(){
    $.ajax({
        type: "POST",
        url: "/getData",
        contentType: "application/json",
        dataType: 'json' ,
    }).done( function(data) {
        vitesse = data.vitesseReel;
        vitesseDes = data.vitesseDesiree;
        position = data.positions;
        messages = data.messages;
        x.push(time++);
        for(var k = 0; k < NumberOfTrains; k++){
            vec_reel[k].push(parseInt(vitesse[k]));
            vec_des[k].push(parseInt(vitesseDes[k]));
        }
        PrintMessages();
        setTrains();
        UpdateMap();
        if (plotFlag && document.getElementsByName("checkbox")[0].checked){
            plotGraph(LastPlot);
        }
        
    });

}, 500);

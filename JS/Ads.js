

var IndexActual=0;
var ImatgeActual=0;
var NumImatges=1;

var IndexAnterior=0;

ImatgesAtzar=new Array();
ImatgesAtzar[0]=new Array();
ImatgesAtzar[1]=new Array();
j=0;
ImatgesAtzar[0][j]="http://4.bp.blogspot.com/_f3d3llNlZKQ/S216ENfMWzI/AAAAAAAAByY/ioQI7pn3J5c/s400/Ad-Mino.png";
ImatgesAtzar[1][j++]="http://www.cafepress.com/SiegelDisks#link-productCategory-104";
ImatgesAtzar[0][j]="http://1.bp.blogspot.com/_f3d3llNlZKQ/S22BBqqjvnI/AAAAAAAAByk/YFeuVsYuEkU/s400/Ad-Mug.png";
ImatgesAtzar[1][j++]="http://www.cafepress.com/SiegelDisks.430953729";
ImatgesAtzar[0][j]="http://4.bp.blogspot.com/_f3d3llNlZKQ/S22Dh4WR37I/AAAAAAAAByw/NCJx2hHlEDo/s400/Ad-Sigg.png";
ImatgesAtzar[1][j++]="http://www.cafepress.com/SiegelDisks#link-productCategory-110";
ImatgesAtzar[0][j]="http://1.bp.blogspot.com/_f3d3llNlZKQ/S22G5ICVR-I/AAAAAAAABy8/uLu_AYJ-WJw/s400/Ad-Pad-Note.png";
ImatgesAtzar[1][j++]="http://www.cafepress.com/SiegelDisks#link-productCategory-106";
//ImatgesAtzar[0][j]="";
//ImatgesAtzar[1][j++]="";
//ImatgesAtzar[0][j]="";
//ImatgesAtzar[1][j++]="";



var NumTotal=j;

var IdImatges=new Array();

IdImatges[0]="idImatge0";
IdImatges[1]="idImatge1";
IdImatges[2]="idImatge2";
IdImatges[3]="idImatge3";
IdImatges[4]="idImatge4";


PreCarrega=new Array()

var MatriuNumImatges= new Array()

for (n=0;n<NumTotal;n++)
{
    PreCarrega[n]=new Image()
    PreCarrega[n].src=ImatgesAtzar[0][n]
}
for(n=0;n<NumImatges;n++){MatriuNumImatges[n]=-3;}
for(n=0;n<NumImatges;n++){
    do{	
	Potser=Math.floor(Math.random()*(NumTotal));
 	Potser=(Potser>=NumTotal)?Potser-1:Potser;
    }while(Discrimina(Potser,-3, MatriuNumImatges));    
    MatriuNumImatges[n]=Potser;    
}


function Discrimina(Actual, Anterior, Matriu)
{
    if(Actual==Anterior){ return true;}
    for(i=0;i<Matriu.length;i++){
	if(Actual==Matriu[i])
	{return true;}
    }
    return false;
}

function SetOpacitat(objId, opacitat) {
    obj=document.getElementById(objId)
    if(obj==null){return;}
    opacitat = (opacitat == 100)?99.999:opacitat;
    
    obj.style.filter = "alpha(opacity:"+opacitat+")";
    
    obj.style.KHTMLOpacity = opacitat/100;
    
    obj.style.MozOpacity = opacitat/100;
    
    obj.style.opacity = opacitat/100;
}

function fadeIn(obj,opacitat) {
    SetOpacitat(obj, opacitat);
    if (opacitat < 100) {
	//SetOpacitat(obj, opacitat);
	opacitat += 2
	window.setTimeout("fadeIn('"+obj+"',"+opacitat+")", 7);
    }
}

function fadeOut(obj,opacitat) {
    if (opacitat > 0) {
	SetOpacitat(obj, opacitat);
	opacitat -= 2;
	window.setTimeout("fadeOut('"+obj+"',"+opacitat+")", 7);
    }
}

function initImatge(imatgeId) {
    SetOpacitat(imatgeId, 0);
    fadeIn(imatgeId,0);
}
    
document.write('<div>')
for(i=0;i<MatriuNumImatges.length;i++){
    document.write('<a href="'+ImatgesAtzar[1][MatriuNumImatges[i]]+'">');
    document.write('<img style="padding: 0 0 0 0; border-width: 2 px; border-style: solid; border-color:  #D0D0D0; width: 220px; "');
    document.write('src="'+ImatgesAtzar[0][MatriuNumImatges[i]]+'" id="idImatge'+i+'" alt="idImatge'+i+'"/></a>');
}
document.write("</div>")

    


function CicleImatges()
{
    do{
	IndexActual=Math.floor(Math.random()*(MatriuNumImatges.length))
	IndexActual=(IndexActual>=MatriuNumImatges.length)?IndexActual-1:IndexActual;
    }while(IndexActual==IndexAnterior);
    IndexAnterior=IndexActual;
    ImatgeAnterior=MatriuNumImatges[IndexActual];
    do{
	ImatgeActual=Math.floor(Math.random()*(NumTotal))
	ImatgeActual=(ImatgeActual>=NumTotal)?ImatgeActual-1:ImatgeActual;
    }while(Discrimina(ImatgeActual,ImatgeAnterior, MatriuNumImatges));
    MatriuNumImatges[IndexActual]=ImatgeActual;
    //Ara IndexActual t� la imatge que canviar� i que no coincidir�
    //amb l'anterior, ImatgeActual una imatge diferent
    //window.setTimeout("Aux1('"+IdImatges[IndexActual]+"',"+ImatgeActual+")",1300)

}

// function Aux1(Id, Actual)
// {
//     fadeOut(Id,100)
//     window.setTimeout("Aux2('"+Id+"',"+Actual+")",1000)
// }

// function Aux2(Id, Actual)
// {
//     //alert(Id)
//     elem=document.getElementById(Id);
//     if(elem==null){Aux3(Id);}
//     elem.src=ImatgesAtzar[Actual][0];
//     Aux3(Id)
// }
// function Aux3(Id)
// {
//     initImatge(Id)
//     window.setTimeout("AuxiliarCicleImatges()",1000);
// }


// function AuxiliarCicleImatges()
// {
//     window.setTimeout("CicleImatges()",1000)
// }

//CicleImatges()
//initImatge("idImatge3")
//SetOpacitat("idImatge3",80);
//window.setTimeout('SetOpacitat("idImatge3",10)', 30);
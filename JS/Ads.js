

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

document.write("    <a href='http://www.bookdepository.co.uk?a_aid=mostlymathsdotnet&amp;a_bid=b00da36d'>");
document.write("    <img alt='The BookDepository' height='125' src='http://affiliates.bookdepository.co.uk/accounts/default1/banners/125-x-125.jpg' title='The BookDepository' width='125'/>");
document.write("    </a>");
document.write("    <img alt='' height='1' src='http://affiliates.bookdepository.co.uk/scripts/imp.php?a_aid=mostlymathsdotnet&amp;a_bid=b00da36d' style='border:0' width='1'/>");
document.write('<div>');
    document.write('<SCRIPT charset="utf-8" type="text/javascript" src="http://ws.amazon.com/widgets/q?ServiceVersion=20070822&MarketPlace=US&ID=V20070822/US/rbersblog-20/8001/77788e9c-7652-4ddd-bfe9-37f5c65dff3e"> </SCRIPT> <NOSCRIPT><A HREF="http://ws.amazon.com/widgets/q?ServiceVersion=20070822&MarketPlace=US&ID=V20070822%2FUS%2Frbersblog-20%2F8001%2F77788e9c-7652-4ddd-bfe9-37f5c65dff3e&Operation=NoScript">Amazon.com Widgets</A></NOSCRIPT>');
    document.write('<br />');
    //document.write('<iframe src="http://rcm.amazon.com/e/cm?t=rbersblog-20&o=1&p=12&l=st1&mode=books&search=programming%2C%20gardening%2C%20cooking&fc1=000000&lt1=&lc1=3366FF&bg1=FFFFFF&f=ifr" marginwidth="0" marginheight="0" width="300" height="250" border="0" frameborder="0" style="border:none;" scrolling="no"></iframe>');
    for(i=0;i<MatriuNumImatges.length;i++){
	document.write('<a href="'+ImatgesAtzar[1][MatriuNumImatges[i]]+'" onClick="pageTracker._trackEvent(\'CafePress\',\'Ad'+i+'Click\');">');
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
    //Ara IndexActual té la imatge que canviarà i que no coincidirà
    //amb l'anterior, ImatgeActual una imatge diferent
    //window.setTimeout("Aux1('"+IdImatges[IndexActual]+"',"+ImatgeActual+")",1300)

}


function Amazon()
{
    document.write('<SCRIPT charset="utf-8" type="text/javascript" src="http://ws.amazon.com/widgets/q?ServiceVersion=20070822&MarketPlace=US&ID=V20070822/US/rbersblog-20/8006/0a62f6d0-f255-4123-91ad-3aa1a7c36343"> </SCRIPT> <NOSCRIPT><A HREF="http://ws.amazon.com/widgets/q?ServiceVersion=20070822&MarketPlace=US&ID=V20070822%2FUS%2Frbersblog-20%2F8006%2F0a62f6d0-f255-4123-91ad-3aa1a7c36343&Operation=NoScript">Amazon.com Widgets</A></NOSCRIPT>');
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
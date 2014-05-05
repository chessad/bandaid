<!DOCTYPE html>
<html>
  <head>
    <title>band-aid: a new kind of listening.</title>
    <meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
    <style type="text/css">
      img {border-width: 0}
      * {font-family:'Lucida Grande', sans-serif;}
    </style>
    
    <!-- BOOTSTRAP STYLE -->
    <link href="bootstrap/css/bootstrap.min.css" rel="stylesheet">
    <link href="grid.css" rel="stylesheet">
    
    <!--SCRIPTS  -->
    <script src="//ajax.googleapis.com/ajax/libs/jquery/1.8.3/jquery.min.js"></script>
    <script src="bootstrap/js/bootstrap.min.js"></script>
    <script type="text/javascript" src="common/get_key_with_callback.js"></script>
    
    

  </head>
  <body>

    <div class="container">
    <div class="row">
        <div class="col-md-6">
          <h1>band.aid</h1>
          <p>type an artist's name to begin.</p>
          <p>(if having trouble, <a href="http://chessadecain.com/bandaid/clearsessions.php"> click here</a> to clear the session.</p>
          <div id="player">
              <input id="artist" type="text" placeholder="Enter artist name ..." </input>
              <button id="go" class="btn btn-primary"> Create Playlist </button>
          </div>
        </div> <!-- col-md-8 -->
        <div class="col-md-6">
          <div class="panel-heading">
             <h4 class="panel-title">
             <a data-toggle="collapse" data-parent="#accordion" href="#collapseOne">
                <span id="artist"></span>Artist Biography
            </a>
             </h4>
          </div>
         <div id="collapseOne" class="panel-collapse collapse">
            <div class="panel-body" id="bio"></div>
        </div>
      </div>
    </div><!--row -->
    <div class="row">
      <div class="hero-unit col-md-6">
          <div id="info" class=""> </div>
          <div id="all-results" class="container">
            <div id="results"> </div>
          </div>  
      </div> <!-- col-md-5 -->
      <div class="col-md-6 panel-group well" id="accordion">
        <div class="panel-heading">
      <h4 class="panel-title">
        <a data-toggle="collapse" data-parent="#accordion" href="#collapseTwo" id="songtitle">
          placeholder
        </a>
      </h4>
    </div>
    <div id="collapseTwo" class="panel-collapse collapse">
      <div class="panel-body" id="lyrics2"> </div>
    </div>
    
    
       <div class="panel-heading">
      <h4 class="panel-title">
        <a data-toggle="collapse" data-parent="#accordion" href="#collapseThree" id="songtitle">
          
        </a>
      </h4>
    </div>
    <div id="collapseThree" class="panel-collapse collapse">
      <div class="panel-body" id="lyrics2"> </div>
    </div>
      
      
      
      
      </div> <!--col-md-5 -->
  </div> <!--row -->
</div> <!--container -->
        
<script type="text/javascript">
  
 /* function fetchArtistBio(artist) {
  var url = 'http://developer.echonest.com/api/v4/artist/?callback=?'
  $("#all-results").hide();
    info("Generating content ...");
    console.log("Did this work?");
}
*/
 jQuery.ajaxSettings.traditional = true; 
var apiKey = 'NO_API_KEY';

var embed = '<iframe src="https://embed.spotify.com/?uri=spotify:trackset:PREFEREDTITLE:TRACKS" style="width:640px; height:520px;" frameborder="0" allowtransparency="true"></iframe>';


/*    
    FOR THE RECORD THE FOLLOWING FUNCTION(s) TOOK 96 HOURS TO FIGURE OUT. NOT EVEN JOKING.
*/

function fetchBiography(artist) {
  var url = 'http://developer.echonest.com/api/v4/artist/biographies?callback=?';
  
  var args = {
    'api_key': apiKey,
    'format' : 'jsonp',
    'name' : artist,
    'results': 1,
    'license': 'cc-by-sa'
    
  };
  info("Loading " + name);
  $.getJSON(url, args,
        function (data) {
        var gross=(data.response.biographies[0].text);
       
        $("#bio").html(gross);
        $("#name").html(artist);
        }
);   
 //Missing one of these in other fxn?
  }
  


function fetchBasicPlaylist(artist) {
    var url = 'http://developer.echonest.com/api/v4/playlist/static?callback=?';
    info("Creating the playlist ...");

    var args = {
        'artist': artist,
        'format': 'jsonp',
        'api_key' : apiKey,
        'bucket': [ 'id:spotify-WW', 'tracks', 'id:musixmatch-WW'], 'limit' : true,
        'results': 12, 
        'type':'artist'
    };
    
   
   $.getJSON(url, args,
        function(data) {
          var titleArray = [];
          var musixmatchArray = [];
          
            info("");
           // $("#all-results").show();
           //$("#all-results").hide();
            if (! ('songs' in data.response)) {
                error("Can't find that artist");
            } else {
                    var tracks = "";
                    for (var i = 0; i < 12; i++) {
                          var song = data.response.songs[i];
console.log(data.response.songs);
                          var tid = song.tracks[0].foreign_id.replace('spotify-WW:track:', '');
                          tracks = tracks + tid + ',';                    
                         
                          if (song.foreign_ids[0]) {
                            var ics = song.foreign_ids[0].foreign_id.replace('musixmatch-WW:song:', '');
                          }
                          else {
                            ics = "Lyrics unavailable";
                          }
                            musixmatchArray.push(ics);
                         
                          $("#songtitle").html(song.title);
                            console.log(song.title);
                        }
                        
                        var tembed = embed.replace('TRACKS', tracks);
                        tembed = tembed.replace('PREFEREDTITLE', artist + ' playlist');
                        $("#results").html(tembed);
                        $("#all-results").show();
                        
                       
                      
                  }
                  //console.log(musixmatchArray.length);
                 /*Begin musixmatch API*/

                          
                    var mmArrayLength;
                    for (var i=0; i < 12; i++) {                      
                      var url2 = 'http://api.musixmatch.com/ws/1.1/track.lyrics.get?callback=?';
                      
                      var args2 = {
                        'format': 'jsonp',
                        'track_id': musixmatchArray[i],
                       'apikey' : '',

                        
                      }
                      
                    $.getJSON(url2, args2,
                          function(data){
                            if ( 'track_id' == "Lyrics+unavailable" || 'track_id' == "Lyrics unavailable") {
                               //console.log("Lyrics not found :-(");
                               $("#lyrics2").html("Lyrics not found :- ");
                            }
                            else if (! data.message.body) {
                              //console.log("Max rate limit reached");
                              $("#lyrics2").html("Max rate limit reached");
                            }
                            
                            else {
                              var songlyrics = (data.message.body.lyrics.lyrics_body);
                              songlyrics.replace ('\n' , '<br / >');
                              
                              
                              var songcopy = (data.message.body.lyrics.lyrics_copywright);
                              $("#songtitle").html(song.title[i]);
                              $("#lyrics2").html(songlyrics);
                              $("#copy").html(songcopy);
                          //console.log(titleArray);
                          console.log(songlyrics);

                         // console.log(songcopy);
                            }
                        
                          }
                      
                      )  
                    }
                                          
            },

            function() {
                error("Trouble getting blog posts for " + artist);
            }
        );
  }

function go() {
    var artist = $.trim($("#artist").val());
    if (artist.length  > 0) {
        var relevance = $("#relevant").is(':checked');
        fetchBasicPlaylist(artist);
        fetchBiography(artist);
    } else {
        info("Type an artist name first");
    }
}

function info(s) {
    $("#info").removeClass();
    if (s.length > 0) {
        $("#info").addClass("alert alert-info");
    }
    $("#info").text(s);
}

function error(s) {
    $("#info").removeClass();
    if (s.length > 0) {
        $("#info").addClass("alert alert-error");
    }
    $("#info").text(s);
}
    


$(document).ready(function() {
    /* fetchApiKey will fetch the Echo Nest demo key for demos
     hosted on The Echo Nest, otherwise it fetch an empty key */
    fetchApiKey( function(api_key, isLoggedIn) {
        if (!api_key) {
            api_key = 'MY_ECHO_NEST_API_KEY';
        }
        apiKey = api_key;
        $("#all-results").hide();
        $("#go").click(go);
        $("#artist").change(go);
    });
});

</script>


  </body>
  
  <footer>
        <!--Brutus' Betrayal (Tracking URL scripts for musixmatch) 
   <script type="text/javascript" src="http://tracking.musixmatch.com/t1.0/T6ryH1zjuTxolaPXL6pXVUOEtedou+mi9932mZeCsv79Ty9VQzqxh3dr0QuPlzHRN3AatqxkYznDcxCI0H/SuT+3lwYDRBOYtDnLx8wHCcQj32ptdtybdZgb2lAuE08VPtfM2xtH4gCJBiJ6QlHIEGztCoF6XcFe2JhQIa2/JSDK47Jg6i86IVtYfN2OmGh9yLOzxQr00XuUx9fBOcAv93ubNeh7ky3KV7iWhVAAQNjtSShcQV5oLAKSskurljwRv8FKrdpC4/6J9Ffg6Px5rTvuUCq5TJ/5PeMHeTKzHVrBtETM+tgOHOW1L4Z0DrXoQ97hEZh9mKDuOWfrlWBT8cR+hvC5rHA5LwbQmJKmYyWr23ElVRAVzZDDABtSbkED/"
></script>
   <script type="text/javascript" src="http://tracking.musixmatch.com/t1.0/uEhc5PM3UJ4xB/EeYosPr4g3RTFkN4oWi8Bb2b1tFu9WgJafdfPlY0eaib6WLh0TsA/Uy9aS/MZI6kPXKJu8Ec2rtgAPBc1YqtdgJm4/o4/0Hhb1juKu1UL8LwenhCUc1MZ5uUcb2XosfgqtyPrVC988nt1eyNcwfY2+TUL/yDBmc/h0wy5d4oRA1h/0k603VPF+d0K14lTlWEcSwVkaONzyt7KZvCnTuOo4hgYzrhP+Ps/5IJk5Lrdn0E7Vt+EAfYGGcVu+207E+a0qaf1kCqxVYlTQb6zxBJGxX2vmOHp+OSnoQQuHZl3Bv+TQw4iZix+8FiVDZLv8YtoJYF+AlS/go8l/jH1374Vf1cFcLWBCeBeIVMWBSwOCtAlLBY2fBMtiPAYJ46aeI3B84NaGxXsBbmAxPiqt+JLDAQeYYKk=/
"></script>
   
   <script type="text/javascript" src="http://tracking.musixmatch.com/t1.0/1FEnDkW1vfJnXZI7n/gMlYgqJiiKxkW9YbmQTPCFj5nnGrBeG4VhsNQ/ik8cZtn4FWRmqaf///iR45mvk0tt8DHfNDR7GbhhokqqzjoEeDr/jzPC7ca/cFFMEf3Lu+ipJaNviitZZx9EODQhlNKQZiz8Ma675pnJay6geZfP74C/IjhUNV3PmMncLo6DhGb62oA1XT5IFdidoJHjcsNgFdalx+CKwnA+yIlBnSnTpm87WjiW+6PjGCBR2T+GlxS+wYBEqteirEdi0UsJXeflxFRysxJPY8X7YvIOFuY0KG9KnBM9pDzNYFQdHu2FKKXfNGiUSp8wOO4bceVXpbS1/gfGH5greQ+csMF/gl+AXu2WZtkCGXY3gl8MF6G1p19R/
"></script>
   
   <script type="text/javascript" src="http://tracking.musixmatch.com/t1.0/VCjUZPjUKXzB5HnxANO2lC+atBq1ifk/abP2zFg2WmovDkb7sUQX/+CYGavUTbqUUg4SteF1kqLHy7cSp1L3EAWJbwVCJQgYV+nlKG0IL/enU3MOQX+gW94Z+2o3m3fOUE3VWH8gvZLFDhIZrWAEkNR2J1MfbZz0oVyDSY7P8p1E+8MGSX7QDE6Zv+KAMqt7p3ZT8HwiHzmKmBgI6akLDUgBbS1LIuXwvQmSABSFdU10tcoIxuG4jhm6WtvmqaDldsVfkA570VhgNPtU/9QBq7d48iK/xFTWoe86Eb3/tphwi9WcEWAbLByuqWm36lgK0qMRiko2iXk6sP4sl3sg2VEhdR5Kgi8R/vOu+i3xnNLvcqZt6Qekse3VONPOuJYO/
"></script>

  <script type="text/javascript" src="http://tracking.musixmatch.com/t1.0/UdQIC+RjHzp22XL1JbOb8xG7HzVS8VKEUacvytCyJZTI+zMRyVrxIiYsfQOXUrcFloDLqGtHzo+gdeKcax1piZlIdpJ4AiUaySTimVQBPtYI+hSdI6G6ekT5qc+mXqvuPREW1UksA1W9tI52LAOUYmjUuPTS5kBKTSi8x7TFuKVajCf7DAIBy4uRB0vFy3qnxLyqkYjw9Tgcxfeom5npQvkE00jaz0nRhjE+eGeyMYLfkQd1hWAaUR0/A7A7+z/+J6tY31vKvfl67AjF1A/FN8BIG/4PY8fuXMH4xt3hMUVUAoqe6NP1So9ckchak3OsbIlRTCg+BZG5tRKHNNBAHC1jKCdJGqEhgXDdn0AxRcRLemFQVtRFGhJpfTGU7NHc/
"></script>
  
  <script type="text/javscript" src="http://tracking.musixmatch.com/t1.0/G2RXyMdGzeeUTH6Z0rxvGfQZacVaQswhM3o4+VOf08LDsGVqSVv/omcBIBzyKXazGAoDcIX2TytzqPUKwJuV6QibVSDBc9ejlcyYV/sMMfh0twLpKjr9lpnmI8PSs8SwfNKOU7O5CxiYHuMmbHVfsZmpwQw5xEwDraTgrtXUfEiwFQLWsFHz8kxjKKK97oZRzvFaPTCVsv0+nenpFTs+QfrI/q7ulVoi5mxGhsKfR/Uz1XmIv7WfC9UtxGAaRE1hfSGveD50b9LffvL9UgqMCaUb+nQQed6zW5/EmXtn5Dcnw62kxTcu/90GPI2HZcBbP/iODbqnTwqHglXtZST1ovtEEJUIbawFrQk9wqDbtOrKKe0zmh2ktP5Yc07KqyJ8/"
</script>

<script type="text/javascript" src="http://tracking.musixmatch.com/t1.0/f1F6p83W/i8cEdpl6B3uOOkj2z1Vh46DFaPLjapp3Srec1VIk9CAfnU8/J/eDCAXyE+CtXEMaL46Z06O4bFzkahsWtKoV2NqK2LqU8bJ7JEkSICbyDsYy2o3QzFEyxzfqvqIZPqN8QpGLE2OcWMTlslEHuNEi/Y3ckf+evoTpZ1YkXzL+fBRvF6ogFdy2mBCeAFox20aTftgXBX5V9y8HIKstlhmNi/1fqrTcDfWVe9Rv58O68uWX94Ge33GYJS/djuSM/GGYb7IDEBxCGJ+wJGI05yzYSqNoRZxBcDx8kEcVBvyefC5VxKeTiIiqqRBKs5XApdt4/x/f4+GkAfp8Lk+ZDyDGcGwFwZxMK0ZU2EtV104WBbJY739h1jq7lb3/
"></script>
<script type="text/javascript" src="http://tracking.musixmatch.com/t1.0/V6El57rZoo1qBz3wTBBoy67T6GVcNqhscCkMmTHjDolbOgxnPq8rCxn7J5B0RG8FXPljZD1I3qYGpSx26/3Xzuyb06r9zWOM3tnXmvjs4Ypew0Crdl5gCmaKM94NEDD7rPI/C3y5octKE+zzrS6faOIzZ+6/hS5nehFupWtEBS6P55ZnsoIgTezA7G6+zY025Fzammiv35L+p4tQqtkyXGoMG0XPfyOTmDCevW/d/lDSs5DLFk/UKRespaWXhB04L+vWa+nJwT0Ew2lqMypRsqS3tFwWOrr/x2/ytVpmcXBhtysz68ZBzZCAbFS+H0ZEie8sakWx+LBEX9n2Mu+dwY6DqtORpjXmewygrE6yEwDsIE+3pCbZNydxLag8UR4t/
"></script>

<script type="text/javascript" src="http://tracking.musixmatch.com/t1.0/jpyjnQfiSi7R7nkjE94EqAeQfMLwTe5Y1rJa16WtNtbDy20pE11FBL6OrSES/OOgVQBDr8fF3iEBFCQUJmjXKS54EiguVTxDkBW3H8KwOmaZ1g27Uj9edw0j7u0ash5YiPLCJjJXrE723rtVGaAo1CAGYVy6syDzKnVp6gR31sRlBsuYfnkb7/k0ClMPQvx1btvU2e9waI4khYkFRjuBXnLAHvp3zppml+Akfw6PD5PuvdPO7qNsICQRK43VqiECsaCoV4MkAFcexhsPxtz1ACmRbn/zPA8w7PVtIHaA3A1Um9rhuGBuXMSEAncn2e2HHp9wzXKu8s2dGEYVhOGfWr622GeoMcOI3TG3UoNv/WPFQQnFrRvdN75fl950iZTxJWGga5XutYmU7e5wPphZ99+LXPsWBX583FXNuRpm5/0=/
"></script>

<script type="text/javascript" src="http://tracking.musixmatch.com/t1.0/5i0KEuiSrYWKSZoeO0bqHpfKHokQ0TL3y7UnWdNaZvBAdZqlG32iB9eL3vsKHmBBzcn+09UpEdMu1HJh/f28bVMB3f2dREZ5Of5283EYXg9MUI3VneBKX4C0uRJIRpxGBuU26UpIELByw988dmlrKbFPwl7K6hy/dwFYIa0CxMQpcudHzCoemanletZFNsPLF+FiX4faHu8HRK5aT7/Wf7iv8CerXmyF/s2QfvyhflgTbEnr8Dj+YynwG58GzyJSV9Que3Whw8FZwTV2gHC4LHQzGkaA4EtKuyDJVFRW+eEKGUjcNi6/t3XO+2OQIrWXkDgrc/n4+he1HM/DiMbNK/fwGwwOD/2UJgSDDytz0TkbLL7MV9H9M/39rMpC0EW2/
"></script>

<script type="text/javascript" src="http://tracking.musixmatch.com/t1.0/xR5IvDd6G/QnpcJvq+Nu7EhgiU0uo2Exzyn+r4MLrLfDqvHBzTWMOMBZba0AGCys+vt/LsKZkF+XCnExj/VgDCn7X9yWSoHL9pb4MvtxA+j5CkKGzDy7b58gvvyglIpMOeTX6reJ85RrV2yUh9CT9vssvcBj6GW9NZ6hoKhmotVDjmMedDASKWt5esGtYKik9gC6NxKibM0IYbL7cnU0lQi5FE0hfUA6wTVtM9AC5vAFv6kBHVsU7QNViW3uweqHVbodcbsip2gNnubn5A0sv/J8sM+YdV7V0KF6RsCJmFV/aGFwT5xvAajbonTpU4R4cfuG4c0VNghPkg4yn9aAFGLsinOsEkbUvNpEToHdSE9XEulCXwDZ0mOX6yVtuZ+u/
"></script>

<script type="text/javascript" src="http://tracking.musixmatch.com/t1.0/wSMsda+Ns8JpnCj2x0CdrmDAoyjQ57udgRQtjXTSRjPljsh54rOZW6bo2cnPUkikGaIV8yFqFyVUEyEalXn3CITsJC9iQ+j/lMyS03xhSTECUAlokyEtUfYmb7R2l/lXQWnyWSXlEM/4XH24aTbFo2FqOAqxg9vINzGjElJwShud29gcxMiwiXb05taXJQlvrLTtCPS09G/Xkb/WC6y6HaXxxgJmfeLRMHLmKD/j1YMmBFBjVSg2Wbg5gi8JHwvE8E7UoQQBGdu8LRK0h+1JmlB/gtAzNg65as2rcFlhdK/bKyuYSPtSdQG/C4lRjT1HjfYwROHqL/5b7VbUgN/z06PS7B0PUue1Txrw6JTyw42NsUhfCH8TXxm1ES7hvjXa/
"></script>

<script type="text/javascript" src="http://tracking.musixmatch.com/t1.0/cDjvsfD9Bj/O9eY/0Orq2bk+XerxPIpmKsEiyYAne+18VlCGCVrHfcl1UDXGyGxNeL9LnUf45/RWeYmuvXl0li2lxBcQhWb0bIGNV0KIGVwJ/lPlGbPyQJGX2XF8EIcLJvEQAh8tNfhSUMV+yIPCEgdjWiDDq0qZDYFowZZ+ndVCk1bYPWeN14gG0iJs/lL9DYXKcPGHliu5brbjJe0bezHr435VFwXUKDS7Wly4duzTFsLgxLwU+xBJgAWuFMKc3ItAWQM4T0W9r69McGQLeaGKfxvV+knXnk9cwX7Yv6W+bUKK2cde1HN/jPd4BoM5/4arzwhVwSANyfILAu2ygBsCXxY5xHsMTn+JiagO/uMYnp1qcffqZl9XOTjN+Tny/
"></script>

<script type="text/javascript" src="http://tracking.musixmatch.com/t1.0/K7nJbqXmWEky/oxqL1TVXrpbwxo3agtcPomkpTp5h0VHvbsVn4rcYrUWyJqMTle1OcvTwgaXyAW2qKoqYujHNl3XFfuIgMaDU419A9P5UWrRe02Mlo93oLN4gCAlOow/UtKjxaJ+C5FLHSiusAS/iM+xusiIbWl+0H0jgVbXjbJVRDRTVckDFRiLKwB4EHoJkLTuReq020/kloE0RTlIS0/F0Jwmiq+6cpQ5fyBYTDpyDN364Efa3gcCNO3aestn2LFI4ul2Y9HwxCwJV3yNZOatsjuOKsr542DRRztwOx3Erzc+qtbohrVJCcROdaDpvLQPzyA5/kB3zlVc/9g4ngmuJAtu5YmzCYOsSXvnr6Hn9BizBq4BiLja/I8SMKLK/
"></script>


<script type="text/javascript" src="http://tracking.musixmatch.com/t1.0/7vOJf6sBR6haOCz9d/XAP53Bw8KFQs3A/HpWH/ZtJnwPwz1Xa4DiWQWlueZtgEQRaTadVO5r/ZXgb3wnboyWN67fE+rb9mR7j4T0I/8rDcNg0VYUNO7xj1zl4KprB27yLdcIe3urDGv/BHr1VAfJLaVRW6nNeiEen3f9yV3adBWPGuhlSLYfgCXkwDceaFJPfXu4oLHOz37gmEg5gY4iwcqIH8FEKXcfbpBxUqW9yfpw0SkYWzz1AWuilRsk9k2FQi4mUvQ/waAgifoJdN+My4m5INPIQqzFDMxgoPMl3ipxo1PuMKiMD+IGynmwVPq+M6NTHGmPzjO7YioVB3l3IU7FIxIk1pL0ZTfNUM9YpqG0omTgSvXNEgXPeA7HxlV5/
"></script>

<script type="text/javascript" src="http://tracking.musixmatch.com/t1.0/ejSdvzf+BWdLv8rpWb+FoqqnR3RfnKJKGo0hUQl25YMC82ZltuZEO4N0kIrBDgRq3DfjxsDC0beiPmDn5Vx8dWwsm2JGMsR4ga4ANcr7mkRLiLIFk2OBn27xSidDXnb1OX3Pe2LztvbiKD4TwP08qvFyp1bxn3H1xeNPM+/RzHB5THtqSX9TydGTczcmBfPADX9F5+6apcuSLV1zmkUofUR4MfFHKEZbWepIiHYLD+M5FbQXK1o93v83EEIi+WZK8wM8+tDxF/CFxvTvwatNvPeU1aQYyd/35yEwNYEhz3i0iguSM/pfpGcihQJlGs5YYdi/fElu4dyjHtDHKNPYK4WxpFDo5IWLflghjlw9S+0nLcecqVvygZ+KBC2txxvq/
"></script>

<script type="text/javascript" src="http://tracking.musixmatch.com/t1.0/60wJFzeTPFUVpBBBswMuX4H4uFZvRSM4mv6PwKRtHkl6jsDaytjZDxMqtYGj9FvzBBg+hA1qYb9nwRwGzSSoQK8WTKJ6b5k60d/E+hx6KWQ+WpPn2RNE6dkSDr9ZREGbs6B1Trn36v+1PZaTbRc1l7ixTysOM25l79fquWUk8HQWNlUv91TT1kGBHiYNC24KO0LsCHp/0OjU4k2UOkkDn2xhB8tnJCUv0hmQvw3qMc4LmTxoA1yU33LVdjxn5KRonl+eLWsnoCH0SFl3Gj3UcJCe4ijz155pzKO/U8z+C3EiKFIKfA1oMl4WJqQtAy2oZYSYJOika6KPuFZIxkzO6Gs8OYjkJ3serWBNi7ss2yL9spduoKeYqjLIUkrcb0px/
"></script>


<script type="text/javascript" src="http://tracking.musixmatch.com/t1.0/X+LRiMJEfvoEDxVxc2HtQisrviifQH5TFjuQT40zI6NlK2HJtXAwQMYLAorllUoe/3PObomYEMdaGqGD1dZzZ46ZUDI3IsboWZTWmQdPcEwtz9BVaC3dsoIfycNnnMf7N+J7vMct7LUgkXom2zh/zWAbRAWlPIxSW6oMBr40FmV8m9PoarM9czOJLzYVItbgXdylRPp8WzhQ5at/PIEnQITXRhQ3mYiMWzMly1fVRUAzxDuUXWSlYtTdlLyZxATwwG/xTD7Y9M2Qs/pz5QJ6SInPinyQNPp8N3dNbGK5nq3fXvL7/FLxHENcismI4SmE7SNR2xxsQcdGZHE5OiB5hc4OEqm0Mc4sqQ0K+q2AyINbbxy43lupCJkdb1aDt/xn/
"></script>

<script type="text/javascript" src="http://tracking.musixmatch.com/t1.0/759Wl1aP08/TIu/XLYjBESNim/l7RHcBpeovypTTvS2VWgeUz/RZcbQRua2+WLIox1CGZJ361UTPkTlM47da5PGHjvGgSSOUW/ivxnyq5YjL78erPNuTm/sBAU7YK7e7uiDVBRaW+2dvzbEOn0ZTlTzCiqSmXZm5rpQPBgMNyggoB3X08o4/8Sbuh/pqnnXceFvo5eHE+4SM8Rk3JWec+qxZFcc0oELR4QFQkFrlYxSFeWsoebNj5JPYmnK5bGOl42iLi9lRTGshwR3t58tNk4Mf2kFLMMypHi3ufIKo93x/3+xarxS6tmSn6AZ4CC+/aBKiImnXz6DGYPNjI16fj2f+Tej90kw97XNTEffxYFjElzTXwEqF6yYBeoj6dbAI/
"></script>

  -->

  </footer>
</html>

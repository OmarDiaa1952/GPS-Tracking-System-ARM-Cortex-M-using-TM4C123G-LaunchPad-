<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <link
      rel="stylesheet"
      href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta3/dist/css/bootstrap.min.css"
    />
    <link rel="stylesheet" href="style.css" />
    <title>TivaC GPS Tracker</title>
  </head>
  <body>
    <div class="container">
      <div class="row">
        <div id="header" class="col-12 my-5">
          <h1>TivaC GPS Tracker</h1>
        </div>
      </div>
      <div class="row">
        <!--Tracker-->
        <div id="tracker" class="col-md-7 my-3">
          <h2>Tracker</h2>
          <div class="row">
            <div class="col-lg-6">
              <div class="my-3 text-left">
                <label class="form-label">Latitude : </label>
                <label id="lat" class="form-label">NULL</label>
              </div>
              <div class="my-3 text-left">
                <label class="form-label">Longitude : </label>
                <label id="log" class="form-label">NULL</label>
              </div>
              <div class="my-3 text-left">
                <label class="form-label">Speed : </label>
                <label id="speed" class="form-label">NULL</label>
              </div>
              <div class="my-3 text-left">
                <label class="form-label">Total Distance : </label>
                <label id="distance" class="form-label">NULL</label>
              </div>
              <div class="my-3 text-left">
                <label class="form-label">Total Time : </label>
                <label id="time" class="form-label">NULL</label>
              </div>
              <div class="my-3 text-left">
                <label for="rr" class="form-label">Refresh every </label>
                <label id="rr" for="rr" class="form-label">0 seconds</label>
                <input
                  type="range"
                  class="form-range"
                  min="0"
                  max="8"
                  id="rrs"
                  onchange="changeRS()"
                />
              </div>
              <div class="my-3 text-left">
                <button
                  id="toggle"
                  class="btn btn-danger activeHover"
                  onclick="toggleRefresh()"
                >
                  Stop Tracking
                </button>
              </div>
            </div>
            <div class="col-lg-6 py-3">
              <!--Google maps-->
              <iframe
                id="map"
                width="300"
                height="250"
                frameborder="0"
                style="border: 1px solid #000"
                src="https://www.google.com/maps/embed/v1/place?key=AIzaSyAim5J7NUfpZF2AIVhdUsCd4aYux5hDNgs&q=30.064561198816353, 31.278810512942904&zoom=19"
                allowfullscreen
              >
              </iframe>
            </div>
          </div>
        </div>

        <!--Controller-->
        <div id="controller" class="col-md-4 offset-1 my-3 px-4">
          <h2 class="mb-5">Controller</h2>
          <form action="commands.php" method="POST">
            <div class="my-3">
            <div class="row">
                <label class="form-label">Start Point</label>

                <div class="row">
                  <div class="col-6">
                    <label class="form-label">Latitude :</label>
                  </div>
                  <div class="col-6">
                    <input
                      name="latStart"
                      type="text"
                      class="form-control"
                      id="latStart"
                      placeholder="ex: 30.065391"
                    />
                  </div>
                </div>
                <div class="row mt-2">
                  <div class="col-6">
                    <label class="form-label mt-2">Longitude :</label>
                  </div>
                  <div class="col-6">
                  <input
                      name="logStart"
                      type="text"
                      class="form-control"
                      id="logStart"
                      placeholder="ex: 30.065391"
                    />
                  </div>
                </div>
              </div>
            </div>
            <div class="mb-3">
            <div class="row">
                <label class="form-label">End Point</label>

                <div class="row">
                  <div class="col-6">
                    <label class="form-label">Latitude :</label>
                  </div>
                  <div class="col-6">
                    <input
                      name="latEnd"
                      type="text"
                      class="form-control"
                      id="latEnd"
                      placeholder="ex: 30.065391"
                    />
                  </div>
                </div>
                <div class="row mt-2">
                  <div class="col-6">
                    <label class="form-label mt-2">Longitude :</label>
                  </div>
                  <div class="col-6">
                  <input
                      name="logEnd"
                      type="text"
                      class="form-control"
                      id="logEnd"
                      placeholder="ex: 30.065391"
                    />
                  </div>
                </div>
              </div>
            </div>
            <div class="mb-3">
              <input
                name="update"
                type="submit"
                class="form-control btn btn-primary"
                id="set"
                value="Set"
              />
            </div>
          </form>
          
            <div class="mb-3">
              <button
              id = "getData"
              onclick="getCoords()" 
              style="width:100%;"
              class="form-control btn btn-success"
              >
              Get
              </button>
            </div>

            <div class="mb-3">
              <button
              id = "forceReset"
              disabled
              onclick="forceReset()" 
              style="width:100%;"
              class="form-control btn btn-danger"
              >
              Force Reset
              </button>
            </div>
          
        </div>
      </div>
    </div>
    <script
      src="https://cdnjs.cloudflare.com/ajax/libs/axios/0.21.1/axios.min.js"
      defer
    ></script>
    <script src="main.js" defer> </script>
  </body>
</html>

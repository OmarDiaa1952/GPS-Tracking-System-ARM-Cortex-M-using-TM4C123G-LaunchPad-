const latLabel = document.getElementById("lat");
const logLabel = document.getElementById("log");
const speedLabel = document.getElementById("speed");
const distanceLabel = document.getElementById("distance");
const timeLabel = document.getElementById("time");
const iFrame = document.getElementById("map");
const rr = document.getElementById("rr");
const toggle = document.getElementById("toggle");
const rrs = parseInt(document.getElementById("rrs").value);
const url = "http://megz.125mb.com/commands.php";
let active = true;
let rs = (rrs + 2) * 1000;
rr.innerHTML = rs / 1000 + " seconds";
const iFramePart =
  "https://www.google.com/maps/embed/v1/place?key=AIzaSyAim5J7NUfpZF2AIVhdUsCd4aYux5hDNgs&q=";
let inter;

const getData = () => {
  axios.get(url + "?reading").then((res) => {
    const data = res.data.split(",");
    console.log(data[0]);
    if (data[0] == "0") {
      latLabel.innerHTML = "NULL";
      logLabel.innerHTML = "NULL";
      speedLabel.innerHTML = "NULL";
      distanceLabel.innerHTML = "NULL";
      timeLabel.innerHTML = "NULL";
      return;
    }
    console.log(data);
    distanceLabel.innerHTML = parseFloat(data[0]).toFixed(2) + " m";

    latLabel.innerHTML = data[3];
    logLabel.innerHTML = data[4];
    let speed = parseFloat(data[9]);

    const lastTime = data[5] - data[8];
    speed = (speed / lastTime).toFixed(2);
    const speedInKm = (speed * 3.6).toFixed(2);
    speedLabel.innerHTML = speed + "m/s (" + speedInKm + ")km/hr";
    const mapUpdate = data[3] + "," + data[4] + "&zoom=19";
    iFrame.src = iFramePart + mapUpdate;

    let time = (data[2] > 0 ? data[2] : Date.now() / 1000) - data[1];
    time = Math.floor(time);
    inter = setInterval(function () {
      time++;
      let hours = Math.floor(time / (60 * 60)) % 24,
        minutes = Math.floor(time / 60) % 60,
        seconds = Math.floor(time) % 60;
      `${
        hours < 10 ? "0" : ""
      }${hours} : ${minutes < 10 ? "0" : ""}${minutes} :${seconds < 10 ? "0" : ""}${seconds}`;
      timeLabel.innerHTML = hours + ":" + minutes + ":" + seconds;
      if (!active) clear();
      if (data[2] > 0) {
        clear();
        active = 0;
      }
    }, 1000);
  });
};
const clear = () => {
  clearInterval(inter);
};
const changeRS = () => {
  const rrs = parseInt(document.getElementById("rrs").value);
  rs = (rrs + 2) * 1000;
  const rate = rrs + 2;
  rr.innerHTML = rate + " seconds";
};
const toggleRefresh = () => {
  if (active) {
    toggle.classList.remove("btn-danger");
    toggle.classList.remove("activeHover");
    toggle.classList.add("btn-success");
    toggle.classList.add("inActiveHover");
    toggle.innerHTML = "Start Tracking";
    active = !active;
    clear();
  } else {
    toggle.classList.remove("btn-success");
    toggle.classList.remove("inActiveHover");
    toggle.classList.add("btn-danger");
    toggle.classList.add("activeHover");
    toggle.innerHTML = "Stop Tracking";
    active = !active;
    refresh();
  }
};
if (active) refresh();

const getCoords = () => {
  axios
    .get(url + "?startEnd")
    .then((res) => {
      console.log(res);
      const data = res.data.split(",");
      document.getElementById("latStart").value = data[0];
      document.getElementById("logStart").value = data[1];
      document.getElementById("latEnd").value = data[2];
      document.getElementById("logEnd").value = data[3];
    })
    .catch((err) => {
      console.error(err);
    });
};

function refresh() {
  console.log("Called");
  if (active) {
    getData();
    setTimeout(refresh, rs);
  } else {
    clear(inter);
  }
}

const forceReset = () => {
  axios
    .get(url + "?forceReset")
    .then((res) => {})
    .catch((err) => {
      console.error(err);
    });
};

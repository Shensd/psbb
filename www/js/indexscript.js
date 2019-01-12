const HEIGHT = 32;
const WIDTH = 32;

let canvas, ctx;

let init = function() {
    canvas = document.getElementById('canvas');
    ctx = canvas.getContext('2d');
    canvas.height = window.innerHeight;
    canvas.width = window.innerWidth;
}

window.addEventListener("DOMContentLoaded", ()=> {
    init();
});

window.addEventListener("resize", () => {
    canvas.height = window.innerHeight;
    canvas.width = window.innerWidth;
});

let previousX = 0, previousY = 0;

window.addEventListener("mousemove", (e) => {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.save();

    let currentX = e.clientX;
    let currentY = e.clientY;

    let centerX = (currentX - (WIDTH / 2));
    let centerY = (currentY - (HEIGHT / 2));

    let dX = currentX - previousX;
    let dY = currentY - previousY;

    let tempDX = dX;
    let tempDY = dY;

    if(dX < 0 && tempDY > 0) dX = -dX;
    if(dY < 0 && tempDX > 0) dY = -dY; 
    let deg = Math.atan2(dY, dX);

    let threashold = 7;
    if(dX < threashold && dY < threashold) deg = 0;
    console.log("(" + dX + ", " + dY + ") : " + deg);

    let size = Math.max(dX, dY);

    let offsetX = 0, offsetY = 0, boxWidth = WIDTH, boxHeight = HEIGHT;

    /*
    if(dX > dY) {
        offsetX = (dX > WIDTH) ? dX : 0;
        boxWidth = (dX > WIDTH) ? dX : WIDTH;
    } else if (dY > dX) {
        offsetY = (dY > HEIGHT) ? dY : 0;
        boxHeight = (dY > HEIGHT) ? dY : HEIGHT;
    } 
    */

    ctx.translate(currentX - offsetX, currentY - offsetY);
    ctx.rotate(deg);
    ctx.fillStyle = "red";

    ctx.fillRect(-(WIDTH/2), -(HEIGHT/2), 64, boxHeight);

    ctx.restore();

    previousX = currentX;
    previousY = currentY;
});
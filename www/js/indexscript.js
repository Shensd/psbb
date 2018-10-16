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
    let deg = Math.atan(dY / dX);
    let threashold = 7;
    if(dX < threashold && dY < threashold) deg = 0;
    console.log("(" + dX + ", " + dY + ") : " + deg);

    ctx.translate(currentX, currentY);

    ctx.rotate(deg);
    
    ctx.fillStyle = "red";
    let width = (dX > WIDTH) ? dX : WIDTH;
    if(deg < 0) width = -width;
    ctx.fillRect(-(WIDTH/2), -(HEIGHT/2), width, HEIGHT);

    ctx.restore();

    previousX = currentX;
    previousY = currentY;
});
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

    let currentX = e.clientX;
    let currentY = e.clientY;

    let centerX = (currentX - (WIDTH / 2));
    let centerY = (currentY - (HEIGHT / 2));

    let dX = previousX - currentX;
    let dY = previousY - currentY;
    
    ctx.fillStyle = "red";
    ctx.fillRect(centerX, centerY, WIDTH, HEIGHT);

    ctx.beginPath();
    ctx.moveTo(currentX, currentY);
    ctx.lineTo(previousX, previousY);
    ctx.stroke();

    previousX = currentX;
    previousY = currentY;
});
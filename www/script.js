window.addEventListener("DOMContentLoaded", () => {
    window.setInterval(() => {
        let spinner = document.getElementById("spinner");
        let d = new Date().getMilliseconds();
        let x = (Math.sin(d)*100 + (window.innerWidth / 1.5));
        let y = (Math.cos(d)*100 + (window.innerHeight / 2));
        spinner.style.left = `${x}px`;
        spinner.style.top  = `${y}px`;
    }, 100);
});
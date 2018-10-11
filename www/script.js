window.addEventListener("ready", () => {
    window.setInterval(() => {
        let spinner = document.getElementById("spinner");
        let d = new Date().getMilliseconds();
        spinner.style.left = Math.sin(d);
        spinner.style.top  = Math.cos(d);
    }, 100);
});
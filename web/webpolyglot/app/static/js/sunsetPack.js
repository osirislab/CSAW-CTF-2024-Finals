// sunsetPack.js
document.addEventListener('DOMContentLoaded', function() {
    document.body.style.backgroundColor = "#FFEDD5";
    document.body.style.color = "#3B1C1C";

    // Header
    document.querySelector("header").style.backgroundColor = "#FF4500";
    document.querySelector("header h1").style.color = "#FFFFFF";
    document.querySelector("header p").style.color = "#FFE4C4";

    // Links
    document.querySelectorAll("nav ul li a").forEach(link => {
        link.style.color = "#FFFFFF";
        link.style.fontWeight = "bold";
    });

    // Main Content
    document.querySelector(".post-detail").style.backgroundColor = "#FFAA85";
    document.querySelector(".post-detail").style.borderRadius = "8px";
    document.querySelector(".post-detail h2").style.color = "#3B1C1C";
    document.querySelector(".post-content").style.color = "#3B1C1C";
    document.querySelector(".post-date").style.color = "#662200";

    // Footer
    document.querySelector("footer").style.backgroundColor = "#3B1C1C";
    document.querySelector("footer p").style.color = "#FFEDD5";
});

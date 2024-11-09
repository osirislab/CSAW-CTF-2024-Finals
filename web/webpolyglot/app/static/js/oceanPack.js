// oceanPack.js
document.addEventListener('DOMContentLoaded', function() {
    document.body.style.backgroundColor = "#E0F7FA";
    document.body.style.color = "#003333";

    // Header
    document.querySelector("header").style.backgroundColor = "#006994";
    document.querySelector("header h1").style.color = "#FFFFFF";
    document.querySelector("header p").style.color = "#E0F7FA";

    // Links
    document.querySelectorAll("nav ul li a").forEach(link => {
        link.style.color = "#E0F7FA";
        link.style.fontWeight = "bold";
    });

    // Main Content
    document.querySelector(".post-detail").style.backgroundColor = "#B2EBF2";
    document.querySelector(".post-detail").style.borderRadius = "8px";
    document.querySelector(".post-detail h2").style.color = "#006994";
    document.querySelector(".post-content").style.color = "#004D4D";
    document.querySelector(".post-date").style.color = "#00796B";

    // Footer
    document.querySelector("footer").style.backgroundColor = "#004D4D";
    document.querySelector("footer p").style.color = "#E0F7FA";
});

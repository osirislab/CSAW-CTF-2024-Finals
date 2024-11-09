
console.log(1);
document.body.style.backgroundColor = "#121212 !important";
document.body.style.color = "#E0E0E0";

// Header
document.querySelector("header").style.backgroundColor = "#1F1F1F";
document.querySelector("header h1").style.color = "#BB86FC";
document.querySelector("header p").style.color = "#CFD8DC";

// Links
document.querySelectorAll("nav ul li a").forEach(link => {
    link.style.color = "#BB86FC";
    link.style.fontWeight = "bold";
});

// Main Content
document.querySelector(".post-detail").style.backgroundColor = "#333333";
document.querySelector(".post-detail").style.borderRadius = "8px";
document.querySelector(".post-detail h2").style.color = "#BB86FC";
document.querySelector(".post-content").style.color = "#E0E0E0";
document.querySelector(".post-date").style.color = "#888888";

// Footer
document.querySelector("footer").style.backgroundColor = "#1F1F1F";
document.querySelector("footer p").style.color = "#E0E0E0";


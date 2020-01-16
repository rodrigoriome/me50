const jokes = [
    "Hello, world.",
    "This is Mac50.",
    "Your Hash Tables<br>even faster.",
    "Nicer than<br>David's vocabulary.",
    "72 73 33",
    "Mike Smith's favorite song is Turn the Page by Metallica."
];
const jokeIndex = Math.round(Math.random() * (jokes.length - 1));
const jokeElement = document.querySelector("#joke");
if (jokeElement) {
    jokeElement.innerHTML = jokes[jokeIndex];
}

const buyButton = document.querySelector("#buy-button");
if (buyButton) {
    buyButton.addEventListener("click", function(e) {
        alert("Mac50 Pro was added to your cart!");
    });
}

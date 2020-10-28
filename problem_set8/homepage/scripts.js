function search(term)
{
    window.open("https://unsplash.com/s/photos/"+term);
}

document.querySelector("#lake").addEventListener("click", function(){search("lake")});

document.querySelector("#forest").addEventListener("click", function(){search("forest")});

document.querySelector("#sea").addEventListener("click", function(){search("sea")});
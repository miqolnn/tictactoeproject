const socket = new WebSocket("ws://localhost:8080");

const board = document.getElementById("board");
const status = document.getElementById("status");
const startScreen = document.getElementById("startScreen");
const startBtn = document.getElementById("startBtn");
const nameInput = document.getElementById("name");
const difficultySelect = document.getElementById("difficulty");
const resetBtn = document.getElementById("resetBtn");

let cells = [];

for (let i = 0; i < 9; i++) {
    const cell = document.createElement("div");
    cell.classList.add("cell");
    cell.dataset.index = i;
    cell.addEventListener("click", () => handleClick(i));
    board.appendChild(cell);
    cells.push(cell);
}

let gameOver = false;

function handleClick(index) {
    if (gameOver || cells[index].textContent !== "") return;

    markCell(index, "X");
    socket.send((index + 1).toString());
}

function markCell(index, symbol) {
    cells[index].textContent = symbol;
    cells[index].classList.add("taken");
}

startBtn.addEventListener("click", () => {
    const name = nameInput.value.trim();
    const difficulty = difficultySelect.value;

    if (!name) {
        alert("Please enter your name.");
        return;
    }

    socket.send(JSON.stringify({ name, difficulty }));
    startScreen.style.display = "none";
    board.style.display = "grid";
    resetBtn.style.display = "inline";
});


socket.onmessage = (event) => {
    const msg = event.data;

    if (/^\d$/.test(msg)) {
        // AI move
        const aiIndex = parseInt(msg) - 1;
        markCell(aiIndex, "O");
    } else if(msg.startsWith("Player")) {
        status.textContent = "You won!";
        gameOver = true;
    } else if(msg.startsWith("AI")){
        status.textContent = "API won!";
        gameOver = true;
    }
      else if(msg.startsWith("Draw")){
    	status.textContent = "Draw!";
	gameOver = true;
    }
};

resetBtn.addEventListener("click", () => {
    for (let i = 0; i < 9; i++) {
        cells[i].textContent = "";
        cells[i].classList.remove("taken");
    }

    status.textContent = "";
    gameOver = false;

    socket.send("reset");
});

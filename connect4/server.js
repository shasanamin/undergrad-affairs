console.log('Connect4 Starting...')

const http = require('http') 
const fs = require('fs')
const fetch = require('node-fetch')
const ws = require('ws')

const readFile = file => new Promise((res, rej) =>
	fs.readFile(file, 'utf-8', (err,data) => err?rej(err):res(data)))

// to allow game playing via http (instead of manually loading file into browser)
const server = http.createServer(async (req, resp) => {
	if(req.url === '/') {
		resp.end(await readFile('connect4.html'))
	} else if(req.url === '/vue.js') {
		resp.end(await readFile('vue.js'))
	} else if(req.url === '/client.js') {
		resp.end(await readFile('client.js'))
	} else if(req.url === '/connect4.css') {
		resp.end(await readFile('connect4.css'))
	} else{
		resp.end('Unhandled Exception')
	}
})

// playing connect4 right now, but can easily convert this to connectN where N is a positive integer less than min(boardRows, boardCols)
const connectNum = 4;  

const numRows = 6;
const numCols = 7;

const startingBoard =  [[-1, -1, -1, -1, -1, -1, -1],
						[-1, -1, -1, -1, -1, -1, -1],
						[-1, -1, -1, -1, -1, -1, -1],
						[-1, -1, -1, -1, -1, -1, -1],
						[-1, -1, -1, -1, -1, -1, -1],
						[-1, -1, -1, -1, -1, -1, -1],]

changeTurn = currentTurn => {  // toggle between first and second players
	if(!(currentTurn%2))  // first players are even in our scheme (very first player is recognized as 0)
		return ++currentTurn
	else
		return --currentTurn
}

// check whether any empty slots (represented by -1) still remain
checkDraw = board => {
	for(var r=0; r<numRows; r++) {
		for(var c=0; c<numCols; c++) {
			if (board[r][c]===-1)
				return false
		}
	}
	return true
}

// playerID is whatever s/he is represented as on board e.g. 'X' or 'O'
checkWin = (playerID, board) => {
	for(var r=0; r<numRows; r++) {
		for(var c=0; c<numCols; c++){
			if((board[r][c])===playerID) {
				var verticalConnect = 1;
				var horizontalConnect = 1;
				var diagRightConnect = 1;
				var diagLeftConnect = 1;

				// check whether connections equal to connectNum exist in any direction
				for(var k=1; k<connectNum; k++) { 
					if ((r+k)<numRows) {
						if(board[r+k][c]===playerID) {
							verticalConnect++
						}
					}
					if ((c+k)<numCols) {
						if(board[r][c+k]===playerID) {
							horizontalConnect++
						}
					}
					if ((r+k)<numRows && (c+k)<numCols) {
						if(board[r+k][c+k]===playerID) {
							diagRightConnect++
						}
					}
					if((r+k)<numRows && (c-k)>=0) {
						if(board[r+k][c-k]===playerID) {
							diagLeftConnect++
						}
					}
				}
				// player with playerID has won if connections in any direction equal connectNum
				if(verticalConnect===connectNum || horizontalConnect===connectNum || diagRightConnect===connectNum || diagLeftConnect===connectNum) {
					return true
				}
			}
		}
	}
	return false
}

let allPlayers = [];  // contains connection info
let playerNames = [];  // contains player names only
let boards = [];  // equal to number of (active) games

new ws.Server({ server }).on('connection', player =>{
	player.on('message', msg => {
		text = JSON.parse(msg);
		
		if(text.type==='newPlayer') {
			allPlayers.push(player)
			playerNames.push(text.data)
			console.log('Number of Active Players: ' + allPlayers.length)
			if(allPlayers.length > 1 && !(allPlayers.length % 2)) {
				console.log(playerNames[allPlayers.length-2] + ' and ' + playerNames[allPlayers.length-1] + ' matched.')
				console.log('Starting Game ' + allPlayers.length/2)
				boards.push(startingBoard)
				playerIndex = allPlayers.length - 2  // first turn given to first to connect (among the pair whose game is starting)
				allPlayers[playerIndex].send(JSON.stringify({type:'startGame', playerColors:['Red', 'Yellow'], oppName:playerNames[playerIndex+1], playerNum: playerIndex, turn: true}))
				allPlayers[playerIndex+1].send(JSON.stringify({type:'startGame', playerColors:['Yellow', 'Red'], oppName:playerNames[playerIndex], playerNum: playerIndex+1, turn: false}))
			} else {
				console.log('Player' + allPlayers.length + ' waiting for a match')
			}
		}

		else if(text.type==='makeMove') {
			c = text.data
			playerIndex = text.playerNum
			currentBoard = Math.floor(playerIndex/2)  // index of current board
			for(var r=numRows-1; r>=0; r--) {
				// unless such 'deep copy' is done, all boards would be updated every time
				let bd = JSON.parse(JSON.stringify(boards[currentBoard]))
				// check for empty slot in selected column
				if(bd[r][c]===-1) {
					bd[r].splice(c,1,playerIndex)  // putting in player number into boxes really instead of 'X' or 'O'
					boards[currentBoard] = bd
					allPlayers[playerIndex].send(JSON.stringify({type:'playerMove', data:[r,c], turn:false}))
					if(checkWin(playerIndex, boards[currentBoard])) {
						boards[currentBoard] = startingBoard  // restart game

						allPlayers[playerIndex].send(JSON.stringify({type:'result', data:'You WON! Match restarting.'}))
						// display player's move to opponent and tell him/ her that s/he lost
						opponentIndex = changeTurn(playerIndex)
						allPlayers[opponentIndex].send(JSON.stringify({type:'opponentMove', data:[r,c], turn:true}));  // game is over, don't allow opponent to make another move
						allPlayers[opponentIndex].send(JSON.stringify({type:'result', data:'You LOST. Match restarting...'}))
					} else if(checkDraw(boards[currentBoard])) {
						boards[currentBoard] = startingBoard  // restart game

						allPlayers[playerIndex].send(JSON.stringify({type:'result', data:'You Drawn'}))
						opponentIndex = changeTurn(playerIndex)
						allPlayers[opponentIndex].send(JSON.stringify({type:'opponentMove', data:[r,c], turn:true}));
						allPlayers[opponentIndex].send(JSON.stringify({type:'result', data:'Match Drawn'}))
					} else {
						playerIndex = changeTurn(playerIndex)
						allPlayers[playerIndex].send(JSON.stringify({type:'opponentMove', data:[r,c],turn:true}));
					}
					break
				}
			}
		}
	})

	player.on('close', () => {
		console.log('A player disconnected.')
	})
})

server.listen(5000)
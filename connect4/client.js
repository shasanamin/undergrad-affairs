new Vue({
	template: `
		<div>
			<h1>Connect4</h1>
			<font size = '4'>{{message}}</font>
			<div>
  				<div v-for="n in numRows"> 
  					<span v-for="r in numCols">
  						<span class="dot" v-on:mouseover="hover(r-1, 1)" v-on:mouseleave="hover(r-1, 0)" v-on:click="clicked(r-1)" v-bind:style="{backgroundColor: board[n-1][r-1]}"> 
  						</span>
  					</span>
   				</div>
			</div>
		</div>
	`,
	data: {
		numRows: 6,
		numCols: 7,
		message: '',
		name: '',
		turn: false,
		playerNum: 0,
		playerColor: 'Red',
		opponentColor: 'Blue',
		boardColor: '#92E9EC',
		hoverColor: '#38A0A4',
		board: [['#92E9EC','#92E9EC','#92E9EC','#92E9EC','#92E9EC','#92E9EC','#92E9EC'],
				['#92E9EC','#92E9EC','#92E9EC','#92E9EC','#92E9EC','#92E9EC','#92E9EC'],
				['#92E9EC','#92E9EC','#92E9EC','#92E9EC','#92E9EC','#92E9EC','#92E9EC'],
				['#92E9EC','#92E9EC','#92E9EC','#92E9EC','#92E9EC','#92E9EC','#92E9EC'],
				['#92E9EC','#92E9EC','#92E9EC','#92E9EC','#92E9EC','#92E9EC','#92E9EC'],
				['#92E9EC','#92E9EC','#92E9EC','#92E9EC','#92E9EC','#92E9EC','#92E9EC']],
		ws: new WebSocket('ws://localhost:5000')
	},
	methods: {
		clicked(c) {
			if(this.turn) {
				this.ws.send(JSON.stringify({type:'makeMove', playerNum: this.playerNum, data: c}))
			} else {
				alert('Wait for your turn')
			}
		},		
		hover(c, on) {
			if (on) {
				toColor = this.hoverColor
				replacedColor = this.boardColor
			} else {
				toColor = this.boardColor
				replacedColor = this.hoverColor
			}
			for(var i=0; i<this.numRows; i++) {
				if(this.board[i][c]===replacedColor) {
					this.board[i].splice(c,1,toColor)
				}
			}
		},
	},
	mounted() {
		this.ws.onopen = () => {
			console.log('Connected')
			this.name = prompt('Please enter your name: ')
			this.message = 'Waiting for another player to connect.'
			this.ws.send(JSON.stringify({type:'newPlayer', player:this.name, data:this.name}))
		},

		this.ws.onmessage = msg => {
			var temp = JSON.parse(msg.data)
			if(temp.type==='startGame') {
				this.message = `You have been matched against ${temp.oppName} (${temp.playerColors[1]})`
				this.playerColor = temp.playerColors[0];
				this.opponentColor = temp.playerColors[1];
				this.playerNum = temp.playerNum;
				this.turn = temp.turn;
			} else if(temp.type==='opponentMove') {
				cord = temp.data;
				this.board[cord[0]].splice(cord[1],1,this.opponentColor)
				this.turn = temp.turn;
			} else if(temp.type==='playerMove') {
				cord = temp.data;
				this.board[cord[0]].splice(cord[1],1,this.playerColor)
				this.turn = temp.turn;
			} else if(temp.type==='result') {
				alert(temp.data);
				for (var r=0; r<this.numRows; r++) {
					for (var c=0; c<this.numCols; c++) {
						this.board[r].splice(c,1,'#92E9EC')
					}
				}
			}
		}
	}
}).$mount('#root')
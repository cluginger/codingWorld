let current_player;
let game_started = Boolean(false);
let game_board = [[0,0,0], [0,0,0], [0,0,0]];
let round_counter = 0;

var dice = document.getElementById('dice');
var commands = document.getElementById('commands');
var zero_zero = document.getElementById('zero-zero');
var zero_one = document.getElementById('zero-one');
var zero_two = document.getElementById('zero-two');
var one_zero = document.getElementById('one-zero');
var one_one = document.getElementById('one-one');
var one_two = document.getElementById('one-two');
var two_zero = document.getElementById('two-zero');
var two_one = document.getElementById('two-one');
var two_two = document.getElementById('two-two');


dice.addEventListener('click', start_game);

function start_game()
{   
   if(game_started === false)
    {
        clear_gameboard();
        current_player = Math.round(Math.random() * (2 - 1) + 1 );
        let text_who_starts = "Player " + current_player.toString() + " starts the game!!";
        commands.innerText = text_who_starts;
    } 

   game_started = true;
}

function clear_gameboard()
{
    for(var i = 0; i < game_board.length; i++)
    {
        for(var j = 0; j < game_board[i].length; j++) {
            game_board[i][j] = 0;
        }
    }
    zero_zero.style.backgroundColor = "white";
    zero_one.style.backgroundColor = "white";
    zero_two.style.backgroundColor = "white";
    one_one.style.backgroundColor = "white";
    one_zero.style.backgroundColor = "white";
    one_two.style.backgroundColor = "white";
    two_zero.style.backgroundColor = "white";
    two_one.style.backgroundColor = "white";
    two_two.style.backgroundColor = "white";
}

function hint_restart()
{
    setTimeout(function(){
        alert('If you want a rematch just roll the dice!!');}, 1000);
}

function checkwinner()
{
    let winner = 'Player';
    

    //checking y rows for a winner
    if(game_board[0][0] !== 0 && game_board[0][0] === game_board[0][1] && game_board[0][1] === game_board[0][2])
    {

        winner+=game_board[0][0].toString();
        commands.innerText = 'Congratulation ' + winner + ', you are the Winner!!';
        game_started = false;
        hint_restart();
        round_counter = 0;
        
    }
    else if(game_board[1][0] !== 0 && game_board[1][0] === game_board[1][1] && game_board[1][1] === game_board[1][2])
    {
        winner+=game_board[1][0].toString();
        commands.innerText = 'Congratulation ' + winner + ', you are the Winner!!';   
        game_started = false;
        hint_restart();
        round_counter = 0;
    }
    else if(game_board[2][0] !== 0 && game_board[2][0] === game_board[2][1] && game_board[2][1] === game_board[2][2])
    {
        winner+=game_board[2][0].toString();
        commands.innerText = 'Congratulation ' + winner + ', you are the Winner!!';   
        game_started = false;
        hint_restart();
        round_counter = 0;
    }
    //checking x rows for a winner

    else if(game_board[0][0] !== 0 && game_board[0][0] === game_board[1][0] && game_board[1][0] === game_board[2][0])
    {

        winner+=game_board[0][0].toString();
        commands.innerText = 'Congratulation ' + winner + ', you are the Winner!!';
        game_started = false;
        hint_restart();
        round_counter = 0;
        
    }
    else if(game_board[0][1] !== 0 && game_board[0][1] === game_board[1][1] && game_board[1][1] === game_board[2][1])
    {

        winner+=game_board[0][1].toString();
        commands.innerText = 'Congratulation ' + winner + ', you are the Winner!!';
        game_started = false;
        hint_restart();
        round_counter = 0;
    }
    else if(game_board[0][2] !== 0 && game_board[0][2] === game_board[1][2] && game_board[1][2] === game_board[2][2])
    {

        winner+=game_board[0][2].toString();
        commands.innerText = 'Congratulation ' + winner + ', you are the Winner!!';
        game_started = false;
        hint_restart();   
        round_counter = 0;
    }
    //checking the diagonals

    else if(game_board[0][0] !== 0 && game_board[0][0] === game_board[1][1] && game_board[1][1] === game_board[2][2])
    {

        winner+=game_board[0][0].toString();
        commands.innerText = 'Congratulation ' + winner + ', you are the Winner!!';
        game_started = false;
        hint_restart();  
        round_counter = 0; 
    }

    else if(game_board[2][0] !== 0 && game_board[2][0] === game_board[1][1] && game_board[1][1] === game_board[0][2])
    {

        winner+=game_board[2][0].toString();
        commands.innerText = 'Congratulation ' + winner + ', you are the Winner!!';
        game_started = false; 
        hint_restart();  
        round_counter = 0;
    }
//check for a draw
    if(round_counter === 9 && game_started === true)
    {
        commands.innerText = 'There is a draw!!'; 
        game_started = false; 
        hint_restart(); 
        round_counter = 0;
    }

}

// now for every field of the gameboard a eventlistener is made

zero_zero.addEventListener('click', function(){
    if(game_started === true && game_board[0][0] === 0)
    {
        round_counter++;
        if(current_player === 1)
        {
            zero_zero.style.backgroundColor = "red";
            game_board[0][0] = current_player;
            current_player = 2;
            commands.innerText = 'Now it is the turn of Player2!!';
        }
        else
        {
            zero_zero.style.backgroundColor = "blue";
            game_board[0][0] = current_player;
            current_player = 1
            commands.innerText = 'Now it is the turn of Player1!!';
        } 
    }
    checkwinner(); 

} )

zero_one.addEventListener('click', function(){
    if(game_started === true && game_board[0][1] === 0)
    {
        round_counter++;
        if(current_player === 1)
        {
            zero_one.style.backgroundColor = "red";
            game_board[0][1] = current_player;
            current_player = 2;
            commands.innerText = 'Now it is the turn of Player2!!';
        }
        else
        {
            zero_one.style.backgroundColor = "blue";
            game_board[0][1] = current_player;
            current_player = 1
            commands.innerText = 'Now it is the turn of Player1!!';
        } 
    }
    checkwinner(); 

} )

zero_two.addEventListener('click', function(){
    if(game_started === true && game_board[0][2] === 0)
    {
        round_counter++;
        if(current_player === 1)
        {
            zero_two.style.backgroundColor = "red";
            game_board[0][2] = current_player;
            current_player = 2;
            commands.innerText = 'Now it is the turn of Player2!!';
        }
        else
        {
            zero_two.style.backgroundColor = "blue";
            game_board[0][2] = current_player;
            current_player = 1
            commands.innerText = 'Now it is the turn of Player1!!';
        } 
    }
    checkwinner(); 

} )

one_zero.addEventListener('click', function(){
    if(game_started === true && game_board[1][0] === 0)
    {
        round_counter++;
        if(current_player === 1)
        {
            one_zero.style.backgroundColor = "red";
            game_board[1][0] = current_player;
            current_player = 2;
            commands.innerText = 'Now it is the turn of Player2!!';
        }
        else
        {
            one_zero.style.backgroundColor = "blue";
            game_board[1][0] = current_player;
            current_player = 1
            commands.innerText = 'Now it is the turn of Player1!!';
        } 
    }
    checkwinner(); 

} )
one_one.addEventListener('click', function(){
    if(game_started === true && game_board[1][1] === 0)
    {
        round_counter++;
        if(current_player === 1)
        {
            one_one.style.backgroundColor = "red";
            game_board[1][1] = current_player;
            current_player = 2;
            commands.innerText = 'Now it is the turn of Player2!!';
        }
        else
        {
            one_one.style.backgroundColor = "blue";
            game_board[1][1] = current_player;
            current_player = 1
            commands.innerText = 'Now it is the turn of Player1!!';
        } 
    }
    checkwinner(); 

} )

one_two.addEventListener('click', function(){
    if(game_started === true && game_board[1][2] === 0)
    {
        round_counter++;
        if(current_player === 1)
        {
            one_two.style.backgroundColor = "red";
            game_board[1][2] = current_player;
            current_player = 2;
            commands.innerText = 'Now it is the turn of Player2!!';
        }
        else
        {
            one_two.style.backgroundColor = "blue";
            game_board[1][2] = current_player;
            current_player = 1
            commands.innerText = 'Now it is the turn of Player1!!';
        } 
    }
    checkwinner(); 

} )

two_zero.addEventListener('click', function(){
    if(game_started === true && game_board[2][0] === 0)
    {
        round_counter++;
        if(current_player === 1)
        {
            two_zero.style.backgroundColor = "red";
            game_board[2][0] = current_player;
            current_player = 2;
            commands.innerText = 'Now it is the turn of Player2!!';
        }
        else
        {
            two_zero.style.backgroundColor = "blue";
            game_board[2][0] = current_player;
            current_player = 1
            commands.innerText = 'Now it is the turn of Player1!!';
        } 
    }
    checkwinner(); 

} )

two_one.addEventListener('click', function(){
    if(game_started === true && game_board[2][1] === 0)
    {
        round_counter++;
        if(current_player === 1)
        {
            two_one.style.backgroundColor = "red";
            game_board[2][1] = current_player;
            current_player = 2;
            commands.innerText = 'Now it is the turn of Player2!!';
        }
        else
        {
            two_one.style.backgroundColor = "blue";
            game_board[2][1] = current_player;
            current_player = 1
            commands.innerText = 'Now it is the turn of Player1!!';
        } 
    }
    checkwinner(); 

} )

two_two.addEventListener('click', function(){
    if(game_started === true && game_board[2][2] === 0)
    {
        round_counter++;
        if(current_player === 1)
        {
            two_two.style.backgroundColor = "red";
            game_board[2][2] = current_player;
            current_player = 2;
            commands.innerText = 'Now it is the turn of Player2!!';
        }
        else
        {
            two_two.style.backgroundColor = "blue";
            game_board[2][2] = current_player;
            current_player = 1
            commands.innerText = 'Now it is the turn of Player1!!';
        } 
    }
    checkwinner(); 

} )



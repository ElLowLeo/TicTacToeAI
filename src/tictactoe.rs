use std::cmp::{max, min};

fn printBoard(board: [[char; 3]; 3]) {
    println!("\x1b[H\x1b[2J");
    println!("\x1B[33mBem vindo ao jogo da velha! Em Rust!\x1B[0m");

    for i in 0..3 {
        for j in 0..3 {
            print!("{}", board[i][j]);
            if j < 2 {
                print!(" | ");
            }
        }
        println!();
        if i < 2 {
            println!("---------");
        }
    }
}

fn checkWinner(board: [[char; 3]; 3], player: char) -> bool {
    for i in 0..3 {
        if board[i][0] == player && board[i][1] == player && board[i][2] == player {
            return true;
        }
        if board[0][i] == player && board[1][i] == player && board[2][i] == player {
            return true;
        }
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
        || (board[0][2] == player && board[1][1] == player && board[2][0] == player)
    {
        return true;
    }
    false
}

fn getAvailableMoves(board: [[char; 3]; 3]) -> Vec<[usize; 2]> {
    let mut moves = Vec::new();
    for i in 0..3 {
        for j in 0..3 {
            if board[i][j] == ' ' {
                moves.push([i, j]);
            }
        }
    }
    moves
}

fn makeMove(board: &mut [[char; 3]; 3], player: char, row: usize, col: usize) -> bool {
    if board[row][col] == ' ' {
        board[row][col] = player;
        true
    } else {
        false
    }
}

fn minimax(board: &mut [[char; 3]; 3], depth: i32, maximizingPlayer: bool, mut alpha: i32, mut beta: i32) -> i32 {
    if checkWinner(*board, 'O') {
        return 1;
    }
    if checkWinner(*board, 'X') {
        return -1;
    }
    if isBoardFull(*board) {
        return 0;
    }

    if maximizingPlayer {
        let mut maxEval = std::i32::MIN;
        let moves = getAvailableMoves(*board);

        for &movePos in moves.iter() {
            let row = movePos[0];
            let col = movePos[1];

            board[row][col] = 'O';
            let eval = minimax(board, depth + 1, false, alpha, beta);
            board[row][col] = ' ';

            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);

            if beta <= alpha {
                break;
            }
        }
        maxEval
    } else {
        let mut minEval = std::i32::MAX;
        let moves = getAvailableMoves(*board);

        for &movePos in moves.iter() {
            let row = movePos[0];
            let col = movePos[1];

            board[row][col] = 'X';
            let eval = minimax(board, depth + 1, true, alpha, beta);
            board[row][col] = ' ';

            minEval = min(minEval, eval);
            beta = min(beta, eval);

            if beta <= alpha {
                break;
            }
        }
        minEval
    }
}

fn aiMakeMove(board: &mut [[char; 3]; 3]) {
    let mut bestEval = std::i32::MIN;
    let mut bestMove = [0, 0];
    let moves = getAvailableMoves(*board);
    let mut alpha = std::i32::MIN;
    let mut beta = std::i32::MAX;

    for &movePos in moves.iter() {
        let row = movePos[0];
        let col = movePos[1];

        board[row][col] = 'O';
        let eval = minimax(board, 0, false, alpha, beta);
        board[row][col] = ' ';

        if eval > bestEval {
            bestEval = eval;
            bestMove = [row, col];
        }
    }

    if bestEval != std::i32::MIN {
        makeMove(board, 'O', bestMove[0], bestMove[1]);
    }
}

fn isBoardFull(board: [[char; 3]; 3]) -> bool {
    for i in 0..3 {
        for j in 0..3 {
            if board[i][j] == ' ' {
                return false;
            }
        }
    }
    true
}

fn main() {
    let mut board: [[char; 3]; 3] = [[' '; 3]; 3];

    loop {
        printBoard(board);

        loop {
            let mut input = String::new();
            println!("Jogador X - Qual o seu movimento? (1-9): ");
            std::io::stdin().read_line(&mut input).expect("Failed to read line");

            let trimmed = input.trim();
            match trimmed.parse::<usize>() {
                Ok(moveNum) if moveNum >= 1 && moveNum <= 9 => {
                    let row = (moveNum - 1) / 3;
                    let col = (moveNum - 1) % 3;

                    if makeMove(&mut board, 'X', row, col) {
                        break;
                    } else {
                        println!("\x1B[31mMovimento invalido! Tente novamente.\x1B[0m");
                    }
                }
                _ => {
                    println!("\x1B[31mEntrada invalida! Por favor escreva um numero entre 1 e 9.\x1B[0m");
                }
            }
        }

        if checkWinner(board, 'X') {
            printBoard(board);
            println!("\x1B[32mJogador X ganhou!\x1B[0m");
            break;
        }

        if isBoardFull(board) {
            printBoard(board);
            println!("\x1B[33mVelha!\x1B[0m");
            break;
        }

        aiMakeMove(&mut board);

        if checkWinner(board, 'O') {
            printBoard(board);
            println!("\x1B[32mA máquina ganhou!\x1B[0m");
            break;
        }

        if isBoardFull(board) {
            printBoard(board);
            println!("\x1B[33mVelha!\x1B[0m");
            break;
        }
    }
}

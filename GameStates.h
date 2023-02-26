#pragma once
enum GameState { 
	Menu, 
	TargetPractice, 
	UpdateYAML, 
	GameStats, 
	Scoreboard , 
	/*player vs ai states*/
	EnemyGamePlay,
	EnemyGamePlayLose, 
	EnemyGamePlayWin
};
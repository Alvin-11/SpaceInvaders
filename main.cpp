#include "raylib.h"
#include "vector"
#include <array>
#include <iostream>
#include <string>

using std::vector;
struct Bullet {
	Vector2 position;
	bool active;
};

const int BLOCK_WIDTH = 20;
const int BLOCK_HEIGHT = 5;
const int CELL_SIZE = 6;
const int NUM_BLOCKS = 3;

int blocks[NUM_BLOCKS][BLOCK_HEIGHT][BLOCK_WIDTH];  
Vector2 blockPositions[NUM_BLOCKS];               

void InitBlocks() {
	for (int b = 0; b < NUM_BLOCKS; b++) {
		for (int y = 0; y < BLOCK_HEIGHT; y++) {
			for (int x = 0; x < BLOCK_WIDTH; x++) {
				blocks[b][y][x] = 1;
			}
		}
	}
	blockPositions[0] = { 100, 800 };
	blockPositions[1] = { 400, 800 };
	blockPositions[2] = { 800, 800 };
}

void DrawBlocks() {
	for (int b = 0; b < NUM_BLOCKS; b++) {
		int startX = blockPositions[b].x;
		int startY = blockPositions[b].y;
		for (int y = 0; y < BLOCK_HEIGHT; y++) {
			for (int x = 0; x < BLOCK_WIDTH; x++) {
				if (blocks[b][y][x] == 1) {
					DrawRectangle(startX + x * CELL_SIZE, startY + y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GREEN);
				}
			}
		}
	}
}

bool CheckBulletCollision(int bulletX, int bulletY) {
	for (int b = 0; b < NUM_BLOCKS; b++) {
		int blockX = blockPositions[b].x;
		int blockY = blockPositions[b].y;

		int relX = (bulletX - blockX) / CELL_SIZE;
		int relY = (bulletY - blockY) / CELL_SIZE;

		if (relX >= 0 && relX < BLOCK_WIDTH && relY >= 0 && relY < BLOCK_HEIGHT) {
			if (blocks[b][relY][relX] == 1) {
				blocks[b][relY][relX] = 0;
				return true;
			}
		}
	}
	return false;
}

bool CanShoot(float lastShootTime, float shootDelay) {
	if (GetTime() - lastShootTime >= shootDelay) {
		return true;  
	}
	return false;  
}

bool EnemyCanShoote(float lastShootTime, float shootDelay) {
	if (GetTime() - lastShootTime >= (shootDelay*5)) {
		return true;
	}
	return false;
}

void DeleteBullets(vector<Bullet> &bullets) {
	for (int i = 0;i < bullets.size(); i++) {
	
		if (bullets[i].position.y < 0) {
			bullets[i].active = false;
		}
	}

}

void VerifyCollisionEnemySpaceship(vector<Bullet>& bullets, Vector2& positionSpaceShip, Vector2 scale, Texture2D enemyTex, bool& isSpaceshipAlive, Sound hitSound)  {
	float enemyWidth = enemyTex.width * scale.x;
	float enemyHeight = enemyTex.height * scale.y;
	for (int j = 0; j < bullets.size(); j++) {
		if (bullets[j].active == false) continue;
			Rectangle enemyRect = { positionSpaceShip.x, positionSpaceShip.y,enemyWidth , enemyHeight };
			Rectangle bulletRect = { bullets[j].position.x, bullets[j].position.y, 10, 30 };
			if (CheckCollisionRecs(bulletRect, enemyRect) && isSpaceshipAlive) {
			bullets[j].active = false;
			isSpaceshipAlive = false;
			PlaySound(hitSound);
			}
	}
}

void VerifyCollisionMysteryEnemySpaceship(vector<Bullet>& bullets, Vector2& positionSpaceShip, Vector2 scale, Texture2D enemyTex, bool& isSpaceshipAlive, Sound hitSound) {
	float enemyWidth = enemyTex.width * scale.x;
	float enemyHeight = enemyTex.height * scale.y;
	for (int j = 0; j < bullets.size(); j++) {
		if (bullets[j].active == false) continue;
		Rectangle enemyRect = { positionSpaceShip.x, positionSpaceShip.y,enemyWidth , enemyHeight };
		Rectangle bulletRect = { bullets[j].position.x, bullets[j].position.y, 10, 30 };
		if (CheckCollisionRecs(bulletRect, enemyRect) && isSpaceshipAlive) {
			bullets[j].active = false;
			isSpaceshipAlive = false;
			PlaySound(hitSound);
		}
	}
}

void  VerifyCollision(vector<Bullet>& bullets, Vector2(&positions)[20], Vector2 scale, Texture2D enemyTex, bool(&isAlive)[20], Sound hitSound) {
	

	float enemyWidth = enemyTex.width * scale.x;
	float enemyHeight = enemyTex.height * scale.y;
	for (int j = 0; j < bullets.size(); j++) {
		if (bullets[j].active == false) continue;
		for (int i = 0; i < 20; i++) {

			Rectangle enemyRect = { positions[i].x, positions[i].y,enemyWidth , enemyHeight };
			Rectangle bulletRect = { bullets[j].position.x, bullets[j].position.y, 10, 30 };
			if (CheckCollisionRecs(bulletRect, enemyRect) && isAlive[i]) {
				bullets[j].active = false;
				isAlive[i] = false;
				PlaySound(hitSound);
			}
		}
	}
}
void  MysteryShipVerifyCollision(vector<Bullet>& bullets, Vector2 positions, Vector2 scale, Texture2D enemyTex, bool &isAlive, Sound hitSound, int &mysteryShipScore) {


	float enemyWidth = enemyTex.width * scale.x * 0.8;
	float enemyHeight = enemyTex.height * scale.y * 0.8;
	for (int j = 0; j < bullets.size(); j++) {
		if (bullets[j].active == false) continue;
		

			Rectangle enemyRect = { positions.x, positions.y,enemyWidth , enemyHeight };
			Rectangle bulletRect = { bullets[j].position.x, bullets[j].position.y, 10, 30 };
			if (CheckCollisionRecs(bulletRect, enemyRect) && isAlive) {
				bullets[j].active = false;
				isAlive = false;
				PlaySound(hitSound);
				mysteryShipScore += 50;
			}
		
	}
}
void InitializePositions(Vector2 (&positions)[20]) {
	int index = 0;
	for (float y = 200; y <= 500; y += 100) {  // Loop through y values: 100, 200, 300, 400
		for (float x = 100; x <= 700; x += 150) {  // Loop through x values: 100, 250, 400, 550, 700
			positions[index] = { x, y };
			index++;
		}
	}
}


int main() {
	
	const int WINDOWWIDTH = 1000;
	const int WINDOWHEIGHT = 1000;
	InitWindow(WINDOWWIDTH, WINDOWHEIGHT, "Space Invaders");
	InitBlocks();
	Texture2D enemy = LoadTexture("images/enemy.png");
	Texture2D spaceship = LoadTexture("images/spaceship.png");
	Texture2D mysteryship = LoadTexture("images/mysteryship.png");
	InitAudioDevice();
	SetMasterVolume(1.0f);
	Sound hitSound = LoadSound("resources/invaderkilled");

	Sound hitSound2 = LoadSound("resources/shoot");
	Vector2 positions[20] = { { 100, 200 }, { 250, 200 }, { 400, 200 } ,{ 550, 200 }, {700, 200},{ 100, 300 }, { 250, 300 }, { 400, 300 } ,{ 550, 300 }, {700, 300} ,{ 100, 400 }, { 250, 400 }, { 400, 400 } ,{ 550, 400 }, {700, 400} ,{ 100, 500 }, { 250, 500 }, { 400, 500 } ,{ 550, 500 }, {700, 500} };
	bool isAlive[20] = { true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true };
	Vector2 positionSpaceShip = {500,850};
	Vector2 scale = { 0.15f,0.15f};
	SetTargetFPS(120);
	const int speed =10;
	const int enemyspeed = 1;
	bool changeposition = false;
	const int fraction = 0.25;
	vector<Bullet> bullets;
	vector<Bullet> enemyBullets;
	vector<Bullet> mysteryenemyBullets;
	float lastShootTime = 0.0f; 
	float shootDelay = 0.5f;    
	float enemylastShootTime = GetTime();
	float mysteryenemylastShootTime = GetTime();
	float initialmysteryTime = 0.0f;
	float lastmysteryTime = 0.0f;
	bool isSpaceshipAlive = true;
	int HighScore = 0;
	float leftmostX = positions[0].x;
	float rightmostX = positions[0].x;
	int lives = 3;
	bool win = false;
	bool playerlost = false;
	bool mysteryShipisAlive = true;
	int mysteryShipScore = 0;
	Vector2 positionmysteryship = { -100,150 };
	while(!WindowShouldClose())
	{
		if (!win) {
			int randomv = GetRandomValue(0, 50);
			int randomValue = GetRandomValue(0, 19);
			while (!isAlive[randomValue]) {
				randomValue = GetRandomValue(0, 19);
			}
			VerifyCollisionMysteryEnemySpaceship(mysteryenemyBullets, positionSpaceShip, scale, spaceship, isSpaceshipAlive, hitSound);
			MysteryShipVerifyCollision(bullets, positionmysteryship, scale, mysteryship, mysteryShipisAlive, hitSound, mysteryShipScore);
			VerifyCollisionEnemySpaceship(enemyBullets, positionSpaceShip, scale, spaceship, isSpaceshipAlive, hitSound);
			VerifyCollision(bullets, positions, scale, enemy, isAlive, hitSound);
			lastmysteryTime = GetTime(); // mystery ship
			if ((int)(lastmysteryTime - initialmysteryTime) % 30 == 0) { // mystery ship
				positionmysteryship = { -100, 150 };
				mysteryShipisAlive = true;

			}
			positionmysteryship.x += 2;
			if (randomv == 5) { //mystery ship shooting logic
				mysteryenemylastShootTime = GetTime();
				Bullet newBullet;
				newBullet.position = positionmysteryship;
				newBullet.position.x += 80;
				newBullet.active = true;
				mysteryenemyBullets.push_back(newBullet);
			}
			for (int i = 0;i < mysteryenemyBullets.size(); i++) {// mystery ship
				if (mysteryenemyBullets[i].active) {
					mysteryenemyBullets[i].position.y += 3;
				}
			}
			for (int i = 0;i < mysteryenemyBullets.size(); i++) {// mystery ship
				if (mysteryenemyBullets[i].active) {
					if (mysteryenemyBullets[i].position.y >= 1000) mysteryenemyBullets[i].active = false;
				}
			}

			if (!isSpaceshipAlive) {
				lives -= 1;
			}
			HighScore = 0;
			HighScore += mysteryShipScore;
			for (int i = 0; i < 20; i++) {
				if (!isAlive[i]) {
					if (i < 5)HighScore += 40;
					else if (i < 10) HighScore += 30;
					else if (i < 15) HighScore += 20;
					else if (i < 20) HighScore += 10;
				}

			}

			for (int i = 0; i < bullets.size(); i++) {
				if (!bullets[i].active) continue;

				bool result = CheckBulletCollision(bullets[i].position.x, bullets[i].position.y);
				if (result) {
					bullets[i].active = false;
				}
			}
			for (int i = 0; i < mysteryenemyBullets.size(); i++) {
				if (!mysteryenemyBullets[i].active) continue;

				bool result = CheckBulletCollision(mysteryenemyBullets[i].position.x, mysteryenemyBullets[i].position.y);
				if (result) {
					mysteryenemyBullets[i].active = false;
				}
			}

			for (int i = 0; i < enemyBullets.size(); i++) {
				if (!enemyBullets[i].active) continue;

				bool result = CheckBulletCollision(enemyBullets[i].position.x, enemyBullets[i].position.y);
				if (result) {
					enemyBullets[i].active = false;
				}
			}


			DeleteBullets(bullets);
			for (int i = 0;i < bullets.size(); i++) {
				bullets[i].position.y -= 3;
			}
			if (CanShoot(lastShootTime, shootDelay)) {
				if (IsKeyDown(KEY_SPACE)) {
					PlaySound(hitSound2);
					lastShootTime = GetTime();
					Bullet newBullet;
					newBullet.position = positionSpaceShip;
					newBullet.position.x += 80;
					newBullet.active = true;
					bullets.push_back(newBullet);

				}
			}
			if (IsKeyPressed(KEY_Q) && !isSpaceshipAlive) {
				InitBlocks();
				InitializePositions(positions);
				for (int i = 0; i < 20; i++) {
					isAlive[i] = true;
				}
				positionSpaceShip = { 500,850 };
				scale = { 0.15f,0.15f };
				changeposition = false;
				bullets.clear();
				enemyBullets.clear();
				mysteryenemyBullets.clear();
				isSpaceshipAlive = true;
				HighScore = 0;
				lives = 3;
				win = false;
				playerlost = false;
				positionmysteryship = { -100,150 };
				mysteryShipisAlive = true;
				mysteryShipScore = 0;
				leftmostX = positions[0].x;
				rightmostX = positions[0].x;

				lastShootTime = 0.0f;
				shootDelay = 0.5f;
				enemylastShootTime = GetTime();
				mysteryenemylastShootTime = GetTime();
				initialmysteryTime = 0.0f;
				lastmysteryTime = 0.0f;


			}
			if (IsKeyPressed(KEY_B)) PlaySound(hitSound);
			if (IsKeyDown(KEY_ENTER)) isSpaceshipAlive = false;
			if (IsKeyDown(KEY_LEFT)) positionSpaceShip.x -= speed * 0.3;
			if (IsKeyDown(KEY_RIGHT)) positionSpaceShip.x += speed * 0.3;
			if (positionSpaceShip.x < -40) positionSpaceShip.x = -40;
			if ((positionSpaceShip.x + speed) > 870) positionSpaceShip.x = 870 - speed;
			for (int i = 0;i < enemyBullets.size(); i++) {
				enemyBullets[i].position.y += 3;
			}
			if (EnemyCanShoote(enemylastShootTime, shootDelay)) {
				if (isAlive[randomValue]) {
					enemylastShootTime = GetTime();
					Bullet newBullet;
					newBullet.position = positions[randomValue];
					newBullet.position.x += 80;
					newBullet.active = true;
					enemyBullets.push_back(newBullet);
				}
			}
			leftmostX = 0;
			rightmostX = 0;
			for (int i = 0; i < 20; i++) {
				if (isAlive[i]) {
					if (positions[i].x < leftmostX) leftmostX = positions[i].x;
					if (positions[i].x > rightmostX) rightmostX = positions[i].x;
				}
			}

			if (leftmostX < 0) {
				changeposition = true;
			}
			else if (rightmostX + enemyspeed > 850) {
				changeposition = false;
				for (int i = 0; i < 20; i++) {
					if (isAlive[i]) {
						positions[i].y += 10;
					}
				}
			}

			/*if (positions[0].x < 0) {
				changeposition = true;
			}
			else if ((positions[4].x + 50) > 900) {
				changeposition = false;
			}*/
			for (int i = 0; i < 20; i++) {

				if (changeposition) {
					positions[i].x += enemyspeed;
				}
				else {
					positions[i].x -= enemyspeed;
				}
			}
			int tempbool = 0;
			for (int i = 0; i < 20; i++) {
				if (isAlive[i] && positions[i].y + 10 >= 850) {
					tempbool += 1;
				}

			}
			if (tempbool > 0) {
				playerlost = true;
			}
			int tempStorage = 0;
			for (int i = 0; i < 20; i++) {
				if (isAlive[i]) {
					tempStorage += 1;
				}
			}
			if (tempStorage <= 0) {
				win = true;
			}
		}
		else {
			if (IsKeyPressed(KEY_Q)) {
				InitBlocks();
				InitializePositions(positions);
				for (int i = 0; i < 20; i++) {
					isAlive[i] = true;
				}
				positionSpaceShip = { 500,850 };
				scale = { 0.15f,0.15f };
				changeposition = false;
				bullets.clear();
				enemyBullets.clear();
				mysteryenemyBullets.clear();
				isSpaceshipAlive = true;
				HighScore = 0;
				lives = 3;
				win = false;
				playerlost = false;
				positionmysteryship = { -100,150 };
				mysteryShipisAlive = true;
				mysteryShipScore = 0;
				leftmostX = positions[0].x;
				rightmostX = positions[0].x;

				lastShootTime = 0.0f;
				shootDelay = 0.5f;
				enemylastShootTime = GetTime();
				mysteryenemylastShootTime = GetTime();
				initialmysteryTime = 0.0f;
				lastmysteryTime = 0.0f;


			}
}
		
		BeginDrawing();
		ClearBackground(BLACK);
		if (win) {
			DrawText("Winner !!!!", 350, 350, 50, GREEN);
			DrawText(("Your Highscore : " + std::to_string(HighScore)).c_str(), 200, 500, 50, GREEN);
			DrawText("Press the Key	'Q' to Restart the game" , 200, 650, 30, BLUE);
		}
		else if (playerlost) {
			DrawText("GAME OVER", 350, 450, 50, RED);
			DrawText("Press the Key	'Q' to Restart the game", 200, 600, 30, BLUE);
		}
		else if (isSpaceshipAlive && lives > 0) {
			DrawBlocks();
			DrawText(("HighScore : " + std::to_string(HighScore)).c_str(), 100, 0, 50, BLUE);
			if (mysteryShipisAlive) {
				DrawTextureEx(mysteryship, positionmysteryship, scale.x * 0.80, scale.y * 0.80, WHITE);
			}
			Vector2 fakeposition[3] = { {550,0},{ 650,0 } ,{ 750,0 } };

			for (int i = 0; i < lives; i++) {
				DrawTextureEx(spaceship, fakeposition[i], scale.x * 0.5, scale.y * 0.5, WHITE);
			}
			
			for (int i = 0;i < 20; i++) {
				if (isAlive[i]) {
					DrawTextureEx(enemy, positions[i], scale.x*0.75, scale.y*0.75, WHITE);
				}


			}
			if (isSpaceshipAlive) {
				DrawTextureEx(spaceship, positionSpaceShip, scale.x, scale.y, WHITE);
			}
			for (int i = 0;i < bullets.size(); i++) {
				if (bullets[i].active) {
					bullets[i].position.y -= speed * GetFrameTime();  
					DrawRectangle(bullets[i].position.x, bullets[i].position.y, 10, 30, RED);
				}
			}
			for (int i = 0;i < enemyBullets.size(); i++) {
				if (enemyBullets[i].active) {
					enemyBullets[i].position.y -= speed * GetFrameTime(); 
					DrawRectangle(enemyBullets[i].position.x, enemyBullets[i].position.y, 10, 30, WHITE);
				}
			}
			for (int i = 0;i < mysteryenemyBullets.size(); i++) {
				if (mysteryenemyBullets[i].active) {
					mysteryenemyBullets[i].position.y -= speed * GetFrameTime();
					DrawRectangle(mysteryenemyBullets[i].position.x, mysteryenemyBullets[i].position.y, 10, 30, WHITE);
				}
			}
		}
		else if (!isSpaceshipAlive && lives < 0) {
			DrawText("GAME OVER", 350, 450, 50, RED);
			DrawText("Press the Key	'Q' to Restart the game", 200, 600, 30, BLUE);
		}
		else if (!isSpaceshipAlive && lives > 0) {
			isSpaceshipAlive = true;
		
		}
		//DrawTexture(enemy, 10, 10, WHITE);
		
		EndDrawing();
	}
	UnloadSound(hitSound);     
	UnloadSound(hitSound2);     

	CloseAudioDevice();
	CloseWindow();
	return 0;

}